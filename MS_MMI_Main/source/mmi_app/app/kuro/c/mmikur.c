
/*****************************************************************************
** File Name:      mmikur.c                                                  *
** Author:                                                                   *
** Date:           18/08/2007                                                *
** Description:    This file is used to describe kur player                  *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 08/2007       cmzs              Create                                  *
******************************************************************************/

#define _MMIKUR_C_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmi_app_kuro_trc.h"
#ifdef WIN32
#include "std_header.h"
#endif
#include "mmibt_app.h"
#ifdef KURO_SUPPORT
//#include "mmikur_internal.h"
#include "mmifmm_export.h"
//#include "mmimultim.h"
#include "mmiidle_export.h"
//#include "mmikuro_text.h"
//#include "mmipub.h"
#include "mmi_appmsg.h"
//#include "mmikur_id.h"
#include "mmiidle_subwintab.h"
#include "mmikur.h"
#include "mmisd_export.h"
#include "guifont.h"
//#include "mmikur_lyric_internal.h"
#include "mmk_timer.h"
#include "mmicc_export.h"
//#include "mmi_default.h"
#include "audio_api.h"
#include "mmiaudio_ctrl.h"
//#include "mmikur_nv.h"
//#include "bswap.h"
//#include "klrfunction.h"
//#include "deep_sleep.h"
//#include "nv_productionparam_type.h"
#include "nv_item_id.h"
//#include "nvitem.h"
//#include "chng_freq.h"
//#include "freq_cfg.h"
#include "guilcd.h"
//#include "mmikur_musicstyle.h"

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#define KUR_RAND_RANGE              0x7FFF                      //产生随机数的范围
#define KUR_RAND_SPACE              (RAND_MAX/KUR_RAND_RANGE)   //产生随机数的区间跨度
#define KURO_ID_LENGTH              23
//#define KURO_FULL_PATH_MAX_LEN      120 //120 //byte
//#define KURO_FULL_PATH_MAX_LEN    MMIFILE_FULL_PATH_MAX_LEN
/*
#if defined(GSM6901)
    #define MMIKURO_AMOI_PRODUCT_ID   "a616"
#else
    #if defined(GSM6833)
        #define MMIKURO_AMOI_PRODUCT_ID   "a636"
    #else
        #if defined(GSM6828)
            #define MMIKURO_AMOI_PRODUCT_ID   "a525"
        #else 
            #if defined(GSM6830)
                #define MMIKURO_AMOI_PRODUCT_ID   "m600"
            #else
                #define MMIKURO_AMOI_PRODUCT_ID   "0000"
            #endif
        #endif
    #endif  
#endif
*/
const wchar g_mmikur_dir_kur[]       = { 'K', 'u', 'r', 'o' ,0 };
const wchar g_mmikur_dir_kursong[]   = { 'K', 'u', 'r', 'o', 'S', 'o', 'n', 'g' ,0 };
const wchar g_mmikur_klr_ext[]       = { 'k', 'l', 'r', 0 };
//const wchar s_mmikur_list[]          = { 'k', 'u', 'r', 'o', '_', 'l', 'i', 's', 't', '.', 'd', 'a', 't', 0 };

/**--------------------------------------------------------------------------*
 **                         LOCAL DEFINITION                                 *
 **--------------------------------------------------------------------------*/
LOCAL uint32                s_kur_play_sample_rate = 32000;
LOCAL MMIFMM_FILTER_T       s_kur_filter = {0};
LOCAL MMIKUR_PAUSED_TYPE_E  s_kur_pause_type = MMIKUR_PAUSED_BY_NONE;
LOCAL MMIKUR_PLAY_INFO_T    s_kur_play_info = {0};  //播放时需要的信息
LOCAL BOOLEAN               s_is_nv_init = FALSE;
/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/
typedef struct
{
    uint8 kuro_did[23];
} PROD_KURO_PARAM_T;
/**--------------------------------------------------------------------------*
 **                         EXTERNAL DECLARE                                 *
 **--------------------------------------------------------------------------*/
extern WINDOW_TABLE( MMIKUR_MAIN_PLAY_WIN_TAB );

#ifndef WIN32
extern uint32* MMI_GetFreqHandler(void);
#endif
/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : play kur
//  Global resource dependence : none
//  Author: cmzs
//  Note:only for kur module
/*****************************************************************************/
LOCAL BOOLEAN MMIKUR_PlayKurFile(
                    uint16  *kur_full_path,
                    uint16  kur_full_path_len
                    );

/*****************************************************************************/
//  Description : 根据设备的具体情况初始化Sysdat
//  Global resource dependence : none
//  Author: cmzs
//  Note: 
/******************** *********************************************************/
LOCAL void MMIKUR_InitSysDat(KURO_DEV_INFO *psysdat);
/*****************************************************************************/
//  Description : 返回系统缺省的目录(如果U Disk存在则返回U Disk,否则返回T卡)
//  Global resource dependence : none
//  Author: cmzs
//  Note: 
/******************** *********************************************************/
LOCAL char MMIKUR_GetDefaultDisk(void);

/*****************************************************************************/
//  Description : Get all setting info.
//  Global resource dependence : none
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
LOCAL void GetKuroSettingInfo(
                MMIKUR_SETTINGS_INFO_T *setting_info_ptr
                );

/*****************************************************************************/
//  Description : Set all setting info.
//  Global resource dependence : none
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
LOCAL void SetKuroSettingInfo(
                MMIKUR_SETTINGS_INFO_T *setting_info_ptr
                );

/*****************************************************************************/
//  Description : Reset all setting info.
//  Global resource dependence : none
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
LOCAL void ResetKuroSettingInfo(
                    void
                    );

/*****************************************************************************/
//  Description : Get NV settings, and initialize the virables.
//  Global resource dependence : none
//  Author: xingdong.li
//  Note: 
/*****************************************************************************/
LOCAL void InitKuroModuleVariables(
                void
                );

/*****************************************************************************/
//  Description : Initialize kuro search filter.
//  Global resource dependence : 
//  Author:xingdong.li
//  Note: 
/*****************************************************************************/
LOCAL void InitKuroFileFilter(
                void
                );

/*****************************************************************************/
//  Description : initialize Kuro Folders
//  Global resource dependence : none
//  Author: xingdong.li
//  Note: 
/*****************************************************************************/
LOCAL void InitKuroFolder(
                const wchar   *device_ptr,
                uint16        device_len,
                const wchar   *dir_ptr,
                uint16        dir_len
                );

/*****************************************************************************/
//  Description : 获取指定范围内的不重复的随机整数.
//      比如产生0-4之间5个不同的随机数，先产生0-4之间的一个随机数2，
//  将其作为第一个随机数，然后产生0-3之间的一个随机数2，按照顺序除去已经使用的2，
//  则此随机数实际位3，作为第二个随机数，以此类推。
//  Global resource dependence : 
//  Author:jassmine.meng
//  Note: 
/*****************************************************************************/
LOCAL void InitRandomInfo(
                void
                );

/*****************************************************************************/
//  Description : Handle load all kuro waiting win
//  Global resource dependence : 
//  Author:xingdong.li
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleLoadAllKuroWaitingWin(
                         MMI_WIN_ID_T       win_id,
                         MMI_MESSAGE_ID_E   msg_id, 
                         DPARAM             param
                         );

/*****************************************************************************/
//  Description : 获得即将播放的下一首kur的索引.
//  Global resource dependence : 
//  Author:cmzs
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN GetNextKurIndex(//下一首歌曲的索引是否发生变化
                    uint16                start_index,    //in:起始播放歌曲索引
                    uint16                active_index,   //in:正在播放歌曲索引
                    uint16                *next_index_ptr,//out:即将播放歌曲索引
                    uint16                total_num,      //in:歌曲总数
                    MMIKUR_PLAY_MODE_E    play_mode,      //in:播放歌曲模式
                    MMIKUR_RANDOM_INFO_T  *random_info_ptr//in/out:随机播放一次不同随机数信息
                    );

/*****************************************************************************/
//  Description : callback functin for pcm play kuro
//  Global resource dependence : none
//  Author: cmzs
//  Note: 
/*****************************************************************************/
LOCAL AUDIO_STREAM_GET_SRC_RESULT_E MMIKUR_GetSrcCallback(HAUDIO hAudio, uint8** const ppucDataAddr, uint32* const puiDataLength); 

/*****************************************************************************/
//  Description : API of create kuro buffer play handle
//  Global resource dependence : none
//  Author: cmzs
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN MMIKUR_CreateAudioHandle(// SUCCESS - return the audio handle created
                                         // FAILURE - return SCI_NULL
                                         BOOLEAN is_need_a2dp,
                                         const uint16* kurpath
                                         );

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : Get all setting info.
//  Global resource dependence : none
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
LOCAL void GetKuroSettingInfo(
                MMIKUR_SETTINGS_INFO_T *setting_info_ptr
                )
{
    MN_RETURN_RESULT_E  return_value = MN_RETURN_FAILURE;

    MMINV_READ(MMINV_KUR_SET_INFO, setting_info_ptr, return_value);
    if (MN_RETURN_SUCCESS != return_value)
    {
        setting_info_ptr->lyric_style        = KLR_MODE_TWOLINE;
        setting_info_ptr->cur_volume         = MMIKUR_DEFAULT_VOL;
        setting_info_ptr->eq_mode            = MMIKUR_DEFAULT_EQ_MODE;
        setting_info_ptr->play_mode          = MMIKUR_DEFAULT_PLAY_MODE;
        SetKuroSettingInfo(setting_info_ptr);
    }
}

/*****************************************************************************/
//  Description : Set all setting info.
//  Global resource dependence : none
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
LOCAL void SetKuroSettingInfo(
                MMIKUR_SETTINGS_INFO_T *setting_info_ptr
                )
{
	//arvinzhang modify:delete assert
    //SCI_ASSERT(PNULL != setting_info_ptr);
    if(PNULL != setting_info_ptr)
	{
		MMINV_WRITE(MMINV_KUR_SET_INFO,setting_info_ptr);
	}
}

/*****************************************************************************/
//  Description : Reset all setting info.
//  Global resource dependence : none
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
LOCAL void ResetKuroSettingInfo(
                    void
                    )
{
    MMIKUR_SETTINGS_INFO_T  setting_info = {0};

    setting_info.lyric_style        = KLR_MODE_TWOLINE;
    setting_info.cur_volume         = MMIKUR_DEFAULT_VOL;
    setting_info.eq_mode            = MMIKUR_DEFAULT_EQ_MODE;
    setting_info.play_mode          = MMIKUR_DEFAULT_PLAY_MODE;
    SetKuroSettingInfo(&setting_info);
}

/*****************************************************************************/
//  Description : Get NV settings, and initialize the virables.
//  Global resource dependence : none
//  Author: xingdong.li
//  Note: Just for power on
/*****************************************************************************/
LOCAL void InitKuroModuleVariables(
                void
                )
{
    MMIKUR_SETTINGS_INFO_T setting_info = {0};
    
    SCI_MEMSET(&s_kur_play_info,0,sizeof(MMIKUR_PLAY_INFO_T));

    //set play info
    GetKuroSettingInfo(&setting_info); 
    s_kur_play_info.lyric_style         = setting_info.lyric_style;
    s_kur_play_info.cur_vol             = setting_info.cur_volume;
    s_kur_play_info.eq_mode             = setting_info.eq_mode;
    s_kur_play_info.cur_play_mode       = setting_info.play_mode;
    s_kur_play_info.audio_state         = MMIKUR_AUDIO_STOPPED;
    s_kur_play_info.player_state        = MMIKUR_PLAYER_STATE_NONE;
    s_is_nv_init                        = FALSE;
}

/*****************************************************************************/
//  Description : Initialize kuro search filter.
//  Global resource dependence : 
//  Author:xingdong.li
//  Note: 
/*****************************************************************************/
LOCAL void InitKuroFileFilter(
                void
                )
{
    uint32 filter_len = 0;
    
    SCI_MEMSET(s_kur_filter.filter_str, 0x00, sizeof(s_kur_filter.filter_str));

    s_kur_filter.filter_str[filter_len++] = '*';
    s_kur_filter.filter_str[filter_len++] = '.';
    MMI_MEMCPY(
        &s_kur_filter.filter_str[filter_len], sizeof(s_kur_filter.filter_str)-filter_len, 
        MMIKURO_SUFFIX, MMIKURO_SUFFIX_LEN, 
        MMIKURO_SUFFIX_LEN
        );
    filter_len += MMIKURO_SUFFIX_LEN;
}
/*****************************************************************************/
//  Description : initialize kuro dir
//  Global resource dependence : none
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIKUR_InitKurFolder(void)
{
    MMIFILE_DEVICE_E dev    = MMI_DEVICE_UDISK;
    
    //SCI_TRACE_LOW:"[MMIKURO]: MMIAPIKUR_InitKurFolder"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIKUR_332_112_2_18_2_31_31_0,(uint8*)"");
 
    do
    {
        if(MMIAPIFMM_GetDeviceTypeStatus(dev))
        {
            InitKuroFolder(
                            MMIAPIFMM_GetDevicePath(dev),
                            MMIAPIFMM_GetDevicePathLen(dev),
                            MMIKURO_DIR_KURO, 
                            MMIKURO_DIR_KURO_LEN
                            );

            InitKuroFolder(
                            MMIAPIFMM_GetDevicePath(dev), 
                            MMIAPIFMM_GetDevicePathLen(dev),
                            MMIKURO_DIR_KUROSONG, 
                            MMIKURO_DIR_KUROSONG_LEN
                            );
        }
    }while(++dev < MMI_DEVICE_NUM);
}

/*****************************************************************************/
//  Description : Initialize kuro module when app initializes.
//  Global resource dependence : none
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIKUR_Init(void)
{
    InitKuroModuleVariables();
    InitKuroFileFilter();
    MMIAPIKUR_InitKurFolder();
    //MMIKUR_OnPowerOn();
}

/*****************************************************************************/
//  Description : Register kuro module
//  Global resource dependence : none
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIKUR_InitModule(void)
{
    //register menu
    MMIKUR_RegMenuGroup();
    MMIKUR_RegNv();
}

/*****************************************************************************/
//  Description : initialize Kuro Folders
//  Global resource dependence : none
//  Author: xingdong.li
//  Note: 
/*****************************************************************************/
LOCAL void InitKuroFolder(
                const wchar   *device_ptr,
                uint16        device_len,
                const wchar   *dir_ptr,
                uint16        dir_len
                )
{
    wchar   full_path_name[MMIFILE_FULL_PATH_MAX_LEN+1] = {0};
    uint16  full_path_len = MMIFILE_FULL_PATH_MAX_LEN+1;

	//arvinzhang modify:delete assert
	if(NULL == device_ptr || NULL == dir_ptr || 0 >= device_len || 0 >= dir_len)
	{
		return;
	}

    if(MMIAPIFMM_CombineFullPath(
            device_ptr, device_len,
            dir_ptr, dir_len, 
            NULL, 0, 
            full_path_name, &full_path_len
            ))
    { 
        if(!MMIAPIFMM_IsFolderExist((uint16 *)full_path_name, full_path_len))
        {
            MMIAPIFMM_CreateDir((uint16 *)full_path_name, full_path_len);
        }
    }
}

