
/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/

#ifdef JAVA_SUPPORT_SUN
#include "math.h"
#include "javacall_multimedia.h"
#include "mmijava_multimedia.h"
#include "mmijava_main.h"
#include "mmiaudio.h"
#include "mmifmm_export.h"
#include "mmk_app.h"
//#include "mmiset.h" 
#include "mmiset_export.h"
#include "mmi_position.h"
#include "javanotify_multimedia.h"
#include "javacall_logging.h"
#include "javacall_dir.h"
#include "mmicom_string.h"
#include "mmiaudio_ctrl.h"
#include "mmijava_video.h"
#ifdef CMCC_UI_STYLE
#include "mmienvset.h"
#endif
/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#define MMIJAVA_MAX_VOLUME      100
#define SEMITONE_CONST          17.31234049066755       
#define MMIJAVA_FREQ(note)      (exp(note/SEMITONE_CONST)*8.176)  

#define MMIJAVA_BUFFER_MIN_SIXE (256*1024)
/**
 * Native player create.
 * This function create internal information structure that will be used from other native API.
 */

#define AUDIO_CAPTURE_LOCATOR   L"capture://audio"
#define VIDEO_CAPTURE_LOCATOR   L"capture://video"
#define RADIO_CAPTURE_LOCATOR   L"capture://radio"
#define DEVICE_TONE_LOCATOR     L"device://tone"
#define DEVICE_MIDI_LOCATOR     L"device://midi"
#define RTSP_PROTOCOL_PREFIX    L"rtsp://"
typedef enum
{    
    MMIJAVA_MEDIA_UNINIT,
    MMIJAVA_MEDIA_PLAYING,
    MMIJAVA_MEDIA_RECORDING,
    MMIJAVA_MEDIA_PAUSED
} MMIJAVA_MEDIA_PLAYING_STATE_E;
/**--------------------------------------------------------------------------*
 **                         TYPE DEFINITION                                  *
 **--------------------------------------------------------------------------*/
typedef struct 
{
    int32                           AppID;
    int32                           playerId;
    MMIJAVA_MEDIA_INTERFACE_T       *mediaItfPtr;
    BOOLEAN                         isForeground;
    BOOLEAN                         isStoppedByOther;
    BOOLEAN                         isMute;
    MMIJAVA_MEDIA_PLAYING_STATE_E   playingState;
    MMIJAVA_MEDIA_TYPE_T             mediatype;
    int32                           cur_time;//record time for start
    int32                           rate;
    int32                           defaultMediaTime;//record default media time, only for unseekable media that return meaningful time
    int32                           cur_vol;
    wchar                           filename[MMIFILE_FULL_PATH_MAX_LEN+2];
    BOOLEAN                         isTempFile;
    int32                           wholecontentsize;
    int32                           left_size;
    int32                           bufsize;
    void                            *buf;
} MMIJAVA_NATIVE_HANDLE_T;

/**--------------------------------------------------------------------------*
 **                         EXTERNAL DECLARE                                 *
 **--------------------------------------------------------------------------*/
extern MMIJAVA_MEDIA_INTERFACE_T g_audio_itf;               //audio
extern MMIJAVA_MEDIA_INTERFACE_T g_audio_recording_itf;    //recording
extern MMIJAVA_MEDIA_INTERFACE_T g_video_itf;    //g_video_itf
extern MMIJAVA_MEDIA_INTERFACE_T g_dc_dv_itf;            //camera,dv
/**--------------------------------------------------------------------------*
 **                         LOCAL VARIABLE DEFINITION                        *
 **--------------------------------------------------------------------------*/
static int g_media_handle_counter=0;  //do not create two many handler to keep memory usage
static javacall_media_caps s_media_caps[] = //需要根据手机支持的不同情况进行不同的配置
{
    //    mediaFormat,                      contentTypes,                        'whole' protocols,               streaming protocols
#ifdef WAV_SUPPORT   
    { JAVACALL_MEDIA_FORMAT_MS_PCM,         "audio/x-wav audio/wav",             JAVACALL_MEDIA_MEMORY_PROTOCOL|JAVACALL_MEDIA_FILE_LOCAL_PROTOCOL|JAVACALL_MEDIA_HTTP_PROTOCOL,  0 },
#endif
#ifdef MIDI_SUPPORT
    { JAVACALL_MEDIA_FORMAT_MIDI,           "audio/midi audio/mid audio/x-midi", JAVACALL_MEDIA_MEMORY_PROTOCOL|JAVACALL_MEDIA_FILE_LOCAL_PROTOCOL|JAVACALL_MEDIA_HTTP_PROTOCOL,  0 },
    { JAVACALL_MEDIA_FORMAT_SP_MIDI,        "audio/sp-midi",                     JAVACALL_MEDIA_MEMORY_PROTOCOL|JAVACALL_MEDIA_HTTP_PROTOCOL,  0 },
#endif
#ifdef AMR_SUPPORT  
    { JAVACALL_MEDIA_FORMAT_AMR,            "audio/amr",                         JAVACALL_MEDIA_MEMORY_PROTOCOL|JAVACALL_MEDIA_FILE_LOCAL_PROTOCOL|JAVACALL_MEDIA_HTTP_PROTOCOL,  0 },   
#endif
    { JAVACALL_MEDIA_FORMAT_MPEG1_LAYER3,   "audio/mp3 audio/mpeg",                         JAVACALL_MEDIA_MEMORY_PROTOCOL|JAVACALL_MEDIA_FILE_LOCAL_PROTOCOL|JAVACALL_MEDIA_HTTP_PROTOCOL,  0 },   
#ifdef AAC_SUPPORT
    { JAVACALL_MEDIA_FORMAT_MPEG2_AAC,      "audio/aac",                         JAVACALL_MEDIA_MEMORY_PROTOCOL|JAVACALL_MEDIA_FILE_LOCAL_PROTOCOL|JAVACALL_MEDIA_HTTP_PROTOCOL, 0 },
    { JAVACALL_MEDIA_FORMAT_MPEG_4_SVP,     "video/mp4",                         JAVACALL_MEDIA_MEMORY_PROTOCOL|JAVACALL_MEDIA_FILE_LOCAL_PROTOCOL|JAVACALL_MEDIA_HTTP_PROTOCOL, 0 },
#endif  
    { JAVACALL_MEDIA_FORMAT_VIDEO_3GPP,      "video/3gp",                        JAVACALL_MEDIA_MEMORY_PROTOCOL|JAVACALL_MEDIA_FILE_LOCAL_PROTOCOL|JAVACALL_MEDIA_HTTP_PROTOCOL, 0 },
#ifdef AVI_ENC_SUPPORT
    { JAVACALL_MEDIA_FORMAT_AVI,             "video/avi",                        JAVACALL_MEDIA_MEMORY_PROTOCOL|JAVACALL_MEDIA_FILE_LOCAL_PROTOCOL|JAVACALL_MEDIA_HTTP_PROTOCOL, 0 }, 
#endif
    { JAVACALL_MEDIA_FORMAT_M4A,             "audio/m4a",                        JAVACALL_MEDIA_MEMORY_PROTOCOL|JAVACALL_MEDIA_FILE_LOCAL_PROTOCOL|JAVACALL_MEDIA_HTTP_PROTOCOL, 0 }, 
#ifdef WAV_SUPPORT   
    { JAVACALL_MEDIA_FORMAT_CAPTURE_AUDIO,   "audio/x-wav",                      JAVACALL_MEDIA_CAPTURE_PROTOCOL, 0 },
#endif
    { JAVACALL_MEDIA_FORMAT_CAPTURE_VIDEO,   "video/3gp",                        JAVACALL_MEDIA_CAPTURE_PROTOCOL, 0 },
    { JAVACALL_MEDIA_FORMAT_CAPTURE_VIDEO,   "video/jpg",                        JAVACALL_MEDIA_CAPTURE_PROTOCOL, 0 },
    { NULL,                                  NULL,                               0,                               0 }
};

static MMIJAVA_MEDIA_INTERFACE_T* s_itfTable[] = {
    PNULL,   
    &g_video_itf,               // JAVACALL_VIDEO_MPEG4 = 1,   /** MPEG4 video     */
    &g_video_itf,               // JAVACALL_VIDEO_3GPP,        /** 3GPP video      */
#ifdef AVI_ENC_SUPPORT
    &g_video_itf,               // JAVACALL_VIDEO_avi,        /** 3GPP video      */
#endif
#ifdef MIDI_SUPPORT
    &g_audio_itf,               // JAVACALL_AUDIO_MIDI,        /** MIDI audio      */
#endif
#ifdef WAV_SUPPORT
    &g_audio_itf,               // JAVACALL_AUDIO_WAV,         /** WAV audio       */
#endif
    &g_audio_itf,               // JAVACALL_AUDIO_MP3,         /** MP3 audio       */
#ifdef AMR_SUPPORT
    &g_audio_itf,                       // JAVACALL_AUDIO_AMR,         /** AMR audio       */
#endif
    PNULL,                       // JAVACALL_AUDIO_MPEG4,       /** MPEG4 audio     */
    &g_audio_itf,                // JAVACALL_AUDIO_TONE,        /** JTS tone        */
    PNULL,                       // JAVACALL_AUDIO_QCELP,       /** QCELP audio     */
#ifdef AAC_SUPPORT
    &g_audio_itf,            //JAVACALL_AUDIO_M4A
    &g_audio_itf,                       // JAVACALL_AUDIO_AAC,         /** AAC audio       */
#endif
    &g_audio_recording_itf,        // JAVACALL_CAPTURE_AUDIO,     /** Audio capture   */
    &g_dc_dv_itf,//&g_dv_itf,              // JAVACALL_CAPTURE_VIDEO,     /** Video capture   */
#ifdef MIDI_SUPPORT
    PNULL,    // JAVACALL_INTERACTIVE_MIDI,  /** Interactive MIDI */
#endif
    PNULL,   //radio
    PNULL, //rtsp   
    PNULL,
    PNULL
};

// the only player handle between acquire/release device state
LOCAL MMIJAVA_NATIVE_HANDLE_T     *s_cur_handle = PNULL;
// player handle that was paused when lose focus. Because midlets may release player in pauseApp().
LOCAL MMIJAVA_NATIVE_HANDLE_T     *s_pending_handle = PNULL;
// record the state that should resume to after phone call. only playing and recording will resume to
// or to say, s_resume_to_playing_state will record the current state when the player is pending
// after lose focus, pauseAllMedia will be called, and current player will be in paused state until resume.
LOCAL MMIJAVA_MEDIA_PLAYING_STATE_E s_resume_to_playing_state = MMIJAVA_MEDIA_UNINIT;
LOCAL javacall_media_configuration s_cfg = {0};/*lint !e64 */
LOCAL javacall_int32 s_max_volume=MMI_AUDIO_VOLUME_MAX;
/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : Convert mime string to media type constants value
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL MMIJAVA_MEDIA_TYPE_T media_uri_to_type(const javacall_utf16* uri, long length);
/*****************************************************************************/
//  Description : Get temporary file name
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN get_temp_file_name(
                              MMIJAVA_NATIVE_HANDLE_T* handle_ptr,
                              MMIJAVA_MEDIA_TYPE_T mediaType, 
                              wchar* filename  //[OUT]
                              );
/*****************************************************************************/
//  Description :  media_mime_to_type
//  Global resource dependence : none
//  Author: 
//  Note:  
//       
/*****************************************************************************/
LOCAL MMIJAVA_MEDIA_TYPE_T media_mime_to_type( const char* mime);
LOCAL BOOLEAN get_media_temp_path(wchar* /* OUT */ rootPath, int* /* IN | OUT */ rootPathLen);
/*****************************************************************************/
//  Description :  set volume and set mute
//  Global resource dependence : none
//  Author: 
//  Note: 
//           @param handle    Handle to the library
// 
//           @retval JAVACALL_OK
//           @retval JAVACALL_FAIL   
 /*****************************************************************************/
LOCAL void MMIJAVA_SetVolume(javacall_handle handle);
/*****************************************************************************/
//  Description : is_unsupport_paused_media_type
//  Global resource dependence : none
//  Author: 
//  Note: 
//        @param handle    Handle to the library
//        @retval JAVACALL_OK      Success
//        @retval JAVACALL_FAIL    Fail
/*****************************************************************************/
//LOCAL BOOLEAN is_unsupport_paused_media_type( MMIJAVA_MEDIA_TYPE_T mediatype );

/**--------------------------------------------------------------------------*
 **                         Function Implement                               *
 **--------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : Perform global initialization operation
//  Global resource dependence : none
//  Author: 
//  Note: Call this function when VM starts
//        JOK      success
//        JFAIL    fail
/*****************************************************************************/
PUBLIC javacall_result javacall_media_initialize(void) 
{
    MMIFILE_HANDLE hFind;
    MMIFILE_FIND_DATA_T findData;
    int pathLen=MMIFILE_FULL_PATH_MAX_LEN;
    wchar path[MMIFILE_FULL_PATH_MAX_LEN+2];

    if (TRUE == get_media_temp_path(path, &pathLen))
    {
        if (FALSE == MMIAPIFMM_IsFolderExist(path, pathLen))
            MMIAPIFMM_CreateDir(path, pathLen);
        path[pathLen++]=javacall_get_file_separator();
        path[pathLen++] = '*';
        path[pathLen] = '\0';
        javacall_printf(LOG_STR("find=%s"), javacall_wstr2str(path));/*lint !e64 */
        hFind = MMIAPIFMM_FindFirstFile(path, pathLen, &findData);
        if (hFind != SFS_INVALID_HANDLE) {
            path[pathLen-1] = '\0';
            MMIAPICOM_Wstrcat(path, findData.name);
            javacall_printf(LOG_STR("find and delete %s"), javacall_wstr2str(path));/*lint !e64 */
            MMIAPIFMM_DeleteFile(path, NULL);
            while (SFS_NO_ERROR == MMIAPIFMM_FindNextFile(hFind, &findData)) {
                path[pathLen-1] = '\0';
                MMIAPICOM_Wstrcat(path, findData.name);
                javacall_printf(LOG_STR("find and delete %s"), javacall_wstr2str(path));/*lint !e64 */
                MMIAPIFMM_DeleteFile(path, NULL);
            }
            MMIAPIFMM_FindClose(hFind);
        }
    }
    if(g_media_handle_counter != 0)
    {    
        javacall_printf(LOG_STR("g_media_handle_counter = d%"),g_media_handle_counter);        
        g_media_handle_counter = 0;
    }
    javacall_printf(LOG_STR_LEAVE("ok"));
    return JAVACALL_OK;
}

/*****************************************************************************/
//  Description : Perfrom global free operaiton
//  Global resource dependence : none
//  Author: 
//  Note: Call this function when VM ends 
//        JOK      success
//        JFAIL    fail
/*****************************************************************************/
PUBLIC javacall_result javacall_media_finalize(void) 
{
    if(g_media_handle_counter != 0)
    {    
        javacall_printf(LOG_STR("g_media_handle_counter = d%"),g_media_handle_counter);        
    }
    javacall_printf(LOG_STR_LEAVE("ok"));
    return JAVACALL_OK;
}

/**********************************************************************************/
//  Description :   Configure MMAPI library
//                  Get multimedia configuration of the device.
//                  This function should return pointer to static consfiguration structure with
//                  static array of javacall_media_caps values.
//  Global resource dependence : none
//  Author: 
//  Note:
//           @retval JAVACALL_OK               success
//                   JAVACALL_INVALID_ARGUMENT if argument is NULL
/**********************************************************************************/
PUBLIC javacall_result javacall_media_get_configuration(
                    /*OUT*/ const javacall_media_configuration** configuration)
{
    javacall_printf(LOG_STR("call once"));
    s_cfg.audioEncoding = "encoding=audio/amr";
#ifdef SC6600L_ARM7
    s_cfg.videoEncoding = NULL;
    s_cfg.videoSnapshotEncoding = NULL;
#else
    s_cfg.videoEncoding = "encoding=3gp";
    s_cfg.videoSnapshotEncoding = "encoding=jpeg&quality=80";
#endif
    s_cfg.supportMixing = JAVACALL_FALSE;
    s_cfg.supportRecording = JAVACALL_TRUE;
    s_cfg.supportDeviceTone = JAVACALL_TRUE;
    s_cfg.supportDeviceMIDI = JAVACALL_FAIL;/*lint !e64 */
    s_cfg.supportCaptureRadio = JAVACALL_FAIL;/*lint !e64 */
    s_cfg.mediaCaps = s_media_caps;
    *configuration = &s_cfg;
    return JAVACALL_OK;
}

                    
/**********************************************************************************/
//  Description :  This function is called to get all the necessary return values from 
//                  the JavaCall Media functions that can run in asynchronous mode.
//                  This function is called every time the following situation occurs.
//                  A JSR-135 JavaCall API function returned JAVACALL_WOULD_BLOCK and continued
//                  its execution in asynchronous mode. Then it finished the execution and send the
//                  corresponding event to inform Java layer about it. Such events are described
//                  in the description of the enum javacall_media_notification_type after the
//                  event JAVACALL_EVENT_MEDIA_JAVA_EVENTS_MARKER. After the event Java
//                  layer calls javacall_media_get_event_data() to get the return values.
//  Global resource dependence : none
//  Author: 
//  Note:
//           @param handle        handle to the native player that the function having
//                                returned JAVACALL_WOULD_BLOCK was called for.
//           @param eventType     the type of the event, one of 
//                                javacall_media_notification_type (but greater than 
//                                JAVACALL_EVENT_MEDIA_JAVA_EVENTS_MARKER)
//           @param pResult       The event data passed as the param \a data to the
//                                function javanotify_on_media_notification() while
//                                sending the event
//           @param numArgs       the number of return values to get
//           @param args          the pointer to the array to copy the return values to
//
//           @retval JAVACALL_INVALID_ARGUMENT    bad arguments or the function should
//                                                not be called now for this native
//                                                player and eventType (no event has been
//                                                sent, see the function description)
//           @retval JAVACALL_OK                  Success
//           @retval JAVACALL_FAIL                General failure
//           @see JAVACALL_WOULD_BLOCK
//           @see javacall_media_notification_type
//           @see JAVACALL_EVENT_MEDIA_JAVA_EVENTS_MARKER
/**********************************************************************************/
PUBLIC javacall_result javacall_media_get_event_data(
                                                     javacall_handle handle, 
                                                     int eventType, 
                                                     void *pResult, 
                                                     int numArgs, 
                                                     void *args[]
                                                     )
{
    return JAVACALL_OK;
}


LOCAL BOOLEAN jc_convert_file_locator(javacall_const_utf16_string locator, int locatorLen, javacall_utf16_string dst, int dstLen)
{
    javacall_const_utf16_string prefix=L"file:///";
    const int prefixLen=8;

    if (MMIAPICOM_Wstrncmp(locator, prefix, prefixLen) != 0) {
        javacall_printf(LOG_STR_LEAVE("false, not start with file:///"));
        return FALSE;
    }
//dst, MMIFILE_FULL_PATH_MAX_LEN+2)) {
    if (jc_convert_fc2native_path(locator+prefixLen, locatorLen-prefixLen, dst, dstLen)) {
        javacall_printf(LOG_STR_LEAVE("true, path=%s"), javacall_wstr2str(dst));/*lint !e64 */
        return TRUE;
    }
    javacall_printf(LOG_STR_LEAVE("false, fail to convert to native"));
    return FALSE;
}

LOCAL BOOLEAN isSeekSupport(MMIJAVA_MEDIA_TYPE_T audio_type)
{
    BOOLEAN            result = FALSE;
	
    switch(audio_type)
    {
    case JAVACALL_AUDIO_MP3:
#ifdef MP3_SEEK_SUPPORT
        result = TRUE;
#endif
        break;
//    case MMIAUDIO_WMA:
//#ifdef WMA_SEEK_SUPPORT
//        result = TRUE;
//#endif
//        break;
    case JAVACALL_AUDIO_AAC:
#ifdef AAC_SEEK_SUPPORT
        result = TRUE;
#endif
        break;
    case JAVACALL_AUDIO_AMR:
#ifdef AMR_SEEK_SUPPORT
        result = TRUE;
#endif
        break;
    case JAVACALL_AUDIO_MIDI:
#ifdef MIDI_SEEK_SUPPORT
        result = TRUE;
#endif
        break;
    case JAVACALL_AUDIO_WAV:
#ifdef WAV_SEEK_SUPPORT
        result = TRUE;
#endif
        break;
    case JAVACALL_VIDEO_MPEG4:
    case JAVACALL_VIDEO_3GPP:
    case JAVACALL_VIDEO_AVI:
        result = TRUE;
        break;
    default:
//        javacall_printf(LOG_STR("type=%d, unknown"), audio_type);
        break;
    }

//    javacall_printf(LOG_STR_LEAVE("return=%d, type=%d"), result, audio_type);
    return result;
}
/*****************************************************************************/
//  Description : Java MMAPI call this function to create native media handler
//  Global resource dependence : none
//  Author: 
//  Note: This function is called at the first time to initialize native library.
//        You can do your own initializatio job from this function.
//        @param appID         Unique application ID for this playing
//        @param playerId      Unique player object ID for this playing
//                             This unique ID is generated by Java MMAPI library.
//        @param uri           URI unicode string to media data
//        @param uriLength     String length of URI
//        @param handle        Handle of native library.
//    
//       @retval JAVACALL_OK                      Success
//       @retval JAVACALL_CONNECTION_NOT_FOUND    Could not connect to the URL
//       @retval JAVACALL_IO_ERROR                IO error occurred while connecting
//                                                the URL or getting data 
//       @retval JAVACALL_INVALID_ARGUMENT        Invalid URL or other parameter
//       @retval JAVACALL_FAIL                    General failure or the following
//                                                situation. Porting Layer may
//                                                decide to reject the creation for
//                                                some reason. For example, if you
//                                                do not want media from some
//                                                pre-defined "bad" Internet site.
/*****************************************************************************/
PUBLIC javacall_result javacall_media_create(
                                             int appID,
                                             int playerID,
                                             javacall_const_utf16_string uri, 
                                             long uriLength,
                                             /*OUT*/ javacall_handle* handle
                                             )
{
    MMIJAVA_MEDIA_TYPE_T                 mediaType              = JAVACALL_END_OF_TYPE;
    MMIJAVA_NATIVE_HANDLE_T             * handle_ptr            = PNULL;

    javacall_printf(LOG_STR_ENTER("appid=%d, playerid=%d, uri=%s"), appID, playerID, javacall_wstr2str((short*)uri));
    if (PNULL == handle)
    {
        javacall_printf(LOG_STR_LEAVE("invalid argument, null handle"));
        return JAVACALL_INVALID_ARGUMENT;
    }

    if (g_media_handle_counter >= 20) {
        javacall_printf(LOG_STR_LEAVE("fail, two many handler created"));
        return JAVACALL_FAIL;
    }
    if (s_pending_handle != PNULL) {
        javacall_printf(LOG_STR_LEAVE("fail, there are pending handle=0x%x"), s_pending_handle);
        return JAVACALL_FAIL;
    }

    handle_ptr = SCI_ALLOCA(sizeof(MMIJAVA_NATIVE_HANDLE_T));
    if (PNULL == handle_ptr)
    {
        javacall_printf(LOG_STR_LEAVE("fail, out of memory"), handle_ptr);
        return JAVACALL_OUT_OF_MEMORY;
    }
    SCI_MEMSET(handle_ptr,0,sizeof(MMIJAVA_NATIVE_HANDLE_T));

    /* Mime type string to type constants */   
    if (PNULL != uri)
    {
        mediaType = media_uri_to_type(uri,uriLength);
    }
    else
    {
        mediaType = JAVACALL_END_OF_TYPE;
    }
    
    if ((mediaType != JAVACALL_END_OF_TYPE)
        && (TRUE == jc_convert_file_locator(uri, uriLength, handle_ptr->filename, MMIFILE_FULL_PATH_MAX_LEN+2))) {
        javacall_printf(LOG_STR("local file player:%s"), javacall_wstr2str(handle_ptr->filename));  /*lint !e64*/
        handle_ptr->isTempFile = FALSE;
    } else {
        get_temp_file_name(handle_ptr, mediaType,handle_ptr->filename);
        handle_ptr->isTempFile = TRUE;
    }
    handle_ptr->playerId = playerID;
    handle_ptr->AppID    = appID;

    handle_ptr->isForeground = TRUE;
    handle_ptr->mediatype = mediaType;
    handle_ptr->cur_time = 0;
    handle_ptr->rate = 100000;
    handle_ptr->defaultMediaTime = 0;
    handle_ptr->isStoppedByOther = FALSE;
    handle_ptr->isMute = FALSE;
    handle_ptr->playingState = MMIJAVA_MEDIA_UNINIT;
    handle_ptr->cur_vol = 100;
    handle_ptr->bufsize = 0;
    handle_ptr->wholecontentsize = -1;
    handle_ptr->left_size = -1;
    handle_ptr->buf = PNULL;
    
    handle_ptr->mediaItfPtr = s_itfTable[mediaType];

    
    *handle = (javacall_handle)handle_ptr;
    g_media_handle_counter++;


    javacall_printf(LOG_STR_LEAVE("ok, handle=0x%x, type=%d created"), handle_ptr, mediaType);
    return JAVACALL_OK;
}

