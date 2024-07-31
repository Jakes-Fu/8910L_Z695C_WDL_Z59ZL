/*****************************************************************************
** File Name:      mmijava_auido.c                                           *
** Author:         apple.zhang                                               *
** Date:           6/2009                                                    *
** Copyright:      2009 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:                       *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 6/2009       apple.zhang            Create                                *
******************************************************************************/

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/

#ifdef JAVA_SUPPORT_SUN
//#include "mmiset.h"
#include "mmiset_export.h"
#include "mmiaudio.h"
#include "mmiaudio_ctrl.h"
#include "javacall_multimedia.h"
#include "mmijava_multimedia.h"
#include "javacall_logging.h"
#include "java_mmi_text.h"
#include "mmijava_main.h"
/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#define  MMIJAVA_METADATA_COUNTS        4
#define  MMIJAVA_METADATA_LENGTH        32
/**--------------------------------------------------------------------------*
 **                         TYPE DEFINITION                                  *
 **--------------------------------------------------------------------------*/
 //java audio state
typedef enum
{    
    MMIJAVA_AUDIO_PLAYING, //play
    MMIJAVA_AUDIO_PAUSED,  //pause
    MMIJAVA_AUDIO_STOPPED, //stop
    MMIJAVA_AUDIO_STATE_MAX
} MMIJAVA_AUDIO_STATE_E;

//java metadata 
typedef struct  
{
    char  key[MMIJAVA_METADATA_LENGTH];
    char  data[MMIJAVA_METADATA_LENGTH];
}MMIJAVA_METADATA_T;
/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : close audio
//  Global resource dependence : none
//  Author: apple.zhang
//  Note:  return value: 0->successs, 1->failed
/*****************************************************************************/
LOCAL BOOLEAN audio_close(void);
/*****************************************************************************/
//  Description : free resource
//  Global resource dependence : none
//  Author: apple.zhang
//  Note:  return value: 0->successs, 1->failed
/*****************************************************************************/
LOCAL BOOLEAN audio_destroy(void);

/*****************************************************************************/
//  Description : start to play audio
//  Global resource dependence : none
//  Author: apple.zhang
//  Note:  return value: 0->successs, 1->failed
//         filename:需要播放的文件名称
/*****************************************************************************/
LOCAL javacall_result audio_start(wchar *filename,MMIJAVA_MEDIA_TYPE_T mediatype,int32 curtime);

/*****************************************************************************/
//  Description : stop playing audio
//  Global resource dependence : none
//  Author: apple.zhang
//  Note:  return value: 0->successs, 1->failed//        
/*****************************************************************************/
LOCAL javacall_result audio_stop(void);

/*****************************************************************************/
//  Description : pause playing audio
//  Global resource dependence : none
//  Author: apple.zhang
//  Note:  return value: 0->successs, 1->failed//        
/*****************************************************************************/
LOCAL javacall_result audio_pause(void);

/*****************************************************************************/
//  Description : resume playing audio
//  Global resource dependence : none
//  Author: apple.zhang
//  Note:  return value: 0->successs, 1->failed//        
/*****************************************************************************/
LOCAL javacall_result audio_resume(void);

/*****************************************************************************/
//  Description : get current time for the playing audio
//  Global resource dependence : none
//  Author: apple.zhang
//  Note:  return current time for the playing audio
/*****************************************************************************/
LOCAL int32 audio_get_time(void);

/*****************************************************************************/
//  Description : set current time for the playing audio
//  Global resource dependence : none
//  Author: apple.zhang
//  Note:  return current time for the playing audio   
/*****************************************************************************/
LOCAL int32 audio_set_time(int32 ms);

/*****************************************************************************/
//  Description : get total time for the playing audio
//  Global resource dependence : none
//  Author: apple.zhang
//  Note:  return total time for the playing audio   
/*****************************************************************************/
LOCAL int32 audio_get_duration(wchar *filename,MMIJAVA_MEDIA_TYPE_T mediatype);