/*****************************************************************************/
//  Description : Get the head size of kur.
//  Global resource dependence : none
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMIAPIKUR_GetKuroHeadSize(
                void
                )
{   
    return s_kur_play_info.file_info.kuro_header_size;
}

/*****************************************************************************/
//  Description : Get the volume of play kur
//  Global resource dependence : none
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC uint8 MMIKUR_GetVolume(
                void
                )
{   
    return s_kur_play_info.cur_vol;
}

/*****************************************************************************/
//  Description : Set the volume of kur
//  Global resource dependence : none
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIKUR_SetVolume(
                uint8     kur_volume//in
                )
{
    MMIKUR_SETTINGS_INFO_T    setting_info = {0};

	//arvinzhang modify:delete assert
    //SCI_ASSERT(MMISET_VOL_MAX >= kur_volume);
	if(MMISET_VOL_MAX >= kur_volume)
	{
    	GetKuroSettingInfo(&setting_info);
    	setting_info.cur_volume = kur_volume;
    	SetKuroSettingInfo(&setting_info);

    	s_kur_play_info.cur_vol = kur_volume;
	}
}

/*****************************************************************************/
//  Description : Set the volume of kur
//  Global resource dependence : none
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIKUR_SetKurVolume(//volume is change
                    BOOLEAN  is_increase
                    )
{
    BOOLEAN is_vol_change = FALSE;
    uint8   volume        = s_kur_play_info.cur_vol;

    if (is_increase)//增加音量
    {
        if (MMISET_VOL_MAX > volume)
        {
            volume++;
            is_vol_change = TRUE;
        }
    }
    else
    {
        if (MMISET_VOL_ZERO < volume)
        {
            volume--;
            is_vol_change = TRUE;
        }
    }
    
    if (is_vol_change)
    {
        MMIKUR_SetVolume(volume);

        if ((MMIAUDIO_GetIsPlaying()) && 
            (MMIAUDIO_KURO == MMIAUDIO_GetAudioHtype()))
        {
            //MMIAPISET_InitAudioDevice(volume);
            
            if(MMIAPIBT_GetActiveBTHeadset())
            {
                MMIAUDIO_SetVolume(AUDIO_DEVICE_MODE_EARPHONE, volume);
            }
            else
            {
                MMIAPISET_InitAudioDevice(volume);
            }
        }
    }

    return (is_vol_change);
}

/*****************************************************************************/
//  Description : Get the mode of play kur
//  Global resource dependence : none
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC MMIKUR_PLAY_MODE_E MMIKUR_GetPlayMode(void)
{
    return s_kur_play_info.cur_play_mode;
}

/*****************************************************************************/
//  Description : Set the mode of kur
//  Global resource dependence : none
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIKUR_SetPlayMode(
                   MMIKUR_PLAY_MODE_E  play_mode//in
                   )
{
    MMIKUR_SETTINGS_INFO_T    setting_info = {0};

	//arvinzhang modify:delete assert
    //SCI_ASSERT((MMIKUR_PLAY_ONE_ONCE <= play_mode) && (MMIKUR_PLAY_MAX_MODE > play_mode));
	if((MMIKUR_PLAY_ONE_ONCE <= play_mode) && (MMIKUR_PLAY_MAX_MODE > play_mode))
	{
	    GetKuroSettingInfo(&setting_info);
	    setting_info.play_mode = play_mode;
	    SetKuroSettingInfo(&setting_info);

	    s_kur_play_info.cur_play_mode = play_mode;
	}
}

/*****************************************************************************/
//  Description : get the mode of eq
//  Global resource dependence : none
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC MMIAUDIO_EQ_MODE_E MMIKUR_GetEqMode(void)
{
    return s_kur_play_info.eq_mode;
}

/*****************************************************************************/
//  Description : set the mode of kur
//  Global resource dependence : none
//  Author: xingdong.li
//  Note:
/*****************************************************************************/

PUBLIC void MMIKUR_SetEqMode(
                MMIAUDIO_EQ_MODE_E    eq_mode     //in
                )
{
#ifdef MP3_EQ_SUPPORT 
    MMIKUR_SETTINGS_INFO_T    setting_info = {0};

	//arvinzhang modify:delete assert
    //SCI_ASSERT(MMIAUDIO_EQ_MAX_MODE > eq_mode);
	if(MMIAUDIO_EQ_MAX_MODE > eq_mode)
	{
	    GetKuroSettingInfo(&setting_info);
	    setting_info.eq_mode = eq_mode;
	    SetKuroSettingInfo(&setting_info);
	    s_kur_play_info.eq_mode = eq_mode;
	    
	    MMIAUDIO_SetEQMode(eq_mode);
	}
#endif
}

/*****************************************************************************/
//  Description : Get the kuro filter.
//  Global resource dependence : 
//  Author:xingdong.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIKUR_GetKurFilter(
                MMIFMM_FILTER_T * filter_ptr
                )
{
    MMI_MEMCPY(filter_ptr, sizeof(MMIFMM_FILTER_T), &s_kur_filter, 
        sizeof(MMIFMM_FILTER_T), sizeof(MMIFMM_FILTER_T));
    //SCI_TRACE_LOW:"[MMIKURO]: MMIKUR_GetKurFilter, filter = %s, orig_filter = %s"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIKUR_573_112_2_18_2_31_32_1,(uint8*)"ss", filter_ptr->filter_str,s_kur_filter.filter_str);
}
/*****************************************************************************/
//  Description : Load all kuro from fixed folder--"KuroSong".
//  Global resource dependence : 
//  Author:xingdong.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIKUR_LoadAllKuro(
                        void
                        )
{
    MMI_WIN_ID_T    waiting_win_id   = MMIKUR_LOAD_ALL_WAITING_WIN_ID;
    uint32          waiting_time_out = 0;

    //SCI_TRACE_LOW:"[MMIKURO]: MMIKUR_LoadAllKuro, is_loaded = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIKUR_586_112_2_18_2_31_32_2,(uint8*)"d", s_kur_play_info.list_info.is_all_kur_loaded);
    if(!s_kur_play_info.list_info.is_all_kur_loaded)
    {
        MMIPUB_OpenAlertWinByTextId(&waiting_time_out, TXT_COMMON_WAITING, TXT_NULL,
            IMAGE_PUBWIN_WAIT, &waiting_win_id,PNULL,MMIPUB_SOFTKEY_ONE,HandleLoadAllKuroWaitingWin);
    }
    else
    {
        MMK_PostMsg(MMIKUR_PLAY_MENU_WIN_ID, MSG_KUR_LOAD_ALL_KURO_FINISH, PNULL, NULL);
    }
}

/*****************************************************************************/
//  Description : Reset the kuro list.
//  Global resource dependence : 
//  Author: xingdong.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIKUR_ResetList(void)
{
    if(PNULL != s_kur_play_info.list_info.list_array)
    {
        MMIAPIFILEARRAY_Destroy(&s_kur_play_info.list_info.list_array);
        s_kur_play_info.list_info.list_array = PNULL; 
    }
    MMIKUR_ClearRandomInfo();
    SCI_MEMSET(&s_kur_play_info.list_info, 0x00, sizeof(MMIKUR_LIST_INFO_T));
}

/*****************************************************************************/
//  Description : Reset the kuro list.
//  Global resource dependence : 
//  Author: xingdong.li
//  Note: Just set.
/*****************************************************************************/
PUBLIC void MMIKUR_SetKurList(
                MMIKUR_LIST_INFO_T *list_ptr
                )
{
	//arvinzhang modify:delete assert
    //SCI_ASSERT(PNULL != list_ptr);
    if(PNULL != list_ptr)
	{
	    MMI_MEMCPY(&s_kur_play_info.list_info, sizeof(MMIFMM_FILTER_T), list_ptr, 
	        sizeof(MMIKUR_LIST_INFO_T), sizeof(MMIKUR_LIST_INFO_T));
	}
}

/*****************************************************************************/
//  Description : Clear the current random list info.
//  Global resource dependence : 
//  Author:xingdong.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIKUR_ClearRandomInfo(void)
{
    if(PNULL != s_kur_play_info.list_info.random_info.random_index_ptr)
    {
        SCI_FREE(s_kur_play_info.list_info.random_info.random_index_ptr);
        s_kur_play_info.list_info.random_info.random_index_ptr = PNULL;
    }
    s_kur_play_info.list_info.random_info.active_index = 0;
}

/*****************************************************************************/
//  Description : Init the random list.
//  Global resource dependence : 
//  Author:xingdong.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIKUR_InitRandomList(void)
{
    MMIKUR_ClearRandomInfo();
    
    if (0 < MMIKUR_GetTotalNum())
    {
        //if(MMIKUR_PLAY_RANDOM_REPEAT == s_kur_play_info.cur_play_mode||
        //   MMIKUR_PLAY_RANDOM_ONCE   == s_kur_play_info.cur_play_mode)
        {
            InitRandomInfo();
        }
    }
}

/*****************************************************************************/
//  Description : 获取指定范围内的不重复的随机整数.
//      比如产生0-4之间5个不同的随机数，先产生0-4之间的一个随机数2，
//  将其作为第一个随机数，然后产生0-3之间的一个随机数2，按照顺序除去已经使用的2，
//  则此随机数实际位3，作为第二个随机数，以此类推。
//  Global resource dependence : 
//  Author:jassmine.meng
//  Note: 
/*****************************************************************************/
LOCAL void InitRandomInfo(
                void
                )
{
    BOOLEAN     *random_arr_ptr = PNULL; //用来记录某位置是否被使用过,FALSE未使用，TRUE使用
    uint32      i = 0,j = 0,random = 0,pos = 0,random_range = 0;
    uint32      random_temp = 0;
    uint32      min = 0, max = 0, total_num = 0;

    total_num = MMIKUR_GetTotalNum();
    max = total_num - 1;

    s_kur_play_info.list_info.random_info.random_index_ptr = SCI_ALLOC_APP(sizeof(uint32)*total_num);

    SCI_MEMSET(s_kur_play_info.list_info.random_info.random_index_ptr,0x00,(sizeof(uint32)*total_num));
    s_kur_play_info.list_info.random_info.active_index = 0;

    //allocate memory and set 0
    random_arr_ptr = SCI_ALLOC_APP(sizeof(uint8)*total_num);
    SCI_MEMSET(random_arr_ptr,0,(sizeof(uint8)*total_num));

    srand(SCI_GetTickCount());

    for (i=0; i<total_num; i++)
    {
        //产生0-7FFF之间的随机数
        random_range = rand()/KUR_RAND_SPACE;
        //产生min到max之间的随机数
        random_temp = random_range*((max) - (min));
        random = (uint16)((random_temp/KUR_RAND_RANGE + (min))&0xffff);

        pos = 0;
        for (j=0; j<total_num; j++)
        {
            if (!(*(random_arr_ptr+j)))//此位置未被使用
            {
                pos++;
            }
            if (pos == (random+1))
            {
                break;
            }
        }
        s_kur_play_info.list_info.random_info.random_index_ptr[i] = j;
        *(random_arr_ptr+j) = TRUE;
        
        //当前激活的随机数索引
        if (j == s_kur_play_info.active_index)
        {
            s_kur_play_info.list_info.random_info.active_index = i;
        }

        max--;
    }
    
    //free memory
    SCI_FREE(random_arr_ptr);
}

/*****************************************************************************/
//  Description : Handle load all kuro waiting win
//  Global resource dependence : 
//  Author:xingdong.li
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleLoadAllKuroWaitingWin(
                         MMI_WIN_ID_T       win_id,
                         MMI_MESSAGE_ID_E   msg_id, 
                         DPARAM             param
                         )
{
    MMI_RESULT_E    result = MMI_RESULT_TRUE;
    MMIFMM_FILTER_T filter = {0};
    //static BOOLEAN  s_is_finish = TRUE;
    //static BOOLEAN  s_is_searching_udisk = FALSE;
    //static BOOLEAN  s_is_searching_sd    = FALSE;
    //static FILEARRAY  s_udisk_array  = PNULL;
    static FILEARRAY  s_tem_array = PNULL;
    wchar           full_path_name[FMM_SEARCH_FILENAME_MAXLEN+1] = {0};
    uint16          full_path_len = FMM_SEARCH_FILENAME_MAXLEN+1;
    static MMIFILE_DEVICE_E s_searching_device = MMI_DEVICE_UDISK;
    
    //SCI_TRACE_LOW:"[MMIKUR]: HandleLoadAllKuroWaitingWin, msg=%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIKUR_745_112_2_18_2_31_32_3,(uint8*)"d",msg_id);
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        s_searching_device = MMI_DEVICE_UDISK;
        s_tem_array        = MMIAPIFILEARRAY_Create();
        
        MMIPUB_HandleAlertWinMsg(win_id, msg_id, param);
        //s_is_finish = TRUE;
        MMIKUR_ResetList();
        MMK_SendMsg(win_id, MSG_KUR_LOAD_ALL_KURO_FINISH, PNULL);
        break;
        
    case MSG_KUR_LOAD_ALL_KURO_FINISH:
        MMIKUR_GetKurFilter(&filter);
        
        if((s_searching_device == MMI_DEVICE_NUM))
        {
            MMK_CloseWin(win_id);
        }
        else
        {
			if(MMIAPIFILEARRAY_GetArraySize(s_tem_array) > 0)
			{
				if(!MMIAPIFILEARRAY_Combine(s_kur_play_info.list_info.list_array, s_tem_array))
				{
					//SCI_TRACE_LOW:"[MMIMP3]: HandleMp3LoadMyMusicWaitingWin, MMIAPIFILEARRAY_Combine FAIL"
					SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIKUR_772_112_2_18_2_31_32_4,(uint8*)"");
					 MMK_CloseWin(win_id);
				}
			}
            for(; s_searching_device < MMI_DEVICE_NUM; s_searching_device++)
            {
				if(MMIAPIFMM_GetDeviceStatus(MMIAPIFMM_GetDevicePath(s_searching_device), MMIAPIFMM_GetDevicePathLen(s_searching_device)))
				{
					SCI_MEMSET(full_path_name, 0, (FMM_SEARCH_FILENAME_MAXLEN+1) * sizeof(wchar));
					
					MMIAPIFMM_CombineFullPath(
						MMIAPIFMM_GetDevicePath(s_searching_device), 
						MMIAPIFMM_GetDevicePathLen(s_searching_device),
						MMIKURO_DIR_KUROSONG, 
						MMIKURO_DIR_KUROSONG_LEN,
						PNULL, 
						NULL,
						full_path_name,
                        &full_path_len
						);
					
					MMIAPIFMM_SearchFileInPath(
						full_path_name,
                        &full_path_len,
						&filter,
						TRUE,
						FUNC_FIND_FILE,
						s_tem_array,
						MMIKUR_LOAD_ALL_WAITING_WIN_ID,
						MSG_KUR_LOAD_ALL_KURO_FINISH
						);
					break;
				}
			}
			if(MMI_DEVICE_NUM == s_searching_device)
			{
				MMK_CloseWin(win_id);
			}
        }
        break;

    case MSG_APP_CANCEL:
    case MSG_APP_RED:
        //MMIAPIFMM_SearchFileStop();
        //s_is_finish = FALSE;
        break;

    case MSG_CLOSE_WINDOW:
        MMIAPIFILEARRAY_Destroy(&s_tem_array);
        s_tem_array = PNULL; 
        MMIKUR_InitRandomList();
        MMIPUB_HandleAlertWinMsg(win_id, msg_id, param);
        break;
    
    default:
        result = MMIPUB_HandleAlertWinMsg(win_id,msg_id,param);
        break;
    }

    return result;
}

