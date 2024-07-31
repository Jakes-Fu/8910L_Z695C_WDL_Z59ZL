/*
 * Copyright 2003 Aplix Corporation. All Rights Reserved.
 *
 * JBlend and all Aplix- or JBlend-related trademarks and logos are
 * trademarks or registered trademarks of Aplix in the U.S., Japan and
 * other countries.
 */
/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmi_app_ia_java_trc.h"
#include "std_header.h"
#ifdef JAVA_SUPPORT_IA
#include "jblend_config.h"

#include "audioclient.h"
#include "jwe_log.h"
#include "jkmediacontrol.h"
#include "ammediacontrol.h"
#include "ajutil.h"
#include "mmi_default.h"
//#include "mmiset.h"
#include "mmiset_export.h"
//#include "kjava_action.h"
#include "jblend_platform.h"
#include "mediacontrol.h"
#include "media_tonecontrol.h"
#include "mmijava_internal.h"
#include "mmi_appmsg.h"
#include "audio_api.h"
#include "midi_adp.h"
#include "mmisrvmgr.h"
#include "mmisrvaud_api.h"

/**-------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                *
 **--------------------------------------------------------------------------*/

#define SET_ACTION_PARAM(offset, item_p) \
SCI_MEMCPY(kjAudioCmd_p->params + (offset * 4), item_p, 4);

/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------*
 **                         EXTERNAL DECLARE                                 *
 **--------------------------------------------------------------------------*/
extern T_mediaPlayer *m_pLinkTop;
extern PUBLIC void toneCtrl_playNote_callback(MMISRVAUD_REPORT_RESULT_E result, DPARAM param);

/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/
#ifdef AUDIO_USEFILE
#define M_HIDDEN_FILE_MAX_COUNT 10
LOCAL int m_hidden_file_count=0;
#endif

LOCAL JKSint32 s_active_playerID = -1;//用来记录当前正在播放的player ID

#define GET_AUDIO_HANDLE(x) ((x)->m_hAudioHandle)
/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/


//make sure the file_ext array len is enough
LOCAL void get_hidden_file_ext(
char* file_ext,//out
int player_id,//in
int media_type)//in

{
    JWE_LOG(get_hidden_file_ext, ("enter player_id=%d media_type=%d",player_id,media_type));
    //SCI_ASSERT(PNULL != file_ext);
    if(PNULL == file_ext)
    {
        JWE_LOG(get_hidden_file_ext, ("file_ext is PNULL"));
        return;
    }
    
    switch(media_type){
		case JKMEDIA_TYPE_MIDI:
		case JKMEDIA_TYPE_SPMIDI:
			sprintf(file_ext, "%dtemp.midi",player_id); 
			break;
		case JKMEDIA_TYPE_TONE:
			sprintf(file_ext, "%dtemp.jts",player_id); 
			break;
		case JKMEDIA_TYPE_WAVE:
			sprintf(file_ext, "%dtemp.wav",player_id); 
			break;
		case JKMEDIA_TYPE_AMR:
			sprintf(file_ext, "%dtemp.amr",player_id); 
			break;
		case JKMEDIA_TYPE_MP3:
			sprintf(file_ext, "%dtemp.mp3",player_id); 
			break;
		case JKMEDIA_TYPE_AAC:
			sprintf(file_ext, "%dtemp.aac",player_id); 
			break;
		case JKMEDIA_TYPE_M4A:
			sprintf(file_ext, "%dtemp.m4a",player_id); 
			break;
		case JKMEDIA_TYPE_MPEG_AUDIO:
		case JKMEDIA_TYPE_MPEG :
			sprintf(file_ext, "%dtemp.mp4",player_id); 
			break;
		case JKMEDIA_TYPE_H263 :
		case JKMEDIA_TYPE_H263_AUDIO:
			sprintf(file_ext, "%dtemp.3gp",player_id); 
			break;
		case JKMEDIA_TYPE_GIF:
			sprintf(file_ext, "%dtemp.gif",player_id); 
			break;
		default:
			sprintf(file_ext, "%dtemp",player_id); 
			break;
    }
    JWE_LOG(get_hidden_file_ext, ("file_ext =%s",file_ext));
	return;
}