/*****************************************************************************/
//  Description : acquive device to start
//  Global resource dependence : none
//  Author: apple.zhang
//  Note:  return value: 0->successs, 1->failed
//         filename:需要播放的文件名称
/*****************************************************************************/
LOCAL javacall_result acquire_device(wchar *filename);

/*****************************************************************************/
//  Description : release device to start
//  Global resource dependence : none
//  Author: apple.zhang
//  Note:  return value: 0->successs, 1->failed
//         filename:需要播放的文件名称
/*****************************************************************************/
LOCAL javacall_result release_device(int32 appid, int32 playerid);
LOCAL javacall_result audio_set_rate(MMIJAVA_MEDIA_TYPE_T mediatype, int rate);
/*****************************************************************************/
//  Description : stopped by others
//  Global resource dependence : none
//  Author: apple.zhang
//  Note:  return value: 0->successs, 1->failed
/*****************************************************************************/
LOCAL BOOLEAN stopped_by_other(void);

/*****************************************************************************/
//  Description : end call back
//  Global resource dependence : none
//  Author: apple.zhang
//  Note:  
/*****************************************************************************/
LOCAL void audio_end_callback(MMIAUDIO_RESULT result, DPARAM param);
/*****************************************************************************/
//  Description : swith media type
//  Global resource dependence : none
//  Author: apple.zhang
//  Note:  
/*****************************************************************************/
LOCAL MMIAUDIO_TYPE_E switch_javatype_to_audiotype(MMIJAVA_MEDIA_TYPE_T mediatype);
/*****************************************************************************/
// Description : Get metadata info
// Global resource dependence : none
//  Author: 
//  Note: 
 /*****************************************************************************/
LOCAL uint32 get_metadata_info(void);
/*****************************************************************************/
// Description : Get supported meta data key counts for specefic media type
// Global resource dependence : none
//  Author: 
//  Note: 
 /*****************************************************************************/
LOCAL BOOLEAN get_metadata_key_counts(wchar *filename,MMIJAVA_MEDIA_TYPE_T mediatype,/*OUT*/ long* keyCounts) ;
/*****************************************************************************/
// Description : Get meta data key strings by using media type and index value
// Global resource dependence : none
//  Author: 
//  Note: 
 /*****************************************************************************/
LOCAL BOOLEAN get_metadata_key(                                
                                long index, 
                                long bufLength, 
                                char* keyBuf
                                ) ;
/*****************************************************************************/
// Description : Get meta data value strings by using meta data key string
// Global resource dependence : none
//  Author: 
//  Note:  
/*****************************************************************************/
LOCAL BOOLEAN get_metadata(                           
                            const char* key, 
                            long bufLength, 
                            /*OUT*/ char* dataBuf
                            );

/**--------------------------------------------------------------------------*
 **                         LOCAL VARIABLE DEFINITION                        *
 **--------------------------------------------------------------------------*/
LOCAL MMIJAVA_AUDIO_STATE_E s_audio_state = MMIJAVA_AUDIO_STOPPED; //java audio state


static MMIJAVA_MEDIA_BASIC_INTERFACE_T s_audio_basic_itf = 
{   
    audio_close,
    audio_destroy,    
    audio_start,
    audio_stop,
    audio_pause,
    audio_resume,    
    audio_get_time,
    audio_set_time,
    audio_get_duration,   
    acquire_device,
    release_device,
    audio_set_rate,
    stopped_by_other
};

static MMIJAVA_MEDIA_METADATA_INTERFACE_T s_metadata_interface_info = 
{
    get_metadata_key_counts,
    get_metadata_key,
    get_metadata
};

PUBLIC MMIJAVA_MEDIA_INTERFACE_T g_audio_itf = 
{
    &s_audio_basic_itf,    
    PNULL,
    PNULL,
    &s_metadata_interface_info,
    PNULL
};

LOCAL MMIJAVA_METADATA_T s_metadata_info[MMIJAVA_METADATA_COUNTS] = {0};

