#ifdef KURO_SUPPORT
/*****************************************************************************
** File Name:      mmikur_internal.h                                         *
** Author:                                                                   *
** Date:           08/2007                                                *
** Description:    This file is used to mmikur_internal func                 *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 08/2007       cmzs           Create
******************************************************************************/

#ifndef _MMIKUR_INTERNAL1_H_
#define _MMIKUR_INTERNAL1_H_ 

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
//#include "mmiset.h"
#include "mmiset_export.h"
#include "mmikur.h"
#include "mp3_adp.h"
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
#define MMIKUR_DEFAULT_VOL          MMISET_VOL_SEVEN            //Kur default volume
#define MMIKUR_DEFAULT_EQ_MODE      MMIAUDIO_EQ_REGULAR         //Kur default eq mode
#define MMIKUR_DEFAULT_PLAY_MODE    MMIKUR_PLAY_LIST_REPEAT     //Kur default play mode
#define MMIKUR_ZERO_OFFSET          (0)                         //zero play offset

#define MMIKURO_SUFFIX              "kur" 
#define MMIKURO_LYRIC_SUFFIX        "klr"
#define MMIKURO_SUFFIX_LEN           3
#define MMIKURO_DIR_KURO            (g_mmikur_dir_kur)      //kuro
#define MMIKURO_DIR_KURO_LEN        4
#define MMIKURO_DIR_KUROSONG        (g_mmikur_dir_kursong)  //kurosong
#define MMIKURO_DIR_KUROSONG_LEN    8
#define MMIKURO_COUNTER_FILENAME    "\\Kurosong\\IGRS.cnt"
#define MMIKURO_KLR_EXT             (g_mmikur_klr_ext)      //"klr"
#define MMIKURO_KLR_EXT_LEN         3

#define CHUNK(x)  (((x) % 256 != 0)? (( (x) / 256 + 1) * 256) : (x))
#define MMIKURO_READ_PIC_TIMES   50
#define KLR_PIC_DATASIZE 160 * 1024

#define KURO_AUDIOSTREAM_BLOCKSIZE  256
#define KURO_AUDIOSTREAM_BUFFERSIZE 1024
/**--------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                *
 **--------------------------------------------------------------------------*/
typedef struct  
{
    uint8 *pic_read_buf1;
    uint8 *pic_read_buf2;
    uint8 *pic_read_buf_current;
    int32 pic_readed_index1;
    int32 pic_readed_index2;
    int32 pic_reading_index;
    int32 pic_dsp_index;
    BOOLEAN pic_read_finished;
    BOOLEAN is_first_pic_read;
}KLR_PIC_READINFO;  

// play mode
typedef enum
{
    MMIKUR_PLAY_ONE_ONCE,       //单首播放一次
    MMIKUR_PLAY_ONE_REPEAT,     //单首重复播放
    MMIKUR_PLAY_LIST_ONCE,      //列表循环一次
    MMIKUR_PLAY_LIST_REPEAT,    //列表重复循环
    MMIKUR_PLAY_RANDOM_REPEAT,  //列表重复随机
    MMIKUR_PLAY_RANDOM_ONCE,    //列表随机一次
    MMIKUR_PLAY_MAX_MODE
} MMIKUR_PLAY_MODE_E;

//KURO audio state, concept belongs to MMIKURO_PLAYER_AUDIO_ACTIVE
typedef enum
{
    MMIKUR_AUDIO_OPENED,  //open
    MMIKUR_AUDIO_PLAYING, //play
    MMIKUR_AUDIO_PAUSED,  //pause
    MMIKUR_AUDIO_STOPPED, //stop
    MMIKUR_AUDIO_CLOSED,  //close  //not used for now
    MMIKUR_AUDIO_STATE_MAX
} MMIKUR_AUDIO_STATE_E;

//KURO MODULE STATE
typedef enum
{
    MMIKUR_PLAYER_STATE_NONE,   //state none
    MMIKUR_PLAYER_AUDIO_ACTIVE, //audio playing
    MMIKUR_PLAYER_MUSIC_SWITCH, //music switching
    MMIKUR_PLAYER_SWITCH_PAUSED,//pause while switching
    MMIKUR_PLAYER_STATE_MAX
} MMIKUR_PLAYER_STATE_E;