/*****************************************************************************/
//  Description :  media_mime_to_type
//  Global resource dependence : none
//  Author: 
//  Note:  
//       
/*****************************************************************************/
LOCAL MMIJAVA_MEDIA_TYPE_T media_mime_to_type( const char* mime)
{
    uint32                   i           = 0;
    int32 mimeLen;
    char* mimeStr = PNULL;
    MMIJAVA_MEDIA_TYPE_T    media_type  = JAVACALL_END_OF_TYPE;

    mimeLen=(int32)MMIAPICOM_Wstrlen(mime);/*lint !e64 */
    if (mimeLen == 0) {
        javacall_printf(LOG_STR_LEAVE("invalid argument, mime length = 0"));
        return media_type;
    }
    mimeStr=SCI_ALLOCA(mimeLen+1);
    if (mimeStr == PNULL) {
        javacall_printf(LOG_STR_LEAVE("no memory"));
        return media_type;
    }
    SCI_MEMSET( mimeStr, 0, (mimeLen + 1) );
    MMI_WSTRNTOSTR( mimeStr, mimeLen, mime, mimeLen, mimeLen );/*lint !e64 */

    if (strcmp(mimeStr, "image/gif") == 0)
        return JAVACALL_IMAGE_GIF;
    for(i = 0; i < (sizeof(s_media_caps)/sizeof(javacall_media_caps)); i++ )
    {
        if (PNULL != strstr(s_media_caps[i].contentTypes, mimeStr))
        {
            javacall_printf(LOG_STR("i=%d, mediaFormat=%s"), i, s_media_caps[i].mediaFormat);/*lint !e64 */
            if (0 == MMIAPICOM_Stricmp((uint8*)s_media_caps[i].mediaFormat,(uint8*)JAVACALL_MEDIA_FORMAT_MS_PCM))
            {
#ifdef WAV_SUPPORT
                media_type = JAVACALL_AUDIO_WAV;
#endif
            }
#ifdef MIDI_SUPPORT
            else if (0 == MMIAPICOM_Stricmp((uint8*)s_media_caps[i].mediaFormat,(uint8*)JAVACALL_MEDIA_FORMAT_MIDI))
            {
                media_type = JAVACALL_AUDIO_MIDI;
            }
            else if (0 == MMIAPICOM_Stricmp((uint8*)s_media_caps[i].mediaFormat,(uint8*)JAVACALL_MEDIA_FORMAT_SP_MIDI))
            {
                media_type = JAVACALL_AUDIO_MIDI;
            }
#endif
//            else if (0 == MMIAPICOM_Stricmp(s_media_caps[i].mediaFormat,JAVACALL_MEDIA_FORMAT_TONE))
//            {
//                media_type = JAVACALL_AUDIO_TONE;
//            }
#ifdef AMR_SUPPORT
            else if (0 == MMIAPICOM_Stricmp((uint8*)s_media_caps[i].mediaFormat,(uint8*)JAVACALL_MEDIA_FORMAT_AMR))
            {
                media_type = JAVACALL_AUDIO_AMR;
            }
#endif
            else if (0 == MMIAPICOM_Stricmp((uint8*)s_media_caps[i].mediaFormat,(uint8*)JAVACALL_MEDIA_FORMAT_MPEG1_LAYER3))
            {
                media_type = JAVACALL_AUDIO_MP3;
            }
#ifdef AAC_SUPPORT
            else if (0 == MMIAPICOM_Stricmp((uint8*)s_media_caps[i].mediaFormat,(uint8*)JAVACALL_MEDIA_FORMAT_MPEG2_AAC))
            {
                media_type = JAVACALL_AUDIO_AAC;
            }
            else if (0 == MMIAPICOM_Stricmp((uint8*)s_media_caps[i].mediaFormat,(uint8*)JAVACALL_MEDIA_FORMAT_M4A))
            {
                media_type = JAVACALL_AUDIO_M4A;
            }
#endif
            else if (0 == MMIAPICOM_Stricmp((uint8*)s_media_caps[i].mediaFormat,(uint8*)JAVACALL_MEDIA_FORMAT_MPEG_4_SVP))
            {
                media_type = JAVACALL_VIDEO_MPEG4;
            }
            else if (0 == MMIAPICOM_Stricmp((uint8*)s_media_caps[i].mediaFormat,(uint8*)JAVACALL_MEDIA_FORMAT_VIDEO_3GPP))
            {
                media_type = JAVACALL_VIDEO_3GPP;
            }
#ifdef AVI_ENC_SUPPORT
            else if (0 == MMIAPICOM_Stricmp((uint8*)s_media_caps[i].mediaFormat,(uint8*)JAVACALL_MEDIA_FORMAT_AVI))
            {
                media_type = JAVACALL_VIDEO_AVI;
            }   
#endif
            else if (0 == MMIAPICOM_Stricmp((uint8*)s_media_caps[i].mediaFormat,(uint8*)JAVACALL_MEDIA_FORMAT_CAPTURE_AUDIO))
            {
                media_type = JAVACALL_CAPTURE_AUDIO;
            }
            else if (0 == MMIAPICOM_Stricmp((uint8*)s_media_caps[i].mediaFormat,(uint8*)JAVACALL_MEDIA_FORMAT_CAPTURE_VIDEO))
            {
                media_type = JAVACALL_CAPTURE_VIDEO;
            }
            break;
        }
    }
           
    SCI_FREE(mimeStr);
    return media_type;
}

/*****************************************************************************/
//  Description :  Realize native player.
//                 This function will be called by Java Layer to start Realize native player.
//  Global resource dependence : none
//  Author: 
//  Note:  
//        @param handle        Handle to the library
//        @param mime          Mime type unicode string. 
//                                 NULL if unknown
//        @param mimeLength    String length of media MIME type.
// 
//        @retval JAVACALL_OK
//        @retval JAVACALL_FAIL   
/*****************************************************************************/
PUBLIC javacall_result javacall_media_realize(
                                              javacall_handle handle,
                                              javacall_const_utf16_string mime,
                                              long mimeLength
                                              )
{    
    MMIJAVA_NATIVE_HANDLE_T     *handle_ptr     = (MMIJAVA_NATIVE_HANDLE_T*)handle;  

    javacall_printf(LOG_STR_ENTER("mime=%s"), javacall_nwstr2str(mime, mimeLength));    /*lint !e64*/

    if (PNULL == handle_ptr)
    {
        javacall_printf(LOG_STR_LEAVE("invalid argument, null handle"));
        return JAVACALL_INVALID_ARGUMENT;
    }

    if (s_pending_handle != PNULL) {
        javacall_printf(LOG_STR_LEAVE("fail, there are pending handle=0x%x"), s_pending_handle);
        return JAVACALL_FAIL;
    }

    if(JAVACALL_END_OF_TYPE == handle_ptr->mediatype)
    {
        if (PNULL != mime)
        {
            handle_ptr->mediatype = media_mime_to_type((char*)mime);
            if (JAVACALL_END_OF_TYPE == handle_ptr->mediatype)
            {
                javacall_printf(LOG_STR_LEAVE("ok, the type is not supported, let java done it"));
                return JAVACALL_OK;
            }
            get_temp_file_name(handle_ptr, handle_ptr->mediatype,handle_ptr->filename);
            handle_ptr->isTempFile = TRUE;
        }
    }     
             
  
    if ((PNULL == handle_ptr->mediaItfPtr) && (JAVACALL_END_OF_TYPE != handle_ptr->mediatype))
    {
        handle_ptr->mediaItfPtr = s_itfTable[handle_ptr->mediatype];        
    }    

    javacall_printf(LOG_STR("mediatype=%d, handle_ptr->mediaItfPtr->vptrBasic 0x%x"), handle_ptr->mediatype, handle_ptr->mediaItfPtr->vptrBasic);

/*
#define BUS_MONITOR_READ		0x01
#define BUS_MONITOR_WRITE		0x02

	setting.addr           = handle_ptr->mediaItfPtr->vptrBasic;
	setting.addr_mask      = 0x0;
	setting.rdata          = 0xffffffff;
	setting.rdata_mask     = 0x0;
	setting.wdata          = 0xffffffff;
	setting.wdata_mask     = 0x0;
	setting.data_size      = BUS_MONITOR_ALL_SIZE;
	setting.data_size_mask = BUS_MONITOR_ALL_SIZE;
	setting.option         = BUS_MONITOR_WRITE;
	setting.on_raise       = _monitor_callback;
	setting.data           = 0;

	bm_set_point(point, &setting);
*/
    javacall_printf(LOG_STR_LEAVE("ok"));
    return JAVACALL_OK;
}

/*****************************************************************************/
//  Description : Get the format type of media content
//  Global resource dependence : none
//  Author: 
//  Note:  
//          @param handle    Handle to the library 
//          @param format    Format type
// 
//          @retval JAVACALL_OK          Success
//          @retval JAVACALL_FAIL        Fail
/*****************************************************************************/
PUBLIC javacall_result javacall_media_get_format(
                                                 javacall_handle handle, 
                                                 /*OUT*/ javacall_media_format_type* format
                                                 )
{
    MMIJAVA_NATIVE_HANDLE_T     *handle_ptr     = (MMIJAVA_NATIVE_HANDLE_T*)handle;  

    if ((PNULL == handle_ptr)||(PNULL == format))
    {
        javacall_printf(LOG_STR_LEAVE("invalid argument, handle_ptr = %d, formate = %d"), handle_ptr, format);
        return JAVACALL_INVALID_ARGUMENT;
    }

    switch(handle_ptr->mediatype) 
    {
    case JAVACALL_AUDIO_MP3:
        *format = JAVACALL_MEDIA_FORMAT_MPEG1_LAYER3;
        break;
#ifdef WAV_SUPPORT
    case JAVACALL_AUDIO_WAV:
        *format = JAVACALL_MEDIA_FORMAT_MS_PCM;
        break;
#endif
#ifdef MIDI_SUPPORT
    case JAVACALL_AUDIO_MIDI:
        *format = JAVACALL_MEDIA_FORMAT_MIDI;
        break;
    case JAVACALL_INTERACTIVE_MIDI:
        *format = JAVACALL_MEDIA_FORMAT_DEVICE_MIDI;
        break;
#endif

#ifdef AMR_SUPPORT
    case JAVACALL_AUDIO_AMR:
        *format = JAVACALL_MEDIA_FORMAT_AMR;
        break;
#endif
    
    case JAVACALL_VIDEO_3GPP:
        *format = JAVACALL_MEDIA_FORMAT_VIDEO_3GPP;
        break;
    case JAVACALL_AUDIO_TONE:
        *format = JAVACALL_MEDIA_FORMAT_DEVICE_TONE;
        break;
    case JAVACALL_CAPTURE_RADIO:
        *format = JAVACALL_MEDIA_FORMAT_CAPTURE_RADIO;
        break;
        
    case JAVACALL_CAPTURE_VIDEO:
        *format = JAVACALL_MEDIA_FORMAT_CAPTURE_VIDEO;
        break;

    case JAVACALL_CAPTURE_AUDIO:
        *format  = JAVACALL_MEDIA_FORMAT_CAPTURE_AUDIO;
        break;
#ifdef AAC_SUPPORT
    case JAVACALL_AUDIO_AAC:
        *format = JAVACALL_MEDIA_FORMAT_MPEG2_AAC;
        break;

    case JAVACALL_AUDIO_M4A:
        *format = JAVACALL_MEDIA_FORMAT_M4A;
        break;
#endif
    case JAVACALL_VIDEO_MPEG4:
        *format = JAVACALL_MEDIA_FORMAT_MPEG_4_SVP;
        break;
    case JAVACALL_VIDEO_AVI:
        *format = JAVACALL_MEDIA_FORMAT_AVI;
        break;
    case JAVACALL_IMAGE_GIF:
        *format = JAVACALL_MEDIA_FORMAT_UNSUPPORTED;
        break;
    default:
        *format = JAVACALL_MEDIA_FORMAT_UNKNOWN;
        break;
    }

    javacall_printf(LOG_STR_LEAVE("ok, type=%d, format = %s"), handle_ptr->mediatype, *format);
    return JAVACALL_OK;

}

/*****************************************************************************/
//  Description :   Return bitmask of Media Controls supported by native player
// 
//                  Only Media Controls supported by native layer should be indicated
//  Global resource dependence : none
//  Author: 
//  Note: 
//          @param handle    Handle to the library 
//          @param controls  bitmasks for Media Control implemented in native layer
// 
//          @retval JAVACALL_OK          Success
//          @retval JAVACALL_FAIL        Fail
/*****************************************************************************/
PUBLIC javacall_result javacall_media_get_player_controls(
                                                          javacall_handle handle,
                                                          /*OUT*/ int* controls
                                                          )
{
    MMIJAVA_NATIVE_HANDLE_T     *handle_ptr     = (MMIJAVA_NATIVE_HANDLE_T*)handle;  

    if ((PNULL == handle_ptr)||(PNULL == controls))
    {
        javacall_printf(LOG_STR_LEAVE("invalid argument, handle_ptr = %d, controls = %d"), handle_ptr,controls);
        return JAVACALL_INVALID_ARGUMENT;
    }

    *controls |= JAVACALL_MEDIA_CTRL_VOLUME;

    if (TRUE == isSeekSupport(handle_ptr->mediatype))
        *controls |= JAVACALL_MEDIA_CTRL_STOPTIME;
        
    if ((JAVACALL_AUDIO_MIDI == handle_ptr->mediatype)
        ||(JAVACALL_AUDIO_WAV == handle_ptr->mediatype)
        ||(JAVACALL_AUDIO_MP3 == handle_ptr->mediatype)
        ||(JAVACALL_AUDIO_AMR == handle_ptr->mediatype)
        ||(JAVACALL_AUDIO_TONE == handle_ptr->mediatype)
        ||(JAVACALL_AUDIO_M4A == handle_ptr->mediatype)
        ||(JAVACALL_AUDIO_AAC == handle_ptr->mediatype))
    {
        *controls |= JAVACALL_MEDIA_CTRL_RATE;
    }
    if ((JAVACALL_AUDIO_MIDI == handle_ptr->mediatype)
        ||(JAVACALL_AUDIO_MP3 == handle_ptr->mediatype)
        ||(JAVACALL_AUDIO_AMR == handle_ptr->mediatype)
        ||(JAVACALL_AUDIO_AAC == handle_ptr->mediatype))
    {
        *controls |= JAVACALL_MEDIA_CTRL_METADATA;
    }
    if ((JAVACALL_CAPTURE_AUDIO == handle_ptr->mediatype)
        ||(JAVACALL_CAPTURE_VIDEO == handle_ptr->mediatype))
    {
        *controls |= JAVACALL_MEDIA_CTRL_RECORD;
    }

    if ((JAVACALL_VIDEO_3GPP == handle_ptr->mediatype)
        ||(JAVACALL_VIDEO_MPEG4 == handle_ptr->mediatype)
        ||(JAVACALL_CAPTURE_VIDEO == handle_ptr->mediatype)
        ||(JAVACALL_VIDEO_AVI == handle_ptr->mediatype))
    {
        *controls |= JAVACALL_MEDIA_CTRL_VIDEO;
    }

    if (JAVACALL_AUDIO_TONE == handle_ptr->mediatype)
    {
        *controls |= JAVACALL_MEDIA_CTRL_TONE;
    }
   
    javacall_printf(LOG_STR_LEAVE("ok, constrols=0x%x"), *controls);
    return JAVACALL_OK;
}

/*****************************************************************************/
//  Description : Close native media player that created by creat or creat2 API call
//  Global resource dependence : none
//  Author: 
//  Note: After this call, you can't use any other function in this library
//        @param handle  Handle to the library.
//        @retval JAVACALL_OK      success
//       @retval JAVACALL_FAIL    fail
/*****************************************************************************/
PUBLIC javacall_result javacall_media_close(javacall_handle handle) 
{
    BOOLEAN                      ret             = TRUE;
    MMIJAVA_NATIVE_HANDLE_T     *handle_ptr     = (MMIJAVA_NATIVE_HANDLE_T*)handle;  
    
    if (PNULL == handle_ptr)
    {
        javacall_printf(LOG_STR_LEAVE("invalid argument, null handle"));
        return JAVACALL_INVALID_ARGUMENT;
    }

    if ((PNULL != handle_ptr->mediaItfPtr)&&(PNULL != handle_ptr->mediaItfPtr->vptrBasic)) {
        ret = handle_ptr->mediaItfPtr->vptrBasic->close();
        javacall_printf(LOG_STR("close ret=%d"),ret);
    } else {
        javacall_printf(LOG_STR("handle=0x%x has no close()"), handle_ptr);
    }

    if (handle_ptr->isTempFile == TRUE)
        MMIAPIFMM_DeleteFile(handle_ptr->filename,NULL);
    handle_ptr->mediaItfPtr = PNULL;
    SCI_MEMSET(handle_ptr->filename,0,sizeof(handle_ptr->filename));       
    handle_ptr->isForeground = FALSE;
    handle_ptr->cur_time = 0;
    handle_ptr->rate = 100000;
    handle_ptr->defaultMediaTime = 0;
    handle_ptr->isStoppedByOther = FALSE;
    handle_ptr->isMute = FALSE;    
    handle_ptr->playingState = MMIJAVA_MEDIA_UNINIT;
    handle_ptr->playerId = 0;
    if (PNULL != handle_ptr->buf)
    {
        SCI_FREE(handle_ptr->buf);
        handle_ptr->buf = PNULL;
    }
    SCI_FREE(handle_ptr);
    g_media_handle_counter--;

    javacall_printf(LOG_STR_LEAVE("ret=%d, handle=0x%x closed, total %d handle"), ret, handle_ptr, g_media_handle_counter);
    return (ret?JAVACALL_OK:JAVACALL_FAIL);   
}


/*****************************************************************************/
//  Description : finally destroy native media player previously closed by
//  Global resource dependence : none
//  Author: 
//  Note: javacall_media_close. intended to be used by finalizer
//        @param handle  Handle to the library.
//        @retval JAVACALL_OK      Java VM will proceed as if there is no problem
//        @retval JAVACALL_FAIL    Java VM will raise the media exception
/*****************************************************************************/
PUBLIC javacall_result javacall_media_destroy(javacall_handle handle)
{
    javacall_printf(LOG_STR_LEAVE("ok"));    
    if(g_media_handle_counter != 0)
    {    
        javacall_printf(LOG_STR("g_media_handle_counter = d%"),g_media_handle_counter);        
    }
    return JAVACALL_OK;
}

/*****************************************************************************/
//  Description : Request to acquire device resources used to play media data.
//  Global resource dependence : none
//  Author: 
//  Note: You could implement this function to control device resource usage.
//        If there is no valid device resource to play media data, return JAVACALL_FAIL.
//        @param handle  Handle to the library.
//        @retval JAVACALL_OK      Can acquire device
//        @retval JAVACALL_FAIL    There is no valid device resource
/*****************************************************************************/
PUBLIC javacall_result javacall_media_acquire_device(javacall_handle handle) 
{   
    javacall_result             result          = JAVACALL_OK;
    MMIJAVA_NATIVE_HANDLE_T     *handle_ptr     = (MMIJAVA_NATIVE_HANDLE_T*)handle;  
    
    if (PNULL == handle_ptr)
    {
        javacall_printf(LOG_STR_LEAVE("invalid argument, null handle"));
        return JAVACALL_INVALID_ARGUMENT;
    }
    if (s_cur_handle  != PNULL) {
        javacall_printf(LOG_STR_LEAVE("fail, handle=0x%x, s_cur_handle=0x%x"), handle, s_cur_handle);
        return JAVACALL_FAIL;
    }

    if (s_pending_handle != PNULL) {
        javacall_printf(LOG_STR_LEAVE("fail, there are pending handle=0x%x"), s_pending_handle);
        return JAVACALL_FAIL;
    }
    s_cur_handle = handle_ptr;
    if ((PNULL != handle_ptr->mediaItfPtr)&&(PNULL != handle_ptr->mediaItfPtr->vptrBasic))
    {
        result = handle_ptr->mediaItfPtr->vptrBasic->acquire_device(handle_ptr->filename);
        if (result == JAVACALL_OK) {
            mmijava_lock_device_multimedia();
        }
    }
    javacall_printf(LOG_STR_LEAVE("result=%d, handle=0x%x, filename=%s"), result, handle_ptr, javacall_wstr2str(handle_ptr->filename)); /*lint !e64*/
    return result;
}
PUBLIC void MMIJAVA_NotifyMediaAcquireDevice(javacall_result result)
{
    javacall_printf(LOG_STR_ENTER("s_cur_handle=0x%x, status=%d"),s_cur_handle, result);
    if (PNULL != s_cur_handle)
    {
        javanotify_on_media_notification(JAVACALL_EVENT_MEDIA_DEVICE_ACQUIRED, 
                                         s_cur_handle->AppID,
                                         s_cur_handle->playerId, 
                                         result,
                                         PNULL);
        if (result == JAVACALL_OK)
        {
            mmijava_lock_device_multimedia();
        }   
    }
}