/**--------------------------------------------------------------------------*
 **                         Function Implement                               *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : close audio
//  Global resource dependence : none
//  Author: apple.zhang
//  Note:  return value: 0->successs, 1->failed
/*****************************************************************************/
LOCAL BOOLEAN audio_close(void)
{
    if ((s_audio_state == MMIJAVA_AUDIO_PAUSED) || (s_audio_state == MMIJAVA_AUDIO_PLAYING))
    {
        audio_stop();
    }
    return TRUE;    
}
/*****************************************************************************/
//  Description : free resource
//  Global resource dependence : none
//  Author: apple.zhang
//  Note:  return value: 0->successs, 1->failed
/*****************************************************************************/
LOCAL BOOLEAN audio_destroy(void)
{
    return TRUE;
}

/*****************************************************************************/
//  Description : acquive device to start
//  Global resource dependence : none
//  Author: apple.zhang
//  Note:  return value: 0->successs, 1->failed
//         filename:需要播放的文件名称
/*****************************************************************************/
LOCAL javacall_result acquire_device(wchar *filename)
{   
/*
    if(MMIAPISET_IsPermitPlayRing(MMISET_RING_TYPE_JAVA))
    {
        MMIAPISET_StopAllVibrator();              
        MMIAPISET_StopAllRing(TRUE);
        MMIAPISET_SetCurRingType(MMISET_RING_TYPE_JAVA);
*/
        s_audio_state = MMIJAVA_AUDIO_STOPPED;
/*
        result = TRUE;          
    }
    else
    {
        javacall_printf(LOG_STR("acquire_device not permit to start"));
        result = FALSE;
    }
*/  
  
    return JAVACALL_OK;
    

}

/*****************************************************************************/
//  Description : acquive device to start
//  Global resource dependence : none
//  Author: apple.zhang
//  Note:  return value: 0->successs, 1->failed
//         filename:需要播放的文件名称
/*****************************************************************************/
LOCAL javacall_result release_device(int32 appid, int32 playerid)
{   
    if ((s_audio_state == MMIJAVA_AUDIO_PAUSED) || (s_audio_state == MMIJAVA_AUDIO_PLAYING))
    {
        audio_stop();
        s_audio_state = MMIJAVA_AUDIO_STOPPED ;
    }
    return JAVACALL_OK;
}
LOCAL javacall_result audio_set_rate(MMIJAVA_MEDIA_TYPE_T mediatype, int rate)
{
    MMIAUDIO_TYPE_E         music_type      = MMIAUDIO_TYPE_MAX;
    uint32 speed;
    music_type = switch_javatype_to_audiotype(mediatype);
    if (MMIAUDIO_TYPE_MAX == music_type)
    {
        javacall_printf(LOG_STR_LEAVE("fail, unknown audio type"));
        return JAVACALL_FAIL;
    }
    if (MMIAUDIO_MIDI == music_type) {
        switch (rate) {
            case MMIAUDIO_PLAY_RATE_SLOW_2:
                speed=(uint32)(-4);
                break;
            case MMIAUDIO_PLAY_RATE_SLOW_1:
                speed=(uint32)(-2);
                break;
            case MMIAUDIO_PLAY_RATE_NORMAL:
                speed=0;
                break;
            case MMIAUDIO_PLAY_RATE_FAST_1:
                speed=2;
                break;
            case MMIAUDIO_PLAY_RATE_FAST_2:
                speed=4;
                break;
            default:
                return JAVACALL_FAIL;
        }
        MMIAUDIO_AudioSetSpeed(MMIAUDIO_JAVA, speed);
    } else {
        MMIAUDIO_SetPlayRate(rate);
    }
    return JAVACALL_OK;
}

/*****************************************************************************/
//  Description : stopped by others
//  Global resource dependence : none
//  Author: apple.zhang
//  Note:  return value: 0->successs, 1->failed
/*****************************************************************************/
LOCAL BOOLEAN stopped_by_other(void)
{
    s_audio_state=MMIJAVA_AUDIO_STOPPED;
    return TRUE;
}