//暂停类型
typedef enum
{
    MMIKUR_PAUSED_BY_NONE,
    MMIKUR_PAUSED_BY_USER,
    MMIKUR_PAUSED_BY_APP,
    MMIKUR_PAUSED_BY_USER_AND_APP,
    MMIKUR_PAUSED_TYPE_MAX
}MMIKUR_PAUSED_TYPE_E;

//歌词显示方式
typedef enum
{
    KLR_MODE_TWOLINE,       //以两行方式显示歌词
    KLR_MODE_PICONLY,       //仅显示图片
    KLR_MODE_MUTILINE,      //以多行方式显示歌词
    KLR_MODE_MAX,
} KLR_DSP_MODE;

//Kur settings info
typedef struct
{
    KLR_DSP_MODE            lyric_style;        //lyric style
    uint8                   cur_volume;         //volume,from 0 to 7
    MMIAUDIO_EQ_MODE_E      eq_mode;            //eq mode
    MMIKUR_PLAY_MODE_E      play_mode;          //play mode
} MMIKUR_SETTINGS_INFO_T;

//当前播放的动态状态信息
typedef struct
{
    uint32                       total_time;         //当前激活的Kur播放时长
    uint32                       cur_time;           //当前音频播放的播放时刻
    uint32                       total_length;       //当前激活的Kur文件大小(字节长度)
    uint32                       cur_offset;         //当前音频播放的播放位
    uint32                       sample_rate;        //sample rate
    uint32                       bit_rate;           //bit rate
} MMIKUR_PLAYING_INFO_T;

//根据Kur列表产生的在某个范围内的不同的随机整数信息
typedef struct
{
    uint32      active_index;                //当前播放的Kur在random_num数组中的索引
    uint32      *random_index_ptr;           //不同的随机整数
} MMIKUR_RANDOM_INFO_T;

//当前播放的kuro文件信息
typedef struct
{
    uint32              kuro_header_size;
    SFS_HANDLE          kuro_file_handle;
    uint32              kuro_file_length;
    uint32              kuro_read_counter;
    uint32              kuro_pause_pos;
    uint32              kuro_last_play_time;
    uint32              kuro_played_time;
    uint32              kuro_id3tag_size;
    uint32              kuro_bitrate;
    MP3_STREAM_BUFFER_T kuro_mp3_head_info;
    FILEARRAY_DATA_T    kuro_name_info;
    uint8               kuro_mp3_buffer[KURO_AUDIOSTREAM_BUFFERSIZE];
} MMIKUR_FILE_INFO_T;

//当前播放列表的信息
typedef struct
{
    BOOLEAN                     is_from_kdb;        //是否为从kdb文件中获取的信息
    BOOLEAN                     is_all_kur_loaded;  //是否已经加载全部kur文件
    uint16                      kdb_class_index;
    uint16                      kdb_type_index;
    uint32                      total_num;          //list中条目总数目
    FILEARRAY                   list_array;         //指定Array
    MMIKUR_RANDOM_INFO_T        random_info;        //随机列表信息
} MMIKUR_LIST_INFO_T;

//播放控制结构
typedef struct
{
    uint8                        cur_vol;            //当前音量
    uint8                        switch_delay_timer; //歌曲切换延迟timer
    uint32                       start_index;        //起始索引
    uint32                       active_index;       //当前激活的索引
    KLR_DSP_MODE                 lyric_style;        //歌词风格
    MMIAUDIO_EQ_MODE_E           eq_mode;            //EQ模式
    MMIKUR_PLAY_MODE_E           cur_play_mode;      //播放模式
    MMIKUR_PLAYER_STATE_E        player_state;       //播放器状态
    MMIKUR_AUDIO_STATE_E         audio_state;        //当前的audio状态
    MMIKUR_PLAYING_INFO_T        dyn_playing_info;   //当前播放的动态状态信息
    MMIKUR_FILE_INFO_T           file_info;          //当前播放文件
    MMIKUR_LIST_INFO_T           list_info;          //当前播放文件的信息
} MMIKUR_PLAY_INFO_T;

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : register Kur menu group
//  Global resource dependence : none
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void MMIKUR_RegMenuGroup(void);

/*****************************************************************************/
//  Description : register module nv len and max item
//  Global resource dependence : none
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void MMIKUR_RegNv(void);


/*****************************************************************************/
//  Description : display Kur play window
//  Global resource dependence :
//  Author:cmzs
//  Note:
/*****************************************************************************/
PUBLIC void MMIKUR_DisplayKurPlayWin(
                  BOOLEAN           is_update,//is update name and lyric
                  MMI_WIN_ID_T      win_id
                  );

