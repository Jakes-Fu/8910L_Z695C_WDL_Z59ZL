#ifndef __MEDIA_CONTROL_H
#define __MEDIA_CONTROL_H
#ifdef JAVA_SUPPORT_IA
/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "jkmediacontrol.h"
#include "sci_types.h"
#include "jkmediacapture.h"
#include "jkmediaoption.h"
#include "jblend_config.h"
#include "ajms_arc.h"
#include "audioclient.h"
#ifdef TONE_USE_JBLEND_PARSER
#include "media_tonecontrol.h"
#endif
/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/

/* define supported media type */ 
enum _audio_type_tag{
    JKMEDIA_TYPE_TONE  =( JKMEDIA_TYPE_USER_BASE + 1 ), /* 257 */
    JKMEDIA_TYPE_MIDI  =( JKMEDIA_TYPE_USER_BASE + 2 ),
    JKMEDIA_TYPE_SPMIDI = ( JKMEDIA_TYPE_USER_BASE + 3 ),
    JKMEDIA_TYPE_WAVE  =( JKMEDIA_TYPE_USER_BASE + 4 ),
    JKMEDIA_TYPE_MP3   =( JKMEDIA_TYPE_USER_BASE + 5 ),
    JKMEDIA_TYPE_AMR   =( JKMEDIA_TYPE_USER_BASE + 6 ),
    JKMEDIA_TYPE_AAC   =( JKMEDIA_TYPE_USER_BASE + 7 ),
    JKMEDIA_TYPE_M4A  =( JKMEDIA_TYPE_USER_BASE + 8 ),
    JKMEDIA_TYPE_MPEG_AUDIO  = ( JKMEDIA_TYPE_USER_BASE + 9 ),
    JKMEDIA_TYPE_H263_AUDIO   =( JKMEDIA_TYPE_USER_BASE + 10 ),
    JKMEDIA_TYPE_AUDIO      =(  JKMEDIA_TYPE_USER_BASE + 10 ) /* upper limit for audio*/    
};


enum _video_types_tag{
    JKMEDIA_TYPE_MPEG        =(  JKMEDIA_TYPE_AUDIO + 1      ),/* not implement yet */
    JKMEDIA_TYPE_H263       =(  JKMEDIA_TYPE_AUDIO + 2      ),
    JKMEDIA_TYPE_AVI       =(  JKMEDIA_TYPE_AUDIO + 3      ),
    JKMEDIA_TYPE_GIF =(  JKMEDIA_TYPE_AUDIO + 4      ),
    JKMEDIA_TYPE_VIDEO      =( JKMEDIA_TYPE_AUDIO + 5      ) /* upper limit for video */
};

enum _record_audio_tag{
    JKMEDIA_TYPE_RECORD_WAVE=( JKMEDIA_TYPE_DEVICE_BASE + 1 ),
    JKMEDIA_TYPE_RECORD_AMR	=( JKMEDIA_TYPE_DEVICE_BASE + 2 ),
    JKMEDIA_TYPE_RECORD_AUDIO=( JKMEDIA_TYPE_DEVICE_BASE + 3 ) /* upper limit for record audio */
};

enum _record_video_tag{
    JKMEDIA_TYPE_CAMERA =( JKMEDIA_TYPE_RECORD_AUDIO + 1 ),
    JKMEDIA_TYPE_RECORD_H263 = ( JKMEDIA_TYPE_RECORD_AUDIO + 2 ),
    JKMEDIA_TYPE_RECORD_VIDEO=( JKMEDIA_TYPE_RECORD_AUDIO + 3 )/* upper limit for record video */
};


typedef struct T_mediaPlayer {
    struct T_mediaPlayer *pNext;//ok
    JKSint32 id;//ok
    JKSint32 stream_id;//filestream player id
    JKSint32 media_type;    /* media type for JBlend */ //ok
    JKUint8  *pData;//ok
    JKUint32 length;//ok
    JKSint32 loop;//ok
    JKUint32 duration_high;//ok It's getDuration
    JKUint32 duration_low;//ok It's getDuration
    JKSint32 status;//ok
    JKSint32 volume;//ok
    JKUint32 time_high;//ok It's getTime setTime
    JKUint32 time_low;//ok It's getTime setTime
    JKUint32 stoptime_high;//ok It's StopTime 
    JKUint32 stoptime_low;//ok It's StopTime 
    JKSint32 stoptime_timerid;
    JKUint32 playPos; /*current play position media time (millisecond)*/
    JKBool hasExtPaused; 
    //  JKSint32 wait;
    //  JKSint32 waitCount;
    #ifdef TONE_USE_JBLEND_PARSER
    ToneParser *tp;
    #endif
    #ifdef JAVA_SUPPORT_MEDIA_RECORD	
    /* for RecordControl */
    JKSint32 time; //for record function

    int     is_recording;
    int     error_code;	
    int     record_size;
    /* for video recorder and camera */
    int      is_previewing;	
    int	     pending_pause;
    int	     pending_stop;
    int	     pending_unset;
    int	     pending_unload;    
    #endif
    #ifdef JAVA_SUPPORT_MEDIA_CAMERA
    JKBool preview_isvisible;
    #endif
    AudioClient *ac;//ok
} T_mediaPlayer;