/*****************************************************************************/
//  Description : end call back
//  Global resource dependence : none
//  Author: apple.zhang
//  Note:  
/*****************************************************************************/
LOCAL void audio_end_callback(MMIAUDIO_RESULT result, DPARAM param)
{
    s_audio_state = MMIJAVA_AUDIO_STOPPED;
    MMIJAVA_NotifyMediaEnd();
}

/*****************************************************************************/
//  Description : swith media type
//  Global resource dependence : none
//  Author: apple.zhang
//  Note:  
/*****************************************************************************/
LOCAL MMIAUDIO_TYPE_E switch_javatype_to_audiotype(MMIJAVA_MEDIA_TYPE_T mediatype)
{
    MMIAUDIO_TYPE_E         music_type      = MMIAUDIO_TYPE_MAX;
    switch(mediatype) 
    {
#ifdef MIDI_SUPPORT
    case JAVACALL_AUDIO_MIDI:
        music_type = MMIAUDIO_MIDI;
        break;
    case JAVACALL_AUDIO_TONE:
        music_type = MMIAUDIO_MIDI;
        break;
#endif
    case JAVACALL_AUDIO_MP3:
        music_type = MMIAUDIO_MP3;
        break;
#ifdef WAV_SUPPORT
    case JAVACALL_AUDIO_WAV:
        music_type = MMIAUDIO_WAVE;
        break;
#endif
#ifdef AMR_SUPPORT
    case JAVACALL_AUDIO_AMR:
        music_type = MMIAUDIO_AMR;
        break;
#endif

#ifdef AAC_SUPPORT
    case JAVACALL_AUDIO_AAC:
    case JAVACALL_AUDIO_M4A:		
        music_type = MMIAUDIO_AAC;
        break;
#endif

    default:           
        break;
    } 

    return music_type;
}

/*****************************************************************************/
//  Description : start to play audio
//  Global resource dependence : none
//  Author: apple.zhang
//  Note:  return value: 0->successs, 1->failed
//         filename:需要播放的文件名称
/*****************************************************************************/
LOCAL javacall_result audio_start(wchar *filename,MMIJAVA_MEDIA_TYPE_T mediatype,int32 curtime)
{
    MMIAUDIO_TYPE_E         music_type      = MMIAUDIO_TYPE_MAX;
    javacall_result         result          = FALSE;
    uint32                  offset          = 0;     
    AUDIO_FORMATINFO_T      format_info = {0};
    

       
    if (MMIJAVA_AUDIO_STOPPED != s_audio_state)
    {
        audio_stop();
    }    

    if(MMIAPISET_IsPermitPlayRing(MMISET_RING_TYPE_JAVA)&&
        !MMIAUDIO_IsBgPlayPaused(MMIBGPLAY_TYPE_JAVA_BG))
    {
        MMIAPISET_StopAllVibrator();              
        MMIAPISET_StopAllRing(TRUE);
        MMIAPISET_SetCurRingType(MMISET_RING_TYPE_JAVA);

        s_audio_state = MMIJAVA_AUDIO_STOPPED;              
    }
    else
    {
        javacall_printf(LOG_STR_LEAVE("fail, not permit to start"));
        return JAVACALL_FAIL;
    }
    
    music_type = switch_javatype_to_audiotype(mediatype);

    javacall_printf(LOG_STR("mediatype = %d, music_type = %d"), mediatype, music_type);

    if (MMIAUDIO_TYPE_MAX == music_type)
    {
        javacall_printf(LOG_STR_LEAVE("fail, unknown audio type"));
        return JAVACALL_FAIL;
    }

    if (MMIAUDIO_CreateAudioFileHandle(FALSE, MMIAUDIO_JAVA, music_type,(const wchar *)filename))
    {    
        offset = 0;
        if((curtime > 0 ) && (TRUE==MMIAUDIO_AudioGetFileInfo(MMIAUDIO_JAVA,&format_info)))
        {
            if (0 != format_info.uiTotalTime)
            {
                offset = (uint32)(curtime * format_info.uiTotalDataLength / (format_info.uiTotalTime * 1000));/*lint !e737 */
            }
        }
        javacall_printf(LOG_STR("curtime=%d, offset=%d, total time=%d, total length=%d"), curtime, offset, format_info.uiTotalTime, format_info.uiTotalDataLength);

        if(MMIAUDIO_AudioPlay(MMIAUDIO_JAVA,offset,1,audio_end_callback))
        {
            s_audio_state = MMIJAVA_AUDIO_PLAYING;
            javacall_printf(LOG_STR_LEAVE("true, playing"));
            result = JAVACALL_OK;
        }
        else
        {
    
            javacall_printf(LOG_STR_LEAVE("false, play failed"));            
            MMIAUDIO_CloseAudioHandle(MMIAUDIO_JAVA);
            MMIAPISET_ClearCurRingType(MMISET_RING_TYPE_JAVA);
            result = JAVACALL_FAIL;
        }            
   
    }
    else
    {
        javacall_printf(LOG_STR_LEAVE("false, create handle failed"));
        MMIAPISET_ClearCurRingType(MMISET_RING_TYPE_JAVA);
        result = JAVACALL_FAIL;
    }      
   

    return result;    
    
}