/*****************************************************************************/
//  Description : Get kuro file number in the kuro list.
//  Global resource dependence : 
//  Author:xingdong.li
//  Note: 
/*****************************************************************************/
PUBLIC uint32 MMIKUR_GetTotalNum(void)
{
    return s_kur_play_info.list_info.total_num;
}

/*****************************************************************************/
//  Description : Get the playing active index.
//  Global resource dependence : 
//  Author:xingdong.li
//  Note: 
/*****************************************************************************/
PUBLIC uint32 MMIKUR_GetActiveIndex(void)
{
    return s_kur_play_info.active_index;
}

/*****************************************************************************/
//  Description : Set active index.
//  Global resource dependence : 
//  Author:xingdong.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIKUR_SetActiveIndex(
                uint32 index
                )
{
    s_kur_play_info.active_index = index;
}

/*****************************************************************************/
//  Description : Set start index.
//  Global resource dependence : 
//  Author:xingdong.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIKUR_SetStartIndex(
                uint32 index
                )
{
    s_kur_play_info.start_index = index;
}

/*****************************************************************************/
//  Description : load kuro file from kuro list. 
//  Global resource dependence : 
//  Author: xingdong.li
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIKUR_LoadKuroByIndex(
                    uint32 index
                    )
{
    BOOLEAN                 result = FALSE;

    SCI_MEMSET(&s_kur_play_info.file_info, 0x00, sizeof(MMIKUR_FILE_INFO_T));
    MMIKUR_SetActiveIndex(index);
    
    result = MMIKUR_GetKurInfoByIndex(index, &s_kur_play_info.file_info.kuro_name_info);

    //SCI_TRACE_LOW("[MMIKURO]: MMIKUR_LoadKuroByIndex, index = %d, result = %d", index, result);
    //SCI_TRACE_LOW("[MMIKURO]: MMIKUR_LoadKuroByIndex, name_len = %d", s_kur_play_info.file_info.kuro_name_info.name_len);
    return result;
}

/*****************************************************************************/
//  Description : Get Kuro file info from the list by index
//  Global resource dependence : none
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIKUR_GetKurInfoByIndex(
                    uint32              index,
                    FILEARRAY_DATA_T    *kuro_info_ptr
                    )
{
    BOOLEAN result = FALSE;

	//arvinzhang modify:delete assert
    //SCI_ASSERT(PNULL != kuro_info_ptr);
    
    if(PNULL != kuro_info_ptr)
	{    
	    if(s_kur_play_info.list_info.is_from_kdb)
	    {
	        MMIKUR_GetKdbSongPath(
	            kuro_info_ptr->filename,&kuro_info_ptr->name_len,
	            s_kur_play_info.list_info.kdb_class_index,s_kur_play_info.list_info.kdb_type_index,
	            index
	            );
	        result = TRUE;
	    }
	    else
	    {
	        if(PNULL != s_kur_play_info.list_info.list_array &&
	           index <  MMIKUR_GetTotalNum())
	        {
	            result = MMIAPIFILEARRAY_Read(s_kur_play_info.list_info.list_array, index, kuro_info_ptr);
	        }
	    }
	}
    //SCI_TRACE_LOW:"[MMIKURO] MMIKUR_GetKurInfoByIndex index = %d,result = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIKUR_924_112_2_18_2_31_32_5,(uint8*)"dd",index,result);
    return result;
}

/*****************************************************************************/
//  Description : Get the current playing Kuro file info.
//  Global resource dependence : none
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIKUR_GetCurKurInfo(
                    FILEARRAY_DATA_T    *kuro_info_ptr
                    )
{
    BOOLEAN result = FALSE;

	//arvinzhang modify:delete assert
    //SCI_ASSERT(PNULL != kuro_info_ptr);
    
    if(PNULL != kuro_info_ptr && 0 < s_kur_play_info.file_info.kuro_name_info.name_len)
    {
        MMI_MEMCPY(
            kuro_info_ptr, sizeof(FILEARRAY_DATA_T),
            &s_kur_play_info.file_info.kuro_name_info, sizeof(FILEARRAY_DATA_T), 
            sizeof(FILEARRAY_DATA_T)
            );
        result = TRUE;
    }

    return result;
}

/*****************************************************************************/
//  Description : Get Kuro lyric file info from the list by index
//  Global resource dependence : none
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIKUR_GetCurKlrInfo(
                    FILEARRAY_DATA_T    *klr_info_ptr
                    )
{
    uint16  nlength = 0;
    BOOLEAN result = FALSE;

	//arvinzhang modify:delete assert
    //SCI_ASSERT(PNULL != klr_info_ptr);
    
    if(PNULL != klr_info_ptr && MMIKUR_GetCurKurInfo(klr_info_ptr))
    {
        nlength = MMIAPICOM_Wstrlen( klr_info_ptr->filename );
        if(nlength > MMIKURO_KLR_EXT_LEN)
        {
            MMI_WSTRNCPY(
                klr_info_ptr->filename + (nlength - MMIKURO_KLR_EXT_LEN), MMIKURO_KLR_EXT_LEN, 
                MMIKURO_KLR_EXT, MMIKURO_KLR_EXT_LEN, 
                MMIKURO_KLR_EXT_LEN
                );
            result = TRUE;
        }
    }
    return result;
}

/*****************************************************************************/
//  Description : Get the device of the file.
//  Global resource dependence : 
//  Author:xingdong.li
//  Note: 
/*****************************************************************************/
PUBLIC MMIFILE_DEVICE_E MMIKUR_GetFileDevType(
                        const wchar    *file_name,         //[IN]
                        uint16         file_name_len       //[IN]
                        )
{
    MMIFILE_DEVICE_E dev_type = MMI_DEVICE_SYSTEM;
    uint16           device_name[MMIFILE_DEVICE_NAME_MAX_LEN+1] = {0};
    uint16           device_name_len = 0;

	//here,we should set dev from cur file name(full path)
	dev_type = MMIAPIFMM_GetDeviceTypeByPath (file_name, MMIFILE_DEVICE_LEN);

	MMIAPIFMM_SplitFullPath(
        file_name, file_name_len,
        device_name,&device_name_len, 
        PNULL, PNULL, PNULL, PNULL
        );

    return dev_type;
}

/*****************************************************************************/
//  Description : Init kuro list by MMIFILE_FILE_INFO_T
//  Global resource dependence : none
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIKUR_InitKuroListByFileInfo(
                    MMIFILE_FILE_INFO_T *file_info_ptr
                    )
{
    BOOLEAN             result = TRUE;
    FILEARRAY_DATA_T    file_data = {0};

	//arvinzhang modify:delete assert
    //SCI_ASSERT(PNULL != file_info_ptr);
    if(PNULL == file_info_ptr)
	{
		return FALSE;
	}

    MMIKUR_ResetList();
    file_data.name_len = MMIFILE_FILE_NAME_MAX_LEN + 1;
    MMIAPIFMM_CombineFullPath(
        file_info_ptr->device_name,file_info_ptr->device_name_len,
        file_info_ptr->dir_name,file_info_ptr->dir_name_len,
        file_info_ptr->file_name,file_info_ptr->file_name_len,
        file_data.filename,&file_data.name_len
        );
    s_kur_play_info.list_info.list_array = MMIAPIFILEARRAY_Create();
    if(PNULL != s_kur_play_info.list_info.list_array)
    {
        if(MMIAPIFILEARRAY_Add(s_kur_play_info.list_info.list_array, &file_data))
        {
            s_kur_play_info.list_info.total_num = MMIAPIFILEARRAY_GetArraySize(s_kur_play_info.list_info.list_array);
            result = TRUE;
        }
    }
    MMIKUR_InitRandomList();
    s_kur_play_info.active_index = 0;
    s_kur_play_info.start_index  = 0;

    //SCI_TRACE_LOW:"[MMIKURO]: MMIKUR_InitKuroListByFileInfo result = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIKUR_1038_112_2_18_2_31_33_6,(uint8*)"d",result);
    return result;
}

/*****************************************************************************/
//  Description : when format udisk,reset global variabel
//  Global resource dependence : 
//  Author: xingdong.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIKUR_ResetAllKur(void)
{
    //SCI_TRACE_LOW:"[MMIKURO]: MMIAPIKUR_ResetAllKur"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIKUR_1048_112_2_18_2_31_33_7,(uint8*)"");

    MMIAPIKUR_StopKurAndCloseWin();

    MMIKUR_ResetList();
    InitKuroModuleVariables();
}

/*****************************************************************************/
//  Description : whether the kuro player is playing
//  Global resource dependence :
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIKUR_IsPlayerPlaying(void)
{
    BOOLEAN     result = FALSE;

    if (MMIKUR_PLAYER_MUSIC_SWITCH == MMIKUR_GetPlayerState() ||
        MMIKUR_AUDIO_PLAYING       == MMIKUR_GetAudioState())
    {
        result = TRUE;
    }

    return (result);
}

/*****************************************************************************/
//  Description : whether the kuro player is paused
//  Global resource dependence :
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIKUR_IsPlayerPaused(void)
{
    BOOLEAN     result = FALSE;

    if (MMIKUR_PLAYER_SWITCH_PAUSED == MMIKUR_GetPlayerState() ||
        MMIKUR_AUDIO_PAUSED         == MMIKUR_GetAudioState())
    {
        result = TRUE;
    }

    return (result);
}

/*****************************************************************************/
//  Description : whether the kuro player is stopped
//  Global resource dependence : 
//  Author: xingdong.li
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIKUR_IsPlayerStopped(void)
{
    return (MMIKUR_PLAYER_STATE_NONE == MMIKUR_GetPlayerState());
}

/*****************************************************************************/
//  Description : whether the kuro player is not stopped
//  Global resource dependence : 
//  Author: xingdong.li
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIKUR_IsPlayerActive(void)
{
    return (!MMIAPIKUR_IsPlayerStopped());
}

/*****************************************************************************/
//  Description : Get current audio state.
//  Global resource dependence : 
//  Author:xingdong.li
//  Note: 
/*****************************************************************************/
PUBLIC MMIKUR_AUDIO_STATE_E MMIKUR_GetAudioState(
                                    void
                                    )
{
    return s_kur_play_info.audio_state;
}

/*****************************************************************************/
//  Description : Set current audio state.
//  Global resource dependence : 
//  Author:xingdong.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIKUR_SetAudioState(
                        MMIKUR_AUDIO_STATE_E cur_state
                        )
{
    s_kur_play_info.audio_state = cur_state;
}

/*****************************************************************************/
//  Description : Get player state.
//  Global resource dependence : 
//  Author:xingdong.li
//  Note: 
/*****************************************************************************/
PUBLIC MMIKUR_PLAYER_STATE_E MMIKUR_GetPlayerState(
                                    void
                                    )
{
    return s_kur_play_info.player_state;
}

/*****************************************************************************/
//  Description : Set player state.
//  Global resource dependence : 
//  Author:xingdong.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIKUR_SetPlayerState(
                    MMIKUR_PLAYER_STATE_E cur_state
                    )
{
    s_kur_play_info.player_state = cur_state;
}

/*****************************************************************************/
//  Description : Play kuro in a delayed manner, start delay timer
//  Global resource dependence : none
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIKUR_DelayPlayKur(
                    void
                    )
{
    if (s_kur_play_info.switch_delay_timer != 0)
    {
        MMK_StopTimer(s_kur_play_info.switch_delay_timer);
        s_kur_play_info.switch_delay_timer = 0;
    }
    
    if(MMIKUR_PLAYER_SWITCH_PAUSED == MMIKUR_GetPlayerState())
    {
        //SCI_TRACE_LOW:"[MMIKURO] MMIKUR_DelayPlayKur encounter MMIKUR_PLAYER_SWITCH_PAUSED"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIKUR_1168_112_2_18_2_31_33_8,(uint8*)"");
    }
    else
    {
        MMIKUR_SetPlayerState(MMIKUR_PLAYER_MUSIC_SWITCH);
        if(MMIAPIBT_GetActiveBTHeadset())
        {
            s_kur_play_info.switch_delay_timer = MMK_CreateTimerCallback(MMI_1SECONDS, MMIAPIKUR_HandleDelayPlayTimer, NULL, FALSE);
        }
        else
        {
            s_kur_play_info.switch_delay_timer = MMK_CreateTimerCallback(500, MMIAPIKUR_HandleDelayPlayTimer, NULL, FALSE);
        }
    }

    //SCI_TRACE_LOW:"[MMIKURO] MMIKUR_DelayPlayKur Start Timer = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIKUR_1183_112_2_18_2_31_33_9,(uint8*)"d",s_kur_play_info.switch_delay_timer);
}

/*****************************************************************************/
//  Description : Get the delay timer, for playing kur in a delayed manner.
//  Global resource dependence : none
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC uint8 MMIAPIKUR_GetDelayPlayTimer(
                    void
                    )
{
    return s_kur_play_info.switch_delay_timer;
}

/*****************************************************************************/
//  Description : Stop the delay timer, for playing kur in a delayed manner.
//  Global resource dependence : none
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIKUR_StopDelayPlayTimer(
                    void
                    )
{
    //SCI_TRACE_LOW:"[MMIKUR] MMIKUR_StopDelayPlayTimer Stop Timer"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIKUR_1205_112_2_18_2_31_33_10,(uint8*)"");
    if (s_kur_play_info.switch_delay_timer != 0)
    {
        MMK_StopTimer(s_kur_play_info.switch_delay_timer);
        s_kur_play_info.switch_delay_timer = 0;
    }
}

/*****************************************************************************/
//  Description : Play kur in a delayed manner, handle delay timer,begin to play.
//  Global resource dependence : none
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIKUR_HandleDelayPlayTimer(
                    uint8 timer_id,
                    uint32 param
                    )
{
    BOOLEAN result = FALSE;
    
    if (s_kur_play_info.switch_delay_timer != 0)
    {
        MMK_StopTimer(s_kur_play_info.switch_delay_timer);
        s_kur_play_info.switch_delay_timer = 0;
    }

    if(MMIKUR_PLAYER_SWITCH_PAUSED == MMIKUR_GetPlayerState())
    {
        //SCI_TRACE_LOW:"[MMIKUR] MMIAPIKUR_HandleDelayPlayTimer encounter MMIKUR_PLAYER_SWITCH_PAUSED"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIKUR_1232_112_2_18_2_31_33_11,(uint8*)"");
    }
    else
    {
        result = MMIAPIKUR_PlayKurPlayer();
    }

    //SCI_TRACE_LOW:"[MMIKUR] MMIAPIKUR_HandleDelayPlayTimer result = %d!!!"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIKUR_1239_112_2_18_2_31_33_12,(uint8*)"d",result);
    return result;
}

/*****************************************************************************/
//  Description : Start play kuro player
//  Global resource dependence : none
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIKUR_PlayKurPlayer(
                    void
                    )
{
    BOOLEAN result = FALSE;

    MMIAPIKUR_StopKurPlayer();
    MMIKUR_SetPlayerState(MMIKUR_PLAYER_MUSIC_SWITCH);
    
    if(MMIAPIBT_GetActiveBTHeadset())
    {
        result = MMIAPIBT_A2dpOpenDevice(MMIBT_A2DP_KURO_PLAYER);
    }
    else
    {
        result = MMIKUR_PlayKur();
    }
    return result;
}