/*****************************************************************************/
//  Description : Hide toolbar
//  Global resource dependence : 
//  Author:cmzs
//  Note: 
/*****************************************************************************/
PUBLIC void MMIKLR_HideToolBar(void);

/*****************************************************************************/
//  Description : Display Tp Toolbar
//  Global resource dependence : 
//  Author:cmzs
//  Note: 
/*****************************************************************************/
PUBLIC void MMIKLR_DisplayToolBar(BOOLEAN bRefresh);

/*****************************************************************************/
//  Description : display kuro volume window
//  Global resource dependence : 
//  Author:cmzs
//  Note: 
/*****************************************************************************/
PUBLIC void MMIKLR_DisplayKurVolumeWin(BOOLEAN bRefresh);
/*****************************************************************************/
//  Description : display kuro volume window
//  Global resource dependence : 
//  Author:cmzs
//  Note: 
/*****************************************************************************/
PUBLIC void MMIKLR_HideKurVolumeWin(void);

/*****************************************************************************/
//  Description : display klr lyric
//  Global resource dependence : 
//  Author:cmzs
//  Note: 
/*****************************************************************************/
PUBLIC void MMIKUR_DisplayKlr(MMI_WIN_ID_T win_id,BOOLEAN bRepaintAll,BOOLEAN bDspToolBar,KLR_DSP_MODE nMode);

/*****************************************************************************/
//  Description : change Lyric Display Mode
//  Global resource dependence : 
//  Author:cmzs
//  Note: 
/*****************************************************************************/
PUBLIC void MMIKUR_GoToNextLyricMode(void);

/*****************************************************************************/
//  Description : 根据当前播放信息播放Klr
//  Global resource dependence : none
//  Author: cmzs
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIKUR_PlayKlr(void);

/*****************************************************************************/
//  Description : call when Power On
//  Global resource dependence : none
//  Author: Jassmine.Meng
//  Note:only for Kur module
/*****************************************************************************/
PUBLIC void MMIKUR_OnPowerOn(void);

/*****************************************************************************/
//  Description : 在播放Kur的情况下提示文件格式不支持，Kur错误，则根据播放模式继续播放
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIKUR_HandleKurError(
                          MMI_WIN_ID_T      win_id,
                          MMI_MESSAGE_ID_E  msg_id,
                          DPARAM            param
                          );

/*****************************************************************************/
//  Description : start Kur anim timer
//  Global resource dependence : 
//  Author:jassmine.meng
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIKUR_StartAnimTimer(void);

/*****************************************************************************/
//  Description : stop Kur anim timer
//  Global resource dependence : 
//  Author:jassmine.meng
//  Note: 
/*****************************************************************************/
PUBLIC void MMIKUR_StopAnimTimer(
                     void
                     );

/*****************************************************************************/
//  Description : Write SysDat To NvItem
//  Global resource dependence :
//  Author:CMZS
//  Note:
/*****************************************************************************/

PUBLIC BOOLEAN WriteSysDatToNV(uint8 *psysdat);
/*****************************************************************************/
//  Description : Read SysDat From NvItem
//  Global resource dependence :
//  Author:CMZS
//  Note:
/*****************************************************************************/

PUBLIC BOOLEAN ReadSysDatFromNV(uint8 *psysdat);
/*****************************************************************************/
//  Description : Read SysDat From NvItem
//  Global resource dependence :
//  Author:CMZS
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN WriteIgrsKeyToNV(uint8 *pigrskey);
/*****************************************************************************/
//  Description : Read SysDat From NvItem
//  Global resource dependence :
//  Author:CMZS
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN ReadIgrsKeyFromNV(uint8 *pigrskey);

/*****************************************************************************/
//  Description : 重置图片读取信息
//  Global resource dependence : 
//  Author: songbin.zeng
//  Note:
//  Return: 
/*****************************************************************************/
PUBLIC void MMIKUR_ReSetPicReadInfo(void);
/*****************************************************************************/
//  Description : 读入第一张图片(一次读入)
//  Global resource dependence : 
//  Author: songbin.zeng
//  Note:
//  Return: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIKUR_ReadFirstPic(void);
/*****************************************************************************/
//  Description : 歌词播放模式
//  Global resource dependence : 
//  Author: apple.zhang
//  Note:
//  Return: 
/*****************************************************************************/
PUBLIC KLR_DSP_MODE MMIKUR_GetKlrMode(void);
/*****************************************************************************/
//  Description : 音量调节面板是否显示
//  Global resource dependence : 
//  Author: apple.zhang
//  Note:
//  Return: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIKUR_GetbdispVolume(void);