/*****************************************************************************/
//  Description : stop playing audio
//  Global resource dependence : none
//  Author: apple.zhang
//  Note:  return value: 0->successs, 1->failed//        
/*****************************************************************************/
LOCAL javacall_result audio_stop(void)
{
    javacall_printf(LOG_STR_ENTER("audio_stop enter"));    
    MMIAUDIO_AudioStop(MMIAUDIO_JAVA);
    MMIAPISET_ClearCurRingType(MMISET_RING_TYPE_JAVA);
    
    s_audio_state = MMIJAVA_AUDIO_STOPPED;
    
    javacall_printf(LOG_STR_LEAVE("audio_stop result OK"));
    return JAVACALL_OK;
}

/*****************************************************************************/
//  Description : pause playing audio
//  Global resource dependence : none
//  Author: apple.zhang
//  Note:  return value: 0->successs, 1->failed//        
/*****************************************************************************/
LOCAL javacall_result audio_pause(void)
{
    javacall_result result = JAVACALL_FAIL;
    javacall_printf(LOG_STR("audio_pause enter"));

    if(TRUE == MMIAUDIO_AudioPause(MMIAUDIO_JAVA))
    {
        s_audio_state = MMIJAVA_AUDIO_PAUSED;
        result = JAVACALL_OK;
    }

    javacall_printf(LOG_STR_LEAVE("result = %d"),result);
    return result;
}

/*****************************************************************************/
//  Description : resume playing audio
//  Global resource dependence : none
//  Author: apple.zhang
//  Note:  return value: 0->successs, 1->failed//        
/*****************************************************************************/
LOCAL javacall_result audio_resume(void)
{
    javacall_result result = JAVACALL_FAIL;
    javacall_printf(LOG_STR("audio_resume enter"));
    if(TRUE == MMIAUDIO_AudioResume(MMIAUDIO_JAVA))
    {
        s_audio_state = MMIJAVA_AUDIO_PLAYING;
        result = JAVACALL_OK;
    }

    javacall_printf(LOG_STR("result = %d"),result);
    return result;
}

/*****************************************************************************/
//  Description : get current time for the playing audio
//  Global resource dependence : none
//  Author: apple.zhang
//  Note:  return current time for the playing audio
/*****************************************************************************/
LOCAL int32 audio_get_time(void)
{
    AUDIO_PLAYINFO_T    music_play_info = {0};
    int32               time            = -1;//UNKNOWN
  
    if (TRUE == MMIAUDIO_AudioGetPlayingFileInfo(MMIAUDIO_JAVA,&music_play_info)) {
        time = music_play_info.uiCurrentTime * 1000;
        //playing seekable media, so do not return 0.
        if (time == 0)
            time = 1;
    }
    javacall_printf(LOG_STR(" time = %d ms"), time);

    return time;
    
}