/*****************************************************************************/
//  Description : Pause kuro player
//  Global resource dependence : none
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIKUR_PauseKurPlayer(
                    BOOLEAN  is_by_user
                    )
{
    BOOLEAN result = FALSE;

    if(MMIAPIKUR_IsPlayerActive())
    {
        result = MMIKUR_PauseKur(is_by_user);

        if(MMIAPIBT_GetActiveBTHeadset()  &&
           MMIKUR_PAUSED_BY_APP == s_kur_pause_type  &&  //not user included
           !is_by_user)
        {
            MMIAPIBT_A2dpPauseDevice(MMIBT_A2DP_KURO_PLAYER);
        }
    }
    return result;
}

/*****************************************************************************/
//  Description : Resume kuro player
//  Global resource dependence : none
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIKUR_ResumeKurPlayer(
                    BOOLEAN  is_by_user
                    )
{
    BOOLEAN                 result      = FALSE;
    MMIKUR_PAUSED_TYPE_E    pause_type  = MMIKUR_PAUSED_BY_NONE;

    pause_type = is_by_user?(MMIKUR_PAUSED_BY_USER):(MMIKUR_PAUSED_BY_APP);
    
    if(MMIAPIKUR_IsPlayerActive())
    {
        //SCI_TRACE_LOW:"[MMIKUR] MMIAPIKUR_ResumeKurPlayer s_kur_pause_type = %d, pause_type = %d!!!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIKUR_1306_112_2_18_2_31_33_13,(uint8*)"dd",s_kur_pause_type,pause_type);
        s_kur_pause_type &= ~((uint32)pause_type);
        
        if(MMIKUR_PAUSED_BY_NONE == s_kur_pause_type)
        {
            if(MMIAPIBT_GetActiveBTHeadset()  &&
               !is_by_user)
            {
                if(MMIKUR_PLAYER_MUSIC_SWITCH  == MMIKUR_GetPlayerState() ||
                   MMIKUR_PLAYER_SWITCH_PAUSED == MMIKUR_GetPlayerState())
                {
                    result = MMIKUR_ResumeKur();
                }
                else
                {
                    result = MMIAPIBT_A2dpResumeDevice(MMIBT_A2DP_KURO_PLAYER);
                }
            }
            else
            {
                result = MMIKUR_ResumeKur();
            }
        }
    }
    return result;
}

/*****************************************************************************/
//  Description : Stop kur player
//  Global resource dependence : none
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIKUR_StopKurPlayer(
                    void
                    )
{   
    MMIKUR_StopKur();

    if(MMIAPIBT_GetActiveBTHeadset())
    {
        MMIAPIBT_A2dpStopDevice(MMIBT_A2DP_KURO_PLAYER);
    }
}

/*****************************************************************************/
//  Description : Start play kur
//  Global resource dependence : none
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIKUR_PlayKur(
                    void
                    )
{
    BOOLEAN             result = FALSE;
    uint16              active_index = MMIKUR_GetActiveIndex();
    MMI_WIN_ID_T        alert_win_id = MMIKUR_ALERT_WIN_ID;
    FILEARRAY_DATA_T    kur_info = {0};
    FILEARRAY_DATA_T    klr_info = {0};
    uint8               ring_vol = MMIKUR_GetVolume();
    BOOLEAN             kur_exist = FALSE, klr_exist = FALSE;
    uint32              kur_size = 0, klr_size = 0;

    //SCI_TRACE_LOW:"[MMIKURO]: MMIKUR_PlayKur, cur_index = %d, total_num = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIKUR_1366_112_2_18_2_31_33_14,(uint8*)"dd",active_index, MMIKUR_GetTotalNum());

    MMIKUR_StopDelayPlayTimer();
    MMIAUDIO_AudioStop(MMIAUDIO_KURO);
    MMIKUR_SetPlayerState(MMIKUR_PLAYER_MUSIC_SWITCH);
    s_kur_pause_type = MMIKUR_PAUSED_BY_NONE;
    
    if (active_index < MMIKUR_GetTotalNum())
    {
        MMIKUR_GetCurKurInfo(&kur_info);
        MMIKUR_GetCurKlrInfo(&klr_info);
        kur_exist = MMIAPIFMM_IsFileExist(kur_info.filename, kur_info.name_len);
        klr_exist = MMIAPIFMM_IsFileExist(klr_info.filename, klr_info.name_len);
        if(kur_exist)
        {
            MMIAPIFMM_GetFileInfo(kur_info.filename, kur_info.name_len, &kur_size, PNULL, PNULL);
            if(0 != kur_size)
            {
                s_kur_play_info.file_info.kuro_header_size = MMIKUR_GetKurHeaderSize(kur_info.filename);
            }
        }
        if(klr_exist)
        {
            MMIAPIFMM_GetFileInfo(klr_info.filename, klr_info.name_len, &klr_size, PNULL, PNULL);
        }
        
        //SCI_TRACE_LOW:"[MMIKURO]: MMIKUR_PlayKur, kur_exist = %d, klr_exist = %d, kur_size = %d, klr_size = %d, Kur Head Size = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIKUR_1392_112_2_18_2_31_33_15,(uint8*)"ddddd",kur_exist, klr_exist,kur_size,klr_size,s_kur_play_info.file_info.kuro_header_size);
        
        if(!kur_exist || 
           !klr_exist ||
           0 == klr_size ||
           0 == kur_size ||
           0 == s_kur_play_info.file_info.kuro_header_size
           )
        {
            if(MMK_IsOpenWin(MMIKUR_MAIN_PLAY_WIN_ID))
            {
                if(!kur_exist || 
                   !klr_exist)
                {
                    MMIPUB_OpenAlertWinByTextId(PNULL,TXT_KUR_NO_MP3,TXT_NULL,
                        IMAGE_PUBWIN_WARNING,&alert_win_id,PNULL,MMIPUB_SOFTKEY_ONE,MMIKUR_HandleKurError);
                }
                else if(0 == klr_size || 0 == kur_size)
                {
                    MMIPUB_OpenAlertWinByTextId(PNULL,TXT_EMPTY_FILE,TXT_NULL,
                        IMAGE_PUBWIN_WARNING,&alert_win_id,PNULL,MMIPUB_SOFTKEY_ONE,MMIKUR_HandleKurError);
                }
                else if(0 == s_kur_play_info.file_info.kuro_header_size)
                {
                    MMIPUB_OpenAlertWinByTextId(PNULL,TXT_KUR_FILE_ERROR,TXT_NULL,
                        IMAGE_PUBWIN_WARNING,&alert_win_id,PNULL,MMIPUB_SOFTKEY_ONE,MMIKUR_HandleKurError);
                }
            }
            else
            {
                MMIKUR_CallBackAfterPlay(MMIAUDIO_RLT_SUCCESS, NULL);
            }
            MAIN_SetIdleBgPlayName(FALSE, MMIBGPLAY_TYPE_KURO);
            MMISUB_SetSubLcdDisplay(FALSE,FALSE,SUB_CONTENT_MP3,PNULL);
        }
        else
        {       
            result = MMIKUR_PlayKurFile(kur_info.filename,kur_info.name_len);
        }
    }
    else
    {
        MMIPUB_OpenAlertWinByTextId(PNULL,TXT_KUR_NO_MP3,TXT_NULL,
            IMAGE_PUBWIN_WARNING,&alert_win_id,PNULL,MMIPUB_SOFTKEY_ONE, PNULL);
        MMIAPIKUR_StopKurPlayer();
    }

    //SCI_TRACE_LOW:"[MMIKUR]:MMIKUR_PlayKur, result = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIKUR_1439_112_2_18_2_31_33_16,(uint8*)"d",result);
    return (result);
}

/*****************************************************************************/
//  Description : Pause the KURO BG player
//  Global resource dependence : 
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIKUR_PauseKur(
                    BOOLEAN  is_by_user
                    )
{
    BOOLEAN                 result      = FALSE;
    MMIKUR_PAUSED_TYPE_E    pause_type  = MMIKUR_PAUSED_BY_NONE;

    pause_type = is_by_user?(MMIKUR_PAUSED_BY_USER):(MMIKUR_PAUSED_BY_APP);
    
    //SCI_TRACE_LOW:"[MMIKUR] MMIKUR_PauseKur player_state = %d, audio_state = %d, s_kur_pause_type = %d, pause_type = %d!!!"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIKUR_1457_112_2_18_2_31_33_17,(uint8*)"dddd",MMIKUR_GetPlayerState(), MMIKUR_GetAudioState(),s_kur_pause_type,pause_type);

    s_kur_pause_type |= (uint32)pause_type;

    switch(MMIKUR_GetPlayerState())
    {
        case MMIKUR_PLAYER_STATE_NONE:
            break;
        case MMIKUR_PLAYER_AUDIO_ACTIVE:
            {
                switch(MMIKUR_GetAudioState())
                {
                    case MMIKUR_AUDIO_OPENED:
                        break;
                        
                    case MMIKUR_AUDIO_PLAYING:
                        if (MMIAUDIO_KURO== MMIAUDIO_GetAudioHtype())
                        {
                            {//cmz's
                                AUDIO_PLAYINFO_T playinfo;
                                uint32           nBufferLeft = 0;
                                MMIAUDIO_AudioGetPlayingFileInfo(MMIAUDIO_KURO,&playinfo);

                                s_kur_play_info.file_info.kuro_pause_pos = playinfo.uiCurrentDataOffset;
                                #ifndef _WIN32//cmzs200712
                                nBufferLeft = AUDIO_GetDecoderDataLen(MMIAUDIO_GetAudioHandle(MMIAUDIO_KURO));
                                s_kur_play_info.file_info.kuro_played_time += nBufferLeft / (1024 * 16 * 8) + 300;
                                s_kur_play_info.file_info.kuro_last_play_time = 0;
                                #endif

                                //SCI_TRACE_LOW:"[MMIKUR]:MMIKUR_PauseKur :kuro_pause_pos=%d"
                                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIKUR_1487_112_2_18_2_31_34_18,(uint8*)"d",s_kur_play_info.file_info.kuro_pause_pos);
                            }
                            if(MMIAUDIO_AudioPause(MMIAUDIO_KURO))
                            {
                                MMIKUR_SetAudioState(MMIKUR_AUDIO_PAUSED);
                                MMIKUR_StopAnimTimer();
                                MAIN_SetIdleBgPlayName(TRUE, MMIBGPLAY_TYPE_KURO);
                                MMISUB_SetSubLcdDisplay(FALSE,TRUE,SUB_CONTENT_MP3,PNULL);
                                result = TRUE;
                            }
                            else
                            {
                                //SCI_TRACE_MID:"[MMIKUR]:MMIKUR_PauseKur:AUDIO FAILED!"
                                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIKUR_1499_112_2_18_2_31_34_19,(uint8*)"");
                                break;
                            }
                        }
                        break;
                        
                    case MMIKUR_AUDIO_PAUSED:
                        break;
                        
                    case MMIKUR_AUDIO_STOPPED:
                        break;
                        
                    case MMIKUR_AUDIO_CLOSED:
                        break;
                    
                    default:
                        break;
                }
            }
            break;

        case MMIKUR_PLAYER_MUSIC_SWITCH:
        case MMIKUR_PLAYER_SWITCH_PAUSED:
            MMIKUR_StopDelayPlayTimer();
            MMIKUR_SetPlayerState(MMIKUR_PLAYER_SWITCH_PAUSED);
            MAIN_SetIdleBgPlayName(TRUE, MMIBGPLAY_TYPE_KURO);
            MMISUB_SetSubLcdDisplay(FALSE,TRUE,SUB_CONTENT_MP3,PNULL);
            result = TRUE;
            break;
        default:
            break;
    }
    MMIAPISET_ClearCurRingType(MMISET_RING_TYPE_KURO_PLAYER);
    if (MMK_IsFocusWin(MMIKUR_MAIN_PLAY_WIN_ID))
    {
        if(MMIKUR_Getbdisptoolbar())
        {
            MMIKLR_DisplayToolBar(TRUE);
        }
        MMITHEME_UpdateRect();
    }
    return result;
}

/*****************************************************************************/
//  Description : Resume KURO BG player.
//  Global resource dependence : 
//  Author: xingdong.li
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIKUR_ResumeKur(
                    void
                    )
{
    BOOLEAN             result = FALSE;
    MMI_WIN_ID_T        alert_win_id = MMIKUR_ALERT_WIN_ID;
    MMIAUDIO_EQ_MODE_E  eq_mode = MMIAUDIO_EQ_REGULAR;
    BOOLEAN             is_active_a2sp = MMIAPIBT_GetActiveBTHeadset();
    
    //SCI_TRACE_LOW:"[MMIKUR] MMIKUR_ResumeKur player_state = %d, audio_state = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIKUR_1557_112_2_18_2_31_34_20,(uint8*)"dd", MMIKUR_GetPlayerState(), MMIKUR_GetAudioState());
    
    switch(MMIKUR_GetPlayerState())
    {
        case MMIKUR_PLAYER_STATE_NONE:
            break;
        case MMIKUR_PLAYER_AUDIO_ACTIVE:
            {
                switch(MMIKUR_GetAudioState())
                {
                    case MMIKUR_AUDIO_OPENED:
                        break;
                        
                    case MMIKUR_AUDIO_PLAYING:
                        break;
                        
                    case MMIKUR_AUDIO_PAUSED:
                        if (!MMIAUDIO_IsBgPlayPaused(MMIBGPLAY_TYPE_KURO) &&
                            MMIAPISET_IsPermitPlayMp3OrRecord(MMISET_RING_TYPE_KURO_PLAYER))
                        {
                            MMIAPISET_InitAudioDevice(MMIKUR_GetVolume());
                            if(MMIAPIBT_GetActiveBTHeadset())
                            {
                                MMIAUDIO_SetVolume(AUDIO_DEVICE_MODE_EARPHONE, MMIKUR_GetVolume());
                            }
                            eq_mode = is_active_a2sp?MMIAUDIO_EQ_REGULAR:s_kur_play_info.eq_mode;
                            MMIKUR_SetEqMode(eq_mode);

                            s_kur_play_info.file_info.kuro_last_play_time = SCI_GetTickCount();
                            
                            if(MMIAUDIO_AudioResume(MMIAUDIO_KURO))
                            {
                                MMIKUR_SetAudioState(MMIKUR_AUDIO_PLAYING);

                                MMIKUR_StartAnimTimer();

                                MAIN_SetIdleBgPlayName(TRUE, MMIBGPLAY_TYPE_KURO);
                                MMISUB_SetSubLcdDisplay(FALSE,TRUE,SUB_CONTENT_MP3,PNULL);
                                
                                result = TRUE;
                            }
                            else
                            {
                                //SCI_TRACE_MID:"[MMIKUR]:MMIKUR_ResumeKur:AUDIO FAILED!"
                                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIKUR_1600_112_2_18_2_31_34_21,(uint8*)"");
                                MMIPUB_OpenAlertWinByTextId(PNULL,TXT_KUR_NO_SUPPORT,TXT_NULL,
                                    IMAGE_PUBWIN_WARNING,&alert_win_id,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);

                                MMIKUR_SetPlayerState(MMIKUR_PLAYER_STATE_NONE);
                                MAIN_SetIdleBgPlayName(FALSE, MMIBGPLAY_TYPE_KURO);
                                MMISUB_SetSubLcdDisplay(FALSE,FALSE,SUB_CONTENT_MP3,PNULL);
                                break;
                            }
                        }
                        break;
                        
                    case MMIKUR_AUDIO_STOPPED:
                        break;
                        
                    case MMIKUR_AUDIO_CLOSED:
                        break;
                    
                    default:
                        break;
                }
            }
            break;

        case MMIKUR_PLAYER_MUSIC_SWITCH:
        case MMIKUR_PLAYER_SWITCH_PAUSED:
            MMIKUR_DelayPlayKur();
            result = TRUE;
            break;
            
        default:
            break;
    }
    if (MMK_IsFocusWin(MMIKUR_MAIN_PLAY_WIN_ID))
    {
        if(MMIKUR_Getbdisptoolbar())
        {
            MMIKLR_DisplayToolBar(TRUE);
        }
        MMITHEME_UpdateRect();
    }
    return result;
}