LOCAL JKSint32 create_media_hidden_file(int player_id,int media_type,uint8 *data,int data_size,char* hidden_file_char)		
{
    KJavaFileHandle 	media_FH = 0;
    KJavaStat 			jstat;
    int 				err = 0; 
    char   				file_ext[16] = {0}; 

    KJavaPathMappingEntry javafolderpathmap;

    JWE_LOG(create_media_hidden_file, ("enter"));

    /*media hidden file count check*/
    if(m_hidden_file_count>M_HIDDEN_FILE_MAX_COUNT){
	 JWE_LOG(create_media_hidden_file, ("exceeds file limit. The limit value is M_HIDDEN_FILE_MAX_COUNT=%d",M_HIDDEN_FILE_MAX_COUNT));
        return -1;
    }
	
    /*media hidden file location*/
    SCI_MEMSET(hidden_file_char,0,sizeof(hidden_file_char));
    kjava_fs_getPath(0, &javafolderpathmap);

    strcat(hidden_file_char,javafolderpathmap.path);//E:/Java/
    
    strcat(hidden_file_char,"mhfdir\\");//E:/Java/mfdir/
    
	get_hidden_file_ext(file_ext,player_id,media_type);
		
    strcat(hidden_file_char,file_ext);

    JWE_LOG(create_media_hidden_file,("hidden_file_char completed path=%s",hidden_file_char));
		
    /*media hidden file create*/
    if( kjava_fs_stat2(hidden_file_char, &jstat) == 0 )
    {
        JWE_LOG(create_media_hidden_file, ("kjava_fs_stat2(hidden_file_char, &jstat) == 0"));
        if ( kjava_fs_remove(hidden_file_char) != 0 )
        {
            JWE_LOG(create_media_hidden_file, ("leave! kjava_fs_remove(hidden_file_char) != 0"));
            return -1;
        }
    }
    if((media_FH = kjava_fs_open(hidden_file_char,CONST_KJAVA_FILE_OPEN_CREATE|CONST_KJAVA_FILE_OPEN_WRITE,0 ,&err)) != NULL)
    {
		JWE_LOG(create_media_hidden_file, ("open file success."));
		if(kjava_fs_close(media_FH, 0) != 0)
		{
			JWE_LOG(create_media_hidden_file, ("leave close file fail."));
			return -1;
		}
    }
    else
    {
        JWE_LOG(create_media_hidden_file, ("leave open file fail. err=%d",err));
        return -1;
    }
	
    /*media hidden file write data to file*/
    if((media_FH = kjava_fs_open(hidden_file_char,CONST_KJAVA_FILE_OPEN_EXCL|CONST_KJAVA_FILE_OPEN_WRITE, 0 ,&err)) == NULL)
    {
        JWE_LOG(create_media_hidden_file, ("leave open file fail."));
        return -1;
    }
    if(kjava_fs_write(data, data_size, media_FH, 0) != KJAVA_FILE_ERR_NONE)
    {
        JWE_LOG(create_media_hidden_file, ("leave!write data error"));
        return -1;
    }
    if(kjava_fs_close(media_FH, 0) != 0)
    {
        JWE_LOG(create_media_hidden_file, ("leave close file fail."));
        return -1;
    }
	
    m_hidden_file_count++;
    JWE_LOG(create_media_hidden_file,("leave filename=%s,m_hidden_file_count=%d",hidden_file_char,m_hidden_file_count));
    return 0;
}


LOCAL JKSint32 destroy_media_hidden_file(const char *media_filename)
{
    //  KJavaFileHandle midi_FH;
    KJavaStat jstat;
    
    JWE_LOG(destroy_media_hidden_file, ("enter media_filename=%s",media_filename));
    
    //check file and remove
    if( kjava_fs_stat2(media_filename, &jstat) == 0 )
    {
        if ( kjava_fs_remove(media_filename) != 0 )
        {
            JWE_LOG(destroy_media_hidden_file, ("leave remove file fail"));
            return -1;
        }
    }
    m_hidden_file_count--;
    JWE_LOG(destroy_media_hidden_file, ("leave!ok"));
    return 0;
}


LOCAL void clean_media_hidden_dir()
{
    KJavaPathMappingEntry   javafolderpathmap = {0};
    int                     javafolderpath_len = 0;
    wchar                   wjavafolder_dir[64] = {0};
    int32                   wjavafolder_dir_length = 0;  
	
    wchar                   whidden_dir[] = L"mhfdir";
    int                     deltreeerr = 0;
	
    JWE_LOG(clean_media_hidden_dir, ("enter"));

    kjava_fs_getPath(0, &javafolderpathmap);

    JWE_LOG(clean_media_hidden_dir, ("javafolderpathmap.path=%s",javafolderpathmap.path));

    javafolderpath_len = strlen(javafolderpathmap.path);
    if(javafolderpathmap.path[javafolderpath_len-1]=='\\')
	javafolderpathmap.path[javafolderpath_len-1] = 0x00;
	
    wjavafolder_dir_length = GUI_UTF8ToWstr(wjavafolder_dir,64, (uint8 *)(javafolderpathmap.path),strlen(javafolderpathmap.path));

    deltreeerr = MMIAPIFMM_DeleteTree(wjavafolder_dir,whidden_dir);

    if(deltreeerr!=SFS_ERROR_NONE)
    {
	    JWE_LOG(clean_media_hidden_dir, ("fail! deltreeerr=%d",deltreeerr));
    }

    m_hidden_file_count = 0;
    JWE_LOG(clean_media_hidden_dir, ("leave"));
    return;
}

LOCAL T_mediaPlayer* getPlayerFromAudioHandle( MMISRV_HANDLE_T handle )
{
    T_mediaPlayer *p = PNULL;

    JWE_LOG(getPlayerFromAudioHandle, ("enter MMISRV_HANDLE_T handle = %d", handle));
    for ( p = m_pLinkTop; p != JK_NULL; p = p->pNext )
    {
        JWE_LOG(getPlayerFromAudioHandle, ("p->ac->m_hAudioHandle = %d", p->ac->m_hAudioHandle));
        if ( p->ac->m_hAudioHandle==handle) 
        {
            JWE_LOG(getPlayerFromAudioHandle, ("leave! p->ac->m_hAudioHandle=%d,p->id=%d",  p->ac->m_hAudioHandle,p->id));
            return p;
        }
    }
    JWE_LOG(getPlayerFromAudioHandle, ("leave. can not get this player."));

    return JK_NULL;
}