/*****************************************************************************/
//  Description : Release device resource. 
//  Global resource dependence : none
//  Author: 
//  Note: Java MMAPI call this function to release limited device resources.
//        @param handle  Handle to the library.
//        @retval JAVACALL_OK      Can release device
//        @retval JAVACALL_FAIL    Can't release device
/*****************************************************************************/
PUBLIC javacall_result javacall_media_release_device(javacall_handle handle) 
{
    javacall_result             result          = JAVACALL_OK;
    MMIJAVA_NATIVE_HANDLE_T     *handle_ptr     = (MMIJAVA_NATIVE_HANDLE_T*)handle;  

    javacall_printf(LOG_STR_ENTER("handle=0x%x, current=0x%x, pending=0x%x"), handle, s_cur_handle, s_pending_handle);

    if (PNULL == handle_ptr)
    {
        javacall_printf(LOG_STR_LEAVE("invalid argument, null handle"));
        return JAVACALL_INVALID_ARGUMENT;
    }

    if (s_cur_handle == handle_ptr)
        s_cur_handle = PNULL;

    if (s_pending_handle == handle_ptr) {
        s_pending_handle = PNULL;
        s_resume_to_playing_state = MMIJAVA_MEDIA_UNINIT;
    }

    handle_ptr->playingState = MMIJAVA_MEDIA_UNINIT;
    if ((PNULL != handle_ptr->mediaItfPtr)&&(PNULL != handle_ptr->mediaItfPtr->vptrBasic))
    {
        result = handle_ptr->mediaItfPtr->vptrBasic->release_device(handle_ptr->AppID, handle_ptr->playerId);
        //mmijava_unlock_device_multimedia();       //released when jvm is shuted down or paused
    }
    //mmijava_unlock_device_multimedia();
    javacall_printf(LOG_STR_LEAVE("result=%d, handle=0x%x"), result, handle_ptr);
    return result;
}
PUBLIC void MMIJAVA_NotifyMediaRelease(javacall_result result, int32 appid, int32 playerid)
{
    javacall_printf(LOG_STR_ENTER("s_cur_handle=0x%x, status=%d"),s_cur_handle, result);
    javanotify_on_media_notification(JAVACALL_EVENT_MEDIA_DEVICE_RELEASED,
                                         appid,
                                         playerid, 
                                         result,
                                         PNULL);
}

/*****************************************************************************/
//  Description :  Ask to the native layer if it will handle media download from specific URL.
//                  Is media download for specific URL (provided in javacall_media_create)
//                  will be handled by native layer or Java layer?
//                  If isHandled is JAVACALL_TRUE, Java do not call 
//                  javacall_media_do_buffering function
//                  In this case, native layer should handle all of data gathering by itself
//  Global resource dependence : none
//  Author: 
//  Note: 
//           @param handle    Handle to the library
//           @param isHandled JAVACALL_TRUE if native player will handle media download
//           @retval JAVACALL_OK      
//           @retval JAVACALL_FAIL   
/*****************************************************************************/
PUBLIC javacall_result javacall_media_download_handled_by_device(
                                                                 javacall_handle handle,
                                                                 /*OUT*/ javacall_bool* isHandled
                                                                 ) 
{
    MMIJAVA_NATIVE_HANDLE_T     *handle_ptr     = (MMIJAVA_NATIVE_HANDLE_T*)handle;     

    if ((PNULL == handle_ptr)||(PNULL == isHandled))
    {
        javacall_printf(LOG_STR_LEAVE("invalid argument, handle_ptr = %d, isHandled = %d"), handle_ptr, isHandled);
        return JAVACALL_INVALID_ARGUMENT;
    }
    
    if ((JAVACALL_CAPTURE_AUDIO == handle_ptr->mediatype) || (JAVACALL_CAPTURE_VIDEO == handle_ptr->mediatype)||(JAVACALL_AUDIO_TONE == handle_ptr->mediatype))
    {
        javacall_printf(LOG_STR_LEAVE("ok, download by native, mediatype=%d, "), handle_ptr->mediatype);
        *isHandled = JAVACALL_TRUE;
    } else if (handle_ptr->isTempFile == FALSE) {
        *isHandled = JAVACALL_TRUE;
    }
    else
    {
        javacall_printf(LOG_STR_LEAVE("ok, download by java, mediatype=%d, "), handle_ptr->mediatype);
        *isHandled = JAVACALL_FALSE;
    }
    
    return JAVACALL_OK;
   
}

/*****************************************************************************/
//  Description :  This function returns desired size of Java Layer buffer for downloaded media content
//                  It is possible if function returns different values for the same player in case of:
//                  - format of media data is unknown
//                  - format of media data is successfully discovered
//                  Java Layer will call this function two times to create/update java Layer buffers:
//                     1) before downloading media content
//                     2) after 
//  Global resource dependence : none
//  Author: 
//  Note: 
//          @param handle    Handle to the library
//          @param java_buffer_size  Desired size of java buffer
//          @param first_data_size  Size of the first chunk of media data, 
//                                   provided from Java to native
// 
//          @retval JAVACALL_OK
//          @retval JAVACALL_FAIL
//          @retval JAVACALL_NOT_IMPLEMENTED
/*****************************************************************************/
PUBLIC javacall_result javacall_media_get_java_buffer_size(
                                                           javacall_handle handle,
                                                           /*OUT*/ long* java_buffer_size, 
                                                           /*OUT*/ long* first_chunk_size
                                                           )
{
    MMIJAVA_NATIVE_HANDLE_T     *handle_ptr     = (MMIJAVA_NATIVE_HANDLE_T*)handle; 

    if ((PNULL == handle_ptr)||(PNULL == java_buffer_size)||(PNULL == first_chunk_size))
    {
        javacall_printf(LOG_STR_LEAVE("invalid argument, handle_ptr = %d, java_buff_size = %d, first_chunk_size = %d"),handle_ptr, java_buffer_size, first_chunk_size);
        return JAVACALL_INVALID_ARGUMENT;
    }

    if (-1 == handle_ptr->wholecontentsize)
    {
        *java_buffer_size = MMIJAVA_BUFFER_MIN_SIXE;
        *first_chunk_size = MMIJAVA_BUFFER_MIN_SIXE;
    }
    else if (MMIJAVA_BUFFER_MIN_SIXE < handle_ptr->wholecontentsize)
    {
        *java_buffer_size = MMIJAVA_BUFFER_MIN_SIXE;
        *first_chunk_size = MMIJAVA_BUFFER_MIN_SIXE;
    }
    else
    {
        *java_buffer_size = handle_ptr->wholecontentsize;
        *first_chunk_size = handle_ptr->wholecontentsize;
    }
    javacall_printf(LOG_STR_LEAVE("ok, return size=%d, whole size=%d"), *java_buffer_size, handle_ptr->wholecontentsize);
    return JAVACALL_OK;
}

/*****************************************************************************/
//  Description :  This function is called by Java Layer to notify javacall implementation about 
//                  whole size of media content. This function is called in prefetch stage if 
//                  whole size of media content is known only.
//  Global resource dependence : none
//  Author: 
//  Note: 
//          @param handle    Handle to the library
//          @param whole_content_size  size of whole media content
// 
//          @retval JAVACALL_OK
//          @retval JAVACALL_FAIL
/*****************************************************************************/
PUBLIC javacall_result javacall_media_set_whole_content_size(
                                                             javacall_handle handle,
                                                             long whole_content_size
                                                             )
{
    MMIJAVA_NATIVE_HANDLE_T     *handle_ptr     = (MMIJAVA_NATIVE_HANDLE_T*)handle; 
    
    if (PNULL == handle_ptr)
    {
        javacall_printf(LOG_STR_LEAVE("invalid argument, null handle"));
        return JAVACALL_INVALID_ARGUMENT;
    }

    handle_ptr->wholecontentsize = whole_content_size;
    handle_ptr->left_size = whole_content_size;

    javacall_printf(LOG_STR_LEAVE("ok, whole_content_size=%d"), whole_content_size);
    return JAVACALL_OK;    
}

/*****************************************************************************/
//  Description :  Get native buffer address to store media content
//  Global resource dependence : none
//  Author: 
//  Note: 
//          @param handle    Handle to the library
//          @param buffer    Native layer provides address of data buffer for media content. 
//                           Java layer will store downloaded media data to the provided buffer.
//                           The size of data stored in the buffer should be equal or divisible 
//                           by minimum media data chunk size and less or equal to max_size
//          @param max_size  The maximum size of data can be stored in the buffer
// 
//          @retval JAVACALL_OK
//          @retval JAVACALL_FAIL   
/*****************************************************************************/
PUBLIC javacall_result javacall_media_get_buffer_address(
                                                         javacall_handle handle, 
                                                         /*OUT*/ const void** buffer, 
                                                         /*OUT*/ long* max_size
                                                         )
{
    MMIJAVA_NATIVE_HANDLE_T     *handle_ptr     = (MMIJAVA_NATIVE_HANDLE_T*)handle;      

    if ((PNULL == handle_ptr)||(PNULL == max_size)||(PNULL == buffer))
    {
        javacall_printf(LOG_STR("handle_ptr = %d"),handle_ptr);
        return JAVACALL_INVALID_ARGUMENT;
    }

    if (handle_ptr->buf == PNULL)
    {
        handle_ptr->buf = SCI_ALLOCA(MMIJAVA_BUFFER_MIN_SIXE);
        if (PNULL == handle_ptr->buf)
        {
            javacall_printf(LOG_STR_LEAVE("there is no space"));
            return JAVACALL_OUT_OF_MEMORY;
        }
    }
    *buffer = handle_ptr->buf;
    *max_size = MMIJAVA_BUFFER_MIN_SIXE; 
    return JAVACALL_OK;
}

/*****************************************************************************/
//  Description : Java MMAPI call this function to send media data to this library
//                  This function can be called multiple time to send large media data
//                  Native library could implement buffering by using any method (for example: file, heap and etc...)
//                  And, buffering occured in sequentially. not randomly.
//  Global resource dependence : none
//  Author: 
//  Note: When there is no more data, the buffer is set to NULL and the length to -1,OEM should care about this case.
//        @param handle    Handle to the library
//        @param buffer    Media data buffer pointer,Can be NULL at end of buffering.
//        @param length    Length of media data, Can be -1 at end of buffering.If success returns length of data processed.
//        @param need_more_data    returns JAVACALL_FALSE if no more data is required at the moment, otherwise returns JAVACALL_TRUE
//        @param next_chunk_size   next expected buffering data size
//                                 must be divisible by this value
//        @retval JAVACALL_OK
//        @retval JAVACALL_FAIL
//        @retval JAVACALL_INVALID_ARGUMENT
/*****************************************************************************/
PUBLIC javacall_result javacall_media_do_buffering(
                                                   javacall_handle handle, 
                                                   const void* buffer, 
                                                   long *length, //INOUT
                                                   javacall_bool* need_more_data,//OUT
                                                   long* next_chunk_size //OUT
                                                   )
{
    MMIJAVA_NATIVE_HANDLE_T     * handle_ptr    = (MMIJAVA_NATIVE_HANDLE_T*)handle;   
    MMIFILE_HANDLE        file_handle     = 0;
    uint32                       write_size      = 0;    
    MMIFILE_ERROR_E             fret;

    if ((PNULL == handle_ptr)||(PNULL == length))
    {
        javacall_printf(LOG_STR_LEAVE("invalid argument, length = %p, handle_ptr = %p"), length, handle_ptr);
        return JAVACALL_INVALID_ARGUMENT;
    }

    //javacall_printf(LOG_STR_ENTER("length=%d, buffer=%p"), *length, buffer);
    if ((-1 == *length)||(PNULL == buffer))
    {
        //no more data to buffering
        if (PNULL != handle_ptr->buf)
        {
            SCI_FREE(handle_ptr->buf);
            handle_ptr->buf = PNULL;
        }
        javacall_printf(LOG_STR_LEAVE("ok, end of buffer"));
//            MMIAPIFMM_CloseFile(file_handle);
//            file_handle = 0;
        return JAVACALL_OK;
    }
    
    if ((PNULL == need_more_data)||(PNULL == next_chunk_size))
    {
        javacall_printf(LOG_STR_LEAVE("invalid argument, need_more_data = %p, next_chunk_size = %p"), need_more_data, next_chunk_size);
        return JAVACALL_INVALID_ARGUMENT;
    }
//    if(0 == file_handle)
//    {    
//        javacall_printf(LOG_STR("create new file handle"));
//        //create file hanle at first time, keep being open ,util buffering is finished
//        file_handle = MMIAPIFMM_CreateFile(handle_ptr->filename,SFS_MODE_WRITE|SFS_MODE_READ|SFS_MODE_OPEN_ALWAYS, NULL, NULL);/*lint !e655*/
//    }   
        file_handle = MMIAPIFMM_CreateFile(handle_ptr->filename,SFS_MODE_WRITE|SFS_MODE_READ|SFS_MODE_OPEN_ALWAYS, NULL, NULL);/*lint !e655*/
//    }   
    if (SFS_INVALID_HANDLE == file_handle)
    {    
        javacall_printf(LOG_STR_LEAVE("fail, create file=%s error"), javacall_wstr2str(handle_ptr->filename));/*lint !e64 */
        return JAVACALL_FAIL;
    }
        
    javacall_printf(LOG_STR("length=%d, left=%d, whole=%d"), *length, handle_ptr->left_size, handle_ptr->wholecontentsize);
    if ((handle_ptr->wholecontentsize != -1) && (handle_ptr->left_size != -1))
        fret=(MMIFILE_ERROR_E)MMIAPIFMM_SetFilePointer(file_handle, (handle_ptr->wholecontentsize-handle_ptr->left_size), SFS_SEEK_BEGIN);
    else
        fret=(MMIFILE_ERROR_E)MMIAPIFMM_SetFilePointer(file_handle, 0, SFS_SEEK_END);
    if(SFS_ERROR_NONE != fret)
    {
        MMIAPIFMM_CloseFile(file_handle);
//        file_handle = 0;
        javacall_printf(LOG_STR_LEAVE("fail, set file pointer=%d error"), file_handle);
        return JAVACALL_FAIL;
    }
    //todo: may be length > max buffer size
    fret=MMIAPIFMM_WriteFile(file_handle,buffer, *length,&write_size,NULL);
    MMIAPIFMM_CloseFile(file_handle);
    if(SFS_ERROR_NONE != fret)
    {
        javacall_printf(LOG_STR_LEAVE("fail, write file=%s return %d, wrote=%d"), javacall_wstr2str(handle_ptr->filename), fret, write_size);/*lint !e64 */
//        MMIAPIFMM_CloseFile(file_handle);
//        file_handle = 0;
        return JAVACALL_FAIL;
    }   

    if ((handle_ptr->wholecontentsize != -1) && (handle_ptr->left_size != -1))
    {
        *length = write_size;
        if ((int32)write_size >= handle_ptr->left_size)
        {
            *need_more_data = JAVACALL_FALSE;     
            handle_ptr->left_size = 0;
            *next_chunk_size = 0;
        }
        else
        {
            *need_more_data = JAVACALL_TRUE;
            handle_ptr->left_size = handle_ptr->left_size - (int32)write_size;
            if (handle_ptr->left_size >= MMIJAVA_BUFFER_MIN_SIXE)
            {
                *next_chunk_size = MMIJAVA_BUFFER_MIN_SIXE;
            }
            else
            {
                *next_chunk_size = handle_ptr->left_size;
            }
        }
    }
    else
    {
        *need_more_data = JAVACALL_TRUE;
        *next_chunk_size = MMIJAVA_BUFFER_MIN_SIXE;
    }
    javacall_printf(LOG_STR_LEAVE("ok, wrote=%d, left=%d, next=%d, need_data=%d"),*length,handle_ptr->left_size,*next_chunk_size,*need_more_data);
    return JAVACALL_OK;
}


/*****************************************************************************/
//  Description : MMAPI call this function to clear(delete) buffered media data
//                You have to clear any resources created from previous buffering
//  Global resource dependence : none
//  Author: 
//  Note: 
//        @param handle    Handle to the library
//        @retval JAVACALL_OK      Can clear buffer
//        @retval JAVACALL_FAIL    Can't clear buffer
/*****************************************************************************/
PUBLIC javacall_result javacall_media_clear_buffer(javacall_handle handle) 
{
    MMIJAVA_NATIVE_HANDLE_T     * handle_ptr    = (MMIJAVA_NATIVE_HANDLE_T*)handle;   
    
    javacall_printf(LOG_STR_ENTER("handle_ptr = %x"), handle_ptr);
    if (PNULL == handle_ptr)
    {
        javacall_printf(LOG_STR_LEAVE("invalid argument, null handle"));
        return JAVACALL_INVALID_ARGUMENT;
    }

    if (handle_ptr->isTempFile == TRUE)
    MMIAPIFMM_DeleteFile(handle_ptr->filename,NULL);
    if (PNULL != handle_ptr->buf)
    {
        SCI_FREE(handle_ptr->buf);
        handle_ptr->buf = PNULL;
    }
    return JAVACALL_OK;
}

/*****************************************************************************/
//  Description :  Prefetch native player. 
//                 This function will be called by Java Layer to Prefetch native player.
//  Global resource dependence : none
//  Author: 
//  Note: 
//           @param handle    Handle to the library
// 
//           @retval JAVACALL_OK
//           @retval JAVACALL_FAIL   
 /*****************************************************************************/
PUBLIC javacall_result javacall_media_prefetch(javacall_handle handle)
{
    javacall_printf(LOG_STR_LEAVE("ok"));
    return JAVACALL_OK;
}

/*****************************************************************************/
//  Description : Start media playing
//  Global resource dependence : none
//  Author: 
//  Note: 
//        @param handle    Handle to the library
//        @retval JAVACALL_OK      Success
//        @retval JAVACALL_FAIL    Fail
/*****************************************************************************/
PUBLIC javacall_result javacall_media_start(javacall_handle handle) 
{
    javacall_result             result          = JAVACALL_OK;
    MMIJAVA_NATIVE_HANDLE_T     *handle_ptr     = (MMIJAVA_NATIVE_HANDLE_T*)handle;
    
    if (PNULL == handle_ptr)
    {
        javacall_printf(LOG_STR_LEAVE("invalid argument, null handle"));
        return JAVACALL_INVALID_ARGUMENT;
    }

    javacall_printf(LOG_STR_ENTER("handle=0x%x, playingState=%d"), handle, handle_ptr->playingState);
    if ((PNULL != handle_ptr->mediaItfPtr)&&(PNULL != handle_ptr->mediaItfPtr->vptrBasic))
    {
     //no need to check here, mmijava_audio.c audio_start will check
//      if (MMIAPISET_GetCurRingType() != MMISET_RING_TYPE_JAVA)
//         {
//	        if(MMIAPISET_IsPermitPlayRing(MMISET_RING_TYPE_JAVA)&&
//	            !MMIAUDIO_IsBgPlayPaused(MMIBGPLAY_TYPE_JAVA_BG))
//	        {
            result = handle_ptr->mediaItfPtr->vptrBasic->start(handle_ptr->filename,handle_ptr->mediatype,handle_ptr->cur_time);
//	        }
//	        else
//	        {
//	            ret = FALSE;
//	        }
   }
    
    if (result == JAVACALL_OK)
    {
        handle_ptr->playingState = MMIJAVA_MEDIA_PLAYING;
    }   
    
    javacall_printf(LOG_STR_LEAVE("result=%d, handle=0x%x"), result, handle_ptr);
    return result;
}

PUBLIC void MMIJAVA_NotifyMediaStart(javacall_result result)
{
    javacall_printf(LOG_STR_ENTER("s_cur_handle=0x%x, result=%d"),s_cur_handle, result);
    if (PNULL != s_cur_handle)
    {
        javanotify_on_media_notification(JAVACALL_EVENT_MEDIA_STARTED, 
                                         s_cur_handle->AppID,
                                         s_cur_handle->playerId, 
                                         result,
                                         PNULL);
        if (result == JAVACALL_OK)
        {
            s_cur_handle->playingState = MMIJAVA_MEDIA_PLAYING;
        }   
    }
}

/*****************************************************************************/
//  Description : Stop media playing
//  Global resource dependence : none
//  Author: 
//  Note: 
//        @param handle    Handle to the library
//        @retval JAVACALL_OK      Success
//        @retval JAVACALL_FAIL    Fail
/*****************************************************************************/
PUBLIC javacall_result javacall_media_stop(javacall_handle handle) 
{
    javacall_result             result          = JAVACALL_OK;
    MMIJAVA_NATIVE_HANDLE_T     *handle_ptr     = (MMIJAVA_NATIVE_HANDLE_T*)handle;
   
    if (PNULL == handle_ptr)
    {
        javacall_printf(LOG_STR_LEAVE("invalid argument, null handle"));
        return JAVACALL_INVALID_ARGUMENT;
    }

    if ((PNULL != handle_ptr->mediaItfPtr)&&(PNULL != handle_ptr->mediaItfPtr->vptrBasic))
    {
        handle_ptr->cur_time = 0;
        result = handle_ptr->mediaItfPtr->vptrBasic->stop();
    }
    handle_ptr->playingState = MMIJAVA_MEDIA_UNINIT;

    javacall_printf(LOG_STR_LEAVE("result=%d, handle=0x%x"), result, handle_ptr);
    return result;
}
PUBLIC void MMIJAVA_NotifyMediaStop(javacall_result result)
{
    javacall_printf(LOG_STR_ENTER("s_cur_handle=0x%x, result=%d"),s_cur_handle, result);
    if (PNULL != s_cur_handle)
    {
        javanotify_on_media_notification(JAVACALL_EVENT_MEDIA_STOPPED, 
                                         s_cur_handle->AppID,
                                         s_cur_handle->playerId, 
                                         result,
                                         PNULL);
    }
}