/*****************************************************************************/
//  Description : set current time for the playing audio
//  Global resource dependence : none
//  Author: apple.zhang
//  Note:  return current time for the playing audio   
/*****************************************************************************/
LOCAL int32 audio_set_time(int32 ms)
{ 
    if (FALSE == MMIAUDIO_AudioSeekByTime(MMIAUDIO_JAVA, ms / 1000))
    {
        javacall_printf(LOG_STR("fail"));
        return -1;
    }
    
    javacall_printf(LOG_STR("audio_set_time offset = %d"),ms);

    return ms;
}

/*****************************************************************************/
//  Description : get total time for the playing audio
//  Global resource dependence : none
//  Author: apple.zhang
//  Note:  return total time for the playing audio   
/*****************************************************************************/
LOCAL int32 audio_get_duration(wchar *filename,MMIJAVA_MEDIA_TYPE_T mediatype)
{
    AUDIO_FORMATINFO_T      format_info = {0};
    AUDIO_PLAYINFO_T        play_info = {0};
    int32                   time            = 0;  
    MMIAUDIO_TYPE_E         music_type      = MMIAUDIO_TYPE_MAX;
    
    if (MMIJAVA_AUDIO_STOPPED == s_audio_state)
    {
        music_type = switch_javatype_to_audiotype(mediatype);

        javacall_printf(LOG_STR("audio_get_duration mediatype = %d, music_type = %d"), mediatype, music_type);

        if (MMIAUDIO_TYPE_MAX == music_type)
        {
            return 0;
        }

        if (MMIAUDIO_CreateAudioFileHandle(FALSE, MMIAUDIO_JAVA, music_type,(const wchar *)filename))
        {    
            if(MMIAUDIO_AudioGetFileInfo(MMIAUDIO_JAVA,&format_info))
            {
                time = format_info.uiTotalTime * 1000;
            }
            else
            {
                time = -1;
            }
            MMIAUDIO_CloseAudioHandle(MMIAUDIO_JAVA);
        }
    }
    else
    {
        if(MMIAUDIO_AudioGetPlayingFileInfo(MMIAUDIO_JAVA,&play_info))
        {
            time = play_info.uiTotalTime * 1000;
        }
        else
        {
            time = -1;
        }
    }

    javacall_printf(LOG_STR_LEAVE("audio_get_duration time = %d ms" ), time);

    return time;    
}
/*****************************************************************************/
// Description : Get metadata info
// Global resource dependence : none
//  Author: 
//  Note: 
 /*****************************************************************************/
