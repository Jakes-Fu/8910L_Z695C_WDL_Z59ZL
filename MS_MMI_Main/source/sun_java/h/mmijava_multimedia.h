/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
 #ifndef  _MMIJAVA_MULTIMEDIA_H_    
#define  _MMIJAVA_MULTIMEDIA_H_   
#include "sci_types.h"
#include "caf.h"
#include "javacall_defs.h"

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
/**--------------------------------------------------------------------------*
 **                         Type Definition                                  *
 **--------------------------------------------------------------------------*/
typedef enum
{
     JAVACALL_VIDEO_MPEG4        = 1,        //MPEG4 video
     JAVACALL_VIDEO_3GPP         ,        //3GPP video
     JAVACALL_VIDEO_AVI          ,
     #ifdef MIDI_SUPPORT
     JAVACALL_AUDIO_MIDI         ,        //MIDI audio
     #endif
     #ifdef WAV_SUPPORT
     JAVACALL_AUDIO_WAV          ,        //WAV audio
     #endif
     JAVACALL_AUDIO_MP3          ,        //MP3 audio
     #ifdef AMR_SUPPORT
     JAVACALL_AUDIO_AMR          ,        //AMR audio
     #endif
     JAVACALL_AUDIO_MPEG4        ,        //MPEG4 audio
     JAVACALL_AUDIO_TONE         ,        //JTS tone
     JAVACALL_AUDIO_QCELP        ,        //QCELP audio
     #ifdef AAC_SUPPORT
     JAVACALL_AUDIO_M4A          ,       //m4a audio
     #endif
     JAVACALL_AUDIO_AAC          ,       //AAC audio
     JAVACALL_CAPTURE_AUDIO      ,       //Audio capture
     JAVACALL_CAPTURE_VIDEO     ,       //Video capture
     JAVACALL_INTERACTIVE_MIDI   ,       //Interactive MIDI
     JAVACALL_CAPTURE_RADIO      ,       //radio capture
     JAVACALL_CAPTURE_RTSP       ,       //rtsp
     JAVACALL_IMAGE_GIF       ,      
         JAVACALL_END_OF_TYPE
} MMIJAVA_MEDIA_TYPE_T;  //javacall_media_type

typedef struct
{
     BOOLEAN (*close)(void);
     BOOLEAN (*destroy)(void);
     javacall_result (*start)(wchar *filename,MMIJAVA_MEDIA_TYPE_T mediatype,int32 cur_time);
     javacall_result (*stop)(void);
     javacall_result (*pause)(void);
     javacall_result (*resume)(void);
     int32 (*get_time)(void);
     int32 (*set_time)(int32 ms);
     int32 (*get_duration)(wchar *filename,MMIJAVA_MEDIA_TYPE_T mediatype);
     javacall_result (*acquire_device)(wchar *filename);
     javacall_result (*release_device)(int32 appid, int32 playerid);
     javacall_result (*set_rate)(MMIJAVA_MEDIA_TYPE_T mediatype, int rate);     
     BOOLEAN (*stopped_by_other)(void);
}MMIJAVA_MEDIA_BASIC_INTERFACE_T;

typedef struct
{
     BOOLEAN (*start_recording)(wchar *filename);
     BOOLEAN (*pause_recording)(void);
     BOOLEAN (*stop_recording)(void);
     BOOLEAN (*reset_recording)(wchar *filename);
     BOOLEAN (*get_recorded_data_size)(wchar *filename,int32 *size);
     BOOLEAN (*get_recorded_data)(wchar *filename,char *buf, int32 offset, int32 size);
     int (*get_record_content_type_length)(void);
     int (*get_record_content_type)(uint16* content_buf);
}MMIJAVA_MEDIA_RECORD_INTERFACE_T;

typedef struct
{
     BOOLEAN (*get_video_size)(wchar *filename, int32* width,int32* height);
     javacall_result (*set_video_location)(int32 x,int32 y,int32 w,int32 h);
     BOOLEAN (*set_video_visible)(BOOLEAN visible);
     javacall_result (*set_fullscreen_mode)(BOOLEAN full_screen_mode, int32 appid, int32 playerid);
     BOOLEAN (*map_frame_to_time)(wchar *filename, int32 frameNum, int32 *ms);
     BOOLEAN (*map_time_to_frame)(wchar *filename, int32 ms, int32 *frameNum);
     BOOLEAN (*seek_frame)(int32 frameNum);
     BOOLEAN (*skip_frame)(int32 frames);
}MMIJAVA_MEDIA_VIDEO_INTERFACE_T;

typedef struct
{
     BOOLEAN (*get_metadata_key_counts)(wchar *filename,MMIJAVA_MEDIA_TYPE_T mediatype,/*OUT*/ long* keyCounts) ;
     BOOLEAN (*get_metadata_key)(long index,long bufLength,char* keyBuf) ;
     BOOLEAN (*get_metadata)(const char* key,long bufLength,char* dataBuf);
}MMIJAVA_MEDIA_METADATA_INTERFACE_T;

typedef struct{
     int8 (*start_snapshot)(const uint16* imageType, int32 length);
     BOOLEAN (*get_snapshot_data_size)(int32* size);
     BOOLEAN (*get_snapshot_data)(char* buffer, int32 size);
}MMIJAVA_MEDIA_SNAPSHOT_INTERFACE_T;

typedef struct
{
     MMIJAVA_MEDIA_BASIC_INTERFACE_T     *vptrBasic;
     MMIJAVA_MEDIA_RECORD_INTERFACE_T    *vptrRecord;
     MMIJAVA_MEDIA_VIDEO_INTERFACE_T*vptrVideo;
     MMIJAVA_MEDIA_METADATA_INTERFACE_T  *vptrMetaData;
     MMIJAVA_MEDIA_SNAPSHOT_INTERFACE_T*vptrSnapshot;
} MMIJAVA_MEDIA_INTERFACE_T;
PUBLIC void MMIJAVA_NotifyMediaStart(javacall_result result);
PUBLIC void MMIJAVA_NotifyMediaEnd(void);
PUBLIC void MMIJAVA_NotifyMediaStop(javacall_result result);
PUBLIC void MMIJAVA_NotifyMediaPause(javacall_result result);
PUBLIC void MMIJAVA_NotifyMediaResume(javacall_result result);
PUBLIC void MMIJAVA_NotifyMediaAcquireDevice(javacall_result result);
PUBLIC void MMIJAVA_NotifyMediaRelease(javacall_result result, int32 appid, int32 playerid);
PUBLIC void MMIJAVA_NotifyMediaSetVideoFullScreenMode(javacall_result result, int32 appid, int32 playerid);
PUBLIC void MMIJAVA_NotifySnapshotFinished(void);
PUBLIC void MMIJAVA_NotifyMediaSetVideoLocation(javacall_result result);

PUBLIC javacall_result javacall_media_increase_volume(void);
PUBLIC javacall_result javacall_media_decrease_volume(void);

PUBLIC BOOLEAN jc_convert_fc2native_path(javacall_const_utf16_string src, int srcLen, javacall_utf16_string dst, int dstLen);

#endif