/*****************************************************************************/
//  Description : is_unsupport_paused_media_type
//  Global resource dependence : none
//  Author: 
//  Note: 
//        @param handle    Handle to the library
//        @retval JAVACALL_OK      Success
//        @retval JAVACALL_FAIL    Fail
/*****************************************************************************/
#if 0
LOCAL BOOLEAN is_unsupport_paused_media_type( MMIJAVA_MEDIA_TYPE_T mediatype )
{
#ifdef MIDI_SUPPORT	
    if ( JAVACALL_AUDIO_MIDI == mediatype)
    {
        return TRUE;
    }
#endif
    
#ifdef WAV_SUPPORT		
    if ( JAVACALL_AUDIO_WAV == mediatype)
    {
        return TRUE;
    }
#endif
    
#ifdef AMR_SUPPORT	
    if ( JAVACALL_AUDIO_AMR == mediatype)
    {
        return TRUE;
    }          
#endif				
    
    return FALSE;
}
#endif

/*****************************************************************************/
//  Description : Pause media playing
//  Global resource dependence : none
//  Author: 
//  Note: 
//        @param handle    Handle to the library
//        @retval JAVACALL_OK      Success
//        @retval JAVACALL_FAIL    Fail
/*****************************************************************************/
PUBLIC javacall_result javacall_media_pause(javacall_handle handle) 
{
    javacall_result             result          = JAVACALL_OK;
    MMIJAVA_NATIVE_HANDLE_T     *handle_ptr     = (MMIJAVA_NATIVE_HANDLE_T*)handle;
   
    
    if (PNULL == handle_ptr)
    {
        javacall_printf(LOG_STR_LEAVE("invalid argument, null handle"));
        return JAVACALL_INVALID_ARGUMENT;
    }

    javacall_printf(LOG_STR_ENTER("handle=0x%x, playingState=%d"), handle, handle_ptr->playingState);

    if (handle_ptr == s_pending_handle)
    {
        javacall_printf(LOG_STR("handle=0x%x is pending handle, state=%d, resume to=%d"), handle_ptr, handle_ptr->playingState, s_resume_to_playing_state);
        if (s_resume_to_playing_state == MMIJAVA_MEDIA_PLAYING)
        {        
            s_resume_to_playing_state = MMIJAVA_MEDIA_PAUSED;
            javacall_printf(LOG_STR("ok, pending player set resume to pause"));
        }    
        // pending handle has already be paused, so let's return
        return JAVACALL_OK;
    }

    if (handle_ptr->playingState != MMIJAVA_MEDIA_PLAYING) {
        javacall_printf(LOG_STR_LEAVE("ok, no need to pause, playingState=%d"), handle_ptr->playingState);
        return JAVACALL_OK;
    }

    if ((PNULL != handle_ptr->mediaItfPtr)&&(PNULL != handle_ptr->mediaItfPtr->vptrBasic))
    {
        result = handle_ptr->mediaItfPtr->vptrBasic->pause();
    }

    if (result == JAVACALL_OK)
        handle_ptr->playingState = MMIJAVA_MEDIA_PAUSED;

    javacall_printf(LOG_STR_LEAVE("result=%d, handle=0x%x"), result, handle_ptr);
    return result;
}

PUBLIC void MMIJAVA_NotifyMediaPause(javacall_result result)
{
    javacall_printf(LOG_STR_ENTER("s_cur_handle=0x%x, status=%d"),s_cur_handle, result);
    if (PNULL != s_cur_handle)
    {
        javanotify_on_media_notification(JAVACALL_EVENT_MEDIA_STOPPED, 
                                      s_cur_handle->AppID,
                                     s_cur_handle->playerId, 
                                     result,
                                     PNULL);
        if (result == JAVACALL_OK)
        {
            s_cur_handle->playingState = MMIJAVA_MEDIA_PAUSED;
        }   
    }
}

/*****************************************************************************/
//  Description : Resume media playing
//  Global resource dependence : none
//  Author: 
//  Note: 
//        @param handle    Handle to the library
//        @retval JAVACALL_OK      Success
//        @retval JAVACALL_FAIL    Fail
/*****************************************************************************/
PUBLIC javacall_result javacall_media_resume(javacall_handle handle) 
{
    javacall_result             result          = JAVACALL_OK;
    MMIJAVA_NATIVE_HANDLE_T     *handle_ptr     = (MMIJAVA_NATIVE_HANDLE_T*)handle;

    if (PNULL == handle_ptr)
    {
        javacall_printf(LOG_STR_LEAVE("invalid argument, null handle"));
        return JAVACALL_INVALID_ARGUMENT;
    }
    javacall_printf(LOG_STR_ENTER("handle=0x%x, playingState=%d"), handle, handle_ptr->playingState);
    mmijava_lock_device_multimedia();
    
#ifdef CMCC_UI_STYLE
    javacall_media_set_mute(handle, JAVACALL_FALSE);
#endif

    if (handle_ptr == s_pending_handle)
    {
        javacall_printf(LOG_STR("handle=0x%x, state=%d, resume to=%d"), handle_ptr, handle_ptr->playingState, s_resume_to_playing_state);
        if (s_resume_to_playing_state == MMIJAVA_MEDIA_PAUSED)
        {
            s_resume_to_playing_state=MMIJAVA_MEDIA_PLAYING;
            javacall_printf(LOG_STR("ok, pending player set resume to playing"));
        }
        return JAVACALL_OK;
    }
    if ((PNULL != handle_ptr->mediaItfPtr)&&(PNULL != handle_ptr->mediaItfPtr->vptrBasic))
    {
        MMIJAVA_SetVolume(handle_ptr);      //set volume
        result = handle_ptr->mediaItfPtr->vptrBasic->resume();
    }

    if (result == JAVACALL_OK)
        handle_ptr->playingState = MMIJAVA_MEDIA_PLAYING;

    javacall_printf(LOG_STR_LEAVE("result=%d, handle=0x%x"), result, handle_ptr);
    return result;
}

PUBLIC void MMIJAVA_NotifyMediaResume(javacall_result result)
{
    javacall_printf(LOG_STR_ENTER("s_cur_handle=0x%x, status=%d"),s_cur_handle, result);
    if (PNULL != s_cur_handle)
    {
        javanotify_on_media_notification(JAVACALL_EVENT_MEDIA_STARTED, 
                                      s_cur_handle->AppID,
                                     s_cur_handle->playerId, 
                                     result,
                                     PNULL);
        if (result == JAVACALL_OK)
        {
            s_cur_handle->playingState = MMIJAVA_MEDIA_PLAYING;
        }   
    }
}

/*****************************************************************************/
//  Description : Get current playing media time
//  Global resource dependence : none
//  Author: 
//  Note: 
//        @param handle    Handle to the library
//        @param ms        current media time in ms
//        @retval JAVACALL_OK      Success
//        @retval JAVACALL_FAIL    Fail
/*****************************************************************************/
PUBLIC javacall_result javacall_media_get_time(javacall_handle handle, long* ms)
{
    MMIJAVA_NATIVE_HANDLE_T     *handle_ptr     = (MMIJAVA_NATIVE_HANDLE_T*)handle;
   
    if ((PNULL == handle_ptr)||(PNULL == ms))
    {
        javacall_printf(LOG_STR_LEAVE("handle=0x%x, ms=%p"), handle_ptr);
        javacall_printf(LOG_STR_LEAVE("invalid argument,  ms=%p"),  *ms);   /*lint !e613*/
        return JAVACALL_INVALID_ARGUMENT;
    }

    if (isSeekSupport(handle_ptr->mediatype) != TRUE) {
        *ms = -1;
        javacall_printf(LOG_STR_LEAVE(" handle=0x%x does not support seek, return time=%d"), handle, *ms);
        javacall_printf(LOG_STR_LEAVE(" not support seek, return time=%d"),  *ms);
        return JAVACALL_OK;
    }        
    *ms=handle_ptr->defaultMediaTime;
    if (handle_ptr->isStoppedByOther)
    {
        javacall_printf(LOG_STR_LEAVE("ok, stopped by others, handle=0x%x, return time=%d"), handle, *ms);
         javacall_printf(LOG_STR_LEAVE("stopped by others, return time=%d"),  *ms);
        return JAVACALL_OK;
    }

    if (handle_ptr->playingState == MMIJAVA_MEDIA_UNINIT) {
        javacall_printf(LOG_STR_LEAVE("ok, not started, handle=0x%x, return defaultMediaTime = %d"), handle_ptr, handle_ptr->defaultMediaTime);
        javacall_printf(LOG_STR_LEAVE("not started,  return defaultMediaTime = %d"),  handle_ptr->defaultMediaTime);
        *ms = handle_ptr->defaultMediaTime;
 
        return JAVACALL_OK;
    }
 
    if ((PNULL != handle_ptr->mediaItfPtr)&&(PNULL != handle_ptr->mediaItfPtr->vptrBasic))
    {
        *ms = handle_ptr->mediaItfPtr->vptrBasic->get_time();
        if (*ms == -1) {
            //get_time() failed
            *ms = handle_ptr->defaultMediaTime;
        } else if (*ms != 0) {
            handle_ptr->defaultMediaTime = *ms;
        }
        //javacall_printf(LOG_STR_LEAVE("ok, handle=0x%x,  ms=%d"), handle_ptr, *ms);
       // javacall_printf(LOG_STR_LEAVE("ok,  ms=%d"),  *ms);
        return JAVACALL_OK;
    }

    javacall_printf(LOG_STR_LEAVE("ok, not support get_time, handle=0x%x, return time=%d"), handle_ptr, *ms);
    javacall_printf(LOG_STR_LEAVE("not get_time, return time=%d"), *ms);
    return JAVACALL_OK;
}
/*****************************************************************************/
//  Description : Seek to specified time
//  Global resource dependence : none
//  Author: 
//  Note: 
//        @param handle    Handle to the library
//        @param ms        Seek position as ms time
//        @return          If success return time in ms else return -1
/*****************************************************************************/
PUBLIC javacall_result javacall_media_set_time(javacall_handle handle, long *ms) 
{
    int32                        ret            = 0;
    MMIJAVA_NATIVE_HANDLE_T     *handle_ptr     = (MMIJAVA_NATIVE_HANDLE_T*)handle;  
    
    if ((PNULL == handle_ptr)||(PNULL == ms))
    {
        javacall_printf(LOG_STR_LEAVE("invalid argument, hande_ptr=%d, ms = %d"), handle_ptr, ms);
        return JAVACALL_INVALID_ARGUMENT;
    }
    javacall_printf(LOG_STR_ENTER("ms=%d, state=%d"), *ms, handle_ptr->playingState);
    if (handle_ptr->isStoppedByOther)
    {
        handle_ptr->cur_time = *ms;
        handle_ptr->defaultMediaTime = *ms;
        javacall_printf(LOG_STR_LEAVE("ok, handle=0x%x has been stopped by others, set time=%d"), handle, *ms);
        return JAVACALL_OK;
    }
    if (isSeekSupport(handle_ptr->mediatype) != TRUE) {
        *ms = -1;
        javacall_printf(LOG_STR_LEAVE("fail, handle=0x%x does not support seek, return time=%d"), handle, *ms);
        return JAVACALL_FAIL;
    }        

    if ((handle_ptr->playingState != MMIJAVA_MEDIA_PLAYING)
            && (handle_ptr->playingState != MMIJAVA_MEDIA_RECORDING)
            && *ms == 0) {
        handle_ptr->cur_time = *ms;
        handle_ptr->defaultMediaTime = *ms;
        javacall_printf(LOG_STR_LEAVE("ok, set time to 0"));
        return JAVACALL_OK;        
    }
    if ((PNULL != handle_ptr->mediaItfPtr)&&(PNULL != handle_ptr->mediaItfPtr->vptrBasic))
    {
        ret = handle_ptr->mediaItfPtr->vptrBasic->set_time(*ms);
        handle_ptr->cur_time = ret;
        *ms = ret;
        if (ret == -1) {
            javacall_printf(LOG_STR_LEAVE("fail"));
            return JAVACALL_FAIL;
        }
        javacall_printf(LOG_STR_LEAVE("ok, ret = %d"),ret);
        return JAVACALL_OK;
    }
    javacall_printf(LOG_STR_LEAVE("fail, handle=0x%x has no set_time()"),handle_ptr);
    return JAVACALL_FAIL;
}
 
/*****************************************************************************/
//  Description : Get whole media time in ms
//  Global resource dependence : none
//  Author: 
//  Note: 
//        @param handle    Handle to the library
//        @return          If success return time in ms else return -1
/*****************************************************************************/
PUBLIC javacall_result javacall_media_get_duration(javacall_handle handle,long* ms) 
{
    int32                        ret            = 0;
    MMIJAVA_NATIVE_HANDLE_T     *handle_ptr     = (MMIJAVA_NATIVE_HANDLE_T*)handle;
   
    if ((PNULL == handle_ptr)||(PNULL == ms))
    {
        javacall_printf(LOG_STR_LEAVE("hande_ptr=%d, ms = %d"), handle_ptr, ms);
        return JAVACALL_INVALID_ARGUMENT;
    }

    if (handle_ptr->isStoppedByOther)
    {
        javacall_printf(LOG_STR("handle=0x%x has been stopped by others"), handle);
//        return JAVACALL_OK;
    }

    if (isSeekSupport(handle_ptr->mediatype) != TRUE) {
        *ms = -1;
        javacall_printf(LOG_STR_LEAVE("ok, handle=0x%x does not support seek, return time=%d"), handle, *ms);
        return JAVACALL_OK;
    }        
    if ((PNULL != handle_ptr->mediaItfPtr)&&(PNULL != handle_ptr->mediaItfPtr->vptrBasic))
    {
        ret = handle_ptr->mediaItfPtr->vptrBasic->get_duration(handle_ptr->filename,handle_ptr->mediatype);
        javacall_printf(LOG_STR_LEAVE("ok, ret = %d"),ret);
        *ms = ret;
        return JAVACALL_OK;
    }
    *ms = -1;
    javacall_printf(LOG_STR_LEAVE("fail, handle=0x%x has no get_duration()"),handle_ptr);
    return JAVACALL_FAIL;
}

/*****************************************************************************/
//  Description : Get current audio volume,Audio volume range have to be in 0 to 100 inclusive
//  Global resource dependence : none
//  Author: 
//  Note: 
//        @param handle    Handle to the library
//        @param volume    Volume value
//        @retval JAVACALL_OK      Success
//        @retval JAVACALL_NO_DATA_AVAILABLE
/*****************************************************************************/
PUBLIC javacall_result javacall_media_get_volume(javacall_handle handle, long* volume)
{    
    MMIJAVA_NATIVE_HANDLE_T     *handle_ptr     = (MMIJAVA_NATIVE_HANDLE_T*)handle;    

    if ((PNULL == handle_ptr)||(PNULL == volume))
    {
        javacall_printf(LOG_STR_LEAVE("invalid argument, hande_ptr=%d, volume = %d"), handle_ptr, volume);
        return JAVACALL_INVALID_ARGUMENT;
    }
    
        javacall_printf(LOG_STR_LEAVE("ok, volume = %d"),handle_ptr->cur_vol);
        *volume =  handle_ptr->cur_vol;
        return JAVACALL_OK;
    }      
   

/*****************************************************************************/
//  Description : Set audio volume Audio volume range have to be in 0 to 100 inclusive
//  Global resource dependence : none
//  Author: 
//  Note: 
//        @param handle    Handle to the library
//        @param level     Volume value,return actual volume level
//        @return          JAVACALL_OK      Success / JAVACALL_NO_DATA_AVAILABLE
/*****************************************************************************/
PUBLIC javacall_result javacall_media_set_volume(javacall_handle handle,  /*INOUT*/ long* level )
{
    MMIJAVA_NATIVE_HANDLE_T     *handle_ptr     = (MMIJAVA_NATIVE_HANDLE_T*)handle;

    if ((PNULL == handle_ptr)||(PNULL == level))
    {
        javacall_printf(LOG_STR_LEAVE("invalid argument, hande_ptr=%d, level = %d"), handle_ptr, level);
        return JAVACALL_INVALID_ARGUMENT;
    }
    
    handle_ptr->cur_vol = (int32)(*level);
    if (handle_ptr->isStoppedByOther)
    {
        javacall_printf(LOG_STR_LEAVE("ok, handle=0x%x has been stopped by others"), handle);
        return JAVACALL_OK;
    }

    if (handle_ptr->isMute == FALSE) {
        //只有播放状态设置才会改变到实际
        if(MMIJAVA_MEDIA_PLAYING == handle_ptr->playingState|| MMIJAVA_MEDIA_RECORDING == handle_ptr->playingState)
        {
            MMIJAVA_SetVolume(handle_ptr);
            if(mmijava_IsVPRunning())
            {              
               mmijava_SetVideoVolume(handle_ptr->cur_vol);
            }
        }
    }
  
    javacall_printf(LOG_STR_LEAVE("ok, volume = %d"), (int32)(*level));   
    return JAVACALL_OK;
}

PUBLIC javacall_result javacall_media_increase_volume()
{
    MMIJAVA_NATIVE_HANDLE_T * handle_ptr = s_cur_handle;
    javacall_int32 vol;

    if (s_max_volume < MMI_AUDIO_VOLUME_MAX)
        s_max_volume++;
    if (handle_ptr != NULL) {
        if (handle_ptr->playingState == MMIJAVA_MEDIA_PLAYING) {
            vol=handle_ptr->cur_vol;
            javacall_media_set_volume(handle_ptr, &vol);
        }
    }
    javacall_printf(LOG_STR_LEAVE("ok"));
    return JAVACALL_OK;
}

PUBLIC javacall_result javacall_media_decrease_volume()
{
    MMIJAVA_NATIVE_HANDLE_T * handle_ptr = s_cur_handle;
    javacall_int32 vol;

    if (s_max_volume > 0)
        s_max_volume--;
    if (handle_ptr != NULL) {
        if (handle_ptr->playingState == MMIJAVA_MEDIA_PLAYING) {
            vol=handle_ptr->cur_vol;
            javacall_media_set_volume(handle_ptr, &vol);
            }
        }
    javacall_printf(LOG_STR_LEAVE("ok"));
    return JAVACALL_OK;
}
    
/*****************************************************************************/
//  Description : Is audio muted now?
//  Global resource dependence : none
//  Author: 
//  Note: 
//        @param handle   Handle to the library
//        @param mute     JAVACALL_TRUE in mute state, 
//                        JAVACALL_FALSE in unmute state
//        @retval JAVACALL_OK      Success
//        @retval JAVACALL_NO_DATA_AVAILABLE
/*****************************************************************************/
PUBLIC javacall_result javacall_media_is_mute(javacall_handle handle,javacall_bool* mute)
{
    MMIJAVA_NATIVE_HANDLE_T     *handle_ptr     = (MMIJAVA_NATIVE_HANDLE_T*)handle;
     
    if (PNULL == mute)
    {
        javacall_printf(LOG_STR_LEAVE("invalid argument, mute = %d"), mute);
        return JAVACALL_INVALID_ARGUMENT;
    }

    if (handle_ptr->isMute == TRUE)
        *mute=JAVACALL_TRUE;
    else
        *mute=JAVACALL_FALSE;

    javacall_printf(LOG_STR_LEAVE("ok, mute = %d"), *mute);
    return JAVACALL_OK;
}

/*****************************************************************************/
//  Description : Mute, Unmute audio
//  Global resource dependence : none
//  Author: 
//  Note: 
//        @param handle    Handle to the library
//        @param mute      JAVACALL_TRUE to mute, JAVACALL_FALSE to unmute
//        @retval JAVACALL_TRUE    Success
//        @retval JAVACALL_FALSE   Fail
/*****************************************************************************/
PUBLIC javacall_result javacall_media_set_mute(javacall_handle handle, javacall_bool mute)
{
    MMIJAVA_NATIVE_HANDLE_T     *handle_ptr     = (MMIJAVA_NATIVE_HANDLE_T*)handle;
    javacall_printf(LOG_STR_LEAVE("enter mute = %d"), mute);
 
    if (PNULL == handle_ptr)
    {
        javacall_printf(LOG_STR_LEAVE("invalid argument, null handle"));
        return JAVACALL_INVALID_ARGUMENT;
    }

    if (handle_ptr->isStoppedByOther)
    {
        javacall_printf(LOG_STR_LEAVE("ok, handle=0x%x has been stopped by others"), handle);
        return JAVACALL_OK;
    }

    if(mute == JAVACALL_TRUE)
    {
        handle_ptr->isMute = TRUE;
        MMIAUDIO_SetMute(TRUE);
    }
    else
    {
#ifdef  CMCC_UI_STYLE
        uint8 call_ring_type = (MMISET_CALL_RING_TYPE_E)MMIAPIENVSET_GetActiveModeOptValue(MN_DUAL_SYS_1, CALL_RING_TYPE);//单卡，多卡此处需修改逻辑
	    if(MMISET_CALL_VIBRA == call_ring_type || MMISET_CALL_SILENT == call_ring_type)
	    {
            javacall_printf(LOG_STR_LEAVE("call_ring_type is %d"), call_ring_type );

	        handle_ptr->isMute = TRUE;
       	    MMIAUDIO_SetMute(TRUE);
        }
	    else
		{
		    javacall_printf(LOG_STR_LEAVE("call_ring_type is %d"), call_ring_type );
            handle_ptr->isMute = FALSE;        
            MMIAUDIO_SetMute(FALSE);      
		}
#else        
        handle_ptr->isMute = FALSE;        
        MMIAUDIO_SetMute(FALSE);     
#endif
    }

    javacall_printf(LOG_STR_LEAVE("ok,  mute = %d"), mute);
    return JAVACALL_OK;
}


/*****************************************************************************/
// Description : play simple tone
// Global resource dependence : none
//  Author: 
//  Note: 
//        @param note     the note to be played. From 0 to 127 inclusive.
//                        The frequency of the note can be calculated from the following formula:
//                           SEMITONE_CONST = 17.31234049066755 = 1/(ln(2^(1/12)))
//                           note = ln(freq/8.176)*SEMITONE_CONST
//                           The musical note A = MIDI note 69 (0x45) = 440 Hz.
//        @param appID    ID of the application playing the tone
//        @param duration the duration of the note in ms 
//        @param volume   volume of this play. From 0 to 100 inclusive.
//        @retval JAVACALL_OK      Success
//        @retval JAVACALL_FAIL    Fail
 /*****************************************************************************/