/*****************************************************************************/
//  Description : Stop the Kuro BG player.
//  Global resource dependence : 
//  Author: xingdong.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIKUR_StopKur(
                    void
                    )
{
    //SCI_TRACE_LOW:"[MMIKUR] MMIKUR_StopKur player_state = %d, audio_state = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIKUR_1653_112_2_18_2_31_34_22,(uint8*)"dd", MMIKUR_GetPlayerState(), MMIKUR_GetAudioState());
    
    switch(MMIKUR_GetPlayerState())
    {
        case MMIKUR_PLAYER_STATE_NONE:
            break;
        case MMIKUR_PLAYER_AUDIO_ACTIVE:
        case MMIKUR_PLAYER_MUSIC_SWITCH:
        case MMIKUR_PLAYER_SWITCH_PAUSED:
            MMIAPISET_ClearCurRingType(MMISET_RING_TYPE_KURO_PLAYER);
            MMIAUDIO_SetHalfSamplerate(FALSE);

            if(SFS_INVALID_HANDLE != s_kur_play_info.file_info.kuro_file_handle)
            {
                SFS_CloseFile(s_kur_play_info.file_info.kuro_file_handle);
                s_kur_play_info.file_info.kuro_file_handle = PNULL;
            }
            s_kur_play_info.file_info.kuro_last_play_time = 0;
            s_kur_play_info.file_info.kuro_played_time    = 0;
            s_kur_play_info.file_info.kuro_pause_pos      = 0;
    
            MMIAUDIO_AudioStop(MMIAUDIO_KURO);
            MMIKUR_SetAudioState(MMIKUR_AUDIO_STOPPED);
            MMIKUR_SetPlayerState(MMIKUR_PLAYER_STATE_NONE);
            s_kur_pause_type = MMIKUR_PAUSED_BY_NONE;

            MMIKUR_StopAnimTimer();
            MMIKUR_StopLyric();
                        
            MMIKUR_StopDelayPlayTimer();
            MAIN_SetIdleBgPlayName(FALSE, MMIBGPLAY_TYPE_KURO);
            MMISUB_SetSubLcdDisplay(FALSE,FALSE,SUB_CONTENT_MP3,PNULL);

            if (MMK_IsFocusWin(MMIKUR_MAIN_PLAY_WIN_ID))
            {
                MMIKUR_DisplayKurPlayWin(TRUE,MMIKUR_MAIN_PLAY_WIN_ID);
                MMITHEME_UpdateRect();
            }
            #ifndef WIN32
            CHNG_FREQ_RestoreARMClk(MMI_GetFreqHandler());
            #endif
            
            break;
        default:
            break;
    }
}

/*****************************************************************************/
//  Description : play music from my document
//  Global resource dependence : 
//  Author: xingdongli
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIKUR_PlayMyDocumentMusic(
                    MMIFILE_FILE_INFO_T   *music_file_info_ptr
                    )
{
    FILEARRAY_DATA_T    kur_info = {0};
    FILEARRAY_DATA_T    klr_info = {0};
    MMI_WIN_ID_T        alert_win_id = MMIKUR_ALERT_WIN_ID;

    //SCI_TRACE_LOW:"[MMIKURO]: MMIAPIKUR_PlayMyDocumentMusic"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIKUR_1713_112_2_18_2_31_34_23,(uint8*)"");
	//arvinzhang modify:delete assert
	//SCI_ASSERT(PNULL != music_file_info_ptr);
	if(PNULL == music_file_info_ptr)
	{
		return;
	}
    
    if (0 < music_file_info_ptr->file_size)
    {
        MMIAUDIO_PauseOtherBgPlay(MMIBGPLAY_TYPE_KURO);
        MMIAPIKUR_StopKurPlayer();
        MMIKUR_SetPlayerState(MMIKUR_PLAYER_MUSIC_SWITCH);

        MMIKUR_OnPowerOn();
        MMIKUR_InitKuroListByFileInfo(music_file_info_ptr); 
        MMIKUR_LoadKuroByIndex(0);
        MMIKUR_GetCurKurInfo(&kur_info);
        MMIKUR_GetCurKlrInfo(&klr_info);

        if(MMIAPIFMM_IsFileExist(kur_info.filename, kur_info.name_len) &&
           MMIAPIFMM_IsFileExist(klr_info.filename, klr_info.name_len))
        {
            s_kur_play_info.file_info.kuro_header_size = MMIKUR_GetKurHeaderSize(kur_info.filename);

            if(!MMK_IsOpenWin(MMIKUR_MAIN_PLAY_WIN_ID))
            {
#ifdef UI_P3D_SUPPORT
                MMI_Disable3DEffect(MMI_3D_EFFECT_KUR_PLAYER);
#endif             
                MMK_CreateWin((uint32 *)MMIKUR_MAIN_PLAY_WIN_TAB, PNULL);
            }
            MMK_PostMsg(MMIKUR_MAIN_PLAY_WIN_ID,MSG_KUR_PLAY_START,PNULL,0);
        }
        else
        {
            MMIPUB_OpenAlertWinByTextId(PNULL,TXT_KUR_NO_MP3,TXT_NULL,
               IMAGE_PUBWIN_WARNING,&alert_win_id,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
        }
    }
    else
    {
        //SCI_TRACE_LOW:"MMIAPIKUR_PlayMyDocumentMusic:memory = %d,name_len = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIKUR_1750_112_2_18_2_31_34_24,(uint8*)"dd",music_file_info_ptr->file_size,music_file_info_ptr->file_name_len);
        MMIPUB_OpenAlertWinByTextId(PNULL,TXT_EMPTY_FILE,TXT_NULL,
            IMAGE_PUBWIN_WARNING,&alert_win_id,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
    }
}

/*****************************************************************************/
//  Description : KURO 正常播放结束的回调函数。
//                根据播放模式重新显示进度条和button和是否继续播放其他 KURO 等
//  Global resource dependence : 
//  Author: xingdong.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIKUR_PlayKurCallBack(void)
{
    uint16          next_index = 0;
    MMI_WIN_ID_T    win_id = MMIKUR_MAIN_PLAY_WIN_ID;
    
    //SCI_TRACE_LOW:"[MMIKURO]: MMIAPIKUR_PlayKurCallBack:cur_play_mode = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIKUR_1766_112_2_18_2_31_34_25,(uint8*)"d",MMIKUR_GetPlayMode());
    //reset play info
    MMIKUR_SetAudioState(MMIKUR_AUDIO_STOPPED);
    MMIKUR_StopAnimTimer();

    //display KUR main win
    if (MMK_IsFocusWin(win_id))
    {
        MMIKUR_DisplayKurPlayWin(TRUE,MMIKUR_MAIN_PLAY_WIN_ID);
    }

    switch (MMIKUR_GetPlayMode())
    {
    case MMIKUR_PLAY_ONE_ONCE:
        MMIAPIKUR_StopKurPlayer();
        break;

    case MMIKUR_PLAY_ONE_REPEAT:
        MMIAPIKUR_PlayKurPlayer();
        break;

    case MMIKUR_PLAY_LIST_ONCE:
    case MMIKUR_PLAY_LIST_REPEAT:
    case MMIKUR_PLAY_RANDOM_REPEAT:
    case MMIKUR_PLAY_RANDOM_ONCE:
        //get index of next kur
        if (GetNextKurIndex(
                s_kur_play_info.start_index,
                s_kur_play_info.active_index,
                &next_index,
                s_kur_play_info.list_info.total_num,
                s_kur_play_info.cur_play_mode,
                &s_kur_play_info.list_info.random_info
                ))
        {
            if(MMIKUR_LoadKuroByIndex(next_index))
			{
				if(!MMIAUDIO_IsBgPlayPaused(MMIBGPLAY_TYPE_KURO) &&
					MMIKUR_PLAYER_MUSIC_SWITCH == MMIKUR_GetPlayerState())
				{
					MMIKUR_DelayPlayKur();
				}
			}
        }
        else
        {
            if(!MMIKUR_IsBackgroundReady())
            {
                MMK_CloseWin(MMIKUR_MAIN_PLAY_WIN_ID);
            }
            MMIAPIKUR_StopKurPlayer();
        }
        break;

    default:
//        SCI_PASSERT(FALSE,("[MMIKURO]: MMIAPIKUR_PlayKurCallBack:cur_play_mode = %d",MMIKUR_GetPlayMode()));
        break;
    }

    if (!MMK_IsOpenWin(MMIKUR_MAIN_PLAY_WIN_ID))
    {
        if (MMIKUR_PLAYER_STATE_NONE == MMIKUR_GetPlayerState())
        {
            MMIAUDIO_ResumeOtherBgPlay(MMIBGPLAY_TYPE_KURO);
        }
    }
}

/*****************************************************************************/
//  Description : Callback function to MMI AUDIO
//  Global resource dependence : none
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIKUR_CallBackAfterPlay(
                MMIAUDIO_RESULT  result,
                DPARAM           param
                )
{
    //SCI_TRACE_LOW:"[MMIKURO]: MMIKUR_CallBackAfterPlay, MMIAUDIO_RESULT = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIKUR_1843_112_2_18_2_31_34_26,(uint8*)"d",result);

    if(MMK_IsOpenWin(MMIKUR_MAIN_PLAY_WIN_ID))
    {
        MMIAPISET_AlertRingPlayResult(result,param);
    }

    if(SFS_INVALID_HANDLE != s_kur_play_info.file_info.kuro_file_handle)
    {
        SFS_CloseFile(s_kur_play_info.file_info.kuro_file_handle);
        s_kur_play_info.file_info.kuro_file_handle = PNULL;
    }
    s_kur_play_info.file_info.kuro_last_play_time = 0;
    s_kur_play_info.file_info.kuro_played_time    = 0;
    s_kur_play_info.file_info.kuro_pause_pos      = 0;
    
    MMIKUR_StopAnimTimer();    
    MMIKUR_StopLyric();

    if(MMIKUR_PLAYER_SWITCH_PAUSED != MMIKUR_GetPlayerState())
    {
        MMIKUR_SetPlayerState(MMIKUR_PLAYER_MUSIC_SWITCH);
    }

    MMK_PostMsg(VIRTUAL_WIN_ID,MSG_KUR_PLAY_CALL_BACK,PNULL,0);

#ifndef WIN32
    CHNG_FREQ_RestoreARMClk(MMI_GetFreqHandler());
#endif
}

/*****************************************************************************/
//  Description : play kur file
//  Global resource dependence : none
//  Author: cmzs
//  Note:only for kur module
/*****************************************************************************/
LOCAL BOOLEAN MMIKUR_PlayKurFile(
                    uint16  *kur_full_path,
                    uint16  kur_full_path_len
                    )
{
    BOOLEAN             result = FALSE;
    MMIAUDIO_HTYPE      audio_htype = MMIAUDIO_KURO;
    MMI_WIN_ID_T        alert_win_id = MMIKUR_ALERT_WIN_ID;
    MMIAUDIO_EQ_MODE_E  eq_mode = MMIAUDIO_EQ_REGULAR;
    BOOLEAN             is_active_a2sp = MMIAPIBT_GetActiveBTHeadset();
    BOOLEAN             is_lyric_ok = TRUE;
    
    //SCI_TRACE_LOW:"[MMIKURO]: MMIKUR_PlayKurFile, is_active_a2sp = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIKUR_1890_112_2_18_2_31_34_27,(uint8*)"d",is_active_a2sp);

    if (MMIAPISET_IsPermitPlayMp3OrRecord(MMISET_RING_TYPE_KURO_PLAYER) &&
        !MMIAUDIO_IsBgPlayPaused(MMIBGPLAY_TYPE_KURO))
    {
#ifndef WIN32
        CHNG_FREQ_SetArmClk(MMI_GetFreqHandler(),FREQ_INDEX_APP_HIGH);    
#endif

        if(MMIKUR_CreateAudioHandle(is_active_a2sp,(uint16 *)kur_full_path))
        {
            MMIAPIKUR_RecordkurSampleRate(MMIKUR_GetKurSampleRate());
            MMIAPISET_InitAudioDevice(MMIKUR_GetVolume());
            if(MMIAPIBT_GetActiveBTHeadset())
            {
                MMIAUDIO_SetVolume(AUDIO_DEVICE_MODE_EARPHONE, MMIKUR_GetVolume());
            }
            eq_mode = is_active_a2sp?MMIAUDIO_EQ_REGULAR:s_kur_play_info.eq_mode;
            MMIKUR_SetEqMode(eq_mode);
            MMIAUDIO_SetHalfSamplerate(is_active_a2sp);
            
            MMIKUR_SetPlayerState(MMIKUR_PLAYER_AUDIO_ACTIVE);
            MMIKUR_SetAudioState(MMIKUR_AUDIO_PLAYING);
            if(MMK_IsOpenWin(MMIKUR_MAIN_PLAY_WIN_ID))
            {
                is_lyric_ok = MMIKUR_PlayKlr();
                MMIKUR_DisplayKlr(MMIKUR_MAIN_PLAY_WIN_ID, TRUE, TRUE, MMIKUR_GetKlrMode());
                MMITHEME_UpdateRect();
            }

            if(is_lyric_ok)
            {
                if(MMIAUDIO_AudioPlay(audio_htype,MMIKUR_ZERO_OFFSET,1,MMIKUR_CallBackAfterPlay))
                {
                    result = TRUE;
                    if(MMK_IsOpenWin(MMIKUR_MAIN_PLAY_WIN_ID))
                    {
                        MMIKUR_StartAnimTimer();
                    }
                }
            }
        }
#ifndef _WIN32
        if(!result)
        {
            MMIAUDIO_CloseAudioHandle(audio_htype);

            MMIKUR_SetPlayerState(MMIKUR_PLAYER_MUSIC_SWITCH);
            MMIKUR_SetAudioState(MMIKUR_AUDIO_STOPPED);

            if(MMK_IsOpenWin(MMIKUR_MAIN_PLAY_WIN_ID))
            {
                MMIKUR_StopLyric();
                MMIPUB_OpenAlertWinByTextId(PNULL,TXT_COMMON_PLAY_ERROR,TXT_NULL,
                    IMAGE_PUBWIN_WARNING,&alert_win_id,PNULL,MMIPUB_SOFTKEY_ONE,MMIKUR_HandleKurError);
            }
            else
            {
                //MMIKUR_CallBackAfterPlay(MMIAUDIO_RLT_SUCCESS, NULL);
            }
        }
#endif
    }
    MAIN_SetIdleBgPlayName(result, MMIBGPLAY_TYPE_KURO);
    MMISUB_SetSubLcdDisplay(FALSE,result,SUB_CONTENT_MP3,PNULL);
    return result;
}

/*****************************************************************************/
//  Description : 根据当前播放信息播放Klr
//  Global resource dependence : none
//  Author: cmzs
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIKUR_PlayKlr()
{
    FILEARRAY_DATA_T klr_info = {0};
    BOOLEAN          get_result = FALSE, open_result = FALSE, read_result = FALSE;

    MMIKUR_InitLyric();
    MMIKUR_AllocKlrBuffer();
    MMIKUR_ReSetPicReadInfo();
    if(MMIKUR_GetCurKlrInfo(&klr_info))
    {
        get_result = TRUE;
        //if(MMIKLR_OpenLyric(klr_info.filename))
        open_result = MMIKLR_OpenLyric(klr_info.filename);
        {
            //open_result = TRUE;
            if(MMIKUR_ReadFirstPic())
            {
                read_result = TRUE;
            }
        }
    }
    //SCI_TRACE_LOW:"[MMIKURO]: MMIKUR_PlayKlr get_result = %d, open_result = %d,read_result = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIKUR_1983_112_2_18_2_31_34_28,(uint8*)"ddd",get_result,open_result,read_result);
    return read_result;
}

/*****************************************************************************/
//  Description : 获得即将播放的下一首kur的索引.
//  Global resource dependence : 
//  Author:cmzs
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN GetNextKurIndex(//下一首歌曲的索引是否发生变化
                    uint16                start_index,    //in:起始播放歌曲索引
                    uint16                active_index,   //in:正在播放歌曲索引
                    uint16                *next_index_ptr,//out:即将播放歌曲索引
                    uint16                total_num,      //in:歌曲总数
                    MMIKUR_PLAY_MODE_E    play_mode,      //in:播放歌曲模式
                    MMIKUR_RANDOM_INFO_T  *random_info_ptr//in/out:随机播放一次不同随机数信息
                    )
{
    uint16      next_index = 0;
    BOOLEAN     result = FALSE;

	//arvinzhang modify:delete assert
    //SCI_ASSERT(PNULL != next_index_ptr);
    //SCI_ASSERT(PNULL != random_info_ptr);
    if(PNULL == next_index_ptr || PNULL == random_info_ptr)
	{
		return result;
	}
    
    switch (play_mode)
    {
    case MMIKUR_PLAY_LIST_ONCE:
        //得到即将播放歌曲的索引
        if ((active_index+1) > (total_num-1))
        {
            next_index = 0;
        }
        else
        {
            next_index = active_index + 1;
        }

        //列表循环播放一次，判断是否已经播放过
        if (next_index != start_index)
        {
            *next_index_ptr = next_index;
            result = TRUE;
        }
        break;

    case MMIKUR_PLAY_LIST_REPEAT:
        //得到即将播放歌曲的索引
        if ((active_index+1) > (total_num-1))
        {
            next_index = 0;
        }
        else
        {
            next_index = active_index + 1;
        }

        *next_index_ptr = next_index;
        result = TRUE;
        break;

    case MMIKUR_PLAY_RANDOM_REPEAT:
        //避免kur文件少时，重复播放一首歌曲
        if (random_info_ptr->active_index < (total_num-1))
        {
            random_info_ptr->active_index++;
        }
        else
        {
            random_info_ptr->active_index = 0;
        }
        *next_index_ptr = random_info_ptr->random_index_ptr[random_info_ptr->active_index];
        result = TRUE;
        break;

    case MMIKUR_PLAY_RANDOM_ONCE:
        if (random_info_ptr->active_index < (total_num-1))
        {
            random_info_ptr->active_index++;
        }
        else
        {
            random_info_ptr->active_index = 0;
        }
        next_index = random_info_ptr->random_index_ptr[random_info_ptr->active_index];

        //列表循环播放一次，判断是否已经播放过
        if (next_index != start_index)
        {
            *next_index_ptr = next_index;
            result = TRUE;
        }
        break;

    default:
        //SCI_TRACE_LOW:"[MMIKURO]GetNextKurIndex:Don't call this function!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIKUR_2076_112_2_18_2_31_35_29,(uint8*)"");
        break;
    }

    return (result);
}

/*****************************************************************************/
//  Description : 跳转到前一首或者下一首kur。
//              根据播放模式重新显示进度条和button和是否继续播放其他kur等
//  Global resource dependence : 
//  Author:cmzs
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIKUR_GoToPreOrNextKur(
                                       BOOLEAN      is_prev
                                       )
{
    uint16                  next_index = 0;
    uint16                  active_index = MMIKUR_GetActiveIndex();
    uint16                  total_num    = MMIKUR_GetTotalNum();
    MMI_WIN_ID_T            win_id = MMIKUR_MAIN_PLAY_WIN_ID;
    MMIKUR_RANDOM_INFO_T    *random_info_ptr = &s_kur_play_info.list_info.random_info;

    //SCI_TRACE_LOW:"[MMIKURO]: MMIAPIKUR_GoToPreOrNextKur is_prev = %d,cur_play_mode = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIKUR_2098_112_2_18_2_31_35_30,(uint8*)"dd",is_prev,MMIKUR_GetPlayMode());
    //stop anim timer
    MMIKUR_StopAnimTimer();
    MMIAPIKUR_StopKurPlayer();
    
    //跳转到前一首或者下一首KUR
    switch (MMIKUR_GetPlayMode())
    {
    case MMIKUR_PLAY_ONE_ONCE:
    case MMIKUR_PLAY_ONE_REPEAT:
    case MMIKUR_PLAY_LIST_ONCE:
    case MMIKUR_PLAY_LIST_REPEAT:
        if (is_prev)//前一首
        {
            if (active_index > 0)
            {
                next_index = active_index - 1;
            }
            else
            {
                next_index = total_num - 1;
            }
        }
        else//下一首
        {
            if (active_index < (total_num-1))
            {
                next_index = active_index + 1;
            }
            else
            {
                next_index = 0;
            }
        }
        break;

    case MMIKUR_PLAY_RANDOM_REPEAT:
    case MMIKUR_PLAY_RANDOM_ONCE:
        if (is_prev)//前一首
        {
            if (random_info_ptr->active_index > 0)
            {
                random_info_ptr->active_index--;
            }
            else
            {
                random_info_ptr->active_index = total_num -1;
            }
        }
        else
        {
            if (random_info_ptr->active_index < (total_num-1))
            {
                random_info_ptr->active_index++;
            }
            else
            {
                random_info_ptr->active_index = 0;
            }
        }
        next_index = random_info_ptr->random_index_ptr[random_info_ptr->active_index];
        break;

    default:
        break;
    }

    MMIKUR_SetStartIndex(next_index);
    MMIKUR_LoadKuroByIndex(next_index);
    
    if(MMIAPIBT_GetActiveBTHeadset())
    {
        MMIKUR_DelayPlayKur();
    }
    else
    {
        MMIAPIKUR_PlayKurPlayer();
    }
    
    if (MMK_IsFocusWin(win_id))
    {
        MMIKUR_DisplayKurPlayWin(TRUE,MMIKUR_MAIN_PLAY_WIN_ID);
    }
}

/*****************************************************************************/
//  Description : 在播放kur的情况下提示文件格式不支持，kur错误，则根据播放模式继续播放
//  Global resource dependence : 
//  Author: cmzs
//  Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIKUR_HandleKurError(
                        MMI_WIN_ID_T      win_id,
                        MMI_MESSAGE_ID_E    msg_id,
                        DPARAM          param
                        )
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    //SCI_TRACE_LOW:"[MMIKURO]: MMIKUR_HandleKurError, msg_id = 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIKUR_2194_112_2_18_2_31_35_31,(uint8*)"d",msg_id);
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        MMIAPIKUR_StopKurPlayer();
        MMK_CloseWin(MMIKUR_MAIN_PLAY_WIN_ID);
        //if (!MMK_IsOpenWin(MMIKUR_MAIN_PLAY_WIN_ID))
        {
            if (MMIKUR_PLAYER_STATE_NONE == MMIKUR_GetPlayerState())
            {
                MMIAUDIO_ResumeOtherBgPlay(MMIBGPLAY_TYPE_KURO);
            }
        }
        result = MMIPUB_HandleAlertWinMsg(win_id,msg_id,param);
        break;

    default:
        result = MMIPUB_HandleAlertWinMsg(win_id,msg_id,param);
        break;
    }
    return (result);
}