LOCAL void AudioClient_player_end_callback(MMISRVAUD_REPORT_RESULT_E result, DPARAM param)
{
    T_mediaPlayer *pPlayer = PNULL;
    StreamPlayer* pSPlayer  = PNULL;//stream player
    AudioClient *ac = PNULL ;
    uint64 usec = 0;
	
    JWE_LOG(AudioClient_player_end_callback,("enter result = %d",result));
	
    pPlayer = getPlayer( s_active_playerID );

    
    if(pPlayer==PNULL){
        JWE_LOG(AudioClient_player_end_callback,("NULL Player"));
        return;
    }

    ac = pPlayer->ac;
    //增加 stream player的判断,因为pSPlayer->wchar_url是最终最长file playe文件名的地方
	if(ac->isFileStream)
    {
        pSPlayer = getStreamPlayer( pPlayer->stream_id);
        if(pSPlayer == PNULL)
        {
            JWE_LOG(AudioClient_player_end_callback,("NULL pSPlayer"));
            return;
        }
    }

    //s_java_end_callback_func = PNULL;
    /*有的格式必须等到播放完之后才能知道总的持续时间，但展讯上当音乐播放结束后就得不到当前time了*/
    if(!AudioClient_player_get_duration(ac,&usec))
    {
        JWE_LOG(AudioClient_player_end_callback,("AudioClient_player_get_duration fail!"))
    }
    
    ac->m_hAudioHandle = 0;

    if(ac->is_infiniteTimes== TRUE)
    {
        JWE_LOG(AudioClient_player_end_callback,("ac->is_infiniteTimes  == TRUE"))
        ac->isRepeating = 1;
        mediaCtrl_callback(MEDIA_CLIENT_EVENT_END,pPlayer->id,(JKUint32)(usec>>32),(JKUint32)((usec<<32)>>32), 1);
    }
    else
    {
        ac->need_PlayTimes = ac->need_PlayTimes - 1;
        if(ac->need_PlayTimes  == 0)
        {
            JWE_LOG(AudioClient_player_end_callback,("ac->need_PlayTimes  == 0"))
            ac->isEndOfMedia = TRUE;
            mediaCtrl_callback(MEDIA_CLIENT_EVENT_END,pPlayer->id,(JKUint32)(usec>>32),(JKUint32)((usec<<32)>>32), 0);
            s_active_playerID =  -1;

        }
        else
        {
            JWE_LOG(AudioClient_player_end_callback,("ac->need_PlayTimes  != 0"))
            ac->isRepeating = 1;
            mediaCtrl_callback(MEDIA_CLIENT_EVENT_END,pPlayer->id,(JKUint32)(usec>>32),(JKUint32)((usec<<32)>>32), 1);
        }
    }
    
    JWE_LOG(AudioClient_player_end_callback,("leave"))
}

LOCAL BOOLEAN AudioClient_player_notify(MMISRV_HANDLE_T handle, MMISRVMGR_NOTIFY_PARAM_T *param)
{
    MMISRVAUD_REPORT_T *report_ptr = PNULL;
    MMISRVAUD_REPORT_RESULT_E result = 0;
    //SCI_TRACE_LOW:"AudioClient_player_notify() entry, handle=0x%x, param=0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,AUDIOCLIENT_342_112_2_18_2_21_58_0,(uint8*)"dd", handle, param);
    
    if(param != PNULL && handle > 0 && param->data != 0)
    {
        report_ptr = (MMISRVAUD_REPORT_T *)param->data;
        result = (MMISRVAUD_REPORT_RESULT_E)report_ptr->data1;
        if(report_ptr != PNULL)
        {            
            //SCI_TRACE_LOW:"AudioClient_player_notify(), report_ptr->report=%d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,AUDIOCLIENT_350_112_2_18_2_21_58_1,(uint8*)"d", report_ptr->report);
        
            switch(report_ptr->report)
            {
            case MMISRVAUD_REPORT_END:
                AudioClient_player_end_callback(result, 0);
                MMISRVMGR_Free(handle);
                break;
            default:
                break;
            }
        }
    }
    return TRUE;
}

LOCAL BOOLEAN AudioClient_tone_notify(MMISRV_HANDLE_T handle, MMISRVMGR_NOTIFY_PARAM_T *param)
{
    MMISRVAUD_REPORT_T *report_ptr = PNULL;
    MMISRVAUD_REPORT_RESULT_E result = 0;
    //SCI_TRACE_LOW:"AudioClient_player_notify() entry, handle=0x%x, param=0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,AUDIOCLIENT_370_112_2_18_2_21_58_2,(uint8*)"dd", handle, param);
    
    if(param != PNULL && handle > 0)
    {
        report_ptr = (MMISRVAUD_REPORT_T *)param->data;
        result = (MMISRVAUD_REPORT_RESULT_E)report_ptr->data1;
        if(report_ptr != PNULL)
        {            
            //SCI_TRACE_LOW:"AudioClient_tone_notify(), report_ptr->report=%d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,AUDIOCLIENT_378_112_2_18_2_21_58_3,(uint8*)"d", report_ptr->report);
        
            switch(report_ptr->report)
            {
            case MMISRVAUD_REPORT_END:
                toneCtrl_playNote_callback(result, 0);
                MMISRVMGR_Free(handle);                
                break;
            default:
                break;
            }
        }
    }
    return TRUE;
}

    
/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