LOCAL uint32 get_metadata_info(void)
{
    uint32                    counts        = 0;
    AUDIO_FORMATINFO_T        ptfileInfo    = {0};
    //MMI_STRING_T              key_str       = {0};
    uint32                    len           = 0;
    //int32                    text_num      = 0;
    //fucntion prototyped in app module not in sun java module
    //MMI_TEXT_ID_T             *text_array = MMIJAVA_AudioGetMetadataText(&text_num); 
    static char * album_key = "album";
    static char * artist_key = "artist";
    static char * title_key = "title";
    static char * year_key = "year";

    //read text resouce failed
//    if(text_num == 0)
//    {
//        return 0;
//    }
    
    if(MMIAUDIO_AudioGetFileInfo(MMIAUDIO_JAVA,&ptfileInfo))
    {
        
        if (0 != strlen(ptfileInfo.tAudioId3Tag.album))
        {
            //MMI_GetLabelTextByLang(TXT_JAVA_SONG_ALBUM,&key_str); 
            //MMI_GetLabelTextByLang(text_array[0],&key_str); 
            SCI_MEMSET(s_metadata_info[counts].key,0,MMIJAVA_METADATA_LENGTH);
            //MMI_WSTRNTOSTR((uint8*)(s_metadata_info[counts].key),MMIJAVA_METADATA_LENGTH,key_str.wstr_ptr,key_str.wstr_len,MIN(MMIJAVA_METADATA_LENGTH,key_str.wstr_len));               
            len = strlen(album_key) ;
            MMI_MEMCPY(s_metadata_info[counts].key, MMIJAVA_METADATA_LENGTH, album_key, len, MIN(MMIJAVA_METADATA_LENGTH,len));
           len = strlen(ptfileInfo.tAudioId3Tag.album) ;
            SCI_MEMSET(s_metadata_info[counts].data,0,MMIJAVA_METADATA_LENGTH);
            MMI_MEMCPY(s_metadata_info[counts].data,MMIJAVA_METADATA_LENGTH,ptfileInfo.tAudioId3Tag.album,  len, MIN(MMIJAVA_METADATA_LENGTH,len));
            javacall_printf(LOG_STR("get %dth meta %s = %s"), counts, album_key, ptfileInfo.tAudioId3Tag.album);
            counts ++;
        }
        if (0 != strlen(ptfileInfo.tAudioId3Tag.artist))
        {
            //MMI_GetLabelTextByLang(TXT_JAVA_SONG_ARTIST,&key_str);             
            //MMI_GetLabelTextByLang(text_array[1],&key_str);             
            SCI_MEMSET(s_metadata_info[counts].key,0,MMIJAVA_METADATA_LENGTH);
            //MMI_WSTRNTOSTR((uint8*)(s_metadata_info[counts].key),MMIJAVA_METADATA_LENGTH,key_str.wstr_ptr,key_str.wstr_len,MIN(MMIJAVA_METADATA_LENGTH,key_str.wstr_len));
            len = strlen(artist_key) ;
            MMI_MEMCPY(s_metadata_info[counts].key, MMIJAVA_METADATA_LENGTH, artist_key, len, MIN(MMIJAVA_METADATA_LENGTH,len));
            len = strlen(ptfileInfo.tAudioId3Tag.artist);
            SCI_MEMSET(s_metadata_info[counts].data,0,MMIJAVA_METADATA_LENGTH);
            MMI_MEMCPY(s_metadata_info[counts].data,MMIJAVA_METADATA_LENGTH,ptfileInfo.tAudioId3Tag.artist,  len, MIN(MMIJAVA_METADATA_LENGTH, len));
            javacall_printf(LOG_STR("get %dth meta %s = %s"), counts, s_metadata_info[counts].key, s_metadata_info[counts].data);
            counts ++;
        }            
        if (0 != strlen(ptfileInfo.tAudioId3Tag.title))
        {
            //MMI_GetLabelTextByLang(TXT_JAVA_SONG_TITLE,&key_str);      
            //MMI_GetLabelTextByLang(text_array[2],&key_str);               
            SCI_MEMSET(s_metadata_info[counts].key,0,MMIJAVA_METADATA_LENGTH);
            //MMI_WSTRNTOSTR((uint8*)(s_metadata_info[counts].key),MMIJAVA_METADATA_LENGTH,key_str.wstr_ptr,key_str.wstr_len,MIN(MMIJAVA_METADATA_LENGTH,key_str.wstr_len));
            len = strlen(title_key) ;
            MMI_MEMCPY(s_metadata_info[counts].key, MMIJAVA_METADATA_LENGTH, title_key, len, MIN(MMIJAVA_METADATA_LENGTH,len));
            len = strlen(ptfileInfo.tAudioId3Tag.title);
            SCI_MEMSET(s_metadata_info[counts].data,0,MMIJAVA_METADATA_LENGTH);
            MMI_MEMCPY(s_metadata_info[counts].data,MMIJAVA_METADATA_LENGTH,ptfileInfo.tAudioId3Tag.title, len, MIN(MMIJAVA_METADATA_LENGTH, len));
            javacall_printf(LOG_STR("get %dth meta %s = %s"), counts, s_metadata_info[counts].key, s_metadata_info[counts].data);
            counts ++;
        }            
        if (0 != strlen(ptfileInfo.tAudioId3Tag.year))
        {
            //MMI_GetLabelTextByLang(TXT_JAVA_SONG_YEAR,&key_str);                
            //MMI_GetLabelTextByLang(text_array[3],&key_str);                
            SCI_MEMSET(s_metadata_info[counts].key,0,MMIJAVA_METADATA_LENGTH);
            //MMI_WSTRNTOSTR((uint8*)(s_metadata_info[counts].key),MMIJAVA_METADATA_LENGTH,key_str.wstr_ptr,key_str.wstr_len,MIN(MMIJAVA_METADATA_LENGTH,key_str.wstr_len));
            len = strlen(year_key) ;
            MMI_MEMCPY(s_metadata_info[counts].key, MMIJAVA_METADATA_LENGTH, year_key, len, MIN(MMIJAVA_METADATA_LENGTH,len));
            len = strlen(ptfileInfo.tAudioId3Tag.year);
            SCI_MEMSET(s_metadata_info[counts].data,0,MMIJAVA_METADATA_LENGTH);
            MMI_MEMCPY(s_metadata_info[counts].data,MMIJAVA_METADATA_LENGTH,ptfileInfo.tAudioId3Tag.year, len , MIN(4, len));
            javacall_printf(LOG_STR("get %dth meta %s = %s"), counts, s_metadata_info[counts].key, s_metadata_info[counts].data);
            counts ++;
        }
       
    }

    return counts;
}
/*****************************************************************************/
// Description : Get supported meta data key counts for specefic media type
// Global resource dependence : none
//  Author: 
//  Note: 
 /*****************************************************************************/