/*****************************************************************************/
//  Description : 返回系统缺省的目录(如果U Disk存在则返回U Disk,否则返回T卡)
//  Global resource dependence : none
//  Author: cmzs
//  Note: 
/******************** *********************************************************/
LOCAL char MMIKUR_GetDefaultDisk()
{
    char ret = 'D';
    
    ret = *MMIAPIFMM_GetDevicePath(MMIAPIFMM_GetFirstValidDevice());
    return ret;
}
/*****************************************************************************/
//  Description : 根据设备的具体情况初始化Sysdat
//  Global resource dependence : none
//  Author: cmzs
//  Note: 
/******************** *********************************************************/
LOCAL void MMIKUR_InitSysDat(KURO_DEV_INFO *psysdat)
{
    char diskno = 'D';
    diskno = MMIKUR_GetDefaultDisk();
    SCI_MEMSET(psysdat,0,sizeof(KURO_DEV_INFO));
    psysdat->BigEndian = 0;
    psysdat->LyricLanguage = 12;

    psysdat->LyricWidth = 28;
    
    psysdat->ScreenHeight = 320;
    psysdat->ScreenWidth = 240;

    psysdat->MusicFormat = 1;
    psysdat->PicFormat = 2;

    SCI_MEMSET(psysdat->default_disk,0,((KURO_FULL_PATH_MAX_LEN+1)<<1));
    psysdat->default_disk[0] = diskno;
    psysdat->default_disk[1] = ':';
}

/*****************************************************************************/
//  Description : Read SysDat From NvItem
//  Global resource dependence :
//  Author:CMZS
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN ReadIgrsKeyFromNV(uint8 *pigrskey)
{
    uint32                  status = NVERR_NOT_EXIST;
    
    // Read kuro device id from NV.
    status = EFS_NvitemRead(NV_KURO_DID, KURO_ID_LENGTH, (uint8 *)(pigrskey));    
    // If the initial values don't exist.
    if (NVERR_NOT_EXIST == status)
    {
#ifndef WIN32
        NVITEM_UpdateCali(NV_KURO_DID, KURO_ID_LENGTH, (uint8 *)(pigrskey));
#else
        if (!NVITEM_IsSimulate())
        {
            SCI_PASSERT(0, ("KURO DID is not exist. Please download new NVItem !")); /* assert verified */
        }
#endif  // WIN32        
        return FALSE;
    }   
    return TRUE;
}
/*****************************************************************************/
//  Description : Read SysDat From NvItem
//  Global resource dependence :
//  Author:CMZS
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN WriteIgrsKeyToNV(uint8 *pigrskey)
{
#ifndef WIN32
    NVITEM_UpdateCali(NV_KURO_DID, KURO_ID_LENGTH, (uint8 *)(pigrskey));
#endif
    return TRUE;
}

/*****************************************************************************/
//  Description : Read SysDat From NvItem
//  Global resource dependence :
//  Author:CMZS
//  Note:
/*****************************************************************************/

PUBLIC BOOLEAN ReadSysDatFromNV(uint8 *psysdat)
{
    MN_RETURN_RESULT_E      return_value = MN_RETURN_FAILURE;
    MMINV_READ(MMINV_KDB_SYS_DAT,psysdat,return_value);
    return (MN_RETURN_SUCCESS == return_value);
}

/*****************************************************************************/
//  Description : Write SysDat To NvItem
//  Global resource dependence :
//  Author:CMZS
//  Note:
/*****************************************************************************/

PUBLIC BOOLEAN WriteSysDatToNV(uint8 *psysdat)
{
    MMINV_WRITE(MMINV_KDB_SYS_DAT,psysdat);
    return TRUE;
}

/*****************************************************************************/
//  Description : call when udisk start up
//  Global resource dependence : none
//  Author: Jassmine.Meng
//  Note:only for Kur module
/*****************************************************************************/
PUBLIC void MMIAPIKUR_OnUdiskStartup()
{
    MMIKUR_ExportKgcCount();
}

/*****************************************************************************/
//  Description : call when udisk exit
//  Global resource dependence : none
//  Author: Jassmine.Meng
//  Note:only for Kur module
/*****************************************************************************/
PUBLIC void MMIAPIKUR_OnUdiskShutDown()
{
//clear kgccount
    wchar   kuo_cnt_full_path[MMIFILE_FULL_PATH_MAX_LEN+1] = {0}; 
    uint16  kuo_cnt_full_path_len = MMIFILE_FULL_PATH_MAX_LEN+1;
    uint8   kuo_cnt_full_path_gb[MMIFILE_FULL_PATH_MAX_LEN+1] = {0};
    KURO_DEV_INFO kur_sys_dat = {0};
    char diskno = 'D';

    diskno = MMIKUR_GetDefaultDisk();
    SCI_MEMSET(kuo_cnt_full_path_gb,0,(MMIFILE_FULL_PATH_MAX_LEN+1));
    kuo_cnt_full_path_gb[0] = diskno;
    kuo_cnt_full_path_gb[1] = ':';
    strcat(kuo_cnt_full_path_gb,MMIKURO_COUNTER_FILENAME);
    kuo_cnt_full_path_len = GUI_GBToWstr(kuo_cnt_full_path, (const uint8*)kuo_cnt_full_path_gb, SCI_STRLEN(kuo_cnt_full_path_gb));
    MMIKUR_InitSysDat(&kur_sys_dat);
    MMIKUR_ConfigInit(&kur_sys_dat);
    
//if file not exists then clear kgc count in nv
    if(!MMIAPIFMM_IsFileExist(kuo_cnt_full_path,kuo_cnt_full_path_len))
    {
        MMIKUR_ClearKgcCount();
    }
    else
    {
        MMIAPIFMM_DeleteFile((const uint16 *)kuo_cnt_full_path,PNULL);
    }
}
/*****************************************************************************/
//  Description : call when Power On
//  Global resource dependence : none
//  Author: Jassmine.Meng
//  Note:only for Kur module
/*****************************************************************************/
PUBLIC void MMIKUR_OnPowerOn()
{
    KURO_DEV_INFO  kur_sys_dat = {0};
    
    if(!s_is_nv_init)
    {
        MMIKUR_InitSysDat(&kur_sys_dat);
        MMIKUR_ConfigInit(&kur_sys_dat);
        s_is_nv_init = TRUE;
    }
}

/*****************************************************************************/
//  Description : call when Power On
//  Global resource dependence : none
//  Author: Jassmine.Meng
//  Note:only for Kur module
/*****************************************************************************/
PUBLIC BOOLEAN MMIKUR_GetKuroID(char *a_str,uint8 a_length)
{
    BOOLEAN result = TRUE;      

    SCI_MEMCPY(a_str, "SPRD0001CS02TXEHCD9LYEQ", 23);

    
    a_str[KURO_ID_LENGTH] = 0;

    return result;
}