PUBLIC BOOLEAN AudioClient_player_open(int player_id, int media_type, uint8*data,int length,wchar* filename,AudioClient* ac)
{
	JWE_LOG(AudioClient_player_open,("enter player_id=%d media_type=%d data=0x%x length=%d filename=%x ac=0x%x",player_id,media_type,data,length,filename,ac))
	
	if( (data==PNULL) && (length==0) && (filename!=PNULL) )
	{
		JWE_LOG(AudioClient_player_open,("file stream mode."));
		
		switch(media_type) 
		{
		case JKMEDIA_TYPE_TONE:     
		    ac->mmi_AudioFmt = MMISRVAUD_RING_FMT_GENERIC_TONE;//could not in.because toneControl is not use filestream
			break;
		case JKMEDIA_TYPE_MIDI:     
		case JKMEDIA_TYPE_SPMIDI:     
		    ac->mmi_AudioFmt = MMISRVAUD_RING_FMT_MIDI;
			break;
		case JKMEDIA_TYPE_WAVE:       
		    ac->mmi_AudioFmt = MMISRVAUD_RING_FMT_WAVE;
		    break;
		case JKMEDIA_TYPE_MP3:       
		    ac->mmi_AudioFmt = MMISRVAUD_RING_FMT_MP3;
		    break;
		case JKMEDIA_TYPE_AMR:      
		    ac->mmi_AudioFmt = MMISRVAUD_RING_FMT_AMR;
		    break;    
		case JKMEDIA_TYPE_AAC:     
		    ac->mmi_AudioFmt = MMISRVAUD_RING_FMT_AAC;
			break;
		case JKMEDIA_TYPE_M4A:     
		    ac->mmi_AudioFmt = MMISRVAUD_RING_FMT_M4A;
			break;
			
		default:     
		    ac->mmi_AudioFmt  = MMISRVAUD_RING_FMT_MIDI;
			break;
		}  
		
		ac->isFileStream = TRUE;
		ac->wFileStreamUrl = filename;//filename is pStreamPlayer->wchar_url
		
	}
	else
	{
		JWE_LOG(AudioClient_player_open,("storge mode."));
	    switch(media_type) 
	    {
	    case JKMEDIA_TYPE_TONE:     
	        ac->mmi_AudioFmt = MMISRVAUD_RING_FMT_MIDI;//tone file is converted to mid file
	    	break;
	    case JKMEDIA_TYPE_MIDI:     
	    case JKMEDIA_TYPE_SPMIDI:     
	        ac->mmi_AudioFmt = MMISRVAUD_RING_FMT_MIDI;
	    	break;
	    case JKMEDIA_TYPE_WAVE:       
	        ac->mmi_AudioFmt = MMISRVAUD_RING_FMT_WAVE;
	        break;
	    case JKMEDIA_TYPE_MP3:       
	        ac->mmi_AudioFmt = MMISRVAUD_RING_FMT_MP3;
	        break;
	    case JKMEDIA_TYPE_AMR:      
	        ac->mmi_AudioFmt = MMISRVAUD_RING_FMT_AMR;
	        break;    
	    case JKMEDIA_TYPE_AAC:     
	        ac->mmi_AudioFmt = MMISRVAUD_RING_FMT_AAC;
	    	break;
	    case JKMEDIA_TYPE_M4A:     
	        ac->mmi_AudioFmt = MMISRVAUD_RING_FMT_M4A;
	    	break;
	/*
	    case JKMEDIA_TYPE_MPEG_AUDIO:       
	        ac->mmi_AudioFmt = MMISRVAUD_RING_FMT_MP3;
	        break;
	    case JKMEDIA_TYPE_H263_AUDIO:       
	        ac->mmi_AudioFmt = MMISRVAUD_RING_FMT_WAVE;
	        break;		
	*/
	    default:     
	        ac->mmi_AudioFmt  = MMISRVAUD_RING_FMT_MIDI;
	    	break;
	    }  

        #ifdef AUDIO_USEFILE	
        JWE_LOG(AudioClient_player_open,("write to local hidden file(filename depend player_id and media_type)"))
        if(create_media_hidden_file(player_id,media_type,(JKUint8 *)data,length,ac->hidden_file_char)== -1)
        {
            JWE_LOG(AudioClient_player_open,("create file failed!!!"))
            destroy_media_hidden_file(ac->hidden_file_char);
            return FALSE;
        }
	    ac->hidden_file=1;	
        #endif
	}
	ac->native_time = 0;
/*
Since Spreadtrum's MACRO JAVA_SUPPORT not support muti-player,so I do not implement handle
ac->m_hAudioHandle
ac->mmi_AudioHandleType
ac->mmi_AudioFmt
*/
	return TRUE;
}