LOCAL BOOLEAN get_metadata_key_counts(wchar *filename,MMIJAVA_MEDIA_TYPE_T mediatype,/*OUT*/ long* keyCounts) 
{       
    BOOLEAN                   result        = FALSE;
    MMIAUDIO_TYPE_E         music_type      = MMIAUDIO_TYPE_MAX;

    if (MMIJAVA_AUDIO_STOPPED == s_audio_state)
    {
        music_type = switch_javatype_to_audiotype(mediatype);

        javacall_printf(LOG_STR("get_metadata_key_counts mediatype = %d, music_type = %d"), mediatype, music_type);

        if (MMIAUDIO_TYPE_MAX == music_type)
        {
            return FALSE;
        }

        if (MMIAUDIO_CreateAudioFileHandle(FALSE, MMIAUDIO_JAVA, music_type,(const wchar *)filename))
        {    
            *keyCounts = get_metadata_info();
            result = TRUE;                       
            MMIAUDIO_CloseAudioHandle(MMIAUDIO_JAVA);
        }
    }
    else
    {
        *keyCounts = get_metadata_info();     
        result = TRUE;
        
    }    
    return result;
   
}


/*****************************************************************************/
// Description : Get meta data key strings by using media type and index value
// Global resource dependence : none
//  Author: 
//  Note: 
 /*****************************************************************************/
LOCAL BOOLEAN get_metadata_key(                                
                                long index, 
                                long bufLength, 
                                char* keyBuf
                                ) 
{

    javacall_printf(LOG_STR("get_metadata_key index = %dm bufLength = %d"), index, bufLength);

    if ((0 > index) || (MMIJAVA_METADATA_COUNTS <= index)||(PNULL == keyBuf))
    {
        return FALSE;
    }
    
    if (bufLength >= (long)strlen(s_metadata_info[index].key))
    {
        SCI_STRCPY(keyBuf,s_metadata_info[index].key);
        return TRUE;
    }    
    return FALSE;
}

/*****************************************************************************/
// Description : Get meta data value strings by using meta data key string
// Global resource dependence : none
//  Author: 
//  Note:  
/*****************************************************************************/
LOCAL BOOLEAN get_metadata(                           
                            const char* key, 
                            long bufLength, 
                            /*OUT*/ char* dataBuf
                            ) 
{
    int32  i = 0;
    if ((PNULL == key) || (PNULL == dataBuf))
    {
        return FALSE;
    }

    for (i = 0; i < MMIJAVA_METADATA_COUNTS; i++)
    {
        if (0 == MMIAPICOM_Stricmp(key,s_metadata_info[i].key))/*lint !e64*/
        {
            if (bufLength >= SCI_STRLEN(s_metadata_info[i].data))/*lint !e737 !e574*/
            {
                SCI_STRCPY(dataBuf,s_metadata_info[i].data);
                return TRUE;
            }
            else
            {
                return FALSE;
            }
            
        }
    }
    return FALSE;
    
}

#endif