/*****************************************************************************/
//  Description : MMIAPIKUR_Getmp3SampleRate
//  Global resource dependence : 
//  Author:Juan.Zhao
//  Note: 
/*****************************************************************************/
PUBLIC uint32 MMIAPIKUR_GetkurSampleRate(void)
{
    //SCI_TRACE_LOW:"MMIAPIKUR_GetkurSampleRate s_kur_play_sample_rate = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIKUR_2392_112_2_18_2_31_36_32,(uint8*)"d",s_kur_play_sample_rate);
    return s_kur_play_sample_rate;
}

/*****************************************************************************/
//  Description : MMIAPIKUR_Recordmp3SampleRate
//  Global resource dependence : 
//  Author:Juan.Zhao
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIKUR_RecordkurSampleRate(uint32 sample_rate)
{
    s_kur_play_sample_rate = sample_rate;
    //SCI_TRACE_LOW:"MMIAPIKUR_RecordkurSampleRate s_kur_play_sample_rate = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIKUR_2403_112_2_18_2_31_36_33,(uint8*)"d",s_kur_play_sample_rate);
    
}

/*****************************************************************************/
//  Description : Get the current kuro file name.
//  Global resource dependence : 
//  Author: xingdong.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIKUR_GetCurKurName(
                    MMI_STRING_T   *kuro_name_ptr,
                    uint16         max_name_len
                    )
{
    wchar                       file_name[FMM_SEARCH_FILENAME_MAXLEN+1] = {0};
    uint16                      file_name_len = FMM_SEARCH_FILENAME_MAXLEN+1;
    FILEARRAY_DATA_T            file_info = {0};

	if (!kuro_name_ptr || !kuro_name_ptr->wstr_ptr)
		return;
	
//    SCI_PASSERT(PNULL != kuro_name_ptr,("[MMIKURO]:MMIAPIKUR_GetCurKurName, param error!"));
//    SCI_PASSERT(PNULL != kuro_name_ptr->wstr_ptr,("[MMIKURO]:MMIAPIKUR_GetCurKurName, param error!"));

    if(MMIKUR_GetCurKurInfo(&file_info))
    {
        if(MMIAPIFMM_SplitFullPath(
            file_info.filename, 
            file_info.name_len, 
            PNULL, PNULL, PNULL, PNULL, 
            file_name, &file_name_len
            ))
        {
            file_name_len = MIN(max_name_len, file_name_len);
            MMI_WSTRNCPY( kuro_name_ptr->wstr_ptr, file_name_len, file_name, file_name_len, file_name_len );
            kuro_name_ptr->wstr_len = file_name_len;
        }
    }
}

/*****************************************************************************/
//  Description : Whether the kuro lyric file is the current used lyric.
//  Global resource dependence : 
//  Author: xingdong.li
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIKUR_IsKlrUsed(
                    uint16 * file_full_path,
                    uint16 file_path_len
                    )
{
    FILEARRAY_DATA_T klr_info = {0};

	//arvinzhang modify:delete assert
    //SCI_ASSERT(PNULL != file_full_path);

    if (PNULL != file_full_path && MMIKUR_AUDIO_STOPPED != MMIKUR_GetAudioState())
    {
        if(MMIKUR_GetCurKlrInfo(&klr_info))
        {
            if(0 == MMIAPICOM_CompareTwoWstrExt(file_full_path,file_path_len, klr_info.filename,klr_info.name_len,FALSE))
            {
                return TRUE;
            }
        }
    }
    return FALSE;
}

/*****************************************************************************/
//  Description : Whether the kuro file is the current play file.
//  Global resource dependence : 
//  Author: xingdong.li
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIKUR_IsKurUsed(
                    uint16 * file_full_path,
                    uint16 file_path_len
                    )
{
    BOOLEAN             result      = FALSE;
    FILEARRAY_DATA_T    kur_info    = {0};

	//arvinzhang modify:delete assert
    //SCI_ASSERT(PNULL != file_full_path);

    if (PNULL != file_full_path && MMIKUR_AUDIO_STOPPED != MMIKUR_GetAudioState())
    {
        if(MMIKUR_GetCurKurInfo(&kur_info))
        {
            if(0 == MMIAPICOM_CompareTwoWstrExt(file_full_path,file_path_len, kur_info.filename,kur_info.name_len,FALSE))
            {
                result = TRUE;
            }
        }
    }
    return result;
}

/*****************************************************************************/
//  Description : Whether the file can be deleted.
//  Global resource dependence : 
//  Author:xingdong.li
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIKUR_IsFileUsed(
                        uint16              *file_path_ptr,
                        uint16              file_path_len
                        )
{
    BOOLEAN result = FALSE;
 
    if(MMIAPIKUR_IsKurUsed(file_path_ptr, file_path_len)      ||
       MMIAPIKUR_IsKlrUsed(file_path_ptr, file_path_len)      ||
       MMIAPIKUR_IsInternalDataFile(file_path_ptr, file_path_len)
       )
    {
        result = TRUE;
    }
    return result;
}

/*****************************************************************************/
//  Description : Whether the file is in the folder of MMIMP3_RESERVED_DIR.
//  Global resource dependence : 
//  Author:xingdong.li
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIKUR_IsInternalDataFile(
                        uint16              *file_path_ptr,
                        uint16              file_path_len
                        )
{
    BOOLEAN     result = FALSE;
    uint16      dir_path[FMM_SEARCH_FILENAME_MAXLEN+1] = {0};
    uint16      dir_len = FMM_SEARCH_FILENAME_MAXLEN;
    uint16      kuo_cnt_full_path[MMIFILE_FULL_PATH_MAX_LEN+1] = {0}; 
    uint16      kuo_cnt_full_path_len = 0;
    MMIFILE_DEVICE_E dev = MMI_DEVICE_NUM;
    
    dev = MMIAPIFMM_GetDeviceTypeByPath(file_path_ptr, MMIFILE_DEVICE_LEN);
    if(MMI_DEVICE_NUM == dev)
    {
        kuo_cnt_full_path[0] = 'D';
        dev                  = MMI_DEVICE_UDISK;
    }
    
    kuo_cnt_full_path[0] = *MMIAPIFMM_GetDevicePath((dev));
    
    kuo_cnt_full_path[1] = MMIFILE_COLON;
    kuo_cnt_full_path_len = MMIAPICOM_Wstrlen(kuo_cnt_full_path);

    MMIAPICOM_StrcatFromStrToUCS2(kuo_cnt_full_path, &kuo_cnt_full_path_len, (const uint8*)MMIKURO_COUNTER_FILENAME, SCI_STRLEN(MMIKURO_COUNTER_FILENAME));
    
    if(MMIAPIFMM_SplitFullPath(
            file_path_ptr, file_path_len, 
            PNULL, NULL, 
            dir_path, &dir_len, 
            PNULL, NULL
            ))
    {
        if(0 == MMIAPICOM_CompareTwoWstrExt(
                    dir_path,dir_len,
                    MMIKURO_DIR_KURO, MMIKURO_DIR_KURO_LEN,
                    FALSE
                    ))
        {
            result = TRUE;
        }
        else if(0 == MMIAPICOM_CompareTwoWstrExt(
                            file_path_ptr, file_path_len,
                            kuo_cnt_full_path,kuo_cnt_full_path_len,
                            FALSE
                            ))
        {
            result = TRUE;
        }
    }
    return result;
}


/*****************************************************************************/
//  Description : get mp3 file sample rate
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIKUR_GetSampleRateBeforePlay(uint32*    sample_rate_ptr)
{
    BOOLEAN             result = FALSE;

    FILEARRAY_DATA_T    kur_info = {0};

    MMIKUR_GetCurKurInfo(&kur_info);
    s_kur_play_info.file_info.kuro_header_size = MMIKUR_GetKurHeaderSize(kur_info.filename);
    *sample_rate_ptr = MMIKUR_GetKurSampleRate();
    //SCI_TRACE_LOW:"[MMIKURO]: MMIAPIKUR_GetSampleRateBeforePlay, cur_index = %d, total_num = %d,sample_rate = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIKUR_2584_112_2_18_2_31_36_34,(uint8*)"ddd",MMIKUR_GetActiveIndex(), MMIKUR_GetTotalNum(),*sample_rate_ptr);

    return (result);
}

/*****************************************************************************/
//  Description : callback functin for pcm play kuro
//  Global resource dependence : none
//  Author: cmzs
//  Note: 
/*****************************************************************************/
PUBLIC uint32 MMIKURO_GetKurPlayedTime(void)
{
#ifdef WIN32
    uint32 nRet = 0;
    if(s_kur_play_info.file_info.kuro_last_play_time ==0)
    {    
        s_kur_play_info.file_info.kuro_last_play_time = SCI_GetTickCount();
    }
    else
    {    
        nRet = SCI_GetTickCount() - s_kur_play_info.file_info.kuro_last_play_time;
    }
    return nRet;
#else
    if(s_kur_play_info.file_info.kuro_file_handle != PNULL)
    {
        
        AUDIO_PLAYINFO_T playinfo = {0};        
        MMIAUDIO_AudioGetPlayingFileInfo(MMIAUDIO_KURO,&playinfo);

        if(playinfo.uiCurrentDataOffset > s_kur_play_info.file_info.kuro_id3tag_size)
        {
            s_kur_play_info.file_info.kuro_played_time = (playinfo.uiCurrentDataOffset- s_kur_play_info.file_info.kuro_id3tag_size) * 8 / s_kur_play_info.file_info.kuro_bitrate ;
        }
        /*
        //SCI_TRACE_LOW:"[MMIKURO]: MMIKURO_GetKurPlayedTime uiCurrentDataOffset = %d, kur_id3tag_size =%d,kur_bitrate = %d,cur_time = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIKUR_2619_112_2_18_2_31_36_35,(uint8*)"dddd",playinfo.uiCurrentDataOffset,s_kur_play_info.file_info.kuro_id3tag_size,s_kur_play_info.file_info.kuro_bitrate,s_kur_play_info.file_info.kuro_played_time);
        */
    }
    return s_kur_play_info.file_info.kuro_played_time;
#endif
}
/*****************************************************************************/
//  Description : callback functin for pcm play kuro
//  Global resource dependence : none
//  Author: cmzs
//  Note: 
/*****************************************************************************/
LOCAL AUDIO_STREAM_GET_SRC_RESULT_E MMIKUR_GetSrcCallback(//HAUDIO hAudio, uint8** const ppucDataAddr, uint32* const puiDataLength) 
    HAUDIO hAudio, 
	AUDIO_GETSRCDATA_INFO_T * const ptSrcDataInfo,
	void *pvOtherInfo
	)
{
	//arvinzhang modify:delete assert
    //SCI_ASSERT(ptSrcDataInfo!=PNULL);
    if(PNULL == ptSrcDataInfo)
	{
		return AUDIO_STREAM_GET_SRC_ERROR;
	}

#ifndef _WIN32    
    if(s_kur_play_info.file_info.kuro_last_play_time == 0)
    {   
        s_kur_play_info.file_info.kuro_last_play_time = SCI_GetTickCount();
    }
#endif  

    if(s_kur_play_info.file_info.kuro_read_counter  >= s_kur_play_info.file_info.kuro_file_length - 1)//file end
    {
        //SCI_TRACE_LOW:"[MMIKURO]:MMIKUR_GetSrcCallback----stop, s_kuro_read_counter=%d, s_kuro_file_length=%d, audio_handle=0x%x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIKUR_2645_112_2_18_2_31_36_36,(uint8*)"ddd", s_kur_play_info.file_info.kuro_read_counter, s_kur_play_info.file_info.kuro_file_length, hAudio);
        ptSrcDataInfo->tStreamCurSrcDataInfo.uiDataLength = 0;
        SFS_CloseFile(s_kur_play_info.file_info.kuro_file_handle);
        s_kur_play_info.file_info.kuro_file_handle = 0;
        //SCI_TRACE_LOW:"[MMIKURO]:MMIKUR_GetSrcCallback----after AUDIO_Stop and before AUDIO_CloseHandle g_audio_handle=0x%x."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIKUR_2649_112_2_18_2_31_36_37,(uint8*)"d", hAudio);
       
        return AUDIO_STREAM_GET_SRC_ERROR;
    } 
    
    if(s_kur_play_info.file_info.kuro_pause_pos > 0)  //for audio resume由于解码时必须是以256一个block进行，所以这里需要调整读取的位置
    {
        uint32 nblockread = 0,nreadlen = 0,noffset = 0;
        nblockread = s_kur_play_info.file_info.kuro_pause_pos /  KURO_AUDIOSTREAM_BLOCKSIZE;
        s_kur_play_info.file_info.kuro_read_counter = nblockread *  KURO_AUDIOSTREAM_BLOCKSIZE;
        noffset = s_kur_play_info.file_info.kuro_pause_pos - s_kur_play_info.file_info.kuro_read_counter;

        if(SFS_NO_ERROR != SFS_SetFilePointer(s_kur_play_info.file_info.kuro_file_handle,
                                              s_kur_play_info.file_info.kuro_read_counter + MMIAPIKUR_GetKuroHeadSize(),
                                              SFS_SEEK_BEGIN))
        {
            //SCI_TRACE_LOW:"[MMIKURO]:MMIKUR_GetSrcCallback----SFS_SetFilePointer error."
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIKUR_2665_112_2_18_2_31_36_38,(uint8*)"");
            ptSrcDataInfo->tStreamCurSrcDataInfo.uiDataLength = 0;
            SFS_CloseFile(s_kur_play_info.file_info.kuro_file_handle);
            s_kur_play_info.file_info.kuro_pause_pos = 0;
            return AUDIO_STREAM_GET_SRC_ERROR;
        }
        if(SFS_NO_ERROR != SFS_ReadFile(s_kur_play_info.file_info.kuro_file_handle,
                                            s_kur_play_info.file_info.kuro_mp3_buffer,
                                            KURO_AUDIOSTREAM_BUFFERSIZE,
                                            &nreadlen,
                                            PNULL))
        {
            //SCI_TRACE_LOW:"[MMIKURO]:MMIKUR_GetSrcCallback----SFS_SetFilePointer error."
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIKUR_2677_112_2_18_2_31_36_39,(uint8*)"");
            ptSrcDataInfo->tStreamCurSrcDataInfo.uiDataLength = 0;
            SFS_CloseFile(s_kur_play_info.file_info.kuro_file_handle);
            s_kur_play_info.file_info.kuro_pause_pos = 0;
            return AUDIO_STREAM_GET_SRC_ERROR;
        }
        KuroDecode(s_kur_play_info.file_info.kuro_mp3_buffer,nreadlen,0,0);
        ptSrcDataInfo->tStreamCurSrcDataInfo.pucDataAddr =  s_kur_play_info.file_info.kuro_mp3_buffer + noffset;
        ptSrcDataInfo->tStreamCurSrcDataInfo.uiDataLength = nreadlen - noffset;
        s_kur_play_info.file_info.kuro_read_counter += nreadlen;

        s_kur_play_info.file_info.kuro_pause_pos = 0;
    }
    else
    {
        
        if(SFS_NO_ERROR != SFS_ReadFile(
                                s_kur_play_info.file_info.kuro_file_handle,
                                s_kur_play_info.file_info.kuro_mp3_buffer,
                                KURO_AUDIOSTREAM_BUFFERSIZE,
                                (uint32 *)&(ptSrcDataInfo->uiDataLength),
                                PNULL
                                ))
        {
            //SCI_TRACE_LOW:"[MMIKURO]:MMIKUR_GetSrcCallback----FFS_Read error."
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIKUR_2701_112_2_18_2_31_36_40,(uint8*)"");
            ptSrcDataInfo->tStreamCurSrcDataInfo.uiDataLength = 0;
            SFS_CloseFile(s_kur_play_info.file_info.kuro_file_handle);
            return AUDIO_STREAM_GET_SRC_ERROR;
        }
        KuroDecode(s_kur_play_info.file_info.kuro_mp3_buffer,ptSrcDataInfo->tStreamCurSrcDataInfo.uiDataLength,s_kur_play_info.file_info.kuro_read_counter,0);
        ptSrcDataInfo->tStreamCurSrcDataInfo.pucDataAddr = s_kur_play_info.file_info.kuro_mp3_buffer ;
        if(s_kur_play_info.file_info.kuro_read_counter == 0)
        {
            s_kur_play_info.file_info.kuro_id3tag_size = MMIKUR_GetKurID3TagSize();
            s_kur_play_info.file_info.kuro_bitrate = MMIKUR_GetKurBitRate();
            if(s_kur_play_info.file_info.kuro_bitrate == 0)
                s_kur_play_info.file_info.kuro_bitrate = 128;
        }
        s_kur_play_info.file_info.kuro_read_counter += ptSrcDataInfo->tStreamCurSrcDataInfo.uiDataLength;
    }
    return AUDIO_STREAM_GET_SRC_SUCCESS; 
}

