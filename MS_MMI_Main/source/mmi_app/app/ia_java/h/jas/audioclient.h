#ifndef _AUDIO_CLIENT_H
#define _AUDIO_CLIENT_H
 
/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
 #include "mmisrvaud_api.h"
#include "sig_code.h"
#include "os_api.h"
#ifdef JAVA_SUPPORT_IA
#include "jblend_config.h"
#include "jktypes.h"
#include "media_tonecontrol.h"
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

/**--------------------------------------------------------------------------*
 **                         Type Definition                                  *
 **--------------------------------------------------------------------------*/
typedef enum
{
    AUDIO_TYPE_NONE,
    MIDI_TYPE,
    WAVE_TYPE,
    TONE_TYPE,
    SINGLETONE_TYPE,
    MP3_TYPE,
    AMR_TYPE,
    MMIJAVA_AUDIO_TYPE_MAX
}MMIJAVA_AUDIO_TYPE;

typedef struct _KJavaWmaCommandTag
{
    int32     actionID;   
    char    params[144];
}
KJavaAudioCommand;

enum _KJavaSocketActionTypeEnum
{
    KJAVA_AUDIO_ACTION_UNLOAD = 0,
    KJAVA_AUDIO_ACTION_PLAY,
    KJAVA_AUDIO_ACTION_PAUSE,
    KJAVA_AUDIO_ACTION_STOP,
    KJAVA_AUDIO_ACTION_RESUME,
    _TOTAL_AUDIO__ACTION_COUNT
};

/* interface */

typedef struct tagAudioClient
{
    int32 _playerID;
    int32 _mediaType;
    int32 _eventType;
    int32 _usecH;
    int32 _usecL;
	uint32 _currenttck;
	uint32 _currentoffset;

	BOOLEAN isplayTone;//ok This is for Manager.playTone();

	uint8* pData;//ok
	uint32 length;//ok
	
	uint32 type;
	int32 m_duraion;
	int32 need_PlayTimes;//ok
	BOOLEAN is_infiniteTimes;//ok
	uint32 native_time;//ok √Î
	uint32 native_volume;//ok
	BOOLEAN   isEndOfMedia;//ok

   	MMISRV_HANDLE_T m_hAudioHandle;//ok
	MMISRVAUD_RING_FMT_E mmi_AudioFmt;//ok
	int32 isRepeating;
	#ifdef AUDIO_USEFILE
	char hidden_file_char[64];//ok
	int32 hidden_file;
	#endif
	BOOLEAN isFileStream;
	wchar * wFileStreamUrl;
}AudioClient;


typedef enum
{
    MEDIA_CLIENT_EVENT_END,
    MEDIA_CLIENT_EVENT_STOPPED,
} media_client_event_type_enum;
/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

PUBLIC BOOLEAN AudioClient_player_open(int player_id, int media_type, uint8*data,int length,wchar* filename,AudioClient* ac);
PUBLIC BOOLEAN AudioClient_player_open_tone_sequence(int player_id, int media_type, uint8*data,int length,ToneParser* tp,AudioClient* ac);
PUBLIC BOOLEAN AudioClient_player_close(AudioClient* ac);
PUBLIC BOOLEAN AudioClient_player_play(AudioClient *ac,int loop,JKSint32 id,int volume);
PUBLIC BOOLEAN AudioClient_player_replay(AudioClient *ac,int volume);
PUBLIC BOOLEAN AudioClient_player_pause(AudioClient* ac);
PUBLIC BOOLEAN AudioClient_player_stop(AudioClient* ac);
PUBLIC BOOLEAN AudioClient_player_get_duration(AudioClient *ac,uint64* ptime);
PUBLIC BOOLEAN AudioClient_player_get_media_time(AudioClient *ac,uint64* ptime);
PUBLIC BOOLEAN AudioClient_player_setVolume( AudioClient *ac, int32 volume ) ;
PUBLIC void AudioClient_create_mutex();
PUBLIC void AudioClient_destory_mutex();
PUBLIC BOOLEAN AudioClient_player_set_media_time(AudioClient *ac,uint64 usec);
PUBLIC void AudioClient_player_initialize();
PUBLIC void AudioClient_player_finalize();
/*****************************************************************************/
//  Description :MMIJAVA_AudioClientGetActivePlayerID
//  Global resource dependence :
//  Author:
//  Note: to implement loop play
/*****************************************************************************/
PUBLIC JKSint32 MMIJAVA_AudioClientGetActivePlayerID(void);

PUBLIC BOOLEAN AudioClient_setActualVolume(int32 actual_volume);

#ifdef JAVA_SUPPORT_MEDIA_RECORD  
BOOLEAN jas_audio_recorder_open( int32 java_recorder) ;
BOOLEAN jas_audio_recorder_close(int32 java_recorder) ;
BOOLEAN jas_audio_recorder_start_record(int32 java_recorder); 
BOOLEAN jas_audio_recorder_pause_record(int32 java_recorder); 
BOOLEAN jas_audio_recorder_commit(int32 java_recorder); 
BOOLEAN jas_audio_recorder_reset(int32 java_recorder) ;
BOOLEAN jas_audio_recorder_set_record_location(int32 java_recorder, wchar *path); 
BOOLEAN jas_audio_recorder_set_record_size_limit(int32 java_recorder, int32 limit_size); 
long jas_audio_recorder_get_record_time(int32 java_recorder) ;
#endif

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
#endif
#endif