PUBLIC javacall_result javacall_media_play_tone(int appID,long note, long duration, long volume)
{
    int32                       freq            = MMIJAVA_FREQ(note);/*lint !e524*/

    javacall_printf(LOG_STR("note = %d, duration = %d,volume = %d"),note, duration,volume);
        if ((PNULL != s_cur_handle)
            && ((s_cur_handle->playingState == MMIJAVA_MEDIA_PLAYING)
            || (s_cur_handle->playingState == MMIJAVA_MEDIA_RECORDING)))
        {
            javacall_printf(LOG_STR_LEAVE("ok, current player is playing, no tone"));
            return JAVACALL_OK;
        }

        mmijava_lock_device_multimedia();        
        if(MMIAPISET_IsPermitPlayRing(MMISET_RING_TYPE_JAVA)&&
            !MMIAUDIO_IsBgPlayPaused(MMIBGPLAY_TYPE_JAVA_BG))
        {	
            AUDIO_DEVICE_MODE_TYPE_E  mode = AUDIO_DEVICE_MODE_HANDFREE;
            MMIAPISET_StopAllVibrator();         
            MMIAPISET_StopAllRing(TRUE);
            MMIAPISET_GetDeviceMode(&mode);            
            MMIAUDIO_SetVolume(mode, volume*MMI_AUDIO_VOLUME_MAX/MMIJAVA_MAX_VOLUME);             
            MMIAPISET_SetCurRingType(MMISET_RING_TYPE_JAVA);
            MMIAUDIO_PlayTone(MMIAUDIO_CUSTOM_TONE,0,duration,freq); 
        }
        else
        {
            javacall_printf(LOG_STR_LEAVE("MMIAPISET_IsPermitPlayRing return FALSE"));
            return JAVACALL_OK;
        }

    return JAVACALL_OK;
}

 /*****************************************************************************/
// Description : stop simple tone
// Global resource dependence : none
//  Author: 
//  Note: 
//       @param appID             ID of the application playing the tone
//       @retval JAVACALL_OK      Success
//       @retval JAVACALL_FAIL    Fail
 /*****************************************************************************/
PUBLIC javacall_result javacall_media_stop_tone(int appID)
{
    javacall_printf(LOG_STR("javacall_media_stop_tone"));
    MMIAUDIO_StopTone(MMIAUDIO_CUSTOM_TONE);
    if(PNULL == s_cur_handle)
    {
        MMIAPISET_ClearCurRingType(MMISET_RING_TYPE_JAVA);
        //unlock when jvm paused and exited
        //mmijava_unlock_device_multimedia();        
    }    
    return JAVACALL_OK;
}

/*****************************************************************************/
// Description :  play dual tone (2 tones at the same time)
// Global resource dependence : none
//  Author: 
//  Note: 
//          @param noteA    the note to be played together with noteB. From 0 to 127 inclusive.
//          @param noteB    the note to be played together with noteA. From 0 to 127 inclusive.
//                          The frequency of the note can be calculated from the following formula:
//                             SEMITONE_CONST = 17.31234049066755 = 1/(ln(2^(1/12)))
//                             note = ln(freq/8.176)*SEMITONE_CONST
//                             The musical note A = MIDI note 69 (0x45) = 440 Hz.
//          @param
//          @param appID    ID of the application playing the tone
//          @param duration the duration of the note in ms 
//          @param volume   volume of this play. From 0 to 100 inclusive.
// 
//          @retval JAVACALL_OK      Success
//          @retval JAVACALL_FAIL    Fail. JVM will raise the media exception.
/*****************************************************************************/
PUBLIC javacall_result javacall_media_play_dualtone(int appID, long noteA, long noteB, long duration, long volume)
{
    return JAVACALL_FAIL;
}
/*****************************************************************************/
//  Description : Turn on or off video rendering destination color keying.
//                If this is on OEM native layer video renderer SHOULD use this color key
//                and draw on only the region that is filled with this color value.
//  Global resource dependence : none
//  Author: 
//  Note: 
//          @image html setalpha.png
// 
//          @param handle Handle to the native player
//          @param on     Is color keying on?
//          @param color  Color key
// 
//          @retval JAVACALL_OK      Success
//          @retval JAVACALL_FAIL    Fail
/*****************************************************************************/
PUBLIC javacall_result javacall_media_set_video_color_key(javacall_handle handle,
    javacall_bool on, javacall_pixel color)
{
    return JAVACALL_FAIL;
}


/*****************************************************************************/
//  Description : Get original video width
//  Global resource dependence : none
//  Author: 
//  Note: 
//        @param handle      Handle to the library
//        @param width     Pointer to int variable to get width of video
//                         Caller can assign NULL value if you don't care about width value.
//        @param height    Pointer to int variable to get height of video
//                          Caller can assign NULL value if youe don't care about height value.
//        @retval JAVACALL_OK      Success
//        @retval JAVACALL_FAIL    Fail
/*****************************************************************************/
PUBLIC javacall_result javacall_media_get_video_size(
                                                     javacall_handle handle, 
                                              /*OUT*/ long* width, 
                                              /*OUT*/ long* height
                                              )
{
    BOOLEAN                 ret             = FALSE;    
    MMIJAVA_NATIVE_HANDLE_T *handle_ptr     = (MMIJAVA_NATIVE_HANDLE_T*)handle;       

    if ((PNULL != handle_ptr)&&(PNULL != handle_ptr->mediaItfPtr)&&(PNULL != handle_ptr->mediaItfPtr->vptrVideo))    
    {        
        ret = handle_ptr->mediaItfPtr->vptrVideo->get_video_size(handle_ptr->filename, width,height);
        javacall_printf(LOG_STR_LEAVE("ret = %d"),ret);    
        return (ret?JAVACALL_OK:JAVACALL_FAIL);      
    }
    
    javacall_printf(LOG_STR_LEAVE("fail, handle=0x%x has no get_video_size()"),handle_ptr);
    return JAVACALL_FAIL;
}

/*****************************************************************************/
// Description : Set video rendering position in physical screen
// Global resource dependence : none
//  Author: 
//  Note: 
//           @param handle    Handle to the library 
//           @param x         X position of rendering in pixels
//           @param y         Y position of rendering in pixels
//           @param w         Width of rendering
//           @param h         Height of rendering 
//           @retval JAVACALL_OK      Success
//           @retval JAVACALL_FAIL    Fail
/*****************************************************************************/
PUBLIC javacall_result javacall_media_set_video_location(javacall_handle handle, long x, long y, long w, long h)
{
    javacall_result         result          = JAVACALL_OK;
    MMIJAVA_NATIVE_HANDLE_T *handle_ptr     = (MMIJAVA_NATIVE_HANDLE_T*)handle;       
    javacall_printf(LOG_STR_ENTER("handle=0x%x, (%d, %d, %d, %d)"), handle, x, y, w, h);

    if (x<0 || x>MMI_MAINSCREEN_WIDTH || w<0 || w>MMI_MAINSCREEN_WIDTH || (x+w) > MMI_MAINSCREEN_WIDTH
        || y<0 || y>MMI_MAINSCREEN_HEIGHT || h<0 || h>MMI_MAINSCREEN_HEIGHT || (y+h) > MMI_MAINSCREEN_HEIGHT)
    {
        javacall_printf(LOG_STR_LEAVE("fail, illegal arument"));
        return JAVACALL_FAIL;
    }
    if ((PNULL != handle_ptr)&&(PNULL != handle_ptr->mediaItfPtr)&&(PNULL != handle_ptr->mediaItfPtr->vptrVideo))    
    {        
        result = handle_ptr->mediaItfPtr->vptrVideo->set_video_location(x,y,w,h);        
    }    

    javacall_printf(LOG_STR_LEAVE("result=%d, handle=0x%x"), result, handle_ptr);
    return result;
}

PUBLIC void MMIJAVA_NotifyMediaSetVideoLocation(javacall_result result)
{
    javacall_printf(LOG_STR_ENTER("s_cur_handle=0x%x, status=%d"),s_cur_handle, result);
    if (PNULL != s_cur_handle)
    {
        javanotify_on_media_notification(JAVACALL_EVENT_MEDIA_VIDEO_LOCATION_SET, 
                                         s_cur_handle->AppID,
                                         s_cur_handle->playerId, 
                                         result,
                                         PNULL);
    }
}

/*****************************************************************************/
// Description : Set video preview visible state to show or hide
// Global resource dependence : none
//  Author: 
//  Note: 
//       @param handle    Handle to the library
//       @param visible   JAVACALL_TRUE to show or JAVACALL_FALSE to hide
//       
//       @retval JAVACALL_OK      Success
//       @retval JAVACALL_FAIL    Fail
/*****************************************************************************/
PUBLIC javacall_result javacall_media_set_video_visible(javacall_handle handle, javacall_bool visible)
{
    BOOLEAN                 ret             = FALSE;    
    MMIJAVA_NATIVE_HANDLE_T *handle_ptr     = (MMIJAVA_NATIVE_HANDLE_T*)handle;       
    
    if ((PNULL != handle_ptr)&&(PNULL != handle_ptr->mediaItfPtr)&&(PNULL != handle_ptr->mediaItfPtr->vptrVideo))    
    {        
        ret = handle_ptr->mediaItfPtr->vptrVideo->set_video_visible(visible);        
        javacall_printf(LOG_STR("ret = %d"),ret);    
        return (ret?JAVACALL_OK:JAVACALL_FAIL);
    }    

    javacall_printf(LOG_STR_LEAVE("fail, handle=0x%x has no set_video_visible()"),handle_ptr);
    return JAVACALL_FAIL;
}
    
/*****************************************************************************/
// Description : Start get current snapshot of video data
//               When snapshot operation doen, call callback function to provide snapshot image data to Java.
// Global resource dependence : none
//  Author: 
//  Note:
//       @param handle            Handle to the library
//       @param imageType         Snapshot image type format as unicode string. For example, "jpeg", "png" or "rgb565".
//       @param length            imageType unicode string length
// 
//       @retval JAVACALL_OK          Success.
//       @retval JAVACALL_WOULD_BLOCK This operation could takes long time. 
//                                    After this operation finished, MUST send - IMPL_NOTE: EVENT NAME - by using 
//                                    "javanotify_on_media_notification" function call
//       @retval JAVACALL_FAIL        Fail. Invalid encodingFormat or some errors.
/*****************************************************************************/
PUBLIC javacall_result javacall_media_start_video_snapshot(
                                                           javacall_handle handle, 
                                                           const javacall_utf16* imageType, //目前之用默认的jpeg格式
                                                           long length
                                                           )
{
    int8                 ret             = 0;    
    MMIJAVA_NATIVE_HANDLE_T *handle_ptr     = (MMIJAVA_NATIVE_HANDLE_T*)handle;       
    
    javacall_printf(LOG_STR_ENTER("s_cur_handle=0x%p, handle=0x%p"), s_cur_handle, handle_ptr);
    if ((PNULL != handle_ptr)&&(PNULL != handle_ptr->mediaItfPtr)&&(PNULL != handle_ptr->mediaItfPtr->vptrSnapshot))    
    {        
        ret = handle_ptr->mediaItfPtr->vptrSnapshot->start_snapshot(imageType,length);        
        javacall_printf(LOG_STR("ret = %d"),ret);    
    }    
    else    
    {        
        javacall_printf(LOG_STR("handle_ptr = %x, mediaItfPtr= %x, vptrVideo = %x"),
            handle_ptr,handle_ptr->mediaItfPtr,handle_ptr->mediaItfPtr->vptrVideo); /*lint !e613*/  
    }    
    
    if(-1 == ret)
    {
       return JAVACALL_WOULD_BLOCK;
    }
    else if(1 ==  ret)
    {
       return JAVACALL_OK;
    }
    else
    {
       return JAVACALL_FAIL;
    }
}

/*****************************************************************************/
// Description : Get snapshot data size
// Global resource dependence : none
//  Author: 
//  Note: 
//       @param handle    Handle to the library
//       @param size      Size of snapshot data
// 
//       @retval JAVACALL_OK      Success
//       @retval JAVACALL_FAIL    Fail
/*****************************************************************************/
PUBLIC javacall_result javacall_media_get_video_snapshot_data_size(
                                                                   javacall_handle handle, 
                                                                   /*OUT*/ long* size
                                                                   )
{//zhaohui
    BOOLEAN                 ret             = FALSE;    
    MMIJAVA_NATIVE_HANDLE_T *handle_ptr     = (MMIJAVA_NATIVE_HANDLE_T*)handle;       
    
    if ((PNULL != handle_ptr)&&(PNULL != handle_ptr->mediaItfPtr)&&(PNULL != handle_ptr->mediaItfPtr->vptrSnapshot)&&(PNULL != size))    
    {        
        ret = handle_ptr->mediaItfPtr->vptrSnapshot->get_snapshot_data_size(size);        
        javacall_printf(LOG_STR("ret = %d,size = %d"),ret,*size);    
        return (ret?JAVACALL_OK:JAVACALL_FAIL);
    }    

    javacall_printf(LOG_STR_LEAVE("fail, handle=0x%x has no get_snapshot_data_size()"),handle_ptr);
    return JAVACALL_FAIL;
}

/*****************************************************************************/
// Description : Get snapshot data
// Global resource dependence : none
//  Author: 
//  Note:
//       @param handle    Handle to the library
//       @param buffer    Buffer will contains the snapshot data
//       @param size      Size of snapshot data
// 
//       @retval JAVACALL_OK      Success
//       @retval JAVACALL_FAIL    Fail
/*****************************************************************************/
PUBLIC javacall_result javacall_media_get_video_snapshot_data(
                                                              javacall_handle handle, 
                                                              /*OUT*/ char* buffer, 
                                                              long size)
{//zhaohui
        BOOLEAN                 ret             = FALSE;    
    MMIJAVA_NATIVE_HANDLE_T *handle_ptr     = (MMIJAVA_NATIVE_HANDLE_T*)handle;       
    
    if ((PNULL != handle_ptr)&&(PNULL != buffer))  
    {
        if(PNULL != handle_ptr->mediaItfPtr)
        {
            if(PNULL != handle_ptr->mediaItfPtr->vptrSnapshot)  
            {        
                ret = handle_ptr->mediaItfPtr->vptrSnapshot->get_snapshot_data(buffer,size);        
                javacall_printf(LOG_STR_LEAVE("ret = %d,&buffer = %x"),ret,buffer);    
                return (ret?JAVACALL_OK:JAVACALL_FAIL);
            }  
        }
    }
    javacall_printf(LOG_STR_LEAVE("fail, handle=0x%x has no get_snapshot_data()"),handle_ptr);
    return JAVACALL_FAIL;
}

/*****************************************************************************/
// Description : Set video fullscreen mode
//  Global resource dependence : none
//  Author: 
//  Note:  
//          @param handle    Handle to the library 
//          @param fullScreenMode whether to set video playback in fullscreen mode
// 
//          @retval JAVACALL_OK      Success
//          @retval JAVACALL_FAIL    Fail
//          @retval JAVACALL_NOT_IMPLEMENTED    Native FullScreen mode not implemented
/*****************************************************************************/
PUBLIC javacall_result javacall_media_set_video_full_screen_mode(javacall_handle handle, javacall_bool full_screen_mode)
{
    javacall_result result = JAVACALL_OK;
    MMIJAVA_NATIVE_HANDLE_T *handle_ptr     = (MMIJAVA_NATIVE_HANDLE_T*)handle;       
    
    if ((PNULL != handle_ptr)&&(PNULL != handle_ptr->mediaItfPtr)&&(PNULL != handle_ptr->mediaItfPtr->vptrVideo))    
    {        
        result = handle_ptr->mediaItfPtr->vptrVideo->set_fullscreen_mode(full_screen_mode, handle_ptr->AppID, handle_ptr->playerId);
    }    

    javacall_printf(LOG_STR("result=%d, handle=0x%x"), result, handle_ptr);
    if (PNULL != handle_ptr)
    {
        javacall_printf(LOG_STR_LEAVE("appid=%d, playerid=%d"), handle_ptr->AppID, handle_ptr->playerId);    
    }
    return result;
}

PUBLIC void MMIJAVA_NotifyMediaSetVideoFullScreenMode(javacall_result result, int32 appid, int32 playerid)
{
    javacall_printf(LOG_STR_ENTER("s_cur_handle=0x%x, status=%d, appid=%d, playerid=%d"),s_cur_handle, result, appid, playerid);
    if (appid != -1 && playerid != -1)
        javanotify_on_media_notification(JAVACALL_EVENT_MEDIA_VIDEO_FULLSCREEN_MODE_SET, 
                                      appid, playerid, result, PNULL);
}

/*****************************************************************************/
// Description : Converts the given frame number to the corresponding media time in milli second unit.
//  Global resource dependence : none
//  Author: 
//  Note: 
//           @param handle    Handle to the library 
//           @param frameNum  The input frame number for the conversion
//           @param ms        The converted media time in milli seconds for the given frame
// 
//           @retval JAVACALL_OK      Success
//           @retval JAVACALL_FAIL    Fail
/*****************************************************************************/
PUBLIC javacall_result javacall_media_map_frame_to_time(
                                                        javacall_handle handle, 
                                                        long frameNum, 
                                                        /*OUT*/ long* ms
                                                        )
{
    BOOLEAN                 ret             = FALSE;    
    MMIJAVA_NATIVE_HANDLE_T *handle_ptr     = (MMIJAVA_NATIVE_HANDLE_T*)handle;       
    
    if ((PNULL != handle_ptr)&&(PNULL != handle_ptr->mediaItfPtr)&&(PNULL != handle_ptr->mediaItfPtr->vptrVideo))    
    {        
        ret = handle_ptr->mediaItfPtr->vptrVideo->map_frame_to_time(handle_ptr->filename, frameNum,ms);        
        javacall_printf(LOG_STR_LEAVE("ret = %d"),ret);    
        return (ret?JAVACALL_OK:JAVACALL_FAIL);
    }    

    javacall_printf(LOG_STR_LEAVE("fail, handle=0x%x has no map_frame_to_time()"),handle_ptr);
    return JAVACALL_FAIL;
}

/*****************************************************************************/
// Description : Converts the given media time to the corresponding frame number.
//  Global resource dependence : none
//  Author: 
//  Note: 
//           @param handle    Handle to the library 
//           @param ms        The input media time for the conversion in milli seconds
//           @param frameNum  The converted frame number for the given media time. 
// 
//           @retval JAVACALL_OK      Success
//           @retval JAVACALL_FAIL    Fail 
/*****************************************************************************/
PUBLIC javacall_result javacall_media_map_time_to_frame(
                                                        javacall_handle handle, 
                                                        long ms, 
                                                        /*OUT*/ long* frameNum
                                                        )
{
    BOOLEAN                 ret             = FALSE;    
    MMIJAVA_NATIVE_HANDLE_T *handle_ptr     = (MMIJAVA_NATIVE_HANDLE_T*)handle;       
    
    if ((PNULL != handle_ptr)&&(PNULL != handle_ptr->mediaItfPtr)&&(PNULL != handle_ptr->mediaItfPtr->vptrVideo))    
    {        
        ret = handle_ptr->mediaItfPtr->vptrVideo->map_time_to_frame(handle_ptr->filename, ms, frameNum);        
        javacall_printf(LOG_STR_LEAVE("ret = %d"),ret);    
        return (ret?JAVACALL_OK:JAVACALL_FAIL);
    }    

    javacall_printf(LOG_STR_LEAVE("fail, handle=0x%x has no map_time_to_frame()"),handle_ptr);
    return JAVACALL_FAIL;
}

/*****************************************************************************/
// Description : Seek to a given video frame.
//               If the given frame number is less than the first or larger than the last frame number in the media, 
//               seek  will jump to either the first or the last frame respectively.
//  Global resource dependence : none
//  Author: 
//  Note: 
//           @param handle            Handle to the library 
//           @param frameNum          The frame to seek to
//           @param actualFrameNum    The actual frame that the Player has seeked to
// 
//           @retval JAVACALL_OK      Success
//           @retval JAVACALL_FAIL    Fail
/*****************************************************************************/
PUBLIC javacall_result javacall_media_seek_to_frame(
                                                    javacall_handle handle, 
                                                    long frameNum, 
                                                    /*OUT*/ long* actualFrameNum
                                                    )
{
    int32                        ret            = 0;
    MMIJAVA_NATIVE_HANDLE_T     *handle_ptr     = (MMIJAVA_NATIVE_HANDLE_T*)handle;  
    
    if ((PNULL == handle_ptr)||(PNULL == actualFrameNum))
    {
        javacall_printf(LOG_STR_LEAVE("invalid argument, hande_ptr=%d, actualFrameNum = %d"), handle_ptr, actualFrameNum);
        return JAVACALL_INVALID_ARGUMENT;
    }

    if ((PNULL != handle_ptr->mediaItfPtr)&&(PNULL != handle_ptr->mediaItfPtr->vptrBasic))
    {
        ret = handle_ptr->mediaItfPtr->vptrVideo->seek_frame(frameNum);
        actualFrameNum = frameNum;/*lint !e64 */
        javacall_printf(LOG_STR_LEAVE("ret = %d"),ret);
        return (ret?JAVACALL_OK:JAVACALL_FAIL);
    }    

    javacall_printf(LOG_STR_LEAVE("fail, handle=0x%x has no seek_frame()"),handle_ptr);
    return JAVACALL_FAIL;
}

/*****************************************************************************/
// Description : Skip a given number of frames from the current position.
//  Global resource dependence : none
//  Author: 
//  Note: 
//           @param handle        Handle to the library 
//           @param framesToSkip  The number of frames to skip from the current position. Return number of actual skipped frames
//                                If framesToSkip is negative, it will seek backward by framesToSkip number of frames.
// 
//           @retval JAVACALL_OK      Success
//           @retval JAVACALL_FAIL    Fail
/*****************************************************************************/
PUBLIC javacall_result javacall_media_skip_frames(javacall_handle handle, long* nFrames)
{
    BOOLEAN                        ret            = FALSE;
    MMIJAVA_NATIVE_HANDLE_T     *handle_ptr     = (MMIJAVA_NATIVE_HANDLE_T*)handle;  
    
    if ((PNULL == handle_ptr)||(PNULL == nFrames))
    {
        javacall_printf(LOG_STR_LEAVE("invalid argument, hande_ptr=%d, ms = %d"), handle_ptr, nFrames);
        return JAVACALL_INVALID_ARGUMENT;
    }

    if ((PNULL != handle_ptr->mediaItfPtr)&&(PNULL != handle_ptr->mediaItfPtr->vptrBasic))
    {
        ret = handle_ptr->mediaItfPtr->vptrVideo->skip_frame(nFrames);/*lint !e64 */
        javacall_printf(LOG_STR_LEAVE("ret = %d"),ret);
        return (ret?JAVACALL_OK:JAVACALL_FAIL);
    }    

    javacall_printf(LOG_STR_LEAVE("fail, handle=0x%x has no skip_frame()"),handle_ptr);
    return JAVACALL_FAIL;
}