/*****************************************************************************/
//  Description : 控制调节面板是否显示
//  Global resource dependence : 
//  Author: apple.zhang
//  Note:
//  Return: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIKUR_Getbdisptoolbar(void);
/*****************************************************************************/
//  Description : 设置控制调节面板是否显示
//  Global resource dependence : 
//  Author: apple.zhang
//  Note:
//  Return: 
/*****************************************************************************/
PUBLIC void MMIKUR_Setbdisptoolbar(BOOLEAN is_bdisptoolbar);

/*****************************************************************************/
//  Description : Get the volume of play kur
//  Global resource dependence : none
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC uint8 MMIKUR_GetVolume(
                void
                );

/*****************************************************************************/
//  Description : Set the volume of kur
//  Global resource dependence : none
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIKUR_SetVolume(
                uint8     kur_volume//in
                );

/*****************************************************************************/
//  Description : Get the mode of play kur
//  Global resource dependence : none
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC MMIKUR_PLAY_MODE_E MMIKUR_GetPlayMode(
                            void
                            );

/*****************************************************************************/
//  Description : Set the mode of kur
//  Global resource dependence : none
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIKUR_SetPlayMode(
                   MMIKUR_PLAY_MODE_E  play_mode//in
                   );

/*****************************************************************************/
//  Description : get the mode of eq
//  Global resource dependence : none
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC MMIAUDIO_EQ_MODE_E MMIKUR_GetEqMode(
                            void
                            );

/*****************************************************************************/
//  Description : set the mode of kur
//  Global resource dependence : none
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIKUR_SetEqMode(
                MMIAUDIO_EQ_MODE_E    eq_mode     //in
                );

/*****************************************************************************/
//  Description : Get the kuro filter.
//  Global resource dependence : 
//  Author:xingdong.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIKUR_GetKurFilter(
                MMIFMM_FILTER_T * filter_ptr
                );

/*****************************************************************************/
//  Description : Load all kuro from fixed folder--"KuroSong".
//  Global resource dependence : 
//  Author:xingdong.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIKUR_LoadAllKuro(
                        void
                        );

/*****************************************************************************/
//  Description : Reset the kuro list.
//  Global resource dependence : 
//  Author: xingdong.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIKUR_ResetList(void);


/*****************************************************************************/
//  Description : Set the kuro list.
//  Global resource dependence : 
//  Author: xingdong.li
//  Note: Just set.
/*****************************************************************************/
PUBLIC void MMIKUR_SetKurList(
                MMIKUR_LIST_INFO_T *list_ptr
                );
                
/*****************************************************************************/
//  Description : Clear the current random list info.
//  Global resource dependence : 
//  Author:xingdong.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIKUR_ClearRandomInfo(void);

/*****************************************************************************/
//  Description : Init the random list.
//  Global resource dependence : 
//  Author:xingdong.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIKUR_InitRandomList(void);

/*****************************************************************************/
//  Description : Get kuro file number in the kuro list.
//  Global resource dependence : 
//  Author:xingdong.li
//  Note: 
/*****************************************************************************/
PUBLIC uint32 MMIKUR_GetTotalNum(
                    void
                    );

/*****************************************************************************/
//  Description : Get the playing active index.
//  Global resource dependence : 
//  Author:xingdong.li
//  Note: 
/*****************************************************************************/
PUBLIC uint32 MMIKUR_GetActiveIndex(
                    void
                    );

/*****************************************************************************/
//  Description : Set active index.
//  Global resource dependence : 
//  Author:xingdong.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIKUR_SetActiveIndex(
                uint32 index
                );

/*****************************************************************************/
//  Description : Set start index.
//  Global resource dependence : 
//  Author:xingdong.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIKUR_SetStartIndex(
                uint32 index
                );

/*****************************************************************************/
//  Description : load kuro file from kuro list. 
//  Global resource dependence : 
//  Author: xingdong.li
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIKUR_LoadKuroByIndex(
                    uint32 index
                    );

/*****************************************************************************/
//  Description : Get Kuro file info from the list by index
//  Global resource dependence : none
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIKUR_GetKurInfoByIndex(
                    uint32              index,
                    FILEARRAY_DATA_T    *kuro_info_ptr
                    );