PUBLIC BOOLEAN AudioClient_player_open_tone_sequence(int player_id, int media_type, uint8*data,int length,ToneParser* tp,AudioClient* ac)
{
	JWE_LOG(AudioClient_player_open_tone_sequence,("enter player_id=%d media_type=%d data=0x%x length=%d tp=0x%x ac=0x%x",player_id,media_type,data,length,tp,ac))
	    
	if ( toneToMidi(data,length,tp) == JKMEDIA_RESULT_ERROR ) 
	{
	    return FALSE;
	}
	
#ifdef AUDIO_USEFILE	
    ac->mmi_AudioFmt = MMISRVAUD_RING_FMT_MIDI;//tone file is converted to mid file
    JWE_LOG(AudioClient_player_open_tone_sequence,("media_type=%d tone -> mid file. ",media_type))
    if(create_media_hidden_file(player_id,JKMEDIA_TYPE_MIDI,(JKUint8 *)tp->u8Data,tp->s32DataLen,ac->hidden_file_char)== -1)
    {
        JWE_LOG(mediaCtrl_set,("create file failed!!!"))
        destroy_media_hidden_file(ac->hidden_file_char);
        return FALSE;
    }
	ac->hidden_file=1;
	//if(AudioClient_player_open(pPlayer->media_type,NULL,0,(char*)m_hidden_file_char,pPlayer->ac)==FALSE){
	 //   JWE_LOG(mediaCtrl_set,("AudioClient_player_open fail"))      
	//    destroy_media_hidden_file(m_hidden_file_char);
	//    return JKMEDIA_RESULT_ERROR;
	//}
	
#endif

	return TRUE;
}

PUBLIC BOOLEAN AudioClient_player_close(AudioClient* ac)
{
    JWE_LOG(AudioClient_player_close,("enter ac->hidden_file=%d",ac->hidden_file));
#ifdef MEDIA_USE_ONE_AUDIO_HANDLE
    AudioClient_player_stop(ac);
#endif

#ifdef AUDIO_USEFILE	
    //Delete hidden files!
    if(1 == ac->hidden_file)
    {
        ac->hidden_file=0;
        if ( 0 != kjava_fs_remove(ac->hidden_file_char) )
        {
            JWE_LOG(create_media_hidden_file, ("leave! kjava_fs_remove(hidden_file_char) != 0"));
            return FALSE;
        }

        m_hidden_file_count--;
        JWE_LOG(AudioClient_player_close,("m_hidden_file_count=%d",m_hidden_file_count))
    }
#endif
    return TRUE;

}

PUBLIC BOOLEAN AudioClient_player_play(AudioClient *ac,int loop,JKSint32 id,int volume)
{
    wchar wfilename[MMIFILE_FILE_NAME_MAX_LEN+1] = {0};
    uint32 uiOffset = 0;
    //AUDIO_FORMATINFO_T audio_info = {0};
    const int32 midi_samplerate = 16000;
    const int32 midi_polynum = 16;
    MMISRV_HANDLE_T handle = 0;
    MMISRVMGR_SERVICE_REQ_T req = {0};
    MMISRVAUD_TYPE_T audio_srv = {0};
    
    JWE_LOG(AudioClient_player_play,("enter ac->pData=0x%x ac->length=%d ac->hidden_file_char=%s ac->hidden_file=%d ac->mmi_AudioFmt=%d",ac->pData,ac->length,ac->hidden_file_char,ac->hidden_file,ac->mmi_AudioFmt));
    
#ifdef MEDIA_USE_ONE_AUDIO_HANDLE
    if(0 != GET_AUDIO_HANDLE(ac))
    {
		JWE_LOG(AudioClient_player_play,("handle!=PNULL =%d. It must be existed another media player is playing.", GET_AUDIO_HANDLE(ac)));
		return FALSE;
    }
#endif

    req.notify = AudioClient_player_notify;
    req.pri = MMISRVAUD_PRI_NORMAL;
    
    audio_srv.duation = 0;
    audio_srv.eq_mode = 0;
    audio_srv.is_mixing_enable = FALSE;
    audio_srv.play_times = loop;
    audio_srv.all_support_route = MMISRVAUD_ROUTE_SPEAKER | MMISRVAUD_ROUTE_EARPHONE;
    audio_srv.volume = MMIJAVA_CalculateActualVolume(MMIJAVA_GetPlatformVolume(),volume);

    if((ac->isFileStream) && (PNULL != ac->wFileStreamUrl))
    {
            audio_srv.info.ring_file.type = MMISRVAUD_TYPE_RING_FILE;
            audio_srv.info.ring_file.name = ac->wFileStreamUrl;
            audio_srv.info.ring_file.name_len = MMIAPICOM_Wstrlen(ac->wFileStreamUrl);
    
            JWE_LOG(AudioClient_player_play,("ac->isFileStream == TRUE"));
            handle = MMISRVMGR_Request(STR_SRV_AUD_NAME, &req, &audio_srv);
            if(handle > 0)
            {
                ac->m_hAudioHandle = handle;
                ac->need_PlayTimes = loop;

                JWE_LOG(AudioClient_player_play,("CreateAudioFileHandle ac->m_hAudioHandle=%d",ac->m_hAudioHandle));
                if (!MMISRVAUD_Play(GET_AUDIO_HANDLE(ac), uiOffset))
                {
                    JWE_LOG(AudioClient_player_play,("MMISRVAUD_Play false"));
                    AudioClient_player_stop(ac);
                    return FALSE;
                }
                s_active_playerID = id;
                JWE_LOG(AudioClient_player_play,("MMISRVAUD_Play true"));
                return TRUE;
            }
            else
            {
                JWE_LOG(AudioClient_player_play,("REQ audio handle fail"));
                return FALSE;
            }

	}
	else
	{
            JWE_LOG(AudioClient_player_play,("ac->isFileStream == FALSE"));
            if(ac->hidden_file == 1)
            {
                JWE_LOG(AudioClient_player_play,("ac->hidden_file == 1"));
                MMIAPICOM_StrToWstr((uint8*)ac->hidden_file_char,wfilename);

                audio_srv.info.ring_file.type = MMISRVAUD_TYPE_RING_FILE;
                audio_srv.info.ring_file.name = wfilename;
                audio_srv.info.ring_file.name_len = MMIAPICOM_Wstrlen(wfilename);
        
                handle = MMISRVMGR_Request(STR_SRV_AUD_NAME, &req, &audio_srv);

            	    if(handle > 0)
            	    {            	    
                        ac->m_hAudioHandle = handle;
                                                
                        JWE_LOG(AudioClient_player_play,("CreateAudioFileHandle ac->m_hAudioHandle=%d",ac->m_hAudioHandle));

                        if (!MMISRVAUD_Play(GET_AUDIO_HANDLE(ac), uiOffset))
                        {
                            JWE_LOG(AudioClient_player_play,("MMISRVAUD_Play false"));
                            AudioClient_player_stop(ac);
                            return FALSE;
                        }

                        s_active_playerID = id;
                        JWE_LOG(AudioClient_player_play,("MMISRVAUD_Play true"));
                        return TRUE;
                }
                else
                {
                    JWE_LOG(AudioClient_player_play,("REQ audio handle false!"));
                    return FALSE;
                }
            }
            else
            {
                	JWE_LOG(AudioClient_player_play,("ac->hidden_file != 1 (use buf)"));
                	if (PNULL != ac->pData)
                	{
                            audio_srv.info.ring_buf.type = MMISRVAUD_TYPE_RING_BUF;
                            audio_srv.info.ring_buf.data= ac->pData;
                            audio_srv.info.ring_buf.data_len = ac->length;
                	         audio_srv.info.ring_buf.fmt = MMISRVAUD_RING_FMT_MIDI;
                            if(ac->isplayTone)
                            {
                                req.notify = AudioClient_tone_notify; 
                            }
                            else
                            {
                                req.notify = AudioClient_player_notify; 
                            }
                            handle = MMISRVMGR_Request(STR_SRV_AUD_NAME, &req, &audio_srv);
                            
                	        if(handle > 0)
                	        {
                	             ac->m_hAudioHandle = handle;
                                if(ac->isplayTone)
                                {
                                    if (!MMISRVAUD_Play(GET_AUDIO_HANDLE(ac), uiOffset))
                                    {
                                        JWE_LOG(AudioClient_player_play, ("MMISRVAUD_Play fail close handle"));
                                        AudioClient_player_stop(ac);
                                        return  FALSE;		
                                    }
                                }
                                else
                                {   
                                    if (!MMISRVAUD_Play(GET_AUDIO_HANDLE(ac), uiOffset))
                                    {
                                        JWE_LOG(AudioClient_player_play, ("MMISRVAUD_Play fail close handle"));
                                        AudioClient_player_stop(ac);
                                        return  FALSE;		
                                    }
                                    
                                    JWE_LOG(AudioClient_player_play,("MMISRVAUD_Play true"));
                                    s_active_playerID = id;

                                    return TRUE;
                                }	
                	        }
                	        else
                	        {
                                JWE_LOG(AudioClient_player_play, ("REQ audio handle fail"));
                                return  FALSE;
                	        }

                	}
                	else
                	{
                	    JWE_LOG(AudioClient_player_play, ("ac->pData == PNULL"));
                	    return  FALSE;
                	}
                    return TRUE;
          }
     }
}