/*****************************************************************************/
// Description : Get supported meta data key counts for specefic media type
// Global resource dependence : none
//  Author: 
//  Note: 
//       @param mediaType [in]    Media type
//       @param keyCounts [out]   Return meta data key string counts
// 
//       @retval JAVACALL_OK              Success
//       @retval JAVACALL_FAIL            Fail
//       @retval JAVACALL_NOT_IMPLEMENTED MetaDataControl is not implemented for this media type
 /*****************************************************************************/
PUBLIC javacall_result javacall_media_get_metadata_key_counts(
                                                              javacall_handle handle, 
                                                              /*OUT*/ long* keyCounts
                                                              )
{
    BOOLEAN                 ret             = FALSE;    
    MMIJAVA_NATIVE_HANDLE_T *handle_ptr     = (MMIJAVA_NATIVE_HANDLE_T*)handle;       
    
    if ((PNULL != handle_ptr)&&(PNULL != handle_ptr->mediaItfPtr)&&(PNULL != handle_ptr->mediaItfPtr->vptrMetaData))    
    {        
        ret = handle_ptr->mediaItfPtr->vptrMetaData->get_metadata_key_counts(handle_ptr->filename,handle_ptr->mediatype,keyCounts);
        javacall_printf(LOG_STR_LEAVE("ret = %d, key count=%d"),ret, *keyCounts);    
        return (ret?JAVACALL_OK:JAVACALL_FAIL);
    }    

    javacall_printf(LOG_STR_LEAVE("fail, handle=0x%x has no get_metadata_key_counts()"),handle_ptr);
    return JAVACALL_FAIL;
}

/*****************************************************************************/
// Description : Get meta data key strings by using media type and index value
// Global resource dependence : none
//  Author: 
//  Note: 
//       @param mediaType [in]    Media type
//       @param index     [in]    Meta data key string's index value. from 0 to 'key counts - 1'.
//       @param bufLength [in]    keyBuf buffer's size in bytes. 
//       @param keyBuf    [out]   Buffer that used to return key strings. 
//                                NULL value should be appended to the end of string.
// 
//       @retval JAVACALL_OK              Success
//       @retval JAVACALL_OUT_OF_MEMORY     keyBuf size is too small
//       @retval JAVACALL_NOT_IMPLEMENTED  MetaDataControl is not implemented for this media type
//       @retval JAVACALL_FAIL            Fail
 /*****************************************************************************/
PUBLIC javacall_result javacall_media_get_metadata_key(
                                                       javacall_handle handle, 
                                                       long index, 
                                                       long bufLength, 
                                                /*OUT*/ javacall_utf16* keyBuf
                                                )
{
    BOOLEAN                 ret             = FALSE;    
    int len;
    MMIJAVA_NATIVE_HANDLE_T *handle_ptr     = (MMIJAVA_NATIVE_HANDLE_T*)handle;       
    char key[64];
    
    if ((PNULL != handle_ptr)&&(PNULL != handle_ptr->mediaItfPtr)&&(PNULL != handle_ptr->mediaItfPtr->vptrMetaData))    
    {        
        ret = handle_ptr->mediaItfPtr->vptrMetaData->get_metadata_key(index, 64, key);
        javacall_printf(LOG_STR_LEAVE("ret = %d, key=%s"),ret, key);
        len=strlen(key)+1;
        if (len > bufLength)
            len=bufLength;
        MMI_STRNTOWSTR(keyBuf, bufLength, (uint8*)key, 64, len);
        return (ret?JAVACALL_OK:JAVACALL_FAIL);
    }    

    javacall_printf(LOG_STR_LEAVE("fail, handle=0x%x has no get_metadata_key()"),handle_ptr);
    return JAVACALL_FAIL;
}

/*****************************************************************************/
// Description : Get meta data value strings by using meta data key string
// Global resource dependence : none
//  Author: 
//  Note:  
//       @param hLIB      [in]    Handle to the library 
//       @param key       [in]    Meta data key string
//       @param bufLength [in]    dataBuf buffer's size in bytes. 
//       @param dataBuf   [out]   Buffer that used to return meta data strings. 
//                                NULL value should be appended to the end of string.
// 
//       @retval JAVACALL_OK              Success
//       @retval JAVACALL_OUT_OF_MEMORY     dataBuf size is too small
//       @retval JAVACALL_NOT_IMPLEMENTED  MetaDataControl is not implemented for this media type
//       @retval JAVACALL_FAIL            Fail
/*****************************************************************************/
PUBLIC javacall_result javacall_media_get_metadata(
                                                   javacall_handle handle, 
                                                    javacall_const_utf16_string key, 
                                                    long bufLength, 
                                                    /*OUT*/ javacall_utf16* dataBuf
                                                    )
{
    BOOLEAN                 ret             = FALSE;    
    MMIJAVA_NATIVE_HANDLE_T *handle_ptr     = (MMIJAVA_NATIVE_HANDLE_T*)handle;       
    
    if ((PNULL != handle_ptr)&&(PNULL != handle_ptr->mediaItfPtr)&&(PNULL != handle_ptr->mediaItfPtr->vptrMetaData))    
    {        
        ret = handle_ptr->mediaItfPtr->vptrMetaData->get_metadata(key,bufLength,dataBuf);/*lint !e64 */
        javacall_printf(LOG_STR_LEAVE("ret = %d"),ret);    
        return (ret?JAVACALL_OK:JAVACALL_FAIL);
    }    

    javacall_printf(LOG_STR_LEAVE("fail, handle=0x%x has no get_metadata()"),handle_ptr);
    return JAVACALL_FAIL;
}

/*****************************************************************************/
// Description : Get volume for the given channel. 
//               The return value is independent of the master volume, which is set and retrieved with VolumeControl.
// Global resource dependence : none
//  Author: 
//  Note:  
//       @param handle    Handle to the library 
//       @param channel   0-15
//       @param volume    channel volume, 0-127, or -1 if not known
//       
//       @retval JAVACALL_OK      Success
//       @retval JAVACALL_FAIL    Fail
/*****************************************************************************/
PUBLIC javacall_result javacall_media_get_channel_volume(
                                                         javacall_handle handle, 
                                                          long channel, 
                                                          /*OUT*/ long* volume
                                                          )
{
    return JAVACALL_FAIL;
}

/*****************************************************************************/
// Description : Set volume for the given channel. To mute, set to 0. 
//               This sets the current volume for the channel and may be overwritten during playback by events in a MIDI sequence.
// Global resource dependence : none
//  Author: 
//  Note:  
//       @param handle    Handle to the library 
//       @param channel   0-15
//       @param volume    channel volume, 0-127
// 
//       @retval JAVACALL_OK      Success
//       @retval JAVACALL_FAIL    Fail
/*****************************************************************************/
PUBLIC javacall_result javacall_media_set_channel_volume(
                                                         javacall_handle handle, 
                                                         long channel, 
                                                         long volume
                                                         )
{
    return JAVACALL_FAIL;
}

/*****************************************************************************/
// Description : Set program of a channel. 
//               This sets the current program for the channel and may be overwritten during playback by events in a MIDI sequence.
// Global resource dependence : none
//  Author: 
//  Note:  
//       @param handle    Handle to the library 
//       @param channel   0-15
//       @param bank      0-16383, or -1 for default bank
//       @param program   0-127
// 
//       @retval JAVACALL_OK      Success
//       @retval JAVACALL_FAIL    Fail
/*****************************************************************************/
PUBLIC javacall_result javacall_media_set_program(
                                                  javacall_handle handle, 
                                                  long channel, 
                                                  long bank, 
                                                  long program
                                                  )
{
    return JAVACALL_FAIL;
}

/*****************************************************************************/
// Description : Sends a short MIDI event to the device.
// Global resource dependence : none
//  Author: 
//  Note:   
//       @param handle    Handle to the library 
//       @param type      0x80..0xFF, excluding 0xF0 and 0xF7, which are reserved for system exclusive
//       @param data1     for 2 and 3-byte events: first data byte, 0..127
//       @param data2     for 3-byte events: second data byte, 0..127
// 
//       @retval JAVACALL_OK      Success
//       @retval JAVACALL_FAIL    Fail
/*****************************************************************************/
PUBLIC javacall_result javacall_media_short_midi_event(
                                                       javacall_handle handle,
                                                       long type, 
                                                       long data1, 
                                                       long data2
                                                       ) 
{
    return JAVACALL_FAIL;
}

/*****************************************************************************/
// Description : Sends a long MIDI event to the device, typically a system exclusive message.
// Global resource dependence : none
//  Author: 
//  Note:  
//       @param handle    Handle to the library 
//       @param data      array of the bytes to send. 
//                        This memory buffer will be freed after this function returned.
//                        So, you should copy this data to the other internal memory buffer
//                        if this function needs data after return.
//       @param offset    start offset in data array
//       @param length    number of bytes to be sent
// 
//       @retval JAVACALL_OK      Success
//       @retval JAVACALL_FAIL    Fail
/*****************************************************************************/
PUBLIC javacall_result javacall_media_long_midi_event(
                                                      javacall_handle handle,
                                                      const char* data, 
                                                      long offset, 
                                                      /*INOUT*/ long* length
                                                      ) 
{
    return JAVACALL_FAIL;
}


/*****************************************************************************/
// Description : This function is used to ascertain the availability of MIDI bank support
//  Global resource dependence : none
//  Author: 
//  Note:
//          @param handle     Handle to the native player
//          @param supported  return of support availability
//
//          @retval JAVACALL_OK      MIDI Bank Query support is available
//          @retval JAVACALL_FAIL    NO MIDI Bank Query support is available
/*****************************************************************************/
PUBLIC javacall_result javacall_media_is_midibank_query_supported(
                                                                  javacall_handle handle,
                                                                  long* supported
                                                                  ) 
{
    return JAVACALL_FAIL;
}

/*****************************************************************************/
// Description : This function is used to get a list of installed banks. If the custom
//              parameter is true, a list of custom banks is returned. Otherwise, a list of
//              all banks (custom and internal) is returned. This function can be left empty.
//  Global resource dependence : none
//  Author: 
//  Note:
//          @param handle    Handle to the native player
//          @param custom    a flag indicating whether to return just custom banks, or
//                          all banks.
//          @param banklist  an array which will be filled out with the banklist
//          @param numlist   the length of the array to be filled out, and on return
//                           contains the number of values written to the array.
//
//          @retval JAVACALL_OK      Bank List is available
//          @retval JAVACALL_FAIL    Bank List is NOT available
/*****************************************************************************/
PUBLIC javacall_result javacall_media_get_midibank_list(
                                                        javacall_handle handle,
                                                        long custom,
                                                        /*OUT*/short* banklist,
                                                        /*INOUT*/long* numlist
                                                        ) 
{
    return JAVACALL_FAIL;
}


/*****************************************************************************/
// Description : Given a bank, program and key, get name of key. This function applies to
//              key-mapped banks (i.e. percussive banks or effect banks) only. If the returned
//              keyname length is 0, then the key is not mapped to a sound. For melodic banks,
//              where each key (=note) produces the same sound at different pitch, this
//              function always returns a 0 length string. For space saving reasons an
//              implementation may return a 0 length string instead of the keyname. This
//              can be left empty.
//  Global resource dependence : none
//  Author: 
//  Note:
//          @param handle    Handle to the native player
//          @param bank      The bank to query
//          @param program   The program to query
//          @param key       The key to query
//          @param keyname   The name of the key returned.
//          @param keynameLen    The length of the keyname array, and on return the
//                               length of the keyname.
//
//          @retval JAVACALL_OK      Keyname available
//          @retval JAVACALL_FAIL    Keyname not supported
/*****************************************************************************/
PUBLIC javacall_result javacall_media_get_midibank_key_name(
                                                            javacall_handle handle,
                                                            long bank,
                                                            long program,
                                                            long key,
                                                            /*OUT*/javacall_ascii_string keyname,
                                                            /*INOUT*/long* keynameLen
                                                            ) 
{
    return JAVACALL_FAIL;
}

/*****************************************************************************/
// Description : Given the bank and program, get name of program. For space-saving reasons
//              a 0 length string may be returned.
//  Global resource dependence : none
//  Author: 
//  Note:
//          @param handle    Handle to the native player
//          @param bank      The bank being queried
//          @param program   The program being queried
//          @param progname  The name of the program returned
//          @param prognameLen    The length of the progname array, and on return the
//                       length of the progname
//
//          @retval JAVACALL_OK      Program name available
//          @retval JAVACALL_FAIL    Program name not supported
/*****************************************************************************/
PUBLIC javacall_result javacall_media_get_midibank_program_name(
                                                                javacall_handle handle,
                                                                long bank,
                                                                long program,
                                                                /*OUT*/javacall_ascii_string progname,
                                                                /*INOUT*/long* prognameLen
                                                                ) 
{
    return JAVACALL_FAIL;
}

/*****************************************************************************/
// Description : Given bank, get list of program numbers. If and only if this bank is not
//                  installed, an empty array is returned.
//  Global resource dependence : none
//  Author: 
//  Note:
//          @param handle    Handle to the native player
//          @param bank      The bank being queried
//          @param proglist  The Program List being returned
//          @param proglistLen     The length of the proglist, and on return the number
//                                    of program numbers in the list
//
//          @retval JAVACALL_OK     Program list available
//          @retval JAVACALL_FAIL   Program list unsupported
/*****************************************************************************/
PUBLIC javacall_result javacall_media_get_midibank_program_list(
                                                                javacall_handle handle,
                                                                long bank,
                                                                /*OUT*/javacall_ascii_string proglist,
                                                                /*INOUT*/long* proglistLen
                                                                ) 
{
    return JAVACALL_FAIL;
}

/*****************************************************************************/
// Description : Returns the program assigned to the channel. It represents the current state
//                  of the channel. During playbank of the MIDI file, the program may change due
//                  to program change events in the MIDI file. The returned array is represented
//                  by an array {bank, program}. The support of this function is optional.
//  Global resource dependence : none
//  Author: 
//  Note:
//          @param handle    Handle to the native player
//          @param channel   The channel being queried
//          @param prog      The return array (size 2) in the form {bank, program}
//
//          @retval JAVACALL_OK    Program available
//          @retval JAVACALL_FAIL  Get Program unsupported
/*****************************************************************************/
PUBLIC javacall_result javacall_media_get_midibank_program(
                                                           javacall_handle handle,
                                                            long channel,
                                                            /*OUT*/long* prog
                                                            ) 
{
    return JAVACALL_FAIL;
}



/*****************************************************************************/
//  Description : Get media's current playing tempo.
//  Global resource dependence : none
//  Author: 
//  Note:
//          @param hLIB [in]     Handle to the library
//          @param tempo [out]   Current playing tempo
//
//          @retval JAVACALL_OK          Success
//          @retval JAVACALL_FAIL        Fail
/*****************************************************************************/
PUBLIC javacall_result javacall_media_get_tempo(javacall_handle handle, /*OUT*/ long* tempo) 
{
    return JAVACALL_FAIL;
}

/*****************************************************************************/
//  Description : Set media's current playing tempo
//  Global resource dependence : none
//  Author: 
//  Note:
//          @param hLIB [in]     Handle to the library
//          @param tempo [in]    Tempo to set
//
//          @retval JAVACALL_OK          Success
//          @retval JAVACALL_FAIL        Fail
/*****************************************************************************/
PUBLIC javacall_result javacall_media_set_tempo(javacall_handle handle, long tempo)
{
    return JAVACALL_FAIL;
}


/*****************************************************************************/
//  Description : Gets the maximum playback pitch raise supported by the Player
//  Global resource dependence : none
//  Author: 
//  Note:
//          @param handle    Handle to the library 
//          @param maxPitch  The maximum pitch raise in "milli-semitones".
//
//          @retval JAVACALL_OK      Success
//          @retval JAVACALL_FAIL    Fail
 /*****************************************************************************/
PUBLIC javacall_result javacall_media_get_max_pitch(javacall_handle handle, /*OUT*/ long *maxPitch)
{
    return JAVACALL_FAIL;
}

/*****************************************************************************/
//  Description : Gets the minimum playback pitch raise supported by the Player
//  Global resource dependence : none
//  Author: 
//  Note:
//          @param handle    Handle to the library 
//          @param minPitch  The minimum pitch raise in "milli-semitones"
//
//          @retval JAVACALL_OK      Success
//          @retval JAVACALL_FAIL    Fail
/*****************************************************************************/
PUBLIC javacall_result javacall_media_get_min_pitch(javacall_handle handle, /*OUT*/ long* minPitch)
{
    return JAVACALL_FAIL;
}

/*****************************************************************************/
//  Description : Set media's current playing rate
//  Global resource dependence : none
//  Author: 
//  Note:
//          @param handle    Handle to the library 
//          @param pitch     The number of semi tones to raise the playback pitch. It is specified in "milli-semitones"
//
//          @retval JAVACALL_OK      Success
//          @retval JAVACALL_FAIL    Fail
/*****************************************************************************/
PUBLIC javacall_result javacall_media_set_pitch(javacall_handle handle, long pitch)
{
    return JAVACALL_FAIL;
}

/*****************************************************************************/
//  Description : Get media's current playing rate
//  Global resource dependence : none
//  Author: 
//  Note:
//          @param handle    Handle to the library 
//          @param pitch     The current playback pitch raise in "milli-semitones"
//
//          @retval JAVACALL_OK          Success
//          @retval JAVACALL_FAIL        Fail
/*****************************************************************************/
PUBLIC javacall_result javacall_media_get_pitch(javacall_handle handle, /*OUT*/ long* pitch)
{
    return JAVACALL_FAIL;
}

/*****************************************************************************/
// Description : Get maximum rate of media type
// Global resource dependence : none
//  Author: 
//  Note:  
//       @param mediaType [in]    Media type
//       @param maxRate   [out]   Maximum rate value for this media type
// 
//       @retval JAVACALL_OK              Success
//       @retval JAVACALL_NOT_IMPLEMENTED  RateControl is not implemented for this media type
//       @retval JAVACALL_FAIL            Fail
/*****************************************************************************/
PUBLIC javacall_result javacall_media_get_max_rate(javacall_handle handle, /*OUT*/ long* maxRate) 
{
    MMIJAVA_NATIVE_HANDLE_T     *handle_ptr     = (MMIJAVA_NATIVE_HANDLE_T*)handle;    

    if ((PNULL == handle_ptr)||(PNULL == maxRate))
    {
        javacall_printf(LOG_STR_LEAVE("invalid argument, hande_ptr=%d, maxRate = %d"), handle_ptr, maxRate);
        return JAVACALL_INVALID_ARGUMENT;
    }

    *maxRate = 400000; // 4x fast
    javacall_printf(LOG_STR_LEAVE("ok, max rate = %d"), *maxRate);
    return JAVACALL_OK;
}

/*****************************************************************************/
// Description : Get minimum rate of media type
// Global resource dependence : none
//  Author: 
//  Note: 
//       @param mediaType [in]    Media type
//       @param minRate   [out]   Minimum rate value for this media type
// 
//       @retval JAVACALL_OK              Success
//       @retval JAVACALL_FAIL            Fail
/*****************************************************************************/
PUBLIC javacall_result javacall_media_get_min_rate(javacall_handle handle, /*OUT*/ long* minRate) 
{
    MMIJAVA_NATIVE_HANDLE_T     *handle_ptr     = (MMIJAVA_NATIVE_HANDLE_T*)handle;    

    if ((PNULL == handle_ptr)||(PNULL == minRate))
    {
        javacall_printf(LOG_STR_LEAVE("invalid argument, hande_ptr=%d, minRate = %d"), handle_ptr, minRate);
        return JAVACALL_INVALID_ARGUMENT;
    }

    *minRate = 25000; // 4x slow
    javacall_printf(LOG_STR_LEAVE("ok, min rate = %d"), *minRate);
    return JAVACALL_OK;
}

int convert_to_platform_rate(int rate)
{
    static int support_rate[]={25000, /* MMIAUDIO_PLAY_RATE_SLOW_2 */
                               50000, /* MMIAUDIO_PLAY_RATE_SLOW_1 */
                               100000, /* MMIAUDIO_PLAY_RATE_NORMAL */
                               200000, /* MMIAUDIO_PLAY_RATE_FAST_1 */
                               400000 /* MMIAUDIO_PLAY_RATE_FAST_2 */};
    if (rate < (support_rate[0]+support_rate[1])/2)
        return MMIAUDIO_PLAY_RATE_SLOW_2;
    if (rate < (support_rate[1]+support_rate[2])/2)
        return MMIAUDIO_PLAY_RATE_SLOW_1;
    if (rate < (support_rate[2]+support_rate[3])/2)
        return MMIAUDIO_PLAY_RATE_NORMAL;
    if (rate < (support_rate[3]+support_rate[4])/2)
        return MMIAUDIO_PLAY_RATE_FAST_1;
    return MMIAUDIO_PLAY_RATE_FAST_1;
}

/*****************************************************************************/
// Description : Set media's current playing rate
// Global resource dependence : none
//  Author: 
//  Note:  
//       @param hLIB [in] Handle to the library 
//       @param rate [in] Rate to set
//       
//       @retval JAVACALL_OK      Success
//       @retval JAVACALL_FAIL    Fail
/*****************************************************************************/
PUBLIC javacall_result javacall_media_set_rate(javacall_handle handle, long rate) 
{
    MMIJAVA_NATIVE_HANDLE_T     *handle_ptr     = (MMIJAVA_NATIVE_HANDLE_T*)handle;
    javacall_result             result=JAVACALL_OK;
    long                        old_rate;
    MMIAUDIO_PLAY_RATE_E        platform_rate=MMIAUDIO_PLAY_RATE_NORMAL ;
    
    if (PNULL == handle_ptr)
    {
        javacall_printf(LOG_STR_LEAVE("invalid argument, hande_ptr=%d"), handle_ptr);
        return JAVACALL_INVALID_ARGUMENT;
    }

    if ((PNULL != handle_ptr)&&(PNULL != handle_ptr->mediaItfPtr)&&(PNULL != handle_ptr->mediaItfPtr->vptrBasic)) /*lint !e774*/   
    {        
        old_rate=handle_ptr->rate;
        platform_rate=convert_to_platform_rate(rate);
        result = handle_ptr->mediaItfPtr->vptrBasic->set_rate(handle_ptr->mediatype, platform_rate);
        if (result != JAVACALL_OK)
            handle_ptr->rate=old_rate;
    }    

    javacall_printf(LOG_STR_LEAVE("result=%d, handle=0x%x, new rate=%d"), result, handle_ptr, handle_ptr->rate);
    return JAVACALL_OK;
}