/*****************************************************************************/
//  Description : API of create kuro buffer play handle
//  Global resource dependence : none
//  Author: cmzs
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN MMIKUR_CreateAudioHandle(// SUCCESS - return the audio handle created
                                         // FAILURE - return SCI_NULL
                                         BOOLEAN is_need_a2dp,
                                         const uint16* kurpath
                                         )
{
    MP3_STREAM_FORMATINFO_T * mp3_stream_format_info_ptr = PNULL;

    s_kur_play_info.file_info.kuro_mp3_head_info.bType = 1;
    mp3_stream_format_info_ptr = (MP3_STREAM_FORMATINFO_T *)(&(s_kur_play_info.file_info.kuro_mp3_head_info.unMp3StreamData.tMp3FormatInfo));
    mp3_stream_format_info_ptr->uiSampleRate = MMIKUR_GetKurSampleRate();
    mp3_stream_format_info_ptr->uiChannelNum = 2;

    if(SFS_INVALID_HANDLE != s_kur_play_info.file_info.kuro_file_handle)
    {    
        SFS_CloseFile(s_kur_play_info.file_info.kuro_file_handle);
    }
    s_kur_play_info.file_info.kuro_file_handle = SFS_CreateFile((const uint16 *)kurpath,SFS_MODE_OPEN_EXISTING|SFS_MODE_READ,0,0);
    s_kur_play_info.file_info.kuro_pause_pos = 0;
    s_kur_play_info.file_info.kuro_last_play_time = 0;
    s_kur_play_info.file_info.kuro_played_time = 0;

    if (SCI_NULL == s_kur_play_info.file_info.kuro_file_handle)
    {
        // fail to open file
        //SCI_TRACE_LOW:"[MMIKURO]:MMIKUR_CreateAudioHandle----SFS_Create:0x%x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIKUR_2749_112_2_18_2_31_36_41,(uint8*)"d", s_kur_play_info.file_info.kuro_file_handle);
        return FALSE;
    }

    SFS_SetFilePointer(s_kur_play_info.file_info.kuro_file_handle,MMIAPIKUR_GetKuroHeadSize(),SFS_SEEK_BEGIN);
    SFS_GetFileSize(s_kur_play_info.file_info.kuro_file_handle,&s_kur_play_info.file_info.kuro_file_length);
    s_kur_play_info.file_info.kuro_file_length -= MMIAPIKUR_GetKuroHeadSize();
    s_kur_play_info.file_info.kuro_read_counter = 0;

    return MMIAUDIO_CreateAudioStreamingHandle(
                    is_need_a2dp,
                    MMIAUDIO_KURO,
                    MMIAUDIO_MP3,
                    (uint8 *)&s_kur_play_info.file_info.kuro_mp3_head_info,
                    (uint32)(sizeof(MP3_STREAM_BUFFER_T)),
                    (AUDIO_GETSRCDATA_CALLBACK_PFUNC)MMIKUR_GetSrcCallback
                    );
}

/*****************************************************************************/
//  Description : Start play mp3
//  Global resource dependence : none
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIKUR_PlayKurForBt(
                    void
                    )
{
    return MMIKUR_PlayKur();
}
/*****************************************************************************/
//  Description : Pause the KURO BG player
//  Global resource dependence : 
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIKUR_PauseKurForBt(
                    BOOLEAN  is_by_user
                    )
{
    return MMIKUR_PauseKur(is_by_user);
}

/*****************************************************************************/
//  Description : Resume KURO BG player.
//  Global resource dependence : 
//  Author: xingdong.li
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIKUR_ResumeKurForBt(
                    void
                    )
{
    return MMIKUR_ResumeKur();
}

/*****************************************************************************/
//  Description : Stop the Kuro BG player.
//  Global resource dependence : 
//  Author: xingdong.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIKUR_StopKurForBt(
                    void
                    )
{
    MMIKUR_StopKur();
}


/*****************************************************************************/
//  Description : Fatal error.
//  Global resource dependence : 
//  Author: xingdong.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIKUR_FatalError(void)
{
    //SCI_TRACE_LOW:"[MMIKUR]: FATAL ERROR"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIKUR_2818_112_2_18_2_31_36_42,(uint8*)"");
    MMK_PostMsg(VIRTUAL_WIN_ID,MSG_KUR_FATAL_ERROR,PNULL,0);
}

/*****************************************************************************/
//  Description : Handle Fatal Error.
//  Global resource dependence : 
//  Author: xingdong.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIKUR_HandleFatalError(void)
{
    MMIAPIKUR_ResetAllKur();
}

#else   ///////////////////////////////////// #ifdef KURO_SUPPORT  ///////////////////////////

/*****************************************************************************/
//  Description : open kur player
//  Global resource dependence :
//  Author:jassmine.meng
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIKUR_OpenKurPlayer(void)
{
    return;
}

/*****************************************************************************/
//  Description : stop kur play and close play window
//  Global resource dependence :
//  Author:cmzs
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIKUR_StopKurAndCloseWin(void)
{
    return;
}


/*****************************************************************************/
//  Description : close kuro play window
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIKUR_CloseWin(
                void
                )
{
    return;
}

/*****************************************************************************/
//  Description : when format udisk,reset global variabel
//  Global resource dependence : 
//  Author:cmzs
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIKUR_ResetAllKur(void)
{
    return;
}

/*****************************************************************************/
//  Description : 跳转到前一首或者下一首Kur。
//              根据播放模式重新显示进度条和button和是否继续播放其他mp3等
//  Global resource dependence : 
//  Author:cmzs
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIKUR_GoToPreOrNextKur(
                   BOOLEAN      is_prev
                   )
{
    return;
}

/*****************************************************************************/
//  Description : set the volume of kur
//  Global resource dependence : none
//  Author: cmzs
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIKUR_SetKurVolume(//volume is change
                      BOOLEAN  is_increase
                      )
{
    return FALSE;
}

/*****************************************************************************/
//  Description : call when udisk exit
//  Global resource dependence : none
//  Author: Jassmine.Meng
//  Note:only for Kur module
/*****************************************************************************/
PUBLIC void MMIAPIKUR_OnUdiskShutDown(void)
{
    return;
}

/*****************************************************************************/
//  Description : call when udisk start up
//  Global resource dependence : none
//  Author: Jassmine.Meng
//  Note:only for Kur module
/*****************************************************************************/
PUBLIC void MMIAPIKUR_OnUdiskStartup(void)
{
    return;
}

/*****************************************************************************/
//  Description : MMIAPIKUR_Getmp3SampleRate
//  Global resource dependence : 
//  Author:Juan.Zhao
//  Note: 
/*****************************************************************************/
PUBLIC uint32 MMIAPIKUR_GetkurSampleRate(void)
{
    return NULL;
}

/*****************************************************************************/
//  Description : MMIAPIKUR_Recordmp3SampleRate
//  Global resource dependence : 
//  Author:Juan.Zhao
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIKUR_RecordkurSampleRate(uint32 sample_rate)
{
    return;
}

/*****************************************************************************/
//  Description : MMIAPIKUR_IsklrUsed
//  Global resource dependence : 
//  Author:Juan.Zhao
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIKUR_IsKlrUsed(uint16 * file_full_path, uint16 file_path_len)
{
    return FALSE;
}

/*****************************************************************************/
//  Description : Whether the kuro file is the current play file.
//  Global resource dependence : 
//  Author:xingdong.li
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIKUR_IsKurUsed(uint16 * file_full_path,uint16 file_path_len)
{
    return FALSE;
}

/*****************************************************************************/
//  Description : Whether the file can be deleted.
//  Global resource dependence : 
//  Author:xingdong.li
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIKUR_IsFileUsed(
                        uint16              *file_path_ptr,
                        uint16              file_path_len
                        )
{
    return FALSE;
}

/*****************************************************************************/
//  Description : Whether the file is internal data of kuro player.
//  Global resource dependence : 
//  Author:xingdong.li
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIKUR_IsInternalDataFile(
                        uint16              *file_path_ptr,
                        uint16              file_path_len
                        )
{
    return FALSE;
}

/*****************************************************************************/
//     Description : get sum size of all kur nv itme
//    Global resource dependence : 
//  Author: gang.tong
//    Note:
/*****************************************************************************/
PUBLIC uint32 MMIAPIKUR_GetSumNvSize(void)
{
    return NULL;
}

/*****************************************************************************/
//  Description:process blue tooth a2dp avrcp of play/pause mp3
//  Global resource dependence:
//  Author: kelly.li
//  Note:
/*****************************************************************************/
#ifdef  BLUETOOTH_SUPPORT
PUBLIC void MMIAPIKUR_HandleA2dpMsg(
                MMIBT_FUNCTION_TYPE_E action_type
                )
{
    return;
}
#endif

/*****************************************************************************/
//  Description : Initialize kuro module when app initializes.
//  Global resource dependence : none
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIKUR_Init(void)
{
    return;
}

/*****************************************************************************/
//  Description : Register kuro module
//  Global resource dependence : none
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIKUR_InitModule(void)
{
    return;
}

/*****************************************************************************/
//  Description : Get the head size of kur.
//  Global resource dependence : none
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMIAPIKUR_GetKuroHeadSize(
                    void
                    )
{
    return NULL;
}

/*****************************************************************************/
//  Description : initialize kuro dir
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIKUR_InitKurFolder(void)
{
    return;
}

/*****************************************************************************/
//  Description : whether the kuro player is playing
//  Global resource dependence :
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIKUR_IsPlayerPlaying(void)
{
    return FALSE;
}

/*****************************************************************************/
//  Description : whether the kuro player is paused
//  Global resource dependence :
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIKUR_IsPlayerPaused(void)
{
    return FALSE;
}

/*****************************************************************************/
//  Description : whether the kuro player is stopped
//  Global resource dependence : 
//  Author: xingdong.li
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIKUR_IsPlayerStopped(void)
{
    return TRUE;
}

/*****************************************************************************/
//  Description : whether the kuro player is not stopped
//  Global resource dependence : 
//  Author: xingdong.li
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIKUR_IsPlayerActive(void)
{
    return FALSE;
}

/*****************************************************************************/
//  Description : Get the delay timer, for playing kur in a delayed manner.
//  Global resource dependence : none
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC uint8 MMIAPIKUR_GetDelayPlayTimer(
                    void
                    )
{
    return NULL;
}

/*****************************************************************************/
//  Description : Play kur in a delayed manner, handle delay timer,begin to play.
//  Global resource dependence : none
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIKUR_HandleDelayPlayTimer(
                    uint8 timer_id,
                    uint32 param
                    )
{
    return FALSE;
}

/*****************************************************************************/
//  Description : Start play kuro player
//  Global resource dependence : none
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIKUR_PlayKurPlayer(
                    void
                    )
{
    return FALSE;
}

/*****************************************************************************/
//  Description : Pause kuro player
//  Global resource dependence : none
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIKUR_PauseKurPlayer(
                    BOOLEAN  is_by_user
                    )
{
    return FALSE;
}

/*****************************************************************************/
//  Description : Resume kuro player
//  Global resource dependence : none
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIKUR_ResumeKurPlayer(
                    BOOLEAN  is_by_user
                    )
{
    return FALSE;
}

/*****************************************************************************/
//  Description : Stop kur player
//  Global resource dependence : none
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIKUR_StopKurPlayer(
                    void
                    )
{
    return;
}

/*****************************************************************************/
//  Description : play music from my document
//  Global resource dependence : 
//  Author: xingdongli
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIKUR_PlayMyDocumentMusic(
                    MMIFILE_FILE_INFO_T   *music_file_info_ptr
                    )
{
    return;
}

/*****************************************************************************/
//  Description : Get the current kuro file name.
//  Global resource dependence : 
//  Author: xingdong.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIKUR_GetCurKurName(
                    MMI_STRING_T   *kuro_name_ptr,
                    uint16         max_name_len
                    )
{
    return;
}

/*****************************************************************************/
//  Description : mp3 play main win is focus
//  Global resource dependence :
//  Author: Jasmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIKUR_IsPlayerFocus(void)
{
    return FALSE;
}

/*****************************************************************************/
//  Description : KURO 正常播放结束的回调函数。
//                根据播放模式重新显示进度条和button和是否继续播放其他 KURO 等
//  Global resource dependence : 
//  Author: xingdong.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIKUR_PlayKurCallBack(void)
{
    return;
}

/*****************************************************************************/
//  Description : Start play mp3
//  Global resource dependence : none
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIKUR_PlayKurForBt(
                    void
                    )
{
    return FALSE;
}
/*****************************************************************************/
//  Description : Pause the KURO BG player
//  Global resource dependence : 
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIKUR_PauseKurForBt(
                    BOOLEAN  is_by_user
                    )
{
    return FALSE;
}

/*****************************************************************************/
//  Description : Resume KURO BG player.
//  Global resource dependence : 
//  Author: xingdong.li
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIKUR_ResumeKurForBt(
                    void
                    )
{
    return FALSE;
}

/*****************************************************************************/
//  Description : Stop the Kuro BG player.
//  Global resource dependence : 
//  Author: xingdong.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIKUR_StopKurForBt(
                    void
                    )
{
    return;
}

/*****************************************************************************/
//  Description : Fatal error.
//  Global resource dependence : 
//  Author: xingdong.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIKUR_FatalError(void)
{
    return;
}

/*****************************************************************************/
//  Description : Handle Fatal Error.
//  Global resource dependence : 
//  Author: xingdong.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIKUR_HandleFatalError(void)
{
    return;
}

/*****************************************************************************/
//  Description : get mp3 file sample rate
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIKUR_GetSampleRateBeforePlay(uint32*    sample_rate_ptr)
{
    *sample_rate_ptr = 0;
    
    return TRUE;
}
#endif  //KURO_SUPPORT