PUBLIC BOOLEAN AudioClient_player_replay(AudioClient *ac,int volume)
{	
    JWE_LOG(AudioClient_player_replay, ("enter volume =%d",volume));

    AudioClient_setActualVolume(MMIJAVA_CalculateActualVolume(MMIJAVA_GetPlatformVolume(),volume));

    if(!MMISRVAUD_Resume(GET_AUDIO_HANDLE(ac)))
    {   
	    return FALSE;
    }
    
    JWE_LOG(AudioClient_player_replay, ("leave ok"));
    return TRUE;
}


PUBLIC BOOLEAN AudioClient_player_pause(AudioClient* ac)
{
	  /* +TCK fix  mmapi issue. sync tck code.2138 2150 */ 
//    AUDIO_PLAYINFO_T PlayInfo = {0};
    MMISRVAUD_PLAY_INFO_T    PlayInfo = {0};
      
    /* -TCK fix  mmapi issue. sync tck code.2138 2150 */ 

    JWE_LOG(AudioClient_player_pause, ("enter!"));

    if(!MMISRVAUD_Pause(GET_AUDIO_HANDLE(ac))){
    	JWE_LOG(AudioClient_player_pause, ("MMISRVAUD_Pause fail"));
    	return FALSE;
    }
    /* +TCK fix  mmapi issue. sync tck code.2138 2150 */ 
//    if(!MMIAUDIO_AudioGetPlayingFileInfo(MMIAUDIO_JAVA,&PlayInfo))
    MMISRVAUD_GetPlayingInfo(GET_AUDIO_HANDLE(ac), &PlayInfo);

    {    
       JWE_LOG(AudioClient_player_pause, ("MMIAUDIO_AudioGetPlayingFileInfo fail!! when pause"));
	return FALSE;
    }

    JWE_LOG(AudioClient_player_pause, ("PlayInfo.uiCurrentTime =%d",PlayInfo.cur_time));
    ac->native_time=PlayInfo.cur_time;
    /* -TCK fix  mmapi issue. sync tck code.2138 2150 */ 
    return TRUE;
}