/*****************************************************************************/
//  Description : Get the current playing Kuro file info.
//  Global resource dependence : none
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIKUR_GetCurKurInfo(
                    FILEARRAY_DATA_T    *kuro_info_ptr
                    );

/*****************************************************************************/
//  Description : Get Kuro lyric file info from the list by index
//  Global resource dependence : none
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIKUR_GetCurKlrInfo(
                    FILEARRAY_DATA_T    *klr_info_ptr
                    );

/*****************************************************************************/
//  Description : Get the device of the file.
//  Global resource dependence : 
//  Author:xingdong.li
//  Note: 
/*****************************************************************************/
PUBLIC MMIFILE_DEVICE_E MMIKUR_GetFileDevType(
                        const wchar    *file_name,         //[IN]
                        uint16         file_name_len       //[IN]
                        );

/*****************************************************************************/
//  Description : Init kuro list by MMIFILE_FILE_INFO_T
//  Global resource dependence : none
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIKUR_InitKuroListByFileInfo(
                    MMIFILE_FILE_INFO_T *file_info_ptr
                    );

/*****************************************************************************/
//  Description : Start play mp3
//  Global resource dependence : none
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIKUR_PlayKur(
                    void
                    );

/*****************************************************************************/
//  Description : Pause the KURO BG player
//  Global resource dependence : 
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIKUR_PauseKur(
                    BOOLEAN  is_by_user
                    );

/*****************************************************************************/
//  Description : Resume KURO BG player.
//  Global resource dependence : 
//  Author: xingdong.li
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIKUR_ResumeKur(
                    void
                    );

/*****************************************************************************/
//  Description : Stop the Kuro BG player.
//  Global resource dependence : 
//  Author: xingdong.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIKUR_StopKur(
                    void
                    );


/*****************************************************************************/
//  Description : Callback function to MMI AUDIO
//  Global resource dependence : none
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIKUR_CallBackAfterPlay(
                MMIAUDIO_RESULT  result,
                DPARAM           param
                );

/*****************************************************************************/
//  Description : Play kuro in a delayed manner, start delay timer
//  Global resource dependence : none
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIKUR_DelayPlayKur(
                    void
                    );

/*****************************************************************************/
//  Description : Stop the delay timer, for playing kur in a delayed manner.
//  Global resource dependence : none
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIKUR_StopDelayPlayTimer(
                    void
                    );

/*****************************************************************************/
//  Description : Get current audio state.
//  Global resource dependence : 
//  Author:xingdong.li
//  Note: 
/*****************************************************************************/
PUBLIC MMIKUR_AUDIO_STATE_E MMIKUR_GetAudioState(
                                    void
                                    );

/*****************************************************************************/
//  Description : Set current audio state.
//  Global resource dependence : 
//  Author:xingdong.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIKUR_SetAudioState(
                        MMIKUR_AUDIO_STATE_E cur_state
                        );

/*****************************************************************************/
//  Description : 背景图片是否读取OK
//  Global resource dependence : 
//  Author: xingdong.li
//  Note:
//  Return: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIKUR_IsBackgroundReady(void);

/*****************************************************************************/
//  Description : call when Power On
//  Global resource dependence : none
//  Author: Jassmine.Meng
//  Note:only for Kur module
/*****************************************************************************/
PUBLIC BOOLEAN MMIKUR_GetKuroID(char *a_str,uint8 a_length);

/*****************************************************************************/
//  Description : Get player state.
//  Global resource dependence : 
//  Author:xingdong.li
//  Note: 
/*****************************************************************************/
PUBLIC MMIKUR_PLAYER_STATE_E MMIKUR_GetPlayerState(
                                    void
                                    );

/*****************************************************************************/
//  Description : Set player state.
//  Global resource dependence : 
//  Author:xingdong.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIKUR_SetPlayerState(
                    MMIKUR_PLAYER_STATE_E cur_state
                    );

/*****************************************************************************/
//  Description : Fatal error.
//  Global resource dependence : 
//  Author: xingdong.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIKUR_FatalError(void);

/*****************************************************************************/
//  Description : callback functin for pcm play kuro
//  Global resource dependence : none
//  Author: cmzs
//  Note: 
/*****************************************************************************/
PUBLIC uint32 MMIKURO_GetKurPlayedTime(void);

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif
#endif