/*****************************************************************************/
//  Description : Get media's current playing rate
//  Global resource dependence : none
//  Author: 
//  Note:
//          @param hLIB [in]     Handle to the library 
//          @param rate [out]    Current playing rate
//
//          @retval JAVACALL_OK          Success
//          @retval JAVACALL_FAIL        Fail
/*****************************************************************************/
PUBLIC javacall_result javacall_media_get_rate(javacall_handle handle, /*OUT*/ long* rate)
{
    MMIJAVA_NATIVE_HANDLE_T     *handle_ptr     = (MMIJAVA_NATIVE_HANDLE_T*)handle;    
    if ((PNULL == handle_ptr) || (PNULL == rate))
    {
        javacall_printf(LOG_STR_LEAVE("invalid argument, hande_ptr=%d"), handle_ptr);
        return JAVACALL_INVALID_ARGUMENT;
    }
    *rate=handle_ptr->rate;
    javacall_printf(LOG_STR_LEAVE("ok, rate=%d"), *rate);
    return JAVACALL_OK;
}

/**
 * Query if recording is supported based on the player's content-type
 *
 * @param handle  Handle to the library
 *
 * @retval JAVACALL_OK          Success
 * @retval JAVACALL_FAIL        Fail
 */
javacall_result javacall_media_supports_recording(javacall_handle handle) 
{       /*lint -e{765}*/
    javacall_printf(LOG_STR_LEAVE("not implemented"));
    return JAVACALL_NOT_IMPLEMENTED;
}

/*****************************************************************************/
//  Description : Is javacall_media_set_recordsize_limit function is working for this player?
//  Global resource dependence : none
//  Author: 
//  Note:
//          @param handle    Handle to the library 
//          @param size      The maximum size bytes of the recording requested as input parameter.
//                           The supported maximum size bytes of the recording which is less than or 
//                           equal to the requested size as output parameter.
// 
//          @retval JAVACALL_OK              Success
//          @retval JAVACALL_FAIL            Fail
//          @retval JAVACALL_NOT_IMPLEMENTED Unsupported
/*****************************************************************************/
PUBLIC javacall_result javacall_media_set_recordsize_limit_supported(javacall_handle handle,long* size) 
{       /*lint -e{765}*/
    javacall_printf(LOG_STR_LEAVE("fail"));
    return JAVACALL_FAIL;
}

/**
 * Specify the maximum size of the recording including any headers.<br>
 * If a size of -1 is passed then the record size limit should be removed.<br>
 * If device don't want to support this feature, just return JAVACALL_FAIL always.
 *
 * @param handle    Handle to the library
 * @param size      The maximum size bytes of the recording requested as input parameter.
 *                  The supported maximum size bytes of the recording which is less than or
 *                  equal to the requested size as output parameter.
 *
 * @retval JAVACALL_OK          Success
 * @retval JAVACALL_FAIL        Fail
 */
javacall_result javacall_media_set_recordsize_limit(javacall_handle handle, /*INOUT*/ long* size) {
    javacall_printf(LOG_STR_LEAVE("not implemented"));
    return JAVACALL_NOT_IMPLEMENTED;
}

/*****************************************************************************/
//  Description : Is this recording transaction is handled by native layer or Java layer?
//  Global resource dependence : none
//  Author: 
//  Note:
//          @param handle    Handle to the library 
//          @param locator   URL locator string for recording data (ex: file:///root/test.wav)
//
//          @retval JAVACALL_OK      This recording transaction will be handled by native layer
//          @retval JAVACALL_FAIL    This recording transaction should be handled by Java layer
/*****************************************************************************/
PUBLIC javacall_result javacall_media_recording_handled_by_native(
                                                                  javacall_handle handle, 
                                                                  javacall_const_utf16_string locator,
                                                                  long locatorLength)
{
    MMIJAVA_NATIVE_HANDLE_T *handle_ptr     = (MMIJAVA_NATIVE_HANDLE_T*)handle;

    javacall_printf(LOG_STR_ENTER("handle=0x%x, len=%d, locator=%s"), handle, locatorLength, javacall_nwstr2str(locator, locatorLength));/*lint !e64 */
    if (PNULL == handle_ptr)
    {
        javacall_printf(LOG_STR_LEAVE("invalid argument, null handle"));
        return JAVACALL_INVALID_ARGUMENT;
    }

    if (jc_convert_file_locator(locator, locatorLength, handle_ptr->filename, MMIFILE_FULL_PATH_MAX_LEN+2)) {
        handle_ptr->isTempFile = FALSE;
        javacall_printf(LOG_STR_LEAVE("ok, native file=%s"), javacall_wstr2str(handle_ptr->filename));/*lint !e64 */
        return JAVACALL_OK;
    }
    javacall_printf(LOG_STR_LEAVE("fail, can not convert"));
    return JAVACALL_FAIL;
}

/*****************************************************************************/
//  Description : Starts the recording. records all the data of the player ( video / audio )
//  Global resource dependence : none
//  Author: 
//  Note:
//          @param handle  Handle to the library 
//
//          @retval JAVACALL_OK          Success
//          @retval JAVACALL_FAIL        Fail
/*****************************************************************************/
PUBLIC javacall_result javacall_media_start_recording(javacall_handle handle) 
{
    BOOLEAN                 ret             = TRUE;
    MMIJAVA_NATIVE_HANDLE_T *handle_ptr     = (MMIJAVA_NATIVE_HANDLE_T*)handle;
    
    if (PNULL == handle_ptr)
    {
        javacall_printf(LOG_STR_LEAVE("invalid argument, null handle"));
        return JAVACALL_INVALID_ARGUMENT;
    }
    
    javacall_printf(LOG_STR_ENTER("handle=0x%x, filename=%s"), handle, javacall_wstr2str(handle_ptr->filename));/*lint !e64 */
    if (handle_ptr == s_pending_handle)
    {
        javacall_printf(LOG_STR("pending handle=0x%x, state=%d, resume to=%d"), handle_ptr, handle_ptr->playingState, s_resume_to_playing_state);
        if (s_resume_to_playing_state == MMIJAVA_MEDIA_PLAYING)
        {
            s_resume_to_playing_state=MMIJAVA_MEDIA_RECORDING;
            javacall_printf(LOG_STR("ok, pending player set resume to recording"));
        }
        return JAVACALL_OK;
    }
    
    if ((PNULL != handle_ptr->mediaItfPtr)&&(PNULL != handle_ptr->mediaItfPtr->vptrRecord)) {
        if (MMIAPICOM_Wstrlen(handle_ptr->filename) == 0)
            get_temp_file_name(handle_ptr, handle_ptr->mediatype, handle_ptr->filename);
        ret = handle_ptr->mediaItfPtr->vptrRecord->start_recording(handle_ptr->filename);
    }    
    
    if (ret)
    {
        handle_ptr->playingState = MMIJAVA_MEDIA_RECORDING;
    }    
    javacall_printf(LOG_STR_LEAVE("%s, handle=0x%x"), (ret? "ok":"fail"), handle_ptr);
    return (ret? JAVACALL_OK:JAVACALL_FAIL);
}

/*****************************************************************************/
//  Description : Pause the recording. this should enable a future call to javacall_media_start_recording. 
//              Another call to javacall_media_start_recording after pause has been called will result 
//              in recording the new data and concatanating it to the previously recorded data.
//  Global resource dependence : none
//  Author: 
//  Note:
//          @param handle  Handle to the library 
//
//          @retval JAVACALL_OK          Success
//          @retval JAVACALL_FAIL        Fail
/*****************************************************************************/
PUBLIC javacall_result javacall_media_pause_recording(javacall_handle handle) 
{
    BOOLEAN                 ret             = FALSE;
    MMIJAVA_NATIVE_HANDLE_T *handle_ptr     = (MMIJAVA_NATIVE_HANDLE_T*)handle;
   
    if (PNULL == handle_ptr)
    {
        javacall_printf(LOG_STR_LEAVE("invalid argument, null handle"));
        return JAVACALL_INVALID_ARGUMENT;
    }
     
    if (handle_ptr == s_pending_handle)
    {
        javacall_printf(LOG_STR("pending handle=0x%x, state=%d, resume to=%d"), handle_ptr, handle_ptr->playingState, s_resume_to_playing_state);
        if (s_resume_to_playing_state == MMIJAVA_MEDIA_RECORDING)
        {
            s_resume_to_playing_state=MMIJAVA_MEDIA_PLAYING;
            javacall_printf(LOG_STR("ok, pending player set resume to playing"));
        }
        return JAVACALL_OK;
    }

    if ((PNULL != handle_ptr->mediaItfPtr)&&(PNULL != handle_ptr->mediaItfPtr->vptrRecord)) {
        ret = handle_ptr->mediaItfPtr->vptrRecord->pause_recording();
    }    

    if (ret)
        handle_ptr->playingState = MMIJAVA_MEDIA_PLAYING;

    javacall_printf(LOG_STR_LEAVE("%s, handle=0x%x"), (ret? "ok":"fail"), handle_ptr);
    return (ret? JAVACALL_OK:JAVACALL_FAIL);
}

/*****************************************************************************/
//  Description : Stop the recording.
//  Global resource dependence : none
//  Author: 
//  Note:
//          @param handle  Handle to the library 
//
//          @retval JAVACALL_OK          Success
//          @retval JAVACALL_FAIL        Fail
/*****************************************************************************/
PUBLIC javacall_result javacall_media_stop_recording(javacall_handle handle) 
{
    BOOLEAN                 ret             = FALSE;
    MMIJAVA_NATIVE_HANDLE_T *handle_ptr     = (MMIJAVA_NATIVE_HANDLE_T*)handle;
   
    if (PNULL == handle_ptr)
    {
        javacall_printf(LOG_STR_LEAVE("invalid argument, null handle"));
        return JAVACALL_INVALID_ARGUMENT;
    }

    if (handle_ptr == s_pending_handle)
    {
        javacall_printf(LOG_STR("pending handle=0x%x, state=%d, resume to=%d"), handle_ptr, handle_ptr->playingState, s_resume_to_playing_state);
        if (s_resume_to_playing_state == MMIJAVA_MEDIA_RECORDING)
        {
            s_resume_to_playing_state=MMIJAVA_MEDIA_PLAYING;
            javacall_printf(LOG_STR("ok, pending player set resume to playing"));
        }
        return JAVACALL_OK;
    }

    if ((PNULL != handle_ptr->mediaItfPtr)&&(PNULL != handle_ptr->mediaItfPtr->vptrRecord)) {
        ret = handle_ptr->mediaItfPtr->vptrRecord->stop_recording();
    }    
    if (ret)
        handle_ptr->playingState = MMIJAVA_MEDIA_PLAYING;

    javacall_printf(LOG_STR_LEAVE("%s, handle=0x%x"), (ret? "ok":"fail"), handle_ptr);
    return (ret? JAVACALL_OK:JAVACALL_FAIL);
}

/*****************************************************************************/
//  Description : The recording that has been done so far should be discarded. (deleted)
//              Recording will be paused before this method is called. 
//              If javacall_media_start_recording is called after this method is called, recording should resume.
//              Calling reset after javacall_media_finish_recording will have no effect on the current recording.
//              If the Player that is associated with this RecordControl is closed, 
//              javacall_media_reset_recording will be called implicitly. 
//  Global resource dependence : none
//  Author: 
//  Note:
//          @param handle  Handle to the library 
//
//          @retval JAVACALL_OK          Success
//          @retval JAVACALL_FAIL        Fail
/*****************************************************************************/
PUBLIC javacall_result javacall_media_reset_recording(javacall_handle handle) 
{
    BOOLEAN                 ret             = FALSE;
    MMIJAVA_NATIVE_HANDLE_T *handle_ptr     = (MMIJAVA_NATIVE_HANDLE_T*)handle;
   
    if (PNULL == handle_ptr)
    {
        javacall_printf(LOG_STR_LEAVE("invalid argument, null handle"));
        return JAVACALL_INVALID_ARGUMENT;
    }

    if (handle_ptr == s_pending_handle)
    {
        javacall_printf(LOG_STR("pending handle=0x%x, state=%d, resume to=%d"), handle_ptr, handle_ptr->playingState, s_resume_to_playing_state);
        if (s_resume_to_playing_state == MMIJAVA_MEDIA_RECORDING)
        {
            s_resume_to_playing_state=MMIJAVA_MEDIA_PLAYING;
            javacall_printf(LOG_STR("ok, pending player set resume to playing"));
        }
        return JAVACALL_OK;
    }

    if ((PNULL != handle_ptr->mediaItfPtr)&&(PNULL != handle_ptr->mediaItfPtr->vptrRecord)) {
        ret = handle_ptr->mediaItfPtr->vptrRecord->reset_recording(handle_ptr->filename);
    }    
    if (handle_ptr->isTempFile == TRUE)
        MMIAPIFMM_DeleteFile(handle_ptr->filename, NULL);
    if (ret)
        handle_ptr->playingState = MMIJAVA_MEDIA_PLAYING;

    javacall_printf(LOG_STR_LEAVE("%s, handle=0x%x"), (ret? "ok":"fail"), handle_ptr);
    return (ret? JAVACALL_OK:JAVACALL_FAIL);
}

/*****************************************************************************/
//  Description : The recording should be completed; 
//              this may involve updating the header,flushing buffers and closing the temporary file if it is used
//              by the implementation.
//              javacall_media_pause_recording will be called before this method is called.
//  Global resource dependence : none
//  Author: 
//  Note:
//          @param handle  Handle to the library 
//
//          @retval JAVACALL_OK          Success
//          @retval JAVACALL_FAIL        Fail
/*****************************************************************************/
PUBLIC javacall_result javacall_media_commit_recording(javacall_handle handle) 
{
    javacall_printf(LOG_STR_LEAVE("ok"));
    return JAVACALL_OK;
}

/*****************************************************************************/
//  Description : Get how much data was returned. 
//              This function can be called after a successful call to javacall_media_finish_recording.
//  Global resource dependence : none
//  Author: 
//  Note:
//          @param handle    Handle to the library 
//          @param size      How much data was recorded
//
//          @retval JAVACALL_OK          Success
//          @retval JAVACALL_FAIL        Fail
/*****************************************************************************/
PUBLIC javacall_result javacall_media_get_recorded_data_size(
                                                             javacall_handle handle, 
                                                             /*OUT*/ long* size
                                                             ) 
{
    BOOLEAN                 ret             = FALSE;
    int32 ret_size;
    MMIJAVA_NATIVE_HANDLE_T *handle_ptr     = (MMIJAVA_NATIVE_HANDLE_T*)handle;
   
    if ((PNULL == handle_ptr)||(PNULL == size))
    {
        javacall_printf(LOG_STR_LEAVE("invalid argument, handle_ptr = %d,size = %d"),handle_ptr,size);
        return JAVACALL_INVALID_ARGUMENT;
    }

    if ((PNULL != handle_ptr->mediaItfPtr)&&(PNULL != handle_ptr->mediaItfPtr->vptrRecord))
    {
        ret = handle_ptr->mediaItfPtr->vptrRecord->get_recorded_data_size(handle_ptr->filename, &ret_size);
        *size=ret_size;
        javacall_printf(LOG_STR_LEAVE("ret = %d,size = %d"),ret,*size);
        return (ret?JAVACALL_OK:JAVACALL_FAIL);
    }    

    javacall_printf(LOG_STR_LEAVE("fail, handle=0x%x has no get_recorded_data_size()"),handle_ptr);
    return JAVACALL_FAIL;
}

/*****************************************************************************/
//Description : Gets the recorded data.
//              This function can be called after a successful call to javacall_media_finish_recording.
//              It receives the data recorded from offset till the size.
//  Global resource dependence : none
//  Author: 
//  Note:
//          @param handle    Handle to the library 
//          @param buffer    Buffer will contains the recorded data
//          @param offset    An offset to the start of the required recorded data
//          @param size      How much data will be copied to buffer
//
//          @retval JAVACALL_OK          Success
//          @retval JAVACALL_FAIL        Fail
/*****************************************************************************/
PUBLIC javacall_result javacall_media_get_recorded_data(
                                                        javacall_handle handle, 
                                                        /*OUT*/ char* buffer, 
                                                        long offset, 
                                                        long size
                                                        ) 
{
    BOOLEAN                 ret             = FALSE;
    MMIJAVA_NATIVE_HANDLE_T *handle_ptr     = (MMIJAVA_NATIVE_HANDLE_T*)handle;
   
    if ((PNULL == handle_ptr)||(PNULL == buffer))
    {
        javacall_printf(LOG_STR_LEAVE("invalid argument, handle_ptr = %d,buffer = %d"),handle_ptr,buffer);
        return JAVACALL_INVALID_ARGUMENT;
    }

    if ((PNULL != handle_ptr->mediaItfPtr)&&(PNULL != handle_ptr->mediaItfPtr->vptrRecord))
    {
        ret = handle_ptr->mediaItfPtr->vptrRecord->get_recorded_data(handle_ptr->filename, buffer,offset,size);
        javacall_printf(LOG_STR_LEAVE("ret = %d,offset = %d, size = %d"),ret,offset,size);
        return (ret?JAVACALL_OK:JAVACALL_FAIL);
    }    

    javacall_printf(LOG_STR_LEAVE("fail, handle=0x%x has no get_recorded_data()"),handle_ptr);
    return JAVACALL_FAIL;
}

/*****************************************************************************/
//  Description : Get the current recording data content type mime string length
//  Global resource dependence : none
//  Author: 
//  Note:
//          @param handle    Handle to the library 
//          @param length    Length of string
// 
//          @retval JAVACALL_OK          Success
//          @retval JAVACALL_FAIL        Fail
/*****************************************************************************/
PUBLIC javacall_result javacall_media_get_record_content_type_length(
                                                         javacall_handle handle,
                                                         int*  length //OUT
                                                         )
{
    int32                       ret             = 0;
    MMIJAVA_NATIVE_HANDLE_T     *handle_ptr     = (MMIJAVA_NATIVE_HANDLE_T*)handle;
   
    if ((PNULL == handle_ptr)||(PNULL == length))
    {
        javacall_printf(LOG_STR_LEAVE("invalid argument, handle_ptr = %d,length = %d"),handle_ptr,length);
        return JAVACALL_INVALID_ARGUMENT;
    }

    if ((PNULL != handle_ptr->mediaItfPtr)&&(PNULL != handle_ptr->mediaItfPtr->vptrRecord))
    {
        ret = handle_ptr->mediaItfPtr->vptrRecord->get_record_content_type_length();
        *length = ret;
        javacall_printf(LOG_STR("ret = %d"),ret);
        return JAVACALL_OK;
    }

    javacall_printf(LOG_STR_LEAVE("fail, handle=0x%x has no get_record_content_type_length()"),handle_ptr);
    *length = -1;
    return JAVACALL_FAIL;
}

/*****************************************************************************/
//Description : Get the current recording data content type mime string length
//              For example : 'audio/x-wav' for audio recording
//  Global resource dependence : none
//  Author: 
//  Note:
//          @param handle                Handle of native player
//          @param contentTypeBuf        Buffer to return content type unicode string
//          @param length                Length of contentTypeBuf as input parameter and 
//                                       length of content type string stored in contentTypeBuf
//
//          @return  JAVACALL_OK          Success  JAVACALL_FAIL        Fail
/*****************************************************************************/
PUBLIC javacall_result javacall_media_get_record_content_type(
                                                    javacall_handle handle, 
                                                    /*OUT*/ javacall_utf16* contentTypeBuf,
                                                    int* length //INOUT
                                                    ) 
{
    int32                       ret             = 0;
    MMIJAVA_NATIVE_HANDLE_T     *handle_ptr     = (MMIJAVA_NATIVE_HANDLE_T*)handle;
   
    if ((PNULL == handle_ptr)||(PNULL == length)||(PNULL == contentTypeBuf))
    {
        javacall_printf(LOG_STR_LEAVE("invalid argument, handle_ptr = %d,length = %d, contentTypeBuf = %d"),handle_ptr,length,contentTypeBuf);
        return JAVACALL_INVALID_ARGUMENT;
    }
    

    if ((PNULL != handle_ptr->mediaItfPtr)&&(PNULL != handle_ptr->mediaItfPtr->vptrRecord))
    {
        ret = handle_ptr->mediaItfPtr->vptrRecord->get_record_content_type(contentTypeBuf);
        javacall_printf(LOG_STR("ret = %d"),ret);
        *length = ret;
        return JAVACALL_OK;
    }

    javacall_printf(LOG_STR_LEAVE("fail, handle=0x%x has no get_record_content_type()"),handle_ptr);
    *length = -1;
    return JAVACALL_FAIL;
}

/*****************************************************************************/
//  Description : Close the recording. Delete all resources related with this recording.
//  Global resource dependence : none
//  Author: 
//  Note: 
//          @param handle    Handle to the library 
// 
//          @retval JAVACALL_OK      Success
//          @retval JAVACALL_FAIL    Fail
/*****************************************************************************/
PUBLIC javacall_result javacall_media_close_recording(javacall_handle handle) 
{
    BOOLEAN                 ret             = TRUE;
    MMIJAVA_NATIVE_HANDLE_T *handle_ptr     = (MMIJAVA_NATIVE_HANDLE_T*)handle;
   
    if (PNULL == handle_ptr)
    {
        javacall_printf(LOG_STR_LEAVE("invalid argument, null handle"));
        return JAVACALL_INVALID_ARGUMENT;
    }

    if ((PNULL != handle_ptr->mediaItfPtr)&&(PNULL != handle_ptr->mediaItfPtr->vptrRecord))
    {
        
        javacall_printf(LOG_STR_LEAVE("ret = %d"),ret);
        return (ret?JAVACALL_OK:JAVACALL_FAIL);
    }    

    javacall_printf(LOG_STR_LEAVE("ok, handle=0x%x has no set_video_visible()"),handle_ptr);
    return JAVACALL_OK;
}