PUBLIC BOOLEAN AudioClient_player_stop(AudioClient* ac)
{
    JWE_LOG(AudioClient_player_stop, ("enter!"));

    MMISRVMGR_Free(GET_AUDIO_HANDLE(ac));
    JWE_LOG(AudioClient_player_stop, ("audio stop is ok and handle will be closed"));		
    ac->m_hAudioHandle= 0;
    ac->native_time= 0;
    ac->isEndOfMedia = TRUE;
    return TRUE;
}

PUBLIC BOOLEAN AudioClient_player_get_duration(AudioClient *ac,uint64* ptime)
{
    MMISRVAUD_PLAY_INFO_T    ptPlayInfo = {0};
    MMISRVAUD_CONTENT_INFO_T  ptFormatInfo = {0};
    wchar               wfilename[MMIFILE_FILE_NAME_MAX_LEN+1] = {0};
    
    JWE_LOG(AudioClient_player_get_duration, ("enter!"));
    
    if ((PNULL == ac)||(PNULL == ptime))
    {
    	JWE_LOG(AudioClient_player_get_duration, ("error input param"));
        return FALSE;
    }
    if( (ac->isFileStream!=TRUE) && (ac->hidden_file!=1) )
    {
    	JWE_LOG(AudioClient_player_get_duration, ("ac->isFileStream==FALSE && ac->hidden_file==0  This player maybe not media_set"));
    	return FALSE;

    }

	if((ac->isFileStream) && (PNULL !=ac->wFileStreamUrl))
	{
		JWE_LOG(AudioClient_player_get_duration, ("ac->isFileStream == TRUE"));
		if(0 != GET_AUDIO_HANDLE(ac))
		{
        	    MMISRVAUD_GetPlayingInfo(GET_AUDIO_HANDLE(ac), &ptPlayInfo);
                JWE_LOG(AudioClient_player_get_duration, ("GET_AUDIO_HANDLE(ac)!=PNULL=%d  ptPlayInfo.total_time(unit second)=%d",GET_AUDIO_HANDLE(ac),ptPlayInfo.total_time));
                *ptime =  (uint64)ptPlayInfo.total_time*1000*1000;//microsecond
                return TRUE;
		}

		JWE_LOG(AudioClient_player_get_duration, ("GET_AUDIO_HANDLE(ac)=%d",GET_AUDIO_HANDLE(ac)));

        	if (MMISRVAUD_GetFileContentInfo(wfilename, MMIAPICOM_Wstrlen(wfilename), &ptFormatInfo))
		{
		    JWE_LOG(AudioClient_player_get_duration, ("MMISRVAUD_GetFileContentInfo GET_AUDIO_HANDLE(ac)=%d ptFormatInfo.uiTotalTime(unit second)=%d",GET_AUDIO_HANDLE(ac),ptFormatInfo.total_time));

		    *ptime =  (uint64)ptFormatInfo.total_time*1000*1000;//microsecond
		    return TRUE;
		}
		else
		{   
		    JWE_LOG(AudioClient_player_get_duration, ("MMISRVAUD_GetFileContentInfo fail"));
		    return FALSE;
		}

	}
	else
	{
		JWE_LOG(AudioClient_player_get_duration, ("ac->isFileStream == FALSE"));
        	MMIAPICOM_StrToWstr((uint8*)ac->hidden_file_char,wfilename);	
        	if(0 != GET_AUDIO_HANDLE(ac))
        	{
        	    MMISRVAUD_GetPlayingInfo(GET_AUDIO_HANDLE(ac), &ptPlayInfo);
        	    JWE_LOG(AudioClient_player_get_duration, ("GET_AUDIO_HANDLE(ac)!=PNULL=%d MMISRVAUD_GetContentInfo ptPlayInfo.total_time(unit second)=%d",GET_AUDIO_HANDLE(ac),ptPlayInfo.total_time));
        	    *ptime =  (uint64)ptPlayInfo.total_time*1000*1000;//microsecond
        	    return TRUE;
        	}

        	//when repeat,or normal 
        	JWE_LOG(AudioClient_player_get_duration, ("GET_AUDIO_HANDLE(ac)==PNULL MMISRVAUD_GetContentInfo=%d",GET_AUDIO_HANDLE(ac)));
        	if (MMISRVAUD_GetFileContentInfo(wfilename, MMIAPICOM_Wstrlen(wfilename), &ptFormatInfo))
        	{
                JWE_LOG(AudioClient_player_get_duration, ("GET_AUDIO_HANDLE(ac)==PNULL MMISRVAUD_GetContentInfo=%d ptFormatInfo.total_time(unit second)=%d",GET_AUDIO_HANDLE(ac),ptFormatInfo.total_time));
                *ptime =  (uint64)ptFormatInfo.total_time*1000*1000;//microsecond
                return TRUE;
        	}
        	else
        	{   
        	    JWE_LOG(AudioClient_player_get_duration, ("Get file info fail"));
        	    return FALSE;
        	}
	}
}