typedef struct _StreamPlayer{
    JKWChar * wchar_url;
    JKSint32 id;
    JKSint32 media_type;
    JKSint32 stream_type;
    struct _StreamPlayer *pNext;
}StreamPlayer;

enum _stream_tag{
    FILE_STREAM =   (1),
    RTSP_STREAM =   (2)
};
/**--------------------------------------------------------------------------*
**                         FUNCTION DEFINITION                              *
**--------------------------------------------------------------------------*/
LOCAL JKSint32 mediaCtrl_Volume_setVolume( JKSint32 id, JKSint32 value );
LOCAL JKSint32 mediaCtrl_Volume_getVolume( JKSint32 id );

JKSint32 mediaCtrl_checkOpen( const JKUint8* pData, const JKSint32 length ,JKSint32 media_type  );
JKSint32 mediaCtrl_check( JKSint32 checkID, JKSint32* pExtraID );
JKSint32 mediaCtrl_checkClose( JKSint32 checkID );
JKSint32 mediaCtrl_getMediaSize( JKSint32 checkID, JKSint32* pWidth, JKSint32* pHeight );
JKSint32 mediaCtrl_create( JKSint32 media_type );
JKSint32 mediaCtrl_destroy( JKSint32 id );
JKSint32 mediaCtrl_set( JKSint32 id, const JKUint8* pData, const JKSint32 length, JKSint32 extraID );
JKSint32 mediaCtrl_load( JKSint32 id );
JKSint32 mediaCtrl_start( JKSint32 id, JKSint32 loop );
JKSint32 mediaCtrl_startLoop( JKSint32 id);
JKSint32 mediaCtrl_stop( JKSint32 id );
JKSint32 mediaCtrl_pause( JKSint32 id );
JKSint32 mediaCtrl_restart( JKSint32 id );
JKSint32 mediaCtrl_unload( JKSint32 id );
JKSint32 mediaCtrl_getState( JKSint32 id );
JKSint32 mediaCtrl_getDuration( JKSint32 id, JKUint32* pUsecH, JKUint32* pUsecL );
JKSint32 mediaCtrl_setTime( JKSint32 id, JKUint32 usecH, JKUint32 usecL );
JKSint32 mediaCtrl_getTime( JKSint32 id, JKUint32* pUsecH, JKUint32* pUsecL );
JKSint32 mediaCtrl_suspendVM( JKSint32 id );
JKSint32 mediaCtrl_resumeVM( JKSint32 id );
void mediaCtrl_callback(int32 event_type,int player_id,JKUint32 UsecH, JKUint32 UsecL,int32 needRepeat);
	
T_mediaPlayer* getPlayer( JKSint32 id );
T_mediaPlayer* createPlayer( int media_type );
JKSint32 destroyPlayer( JKSint32 id );
StreamPlayer *createStreamPlayer(JKWChar *wchar_url);
StreamPlayer *getStreamPlayer(JKSint32 streamId);
JKSint32 destroyStreamPlayer(JKSint32 streamId);


/**--------------------------------------------------------------------------*
**                    EXTERNLA FUNCTION DEFINITION                              *
**--------------------------------------------------------------------------*/
PUBLIC void MMIJAVA_MMAPI_init(void);
PUBLIC void MMIJAVA_MMAPI_fini(void);
PUBLIC void MMIJAVA_MMAPI_upVolume(void);
PUBLIC void MMIJAVA_MMAPI_downVolume(void);

/*****************************************************************************/
//  Description :MMIJAVA_PauseAllMediaPlayer
//  Global resource dependence : none
//  Author: 
//  Note: 此接口用于给MMI 调用，先关闭audio play
/*****************************************************************************/
PUBLIC void MMIJAVA_PauseAllMediaPlayer(void);

/*****************************************************************************/
//  Description :MMIJAVA_ResumeAllMediaPlayer
//  Global resource dependence : none
//  Author: 
//  Note: 此接口用于给MMI 调用，先恢复audio
/*****************************************************************************/
PUBLIC void MMIJAVA_ResumeAllMediaPlayer(void);

/*****************************************************************************/
//  Description : close all playing media when JVM shutdown.
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MMIJAVA_CloseAllMediaPlayer(void);
/*****************************************************************************/
//  Description :MMIJAVA_getPlayingPlayerVolume
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIJAVA_getPlayingPlayerVolume(int32* volume);

#ifdef JAVA_SUPPORT_MEDIA_CAMERA    
PUBLIC void cameraOpt_Preview_visialbe( void );

PUBLIC void cameraOpt_Preview_Invisialbe( void );
#endif
/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif
#endif /* !__MEDIA_CONTROL_H */






	