/*****************************************************************************/
//  Description : This function called by JVM when this player goes to foreground.
//              There is only one foreground midlets but, multiple player can be exits at this midlets.
//              So, there could be multiple players from JVM.
//              Device resource handling policy is not part of Java implementation. It is totally depends on
//              native layer's implementation.
//  Global resource dependence : none
//  Author: 
//  Note:
//          @param handle    Handle to the native player
//          @param appID     ID of the application to be foreground
// 
//          @retval JAVACALL_OK  Somthing happened
//          @retval JAVACALL_OK  Nothing happened
/*****************************************************************************/
PUBLIC javacall_result javacall_media_to_foreground(
                                                    javacall_handle handle,
                                                     const int appID
                                                    )
{
    MMIJAVA_NATIVE_HANDLE_T *handle_ptr     = (MMIJAVA_NATIVE_HANDLE_T*)handle;
    
    javacall_printf(LOG_STR_ENTER("handle_ptr = %d"),handle_ptr);

    if (PNULL == handle_ptr)
    {
        javacall_printf(LOG_STR_LEAVE("invalid argument, null handle"));
        return JAVACALL_INVALID_ARGUMENT;
    }

    handle_ptr->isForeground = TRUE;

    return JAVACALL_OK;
}

/*****************************************************************************/
// Description : This function called by JVM when this player goes to background.
//              There could be multiple background midlets. Also, multiple player can be exits at this midlets.
//              Device resource handling policy is not part of Java implementation. It is totally depends on
//              native layer's implementation.
//  Global resource dependence : none
//  Author: 
//  Note: 
//          @param handle    Handle to the native player
//          @param appID     ID of the application to be background
// 
//          @retval JAVACALL_OK  Somthing happened
//          @retval JAVACALL_OK  Nothing happened
/*****************************************************************************/
PUBLIC javacall_result javacall_media_to_background(
                                                    javacall_handle handle,
                                                    const int appID
                                                    )
{
    MMIJAVA_NATIVE_HANDLE_T *handle_ptr     = (MMIJAVA_NATIVE_HANDLE_T*)handle;
    
    javacall_printf(LOG_STR_ENTER("handle_ptr = %d"),handle_ptr);

    if (PNULL == handle_ptr)
    {
        javacall_printf(LOG_STR_LEAVE("invalid argument, null handle"));
        return JAVACALL_INVALID_ARGUMENT;
    }

    handle_ptr->isForeground = FALSE;
    
    return JAVACALL_OK;
}

/*****************************************************************************/
//  Description : Convert mime string to media type constants value
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL MMIJAVA_MEDIA_TYPE_T media_uri_to_type(const javacall_utf16* uri, long length)
{
    MMIJAVA_MEDIA_TYPE_T             ret         =   JAVACALL_END_OF_TYPE;
    javacall_const_utf16_string      tail        =   PNULL;    
    wchar                            *tail_type[] =   {
                                                        L"mp3",
#ifdef MIDI_SUPPORT
                                                        L"mid",
//                                                        L"jts",
#endif
#ifdef AMR_SUPPORT
                                                        L"amr",
#endif
#ifdef AAC_SUPPORT
                                                        L"m4a",
                                                        L"aac",
#endif
#ifdef WAV_SUPPORT
                                                        L"wav",
#endif
                                                        L"mp4",
                                                        L"3gp",
#ifdef AVI_ENC_SUPPORT
                                                        L"avi"
#endif
                                                            };
    MMIJAVA_MEDIA_TYPE_T             media_type[]=   {JAVACALL_AUDIO_MP3,
#ifdef MIDI_SUPPORT
                                                      JAVACALL_AUDIO_MIDI,
//                                                      JAVACALL_AUDIO_TONE,
#endif
#ifdef AMR_SUPPORT
                                                      JAVACALL_AUDIO_AMR,
#endif
#ifdef AAC_SUPPORT
                                                      JAVACALL_AUDIO_M4A,
                                                      JAVACALL_AUDIO_AAC,
#endif
#ifdef WAV_SUPPORT
                                                      JAVACALL_AUDIO_WAV,
#endif
                                                      JAVACALL_VIDEO_MPEG4,
                                                      JAVACALL_VIDEO_3GPP,
#ifdef AVI_ENC_SUPPORT
                                                      JAVACALL_VIDEO_AVI
#endif
                                                    };
    uint32                           i           =   0;
    uint32                           extlen      =   0;

    
    SCI_ASSERT(PNULL != uri);   /*assert verified*/
    
    
    if (0 == MMIAPICOM_Wstrncmp(uri, VIDEO_CAPTURE_LOCATOR, MMIAPICOM_Wstrlen(VIDEO_CAPTURE_LOCATOR))) 
    {
        ret = JAVACALL_CAPTURE_VIDEO;
    } 
    else if (0 == MMIAPICOM_Wstrncmp(uri, AUDIO_CAPTURE_LOCATOR, MMIAPICOM_Wstrlen(AUDIO_CAPTURE_LOCATOR))) 
    {
        ret = JAVACALL_CAPTURE_AUDIO;
    }
    else if (0 == MMIAPICOM_Wstrncmp(uri, RADIO_CAPTURE_LOCATOR, MMIAPICOM_Wstrlen(RADIO_CAPTURE_LOCATOR))) 
    {
        ret = JAVACALL_CAPTURE_RADIO;
    }
    else if (0 == MMIAPICOM_Wstrncmp(uri, RTSP_PROTOCOL_PREFIX, MMIAPICOM_Wstrlen(RTSP_PROTOCOL_PREFIX))) 
    {
        ret = JAVACALL_CAPTURE_RTSP;
    }
#ifdef MIDI_SUPPORT
    else if (0 == MMIAPICOM_Wstrncmp(uri,DEVICE_MIDI_LOCATOR,MMIAPICOM_Wstrlen(DEVICE_MIDI_LOCATOR))) 
    {
        ret = JAVACALL_INTERACTIVE_MIDI;
    } 
#endif
    else 
    {
        for (i = 0; i < (sizeof(media_type)/sizeof(media_type[0])); i++)
        {        
            extlen = MMIAPICOM_Wstrlen(tail_type[i]);
            tail = uri + length - extlen;
            //javacall_printf(LOG_STR("tail=%s"), javacall_wstr2str(tail));
            //javacall_printf(LOG_STR("tail[%d]=%s"), i, javacall_wstr2str(tail_type[i]));
            if (0 == MMIAPICOM_CompareTwoWstrExt(tail, extlen, tail_type[i], extlen, FALSE)) 
            {
                javacall_printf(LOG_STR("found type=%d"), media_type[i]);
                ret = media_type[i];
                break;
            }     
        }
    }

    javacall_printf(LOG_STR_LEAVE("media_uri_to_type ret = %d"),ret);
    return ret;
}

/*****************************************************************************/
//  Description :  get_temp_dir_path for multimedia
//  Global resource dependence : none
//  Author: 
//  Note:  
//       
/*****************************************************************************/
LOCAL BOOLEAN get_media_temp_path(wchar* path, int* pathLen)
{
    javacall_utf16 filesep[2];

    if (JAVACALL_OK != javacall_dir_get_root_path(path, pathLen))
    {        
        javacall_printf(LOG_STR_LEAVE("false, get root path fail"));
        return FALSE;
    }

    filesep[0] = javacall_get_file_separator();
    filesep[1] = 0;

    MMIAPICOM_Wstrcat(path, filesep);
    MMIAPICOM_Wstrcat(path, L"tmp");
    *pathLen = MMIAPICOM_Wstrlen(path);
    return TRUE;
}
/*****************************************************************************/
//  Description : Get temporary file name
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN get_temp_file_name(
                              MMIJAVA_NATIVE_HANDLE_T* handle_ptr,
                              MMIJAVA_MEDIA_TYPE_T mediaType, 
                              wchar* filename  //[OUT]
                              )
{   
    char            * subfix                                    = PNULL;
    uint16          full_path_len                               = MMIFILE_FULL_PATH_MAX_LEN+2;
//    uint16          random                                      = 0;
    uint8           tmp[10]                                     = {0};
    wchar           tmp2[10]                                    = {0};
    int rootPathLen = MMIFILE_FULL_PATH_MAX_LEN;
    uint16          tmp2_len                                     = 0;
    int i;

    SCI_ASSERT(PNULL != filename);  /*assert verified*/

    switch(mediaType) 
    {
#ifdef MIDI_SUPPORT
    case JAVACALL_AUDIO_MIDI:
        subfix = "mid";
        break;
#endif    
#ifdef WAV_SUPPORT
    case JAVACALL_AUDIO_WAV:
        subfix = "wav";
        break;
#endif
    case JAVACALL_AUDIO_MP3:
        subfix = "mp3";
        break;
    case JAVACALL_VIDEO_MPEG4:
        subfix = "mp4";
        break;

#ifdef AVI_ENC_SUPPORT
    case JAVACALL_VIDEO_AVI:
        subfix = "avi";
        break;
#endif

    case JAVACALL_VIDEO_3GPP:
    case JAVACALL_CAPTURE_VIDEO:
        subfix = "3gp";
        break;
        
#ifdef AAC_SUPPORT
    case JAVACALL_AUDIO_AAC:
        subfix = "aac";
        break;

     case JAVACALL_AUDIO_M4A:
        subfix = "m4a";
        break;     
#endif
#ifdef AMR_SUPPORT  
    case JAVACALL_CAPTURE_AUDIO:      
    case JAVACALL_AUDIO_AMR:
        subfix = "amr";
        break;
#endif    
    case JAVACALL_AUDIO_TONE:
        subfix = "xts";
        break;
    case JAVACALL_IMAGE_GIF:
        subfix = "gif";
        break;
    default:
        javacall_printf(LOG_STR_LEAVE("false, mediaType = %d"),mediaType);
        return FALSE;
    }  

    if (TRUE != get_media_temp_path(filename, &rootPathLen))
    {        
        javacall_printf(LOG_STR_LEAVE("false, get root path fail"));
        return FALSE;
    }
    filename[rootPathLen]=javacall_get_file_separator();
    rootPathLen++;
   
    for (i=0; i<4; i++)
    {
        //generate random
        sprintf((char*)tmp, "%02d%02d%01d.%s", handle_ptr->playerId, handle_ptr->AppID, i, subfix); 
        MMIAPICOM_StrToWstr(tmp, tmp2);    
        tmp2_len=MMIAPICOM_Wstrlen(tmp2);
    
        if (rootPathLen+tmp2_len > MMIFILE_FULL_PATH_MAX_LEN)
        {
            javacall_printf(LOG_STR_LEAVE("false, mediaType = %d, rootPathLen=%d, tmp2_len=%d"),mediaType, rootPathLen, tmp2_len);
            return FALSE;
        }

        MMI_WSTRNCPY(filename+rootPathLen, MMIFILE_FULL_PATH_MAX_LEN-rootPathLen,
            tmp2, tmp2_len, tmp2_len);
        filename[rootPathLen+tmp2_len+1]=PNULL;
        if(FALSE == MMIAPIFMM_IsFileExist(filename, full_path_len))
            {
            javacall_printf(LOG_STR_LEAVE("ok, mediatype = %d, filename=%s"), mediaType, javacall_wstr2str(filename));/*lint !e64 */
            return TRUE;  
            }
        if (SFS_NO_ERROR == MMIAPIFMM_DeleteFile(filename, NULL))
        {

    javacall_printf(LOG_STR_LEAVE("ok, mediatype = %d, filename=%s"), mediaType, javacall_wstr2str(filename));/*lint !e64 */
    return TRUE;  
        }
    }
    javacall_printf(LOG_STR_LEAVE("fail, mediatype = %d, filename=%s"), mediaType, javacall_wstr2str(filename));/*lint !e64 */
    return FALSE;  
}

/*****************************************************************************/
//  Description :  set volume and set mute
//  Global resource dependence : none
//  Author: 
//  Note: 
//           @param handle    Handle to the library
// 
//           @retval JAVACALL_OK
//           @retval JAVACALL_FAIL   
 /*****************************************************************************/
LOCAL void MMIJAVA_SetVolume(javacall_handle handle)
{
    MMIJAVA_NATIVE_HANDLE_T     *handle_ptr     = (MMIJAVA_NATIVE_HANDLE_T*)handle;
    AUDIO_DEVICE_MODE_TYPE_E  mode = AUDIO_DEVICE_MODE_HANDFREE;
    MMIAPISET_GetDeviceMode(&mode);    
    MMIAUDIO_SetVolume(mode, (uint32)(handle_ptr->cur_vol*s_max_volume/MMIJAVA_MAX_VOLUME));
    javacall_printf(LOG_STR(" mode= %d, volume =%d"), mode,  handle_ptr->cur_vol);  
}

/*****************************************************************************/
//  Description : set status for having stopped by other
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MMIJAVA_SetIsStoppedbyOther(BOOLEAN is_stopped)
{


    if(PNULL != s_cur_handle)
    {
        if (s_cur_handle->playingState != MMIJAVA_MEDIA_UNINIT)
        {
            javacall_printf(LOG_STR("set handle=0x%x stopped=%d"), s_cur_handle, is_stopped);
            if (is_stopped) {
                s_cur_handle->playingState = MMIJAVA_MEDIA_UNINIT;
/*                if (s_cur_handle->isStoppedByOther != is_stopped) 
                {
                    //this maybe called mutil times, let only stop at the first time
                    if ((PNULL != s_cur_handle->mediaItfPtr)&&(PNULL != s_cur_handle->mediaItfPtr->vptrBasic))
                    {        
                        ret = s_cur_handle->mediaItfPtr->vptrBasic->stopped_by_other();        
                        javacall_printf(LOG_STR_LEAVE("ret = %d"), ret);    
                    }    
                }*/
                s_cur_handle->isStoppedByOther = is_stopped;
            }
        }
    }
}

/*****************************************************************************/
//  Description : pause current playing media when JVM lose focus.
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MMIJAVA_PauseAllMediaPlayer()
{
    javacall_printf(LOG_STR_ENTER("s_cur_handle=0x%x"), s_cur_handle);

    s_pending_handle = PNULL;
    s_resume_to_playing_state = MMIJAVA_MEDIA_UNINIT;

    if(PNULL != s_cur_handle)
    {
        javacall_printf(LOG_STR("s_cur_handle->playingState=%d"), s_cur_handle->playingState);
        switch (s_cur_handle->playingState)
        {
            case MMIJAVA_MEDIA_RECORDING:
                javacall_media_pause_recording(s_cur_handle);
                javacall_media_pause(s_cur_handle);
                s_resume_to_playing_state = MMIJAVA_MEDIA_RECORDING;  
                break;
            case MMIJAVA_MEDIA_PLAYING:
                javacall_media_pause(s_cur_handle);
                s_resume_to_playing_state = MMIJAVA_MEDIA_PLAYING;   
                break;
            default:
                break;
        }
        s_pending_handle = s_cur_handle;
    }
    javacall_printf(LOG_STR_LEAVE("s_cur_handle=0x%x"), s_cur_handle);
}

/*****************************************************************************/
//  Description : resume current playing media when JVM get focus.
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MMIJAVA_ResumeAllMediaPlayer()
{
    MMIJAVA_NATIVE_HANDLE_T* handle = PNULL;

    javacall_printf(LOG_STR_ENTER("s_cur_handle=0x%x, s_pending_handle=0x%x"), s_cur_handle, s_pending_handle);
    if (PNULL != s_pending_handle)
    {
        handle=s_pending_handle;
        s_pending_handle=PNULL;
        javacall_printf(LOG_STR("pend handle state=%d, resume to = %d"), handle->playingState, s_resume_to_playing_state);
        if ((handle->playingState != s_resume_to_playing_state) && (handle->playingState == MMIJAVA_MEDIA_PAUSED))
        {
           switch (s_resume_to_playing_state)
            {
                case MMIJAVA_MEDIA_RECORDING:
                    javacall_media_resume(handle);
                    javacall_media_start_recording(handle);
                    break;
                case MMIJAVA_MEDIA_PLAYING:
                    javacall_media_resume(handle);
                    break;
                default:
                    break;                    
            }
        }
    }
    s_resume_to_playing_state = MMIJAVA_MEDIA_UNINIT;
    javacall_printf(LOG_STR_LEAVE("s_cur_handle=0x%x, s_pending_handle=0x%x"), s_cur_handle, s_pending_handle);
}

/*****************************************************************************/
//  Description : close current playing media when JVM shutdown.
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MMIJAVA_CloseAllMediaPlayer()
{
    MMIJAVA_NATIVE_HANDLE_T* handle;
    javacall_printf(LOG_STR_ENTER("s_cur_handle=0x%x"), s_cur_handle);
    if(PNULL != s_cur_handle)
    {
        handle=s_cur_handle;
        switch (handle->playingState)
        {
        case MMIJAVA_MEDIA_RECORDING:
            javacall_media_stop_recording(handle);
            javacall_media_stop(handle);
            break;
        case MMIJAVA_MEDIA_PLAYING:
            javacall_media_stop(handle);
            break;
        default:
            break;
        }
        javacall_media_release_device(handle);
        if ((PNULL != handle->mediaItfPtr)&&(PNULL != handle->mediaItfPtr->vptrBasic)) {
            handle->mediaItfPtr->vptrBasic->close();
        }
    }
    javacall_printf(LOG_STR_LEAVE("s_cur_handle=0x%x"), s_cur_handle);
}

/*****************************************************************************/
//  Description : pause current playing media when .
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MMIJAVA_PauseCurrentVideoMediaPlayer()
{
    javacall_printf(LOG_STR_ENTER("s_cur_handle=0x%x"), s_cur_handle);

    s_pending_handle = PNULL;
    s_resume_to_playing_state = MMIJAVA_MEDIA_UNINIT;

    if(PNULL != s_cur_handle)
    {
        javacall_printf(LOG_STR("s_cur_handle mediatype=%d, playingState=%d"), s_cur_handle->mediatype, s_cur_handle->playingState);
        if (s_cur_handle->mediatype == JAVACALL_CAPTURE_VIDEO) {          
            switch (s_cur_handle->playingState)
            {
                case MMIJAVA_MEDIA_RECORDING:
                    javacall_media_pause_recording(s_cur_handle);
                    javacall_media_pause(s_cur_handle);
                    s_resume_to_playing_state = MMIJAVA_MEDIA_RECORDING;
                    break;
                case MMIJAVA_MEDIA_PLAYING:
                    javacall_media_pause(s_cur_handle);
                    s_resume_to_playing_state = MMIJAVA_MEDIA_PLAYING;
                    break;
                default:
                    break;
            }
            s_pending_handle = s_cur_handle;
        }
    }
    javacall_printf(LOG_STR_LEAVE("s_cur_handle=0x%x"), s_cur_handle);
}
/*****************************************************************************/
//  Description : resume current playing media when JVM get focus.
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MMIJAVA_ResumeCurrentVideoMediaPlayer()
{
    MMIJAVA_NATIVE_HANDLE_T* handle = PNULL;

    javacall_printf(LOG_STR_ENTER("s_cur_handle=0x%x, s_pending_handle=0x%x"), s_cur_handle, s_pending_handle);
    if (PNULL != s_pending_handle)
    {
        handle=s_pending_handle;
        javacall_printf(LOG_STR("pend handle mediatype=%d, state=%d, resume to = %d"), handle->mediatype, handle->playingState, s_resume_to_playing_state);
        if (s_cur_handle->mediatype == JAVACALL_CAPTURE_VIDEO) {
            s_pending_handle=PNULL;
            if ((handle->playingState != s_resume_to_playing_state) && (handle->playingState == MMIJAVA_MEDIA_PAUSED))
            {
                switch (s_resume_to_playing_state)
                {
                    case MMIJAVA_MEDIA_RECORDING:
                        javacall_media_resume(handle);
                        javacall_media_start_recording(handle);
                        break;
                    case MMIJAVA_MEDIA_PLAYING:
                        javacall_media_resume(handle);
                        break;
                    default:
                        break;                    
                }
            }
        }
    }
    s_resume_to_playing_state = MMIJAVA_MEDIA_UNINIT;
    javacall_printf(LOG_STR_LEAVE("s_cur_handle=0x%x, s_pending_handle=0x%x"), s_cur_handle, s_pending_handle);
}

/*****************************************************************************/
//  Description : MMIJAVA_NotyfyMediaEnd
//  Global resource dependence : none
//  Author: hui.zhao
//  Note:
/*****************************************************************************/
PUBLIC void MMIJAVA_NotifyMediaEnd(void)
{
  javacall_printf(LOG_STR_ENTER("s_cur_handle = %x"),s_cur_handle);
  if (PNULL != s_cur_handle)
    {
        javanotify_on_media_notification(JAVACALL_EVENT_MEDIA_END_OF_MEDIA, 
                                      s_cur_handle->AppID,
                                     s_cur_handle->playerId, 
                                     JAVACALL_OK,
                                     PNULL);

        s_cur_handle->playingState = MMIJAVA_MEDIA_UNINIT;
        javacall_media_get_duration(s_cur_handle, &s_cur_handle->defaultMediaTime);
        javacall_printf(LOG_STR_LEAVE("s_cur_handle=0x%x, appid=%d, playerid=%d, defaultMediaTime=%d"),
                        s_cur_handle, s_cur_handle->AppID, s_cur_handle->playerId, s_cur_handle->defaultMediaTime);
    } else {
        javacall_printf(LOG_STR_ENTER("ERROR!!! s_cur_handle=NULL"));
    }
    
}

/*****************************************************************************/
//  Description : find all JAR file from file system and sort them
//  Global resource dependence : none
//  Author: arthur.peng 
//  Note:
/*****************************************************************************/
PUBLIC void MMIJAVA_NotifySnapshotFinished(void)
{
    javacall_printf(LOG_STR("s_cur_handle=0x%p"), s_cur_handle);
    if (PNULL != s_cur_handle)
    {
        javanotify_on_media_notification(JAVACALL_EVENT_MEDIA_SNAPSHOT_FINISHED, 
                                      s_cur_handle->AppID,
                                     s_cur_handle->playerId, 
                                     JAVACALL_OK,
                                     PNULL);
    }
    
}
#if 0
/**
 * Post native media event to Java event handler
 *
 * @param type          Event type
 * @param appId         Application ID
 * @param playerId      Player ID
 * @param status        Event status
 * @param data          Data for this event type
 */
void javanotify_on_media_notification(javacall_media_notification_type type,
                                      int appId,
                                      int playerId,
                                      javacall_result status,
                                      void *data) {
#if ENABLE_JSR_135
    midp_jc_event_union e;
    REPORT_INFO4(LC_MMAPI, "javanotify_on_media_notification type=%d appId=%d playerId%d status=%d\n", type, appId, playerId, status);

    e.eventType = MIDP_JC_EVENT_MULTIMEDIA;
    e.data.multimediaEvent.mediaType  = type;
    e.data.multimediaEvent.appId      = appId;
    e.data.multimediaEvent.playerId   = playerId;
    e.data.multimediaEvent.status     = (int) status;
    e.data.multimediaEvent.data.num32 = (int) data;

    midp_jc_event_send(&e);
#endif
}
#endif
#endif