PUBLIC BOOLEAN AudioClient_player_set_media_time(AudioClient *ac,uint64 usec)
{

	uint32 time=0; 
	time = (uint32)(usec/(1000*1000));//秒
	
	JWE_LOG(AudioClient_player_set_media_time, ("enter time=%d",time));
	
	if(GET_AUDIO_HANDLE(ac)==PNULL)
	{
	    JWE_LOG(AudioClient_player_set_media_time, ("GET_AUDIO_HANDLE(ac)==PNULL"));
	    return FALSE;
	}
	JWE_LOG(AudioClient_player_set_media_time, ("GET_AUDIO_HANDLE(ac)=%d",GET_AUDIO_HANDLE(ac)));

	if(!MMISRVAUD_SeekByTime(ac->m_hAudioHandle, time)){
		JWE_LOG(AudioClient_player_set_media_time, ("fail"));	
		return FALSE;
	}
	
	ac->native_time = (uint32)usec;
	
	return TRUE;
}

PUBLIC BOOLEAN AudioClient_player_get_media_time(AudioClient *ac,uint64* ptime)
{
	MMISRVAUD_PLAY_INFO_T PlayInfo = {0};    

	JWE_LOG(AudioClient_player_get_media_time, ("enter"));
	if(GET_AUDIO_HANDLE(ac) == 0)
	{
	    JWE_LOG(AudioClient_player_get_media_time, ("GET_AUDIO_HANDLE(ac)==PNULL"));

	    *ptime =  0;
	    ac->native_time=0;
	    return TRUE;
	}

	JWE_LOG(AudioClient_player_get_media_time, ("GET_AUDIO_HANDLE(ac)==%d",GET_AUDIO_HANDLE(ac)));
	if(MMISRVAUD_GetPlayingInfo(GET_AUDIO_HANDLE(ac), &PlayInfo))
	{    
	    JWE_LOG(AudioClient_player_get_media_time, ("ok PlayInfo.uiCurrentTime =%d",PlayInfo.cur_time));
	    ac->native_time=PlayInfo.cur_time;
	    *ptime =  (uint64)PlayInfo.cur_time*1000*1000;
	    return TRUE;
	}
	else
	{     
	    JWE_LOG(AudioClient_player_get_media_time, ("false"));
	    /*
	    ac->_usecH = 0;
	    ac->_usecL = SCI_GetTickCount() - ac->_currenttck + ac->_currentoffset;

	   if(ac->_currenttck == 0)
	   {  
	        ac->_usecL = 0;
	   }
	    *nCurPlayTime = ac->_usecL * 1000;
	    //SCI_TRACE_LOW:"java nCurPlayTime ac->_usecL %d"
	    SCI_TRACE_ID(TRACE_TOOL_CONVERT,AUDIOCLIENT_909_112_2_18_2_21_59_4,(uint8*)"d",*nCurPlayTime);
	 
	    */
	    return FALSE;
	}
}

PUBLIC BOOLEAN AudioClient_setActualVolume(int32 actual_volume)
{
    JKSint32 id = 0;
    T_mediaPlayer *player_ptr = PNULL;
        

    id = MMIJAVA_AudioClientGetActivePlayerID();	
    player_ptr = getPlayer(id);
    
    if(player_ptr != PNULL)
    {
        JWE_LOG(AudioClient_setActualVolume,("enter actual_volume=%d, handle=0x%x",actual_volume, GET_AUDIO_HANDLE(player_ptr->ac)));
        return MMISRVAUD_SetVolume(GET_AUDIO_HANDLE(player_ptr->ac), actual_volume);
    }
    else
    {
        JWE_LOG(AudioClient_setActualVolume,("enter actual_volume=%d, no active player"));
        return FALSE;
    }
}


PUBLIC BOOLEAN AudioClient_player_setVolume( AudioClient *ac, int32 volume ) 
{
    int32                           nativeVolume = 0;

    JWE_LOG(AudioClient_player_setVolume,("enter"));

    nativeVolume= MMIJAVA_CalculateActualVolume(MMIJAVA_GetPlatformVolume(), volume);

    return MMISRVAUD_SetVolume(GET_AUDIO_HANDLE(ac), nativeVolume);
}


PUBLIC void AudioClient_player_initialize()
{
    return;
}
PUBLIC void AudioClient_player_finalize()
{
    JWE_LOG(AudioClient_player_finalize,("enter"))
    clean_media_hidden_dir();
    //MMIAPISET_StopAllVibrator(); 
    //MMIAPISET_StopAllRing(TRUE);
    //MMIAPISET_SetCurRingType(MMISET_RING_TYPE_JAVA); 

}

/*****************************************************************************/
//  Description :MMIAPIJAVA_AudioClientPlayAgain
//  Global resource dependence :
//  Author:
//  Note: to implement loop play
/*****************************************************************************/
PUBLIC void MMIAPIJAVA_AudioClientPlayAgain(void)
{
    JWE_LOG(MMIAPIJAVA_AudioClientPlayAgain,("enter"));

    if(s_active_playerID >= 0)//to do something in future,control is bad,murphy.xie
    {
        mediaCtrl_startLoop(s_active_playerID);
    }
}

/*****************************************************************************/
//  Description :MMIJAVA_AudioClientGetActivePlayerID
//  Global resource dependence :
//  Author:
//  Note: to implement loop play
/*****************************************************************************/
PUBLIC JKSint32 MMIJAVA_AudioClientGetActivePlayerID(void)
{
    JWE_LOG(MMIJAVA_AudioClientGetActivePlayerID,("enter"));
    return s_active_playerID;
}
#endif
