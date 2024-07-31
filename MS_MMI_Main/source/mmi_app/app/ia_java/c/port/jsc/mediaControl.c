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
 #ifdef JAVA_SUPPORT_IA
#include "mmijava_internal.h"
#include "mediacontrol.h"
// #include "kjava_sys_gfx.h"
#include "jkmediastream.h"
#include "jkmediaextension.h"
#include "ammediacontrol.h"//ammediapostevent
/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#define DEFAULT_VOLUME          20
#define MEDIACONTROL_STOPTIME_DETECT_INTERVAL   	    500
#define MMIJAVA_MEDIA_SUFFIX_LEN      6 

/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/
JKT_Media_VolumeControl mediaCtrl_VolumeCtrl = {
    mediaCtrl_Volume_setVolume,
    mediaCtrl_Volume_getVolume
};
/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/
PUBLIC T_mediaPlayer*   m_pLinkTop = JK_NULL;
LOCAL JKSint32          m_nNextPlayerID = 1;
PUBLIC StreamPlayer*    stream_player_head = JK_NULL;
LOCAL JKSint32          m_nNextStreamPlayerID=1;
LOCAL BOOLEAN           s_is_paused_external = FALSE; //用来区分是应用自身的pause还是porting的操作
/**--------------------------------------------------------------------------*
 **                         EXTERNAL DECLARE                                 *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : mediaCtrl_is_unsupport_paused_media_type
//  Global resource dependence : none
//  Author: 
//  Note: 
//        @param handle    Handle to the library
//        @retval TRUE      Success
//        @retval FALSE     Fail
/*****************************************************************************/
LOCAL BOOLEAN mediaCtrl_is_unsupport_paused_media_type(JKSint32 mediatype )
{
#ifdef MIDI_SUPPORT	
    if ( JKMEDIA_TYPE_MIDI == mediatype)
    {
        return TRUE;
    }
#endif
    
#ifdef WAV_SUPPORT		
    if ( JKMEDIA_TYPE_WAVE == mediatype)
    {
        return TRUE;
    }
#endif
    
#ifdef AMR_SUPPORT	
    if ( JKMEDIA_TYPE_AMR == mediatype)
    {
        return TRUE;
    }          
#endif				
    
    return FALSE;
}


LOCAL JKSint32 mediaCtrl_Volume_setVolume( JKSint32 id, JKSint32 value )
{
	T_mediaPlayer *pPlayer = getPlayer( id );
	
	JWE_LOG(mediaCtrl_Volume_setVolume, ("enter id=%d value=%d", id,value)); 
	
	if ( pPlayer == JK_NULL ) 
	{
	    JWE_LOG(mediaCtrl_Volume_setVolume, ("leave! null player"));
	    return JKMEDIA_RESULT_ERROR;
	}
	
	if ( (value >= 0) && (value <= 127) ) 
	{
		if(pPlayer->media_type<JKMEDIA_TYPE_AUDIO){

			if(pPlayer->status==JKMEDIA_STATE_STARTED){
				JWE_LOG(mediaCtrl_Volume_setVolume, ("pPlayer->status=%d",pPlayer->status)); 
				if(!AudioClient_player_setVolume(pPlayer->ac, value))
				{
					JWE_LOG(mediaCtrl_Volume_setVolume, ("AudioClient_player_setVolume fail")); 
					return JKMEDIA_RESULT_ERROR;
				}
			}
			pPlayer->volume = value;
			return JKMEDIA_RESULT_OK;
		}
		else{
			JWE_LOG(mediaCtrl_Volume_setVolume, ("not implement video")); 
			return JKMEDIA_RESULT_ERROR;
		}
	}
	
	JWE_LOG(mediaCtrl_Volume_setVolume, ("value >=128 or value <0 error "));
	return JKMEDIA_RESULT_ERROR;
}


/**
 * Get current volume.
 */
LOCAL JKSint32 mediaCtrl_Volume_getVolume( JKSint32 id )
{
    T_mediaPlayer *pPlayer = getPlayer( id );
    JWE_LOG(mediaCtrl_Volume_getVolume, ("enter id=%d",id));
    if ( pPlayer == JK_NULL ) 
    {
        JWE_LOG(mediaCtrl_Volume_getVolume, ("leave error null  player"));
        return JKMEDIA_RESULT_ERROR;
    }
    
    return (pPlayer->volume);
}

JKSint32 mediaCtrl_startLoop( JKSint32 id)
{
    T_mediaPlayer* pPlayer;	
    JWE_LOG(mediaCtrl_startLoop, ("enter,id=%d",id));
		
    pPlayer = getPlayer( id );
    if ( pPlayer ==JK_NULL) {
        JWE_LOG(mediaCtrl_startLoop,("fail null player"));
        return JKMEDIA_RESULT_ERROR;
    }

    if( pPlayer->ac->isEndOfMedia==TRUE){
        pPlayer->ac->isEndOfMedia=FALSE;
    }
	
    if(pPlayer->media_type < JKMEDIA_TYPE_DEVICE_BASE ) {
	if(pPlayer->media_type < JKMEDIA_TYPE_AUDIO){/*_audio_type_tag*/
	     if(AudioClient_player_play(pPlayer->ac,pPlayer->ac->need_PlayTimes,id,pPlayer->volume)==FALSE){
                JWE_LOG(mediaCtrl_startLoop,("call AudioClient_player_play fail"));
                pPlayer->status=JKMEDIA_STATE_ERROR;
                return JKMEDIA_RESULT_ERROR;
            }    
	}
	else{/*_video_type_tag*/
		  JWE_LOG(mediaCtrl_startLoop,("Video not implement"));
		  return JKMEDIA_RESULT_ERROR;
	}
    }
    else if(pPlayer->media_type<JKMEDIA_TYPE_RECORD_AUDIO){
	  JWE_LOG(mediaCtrl_startLoop,("Video not implement"));
	  return JKMEDIA_RESULT_ERROR;
    }
    else if(pPlayer->media_type<JKMEDIA_TYPE_RECORD_VIDEO){
	  JWE_LOG(mediaCtrl_startLoop,("Video not implement"));
	  return JKMEDIA_RESULT_ERROR;
    }


    pPlayer->status = JKMEDIA_STATE_STARTED;
    JWE_LOG(mediaCtrl_startLoop,("leave"));
    return JKMEDIA_RESULT_OK;
}

JKSint32 mediaCtrl_checkOpen( const JKUint8* pData, const JKSint32 length ,JKSint32 media_type  ) 
{
    T_mediaPlayer * pPlayer = NULL;
    JKSint32 result = 0;

    JWE_LOG(mediaCtrl_checkOpen,("enter,pData=0x%08x,length=%d,media_type=%d,",pData,length,media_type));
	
    pPlayer = createPlayer(media_type);
    if(pPlayer == JK_NULL)
    {
		JWE_LOG(mediaCtrl_checkOpen,("check open fail"));
		return JKMEDIA_RESULT_ERROR;
    }
	
    JWE_LOG(mediaCtrl_checkOpen,("createPlayer success player id = %d",pPlayer->id));
		
    result = JKMEDIA_RESULT_OK;//result = mediaCtrl_set(pPlayer->id,pData,length,0); @murphy to do check
    if(result == JKMEDIA_RESULT_ERROR) /*lint !e774*/
    {
		JWE_LOG(mediaCtrl_checkOpen,("check open fail"));
		destroyPlayer(pPlayer->id);
		return JKMEDIA_RESULT_ERROR;
    }
    else
    {
		JWE_LOG(mediaCtrl_checkOpen,("ok return check id=%d",pPlayer->id + JKMEDIA_CHECK_ID_BASE));
    }
	
    return pPlayer->id  + JKMEDIA_CHECK_ID_BASE;
}

JKSint32 mediaCtrl_check( JKSint32 checkID, JKSint32* pExtraID ) 
{
     T_mediaPlayer* pPlayer;
     StreamPlayer* pSPlayer ;
    JWE_LOG(mediaCtrl_check,( "checkID=%d",checkID));
    pPlayer = getPlayer(  checkID - JKMEDIA_CHECK_ID_BASE );
    
    if ( pPlayer == JK_NULL )
    {
        JWE_LOG(mediaCtrl_check,("checkl fail.null player(normal player) for check id"));
        pSPlayer = getStreamPlayer(  checkID - JKMEDIA_CHECK_ID_BASE );
        JWE_LOG(mediaCtrl_check,( "checkID=%d the streamplayer's id is %d",checkID, pSPlayer->id))
        if(pSPlayer == JK_NULL)
        {
            JWE_LOG(mediaCtrl_check,("null stream player"));
            return JKMEDIA_RESULT_ERROR;
        }
    }
	
    /* didnt check */
    return JKMEDIA_RESULT_OK;

}



JKSint32 mediaCtrl_checkClose( JKSint32 checkID ) 
{   
    T_mediaPlayer* pPlayer;
    JKSint32 result = 0;
    JWE_LOG(mediaCtrl_checkClose,("enter, checkID=%d",checkID));
    pPlayer = getPlayer( checkID - JKMEDIA_CHECK_ID_BASE );
    if ( pPlayer == JK_NULL ) 
    {
		JWE_LOG(mediaCtrl_checkClose,("check close fail.null player for this check id"));
		destroyPlayer(checkID - JKMEDIA_CHECK_ID_BASE);			
		return JKMEDIA_RESULT_ERROR;
    }
   
    result = JKMEDIA_RESULT_OK;//result = mediaCtrl_set(pPlayer->id,NULL,0,0);

    if(result == JKMEDIA_RESULT_ERROR) /*lint !e774*/
    {
        JWE_LOG(mediaCtrl_checkClose, ("check close fail."));
	 destroyPlayer(checkID - JKMEDIA_CHECK_ID_BASE);
        return JKMEDIA_RESULT_ERROR;
    }
    else
    {
        JWE_LOG(mediaCtrl_checkClose,("ok return checkID=%d player->id=%d",checkID,pPlayer->id));
    }
    return destroyPlayer(checkID - JKMEDIA_CHECK_ID_BASE);

}

JKSint32 mediaCtrl_getMediaSize( JKSint32 checkID, JKSint32* pWidth, JKSint32* pHeight )
{   
    T_mediaPlayer *pPlayer=JK_NULL;
    // KJavaGfxInfo *info;
    JWE_LOG(mediaCtrl_getMediaSize, ("enter,checkID=%d",checkID));
    *pWidth = 0;
    *pHeight = 0;
    if(checkID>JKMEDIA_CHECK_ID_BASE){
        JWE_LOG(mediaCtrl_getMediaSize,("for check, ingore it"));
        return JKMEDIA_RESULT_OK;
    }
    
    pPlayer=getPlayer(checkID);
    if(pPlayer==JK_NULL){
        JWE_LOG(mediaCtrl_getMediaSize,("null player"));
        return JKMEDIA_RESULT_ERROR;
    }

    if(pPlayer->media_type<JKMEDIA_TYPE_AUDIO){
        JWE_LOG(mediaCtrl_getMediaSize,("audio cases"));
        return JKMEDIA_RESULT_OK;
    }
    else{
	 JWE_LOG(mediaCtrl_getMediaSize,("video cases.unsupport now"));
	 return JKMEDIA_RESULT_OK;
    }

}



JKSint32 mediaCtrl_create( JKSint32 media_type ) 
{
    T_mediaPlayer* pPlayer=JK_NULL;
    JWE_LOG(mediaCtrl_create, ("enter media_type=%d",media_type));;

    pPlayer=createPlayer(media_type);
    if ( pPlayer == JK_NULL ) {
        JWE_LOG(mediaCtrl_create, ("create Player fail"));
        return 0;
    }
    MMIJAVA_lock_device_multimedia();
    pPlayer->volume = (JKSint32)DEFAULT_VOLUME;
    //pPlayer->ac->native_volume = AudioClient_volumeConv(pPlayer->volume);

    JWE_LOG(mediaCtrl_create,( "create Player succeed id is %d", pPlayer->id));
    return pPlayer->id;
}

JKSint32 mediaCtrl_destroy( JKSint32 id )
{
    T_mediaPlayer *pPlayer = getPlayer( id );
    
    JWE_LOG(mediaCtrl_destroy, ("enter player id=%d",id));
    
    if ( pPlayer == JK_NULL ) 
    {
        JWE_LOG(mediaCtrl_destroy, ("find id error "));
        return JKMEDIA_RESULT_ERROR;
    }
    JWE_LOG(mediaCtrl_destroy, ("leave! ok "));      
    return destroyPlayer( pPlayer->id );

}


JKSint32 mediaCtrl_set( JKSint32 id, const JKUint8* pData, const JKSint32 length, JKSint32 extraID )
{
	T_mediaPlayer *pPlayer;

	JWE_LOG(mediaCtrl_set,("enter,player id=%d,pData=0x%08x length=%d extraID=%d",id,pData,length,extraID));

	pPlayer = getPlayer( id );
	if ( pPlayer == JK_NULL ) {
	    JWE_LOG(mediaCtrl_set,("set fail. null player id"));
	    return JKMEDIA_RESULT_ERROR;
	}

	if( pPlayer->media_type < JKMEDIA_TYPE_DEVICE_BASE) {/*_audio_type_tag and _video_types_tag*/
	    if ( !pData ) {/*set(null) close native mmapi handle.free platform resources*/
		 JWE_LOG(mediaCtrl_set,("close native player")); 
	        if(pPlayer->media_type<JKMEDIA_TYPE_AUDIO){/*_audio_type_tag*/
			 AudioClient_player_close(pPlayer->ac);
	        }
	        else {  /*_video_types_tag*/
			 JWE_LOG(mediaCtrl_set,("not implement video"));
			 #if 0
			 Close video handle
		 	#endif
	        }
	        pPlayer->status = JKMEDIA_STATE_NODATA;
	        return JKMEDIA_RESULT_OK;
	    }  
           /* if(!pData) */
	    else{   /* set(data).open native mmapi handle */
	        if(length<=0){
	            JWE_LOG(mediaCtrl_set,("!!invalid data size"));
	            return JKMEDIA_RESULT_ERROR;
	        }
	        pPlayer->pData = ( JKUint8* )pData;
	        pPlayer->length = ( JKSint32 )length;

	        if(pPlayer->media_type<JKMEDIA_TYPE_AUDIO){/*_audio_type_tag*/
	              if(pPlayer->media_type==JKMEDIA_TYPE_TONE){ /* TONE */
	                    if(!AudioClient_player_open_tone_sequence(pPlayer->id,pPlayer->media_type,(uint8*)pData,length,pPlayer->tp,pPlayer->ac))
	                    	{
					JWE_LOG(mediaCtrl_set,(" AudioClient_player_open_tone_sequence failed!!!"));
					return JKMEDIA_RESULT_ERROR;
				}

	              }   /* TONE */
			else{
				if(!AudioClient_player_open(pPlayer->id,pPlayer->media_type,(uint8*)pData,length,PNULL,pPlayer->ac))
				{
					JWE_LOG(mediaCtrl_set,(" AudioClient_player_open failed!!!"));
					return JKMEDIA_RESULT_ERROR;
				}
			}

	        }
	        else{/*_video_types_tag*/
			    JWE_LOG(mediaCtrl_set,("Video not implement"));
			    return JKMEDIA_RESULT_ERROR;
		    }

	    }   /* else part of if(!pData) */
	}  
	else if( pPlayer->media_type > JKMEDIA_TYPE_DEVICE_BASE){
		JWE_LOG(mediaCtrl_set,("Rcord not implement"));
		return JKMEDIA_RESULT_ERROR;
	}
	
	pPlayer->status = JKMEDIA_STATE_DATA_SET;
	pPlayer->time_low = 0;
	pPlayer->time_high= 0;
	return JKMEDIA_RESULT_OK;
}

/*
Note:
Not implement mediaCtrl_load mediaCtrl_unload.
Some problem will in prefetch and deallocate
*/
JKSint32 mediaCtrl_load( JKSint32 id ) 
{
    T_mediaPlayer * pPlayer ;
    JWE_LOG(mediaCtrl_load, ("enter,id=%d",id));
    pPlayer = getPlayer( id );
    if(pPlayer==JK_NULL){
        JWE_LOG(mediaCtrl_load,("load fail. null player"));
        return JKMEDIA_RESULT_ERROR;
    }
    pPlayer->status = JKMEDIA_STATE_READY;
    return JKMEDIA_RESULT_OK;
}



JKSint32 mediaCtrl_start( JKSint32 id, JKSint32 loop )
{
    T_mediaPlayer* pPlayer;
    JWE_LOG(mediaCtrl_start, ("enter,id=%d loop=%d",id,loop));
		
    pPlayer = getPlayer( id );
    if ( pPlayer ==JK_NULL) {
        JWE_LOG(mediaCtrl_start,("fail null player"));
        return JKMEDIA_RESULT_ERROR;
    }

    if(pPlayer->status == JKMEDIA_STATE_STARTED)//判断是否
    {
        JWE_LOG(mediaCtrl_start,("ERROR STATUS pPlayer->status =%d",pPlayer->status ));
        return JKMEDIA_RESULT_OK;
    }
    
    if( pPlayer->ac->isEndOfMedia==TRUE){
        pPlayer->ac->isEndOfMedia=FALSE;
    }
	
    pPlayer->loop = loop;
	
    if(loop == 0)
	    pPlayer->ac->is_infiniteTimes = TRUE;
    else
	    pPlayer->ac->is_infiniteTimes = FALSE;
    
    MMIJAVA_lock_device_multimedia();// 有些情况下，不进入create，也有可能进入这个函数
    
    if(pPlayer->media_type < JKMEDIA_TYPE_DEVICE_BASE ) {
	if(pPlayer->media_type < JKMEDIA_TYPE_AUDIO){/*_audio_type_tag*/
	     if(AudioClient_player_play(pPlayer->ac,loop,id,pPlayer->volume)==FALSE){
                JWE_LOG(mediaCtrl_start,("call AudioClient_player_play fail"));
                pPlayer->status=JKMEDIA_STATE_ERROR;
                return JKMEDIA_RESULT_ERROR;
            }    
	}
	else{/*_video_type_tag*/
		  JWE_LOG(mediaCtrl_start,("Video not implement"));
		  return JKMEDIA_RESULT_ERROR;
	}
    }
    else if(pPlayer->media_type<JKMEDIA_TYPE_RECORD_AUDIO){
	  JWE_LOG(mediaCtrl_start,("Video not implement"));
	  //return JKMEDIA_RESULT_ERROR;
    }
    else if(pPlayer->media_type<JKMEDIA_TYPE_RECORD_VIDEO){
	  JWE_LOG(mediaCtrl_start,("Video not implement"));
	  return JKMEDIA_RESULT_ERROR;
    }

    pPlayer->playPos = SCI_GetTickCount();
    pPlayer->status = JKMEDIA_STATE_STARTED;
    JWE_LOG(mediaCtrl_start,("leave"));
    return JKMEDIA_RESULT_OK;
}

JKSint32 mediaCtrl_stop( JKSint32 id ) 
{
    T_mediaPlayer* pPlayer;
    
    JWE_LOG(mediaCtrl_stop, ("enter,id=%d",id));
    pPlayer = getPlayer( id );
    if ( pPlayer ==JK_NULL) {
        JWE_LOG(mediaCtrl_stop,("fail null player"));
        return JKMEDIA_RESULT_ERROR;
    }     
    #ifdef JAVA_SUPPORT_MEDIA_RECORD    
    pPlayer->pending_stop=1;
    #endif
    
    if(pPlayer->media_type < JKMEDIA_TYPE_DEVICE_BASE){
        if(pPlayer->media_type < JKMEDIA_TYPE_AUDIO ){/*_audio_type_tag*/
            if(AudioClient_player_stop(pPlayer->ac)==FALSE){
                JWE_LOG(mediaCtrl_stop,("AudioClient_player_pause could not pause success. returns FALSE"));
		   return JKMEDIA_RESULT_ERROR;
            }
			
        }
        else{/*_video_type_tag*/
 		  JWE_LOG(mediaCtrl_stop,("Video Not implement"));
		  return JKMEDIA_RESULT_ERROR;	
        }

    }
    else if(pPlayer->media_type<JKMEDIA_TYPE_RECORD_AUDIO){
         JWE_LOG(mediaCtrl_stop,("Audio Record Not implement"));
	  return JKMEDIA_RESULT_ERROR;	 	
    }
    else if(pPlayer->media_type<JKMEDIA_TYPE_RECORD_VIDEO){
	  JWE_LOG(mediaCtrl_stop,("Video Record Not implement"));
	  return JKMEDIA_RESULT_ERROR;	
    }
    
    #ifdef JAVA_SUPPORT_MEDIA_RECORD    
    pPlayer->pending_stop=0;  
    #endif
    pPlayer->time_low = 0;
    pPlayer->time_high = 0;
    pPlayer->status = JKMEDIA_STATE_READY;
    return JKMEDIA_RESULT_OK;
}

JKSint32 mediaCtrl_pause( JKSint32 id ) 
{
    T_mediaPlayer* pPlayer;
    uint64 usec;
    JWE_LOG(mediaCtrl_pause,("enter,id=%d",id));

    pPlayer = getPlayer( id );
    if ( pPlayer==JK_NULL ) {
        JWE_LOG(mediaCtrl_pause,("fail.null player"));
        return JKMEDIA_RESULT_ERROR;
    }
    
    if(pPlayer->status != JKMEDIA_STATE_STARTED)//判断是否
    {
        JWE_LOG(mediaCtrl_pause,("ERROR STATUS pPlayer->status =%d",pPlayer->status ));
        return JKMEDIA_RESULT_OK;
    }

    /* if end_of_media ,the state is in READY already ,no need to pause */
    //XXX ,need to verify this,race condition??
    if(pPlayer->ac->isEndOfMedia==JK_TRUE){
        JWE_LOG(mediaCtrl_pause,("call mediaCtrl_pause when EOF "));
        if(!s_is_paused_external)
        {
 		    JWE_LOG(mediaCtrl_pause,("not s_is_paused_external"));
            pPlayer->status = JKMEDIA_STATE_PAUSED;
        }
        return JKMEDIA_RESULT_OK;
    }   
    #if 0
    if(ajms_arc_getState() == AJSC_VMSTATE_SUSPENDED 
		|| ajms_arc_getState() == AJSC_VMSTATE_SUSPENDING)
    {
        JWE_LOG(mediaCtrl_pause,("JVM is suspending,maybe we no need to pause media! because before JVM exe->pause,it will call mediaCtrl_pause first"));
        return JKMEDIA_RESULT_OK;        
    }

    #endif
    if(pPlayer->media_type < JKMEDIA_TYPE_DEVICE_BASE){
        if(pPlayer->media_type < JKMEDIA_TYPE_AUDIO )
        {/*_audio_type_tag*/
           if(!pPlayer->hasExtPaused) //外部导致pause，再次进入后，不再pause
            {
                if (mediaCtrl_is_unsupport_paused_media_type(pPlayer->media_type))
                {
                    if(!AudioClient_player_stop(pPlayer->ac))
                    {
                        JWE_LOG(mediaCtrl_pause,("AudioClient_player_stop could not pause success. returns FALSE"));
                        return JKMEDIA_RESULT_ERROR;
                    }

                }
                else
                {
                    if(!AudioClient_player_pause(pPlayer->ac))
                    {
                        JWE_LOG(mediaCtrl_pause,("AudioClient_player_pause could not pause success. returns FALSE"));
                        return JKMEDIA_RESULT_ERROR;
                    }
                }
            }          
        }
        else{/*_video_type_tag*/
 		  JWE_LOG(mediaCtrl_pause,("Video Not implement"));
		  return JKMEDIA_RESULT_ERROR;	
        }
        usec = (uint64)pPlayer->ac->native_time * 1000 *1000;	
        pPlayer->time_low =  (usec<<32)>>32;
        pPlayer->time_high = usec>>32;
        
        if(!s_is_paused_external)
        {
 		    JWE_LOG(mediaCtrl_pause,("not s_is_paused_external"));
            pPlayer->status = JKMEDIA_STATE_PAUSED;
        }

    }
    else if(pPlayer->media_type<JKMEDIA_TYPE_RECORD_AUDIO){
         JWE_LOG(mediaCtrl_pause,("Record Not implement"));
	  //return JKMEDIA_RESULT_ERROR;	 	
    }
    else if(pPlayer->media_type<JKMEDIA_TYPE_RECORD_VIDEO){
	  JWE_LOG(mediaCtrl_pause,("Record Not implement"));
	  return JKMEDIA_RESULT_ERROR;	
    }
    
    if(s_is_paused_external)
    {
        pPlayer->hasExtPaused = TRUE;
    }

    JWE_LOG(mediaCtrl_pause,("leave"));
    return JKMEDIA_RESULT_OK;
}

//resume the play
JKSint32 mediaCtrl_restart( JKSint32 id ) 
{
    T_mediaPlayer* pPlayer;
    // int vmState;

    JWE_LOG(mediaCtrl_restart,("enter (only in pause status ),id=%d",id));
    pPlayer = getPlayer( id );
    if ( pPlayer==JK_NULL ) {
        JWE_LOG(mediaCtrl_restart,("fail null player"));
        return JKMEDIA_RESULT_ERROR;        
    }
    
    //区分外部pause,外部pause started状态可以进入
    if( !pPlayer->hasExtPaused && (pPlayer->status == JKMEDIA_STATE_STARTED) )
    {
         JWE_LOG(mediaCtrl_restart,("Do not have to restart media player when it has already started"));
         return JKMEDIA_RESULT_OK;
    }
    
    //把lock 退后到开始播放时
    MMIJAVA_lock_device_multimedia();

    if(pPlayer->media_type < JKMEDIA_TYPE_DEVICE_BASE ) {
	if(pPlayer->media_type < JKMEDIA_TYPE_AUDIO)
     {/*_audio_type_tag*/
        if (mediaCtrl_is_unsupport_paused_media_type(pPlayer->media_type))
        {
            if(AudioClient_player_play(pPlayer->ac,pPlayer->loop,id,pPlayer->volume)==FALSE)
            {
                JWE_LOG(mediaCtrl_restart,("call AudioClient_player_play fail"));
                pPlayer->status=JKMEDIA_STATE_ERROR;
                return JKMEDIA_RESULT_ERROR;
            }    
        }
        else
        {
            if(AudioClient_player_replay(pPlayer->ac,pPlayer->volume)==FALSE)//增加volum参数
            {
                JWE_LOG(mediaCtrl_restart,("call AudioClient_player_play fail"));
                pPlayer->status=JKMEDIA_STATE_ERROR;
                return JKMEDIA_RESULT_ERROR;
            }    
        }
	}
	else{/*_video_type_tag*/
		  JWE_LOG(mediaCtrl_restart,("Video not implement"));
		  return JKMEDIA_RESULT_ERROR;
	}
    }
    else if(pPlayer->media_type<JKMEDIA_TYPE_RECORD_AUDIO){
	  JWE_LOG(mediaCtrl_restart,("Video not implement"));
	  return JKMEDIA_RESULT_ERROR;
    }
    else if(pPlayer->media_type<JKMEDIA_TYPE_RECORD_VIDEO){
	  JWE_LOG(mediaCtrl_restart,("Video not implement"));
	  return JKMEDIA_RESULT_ERROR;
    }
    
    if( pPlayer->ac->isEndOfMedia==TRUE){
        pPlayer->ac->isEndOfMedia=FALSE;
    }
    
    pPlayer->status = JKMEDIA_STATE_STARTED;
    return JKMEDIA_RESULT_OK;
}




/*
Note:
Not implement mediaCtrl_load mediaCtrl_unload.
Some problem will in prefetch and deallocate
*/
JKSint32 mediaCtrl_unload( JKSint32 id ) 
{
    T_mediaPlayer* pPlayer = getPlayer( id );
    
    JWE_LOG(mediaCtrl_unload,("enter,id=%d",id));
    if ( pPlayer == JK_NULL ) {
        JWE_LOG(mediaCtrl_unload,("unload failed. null player"));
        return JKMEDIA_RESULT_ERROR;
    }
    
    pPlayer->status = JKMEDIA_STATE_DATA_SET;
    return JKMEDIA_RESULT_OK;
}




JKSint32 mediaCtrl_getState( JKSint32 id ) 
{
   
    T_mediaPlayer* pPlayer = getPlayer( id );

    if ( pPlayer == JK_NULL ) {
	 JWE_LOG(mediaCtrl_getState, ("can not find player id=%d",id));
        return JKMEDIA_RESULT_ERROR;
    }
    JWE_LOG(mediaCtrl_getState,("state=%d",pPlayer->status));
    return pPlayer->status;
}



JKSint32 mediaCtrl_getDuration( JKSint32 id, JKUint32* pUsecH, JKUint32* pUsecL )
{
    T_mediaPlayer *pPlayer = PNULL;
    uint64 usec = 0;

	*pUsecH = JKMEDIA_DURATION_UNKNOWN;
	*pUsecL = JKMEDIA_DURATION_UNKNOWN;
    
    JWE_LOG(mediaCtrl_getDuration, ("enter"));
    
    if ( id >= JKMEDIA_CHECK_ID_BASE ) 
    {
        JWE_LOG(mediaCtrl_getDuration, ("id is checkID impossible"));
        return JKMEDIA_RESULT_OK;//id -= JKMEDIA_CHECK_ID_BASE;
    }
    pPlayer = getPlayer( id );
    
    if ( pPlayer == JK_NULL ) 
    {
        JWE_LOG(mediaCtrl_getDuration, ("fail player null"));
        return (JKSint32)JKMEDIA_RESULT_ERROR;
    }

    if(pPlayer->media_type>JKMEDIA_TYPE_DEVICE_BASE){
        JWE_LOG(mediaCtrl_getDuration,("for capture media , no duration"))
        return JKMEDIA_RESULT_OK;
    }


    if(pPlayer->media_type<JKMEDIA_TYPE_AUDIO){
		if ( pPlayer->duration_high != JKMEDIA_DURATION_UNKNOWN &&  pPlayer->duration_low != JKMEDIA_DURATION_UNKNOWN) {
			*pUsecL = pPlayer->duration_low;
			*pUsecH = pPlayer->duration_high;
			JWE_LOG(mediaCtrl_getDuration, ("audio leave! ok *pUsecL=%d *pUsecH=%d",*pUsecL,*pUsecH));
			return JKMEDIA_RESULT_OK;	
		}	
		
		if( !AudioClient_player_get_duration(pPlayer->ac,&usec))
		{
		    JWE_LOG(mediaCtrl_getDuration, ("leave! getduration error"));
		    return (JKSint32)JKMEDIA_RESULT_ERROR;
		}
    }
    else if(pPlayer->media_type<JKMEDIA_TYPE_VIDEO)
    {
	
		if ( pPlayer->duration_high != JKMEDIA_DURATION_UNKNOWN &&  pPlayer->duration_low != JKMEDIA_DURATION_UNKNOWN) {
			*pUsecL = pPlayer->duration_low;
			*pUsecH = pPlayer->duration_high;
			JWE_LOG(mediaCtrl_getDuration, ("video leave! ok *pUsecL=%d *pUsecH=%d",*pUsecL,*pUsecH));
			return JKMEDIA_RESULT_OK;	
		}	
		JWE_LOG(mediaCtrl_getDuration, ("Video Not Implement"));
		return (JKSint32)JKMEDIA_RESULT_ERROR;
    }
    else
    {
		JWE_LOG(mediaCtrl_getDuration, ("Video Not Implement"));
		return (JKSint32)JKMEDIA_RESULT_ERROR;
    }


	*pUsecL = (usec<<32)>>32;
	*pUsecH =usec>>32;
	
	pPlayer->duration_low = *pUsecL;
	pPlayer->duration_high = *pUsecH;
	
    JWE_LOG(mediaCtrl_getDuration, ("leave! ok *pUsecL=%d pUsecH=%d",*pUsecL,*pUsecH));
    return JKMEDIA_RESULT_OK;
}


JKSint32 mediaCtrl_setTime( JKSint32 id, JKUint32 usecH, JKUint32 usecL )
{
	T_mediaPlayer *pPlayer = PNULL;
	//uint64 usec = 0;
	
	JWE_LOG(mediaCtrl_setTime, ("enter id=%d usecH=%d usecL=%d",id,usecH,usecL));
	
	pPlayer = getPlayer( id );
	
	if ( pPlayer == JK_NULL ) 
	{
	    JWE_LOG(mediaCtrl_setTime, ("leave! null error "));
	    return JKMEDIA_RESULT_ERROR;
	}

	JWE_LOG(mediaCtrl_setTime, (" pPlayer->status=%d", pPlayer->status));

	if ( pPlayer->status == JKMEDIA_STATE_READY || pPlayer->status == JKMEDIA_STATE_DATA_SET ||pPlayer->status ==JKMEDIA_STATE_PAUSED) 
	{
		JWE_LOG(mediaCtrl_setTime, (" pPlayer->status=JKMEDIA_STATE_READY||JKMEDIA_STATE_DATA_SET||JKMEDIA_STATE_PAUSED"));
		pPlayer->time_low = usecL;
		pPlayer->time_high= usecH;
		return JKMEDIA_RESULT_OK;
	}
	else if ( pPlayer->status == JKMEDIA_STATE_STARTED ) 
	{
		JWE_LOG(mediaCtrl_setTime, ("pPlayer->status == JKMEDIA_STATE_STARTED"));
        #if 0 //支持seek的，无须设置时间，目前平台paused状态进行seek，会导致assert 
	    if(AudioClient_supportSeek(pPlayer->ac)) //支持seek，进行seek,否则只设定时间
        {
    		usec = usecH;
    		usec = usec<<32;
    		usec = usec + usecL;
    		if(!AudioClient_player_set_media_time(pPlayer->ac,usec))
    		{
    			JWE_LOG(mediaCtrl_setTime, ("AudioClient_player_set_media_time fail"));
    			return JKMEDIA_RESULT_ERROR;
    		}
        }
        #endif

		pPlayer->time_low = usecL;
		pPlayer->time_high= usecH;
		return JKMEDIA_RESULT_OK;
	} 
	else
	{
		JWE_LOG(mediaCtrl_setTime, (" pPlayer->status wrong"));
		return JKMEDIA_RESULT_ERROR;
	}

}


JKSint32 mediaCtrl_getTime( JKSint32 id, JKUint32* pUsecH, JKUint32* pUsecL ) 
{
	T_mediaPlayer *pPlayer = PNULL;
	uint64 usec = 0;
	JWE_LOG(mediaCtrl_getTime, ("enter id=%d",id));
	pPlayer = getPlayer( id );
	if ( pPlayer == JK_NULL ) 
	{
	    JWE_LOG(mediaCtrl_getTime, ("leave! null error "));
	    return JKMEDIA_RESULT_ERROR;
	}
	
 	JWE_LOG(mediaCtrl_getTime, ("pPlayer->status=%d",pPlayer->status));
	if(pPlayer->media_type<JKMEDIA_TYPE_AUDIO){
	    
		if ( pPlayer->status == JKMEDIA_STATE_READY ||pPlayer->status ==JKMEDIA_STATE_DATA_SET ||pPlayer->status ==JKMEDIA_STATE_PAUSED) 
		{
			*pUsecH = pPlayer->time_high;
			*pUsecL = pPlayer->time_low;
			 JWE_LOG(mediaCtrl_getTime, ("pPlayer->status == JKMEDIA_STATE_READY ||JKMEDIA_STATE_DATA_SET ||JKMEDIA_STATE_PAUSED *pUsecH=%d *pUsecL=%d",*pUsecH,*pUsecL));
			return JKMEDIA_RESULT_OK;
		}
		else
		{
			if (!AudioClient_player_get_media_time(pPlayer->ac,&usec) ) 
			{
			    JWE_LOG(mediaCtrl_getTime, ("AudioClient_player_get_media_time error "));
			    //pPlayer->time = JKMEDIA_TIME_UNKNOWN;
			    return JKMEDIA_RESULT_ERROR;
			}    
			
		}
	   
	}
	else if(pPlayer->media_type<JKMEDIA_TYPE_VIDEO){
		JWE_LOG(mediaCtrl_getTime, ("Video not implement"));
		return JKMEDIA_RESULT_ERROR;

	}

    //new add 20110124 -murphy.xie
	if((pPlayer->time_low > 0 || pPlayer->time_high > 0) && usec == 0 )
	{
		*pUsecL = pPlayer->time_low ;
		*pUsecH = pPlayer->time_high;
		JWE_LOG(mediaCtrl_getTime, ("leave  ok  *pUsecL=%d  *pUsecH=%d",*pUsecL,*pUsecH));
		return JKMEDIA_RESULT_OK;
	}
	
	*pUsecL = (usec<<32)>>32;
	*pUsecH =usec>>32;
	
	pPlayer->time_low= *pUsecL;
	//+CR254293
	pPlayer->time_high= *pUsecH;
	//-CR254293
	JWE_LOG(midiCtrl_getTime, ("leave  ok  *pUsecL=%d  *pUsecH=%d",*pUsecL,*pUsecH));
	return JKMEDIA_RESULT_OK;
}


/*****************************************************************************/
//  Description : mediaCtrl_is_unsupport_paused_media_type
//  Global resource dependence : none
//  Author: 
//  Note: this function will be called in SUSPENED EVENT CALLBACK TO pause media
/*****************************************************************************/
JKSint32 mediaCtrl_suspendVM( JKSint32 id ) 
{
    //JKSint32 status = 0;

    JKSint32 ret = JKMEDIA_RESULT_ERROR;
    T_mediaPlayer *pPlayer = PNULL;
    JWE_LOG(mediaCtrl_suspendVM, ("enter id=%d",id));
    
    pPlayer = getPlayer( id );
    if ( pPlayer == JK_NULL ) 
    {
        JWE_LOG(mediaCtrl_suspendVM, ("leave! null error "));
        return JKMEDIA_RESULT_ERROR;
    }
    
	#if 0
    status = pPlayer->status;
    JWE_LOG(mediaCtrl_suspendVM, ("pPlayer->status = %d",status));  

    /* Stop playback and close device */
    /*lint -save -e825 */
    switch( pPlayer->status ) 
    {
    case JKMEDIA_STATE_STARTED:
    case JKMEDIA_STATE_PAUSED:
        mediaCtrl_stop( id );
        break;
    case JKMEDIA_STATE_READY:       
        mediaCtrl_unload( id );       
        break;
    default:
        break;
    }
    /*lint -restore */
    /* Do not change current status for resume. */
	
    //pPlayer->status = status;
    //pPlayer->time = usec;
    #endif
    //mediaCtrl_pause(id);
    
    ret = JKMEDIA_RESULT_OK;
    
    JWE_LOG(mediaCtrl_suspendVM, ("leave"));
    return ret;
}


/*****************************************************************************/
//  Description : mediaCtrl_resumeVM
//  Global resource dependence : none
//  Author: 
//  Note: Do the process when JVM is to resumed.
/*****************************************************************************/
JKSint32 mediaCtrl_resumeVM( JKSint32 id ) 
{
    JKSint32 status = 0;

    JKSint32 ret = JKMEDIA_RESULT_ERROR;
    T_mediaPlayer *pPlayer = PNULL;
    AJ_S32 state = ajms_arc_getState() ;
    
    JWE_LOG(mediaCtrl_resumeVM, ("enter id=%d, state = %d",id,state));

    if((AJSC_VMSTATE_DESTROYING == state|| AJSC_VMSTATE_KILLING  == state))//如果jvm正在关闭则不恢复播放
    {
        JWE_LOG(mediaCtrl_resumeVM, ("leave! jvm is not resuming, no need restart "));
        return ret;
    }

    if(MMIJAVA_AudioClientGetActivePlayerID() != id )
    {
        JWE_LOG(mediaCtrl_resumeVM, ("leave! not s_active_playerID"));
        return ret;
    }

    pPlayer = getPlayer( id );
    if ( pPlayer == JK_NULL ) 
    {
        JWE_LOG(mediaCtrl_resumeVM, ("leave! null error "));
        return ret;
    }
    
    /* restoring device etc. */
    status = pPlayer->status;
    JWE_LOG(mediaCtrl_resumeVM, ("pPlayer->status = %d",status));  

    //pPlayer->status = JKMEDIA_STATE_DATA_SET;
    
    switch( status ) 
    {
        case JKMEDIA_STATE_STARTED:
            mediaCtrl_setTime( id,pPlayer->time_high, pPlayer->time_low);
            //mediaCtrl_Volume_setVolume( id, pPlayer->volume ); //会引起其他音频资源的变化
            mediaCtrl_restart(id);
          break;
        case JKMEDIA_STATE_PAUSED:
            break;
        default:
            break;
    }
    ret = JKMEDIA_RESULT_OK;
    pPlayer->hasExtPaused = FALSE;
    
    JWE_LOG(mediaCtrl_resumeVM, ("leave"));  
    return ret;
}




/*=====================================================================
 *
 * Stop Time Control
 *
 *====================================================================*/ 
LOCAL T_mediaPlayer* getPlayerbyStopTimer( JKSint32 timer_id )
{
    T_mediaPlayer *p = PNULL;

    JWE_LOG(getPlayerbyStopTimer, ("enter timer_id = %d", timer_id));
    for ( p = m_pLinkTop; p != JK_NULL; p = p->pNext )
    {
        if ( p->stoptime_timerid == timer_id ) 
        {
            JWE_LOG(getPlayerbyStopTimer, ("leave p->timer_id=%d", p->stoptime_timerid));
            return p;
        }
    }
    JWE_LOG(getPlayerbyStopTimer, ("leave. can not get this player."));

    return JK_NULL;
}

static JKSint32 mediaOpt_StopTime_setStopTime( JKSint32 id, JKUint32 usecH , JKUint32 usecL)
{
    JKSint32 ret = JKMEDIA_RESULT_ERROR;

    T_mediaPlayer* pPlayer;
    uint64 duration64 = 0;
    uint64 stoptime64 = 0;
    // uint64 currenttime64 = 0;
    uint32 elapsedTime; 
	
    JWE_LOG(mediaOpt_StopTime_setStopTime, ("enter id=%d, usecH=%d, usecL=%d", id, usecH, usecL));
    
    pPlayer = getPlayer( id );
    if ( pPlayer == JK_NULL ) 
    {
        JWE_LOG(mediaOpt_StopTime_setStopTime, ("leave! null error "));
        return ret;
    }

    pPlayer->stoptime_high = usecH; 
    pPlayer->stoptime_low = usecL;
	
    if (usecH == JKMEDIA_TIME_MAX_HIGH && JKMEDIA_TIME_MAX_LOW == usecL)
    {
	if(pPlayer->stoptime_timerid!=0)
	{
		JWE_LOG(mediaOpt_StopTime_setStopTime, ("MMK_StopTimer pPlayer->stoptime_timerid=%d",pPlayer->stoptime_timerid));		
		MMK_StopTimer(pPlayer->stoptime_timerid);
	}
       JWE_LOG(mediaOpt_StopTime_setStopTime, ("reset:usecH == JKMEDIA_TIME_MAX_HIGH && JKMEDIA_TIME_MAX_LOW == usecL  JKMEDIA_RESULT_OK"));		
       return JKMEDIA_RESULT_OK;
    }

/*
    if ((pPlayer->state == JKMEDIA_STATE_STARTED) && (pPlayer->stopTime != 0XFFFFFFFF))
    {
        JWE_LOG(mediaOpt_StopTime_setStopTime, ("Player already playing.Return JKMEDIA_RESULT_ERROR"));		
        return JKMEDIA_RESULT_ERROR;
    }
*/
/*
    if ((usecH & 0x80000000) == 0x80000000)
    {
        pMediaData->stopTime = 0;
        JBOB_LOG(MMAPI, ("mediaControl_setStopTime():EXIT:(usecH & 0x80000000) == 0x80000000 Return JKMEDIA_RESULT_OK"));		
        return JKMEDIA_RESULT_OK;
    }
*/
    /* stop time should not exceed media length */
    duration64 = pPlayer->duration_high;
    duration64 = duration64<<32;
    duration64 = duration64 + pPlayer->duration_low;
	
    stoptime64 = usecH;
    stoptime64 = stoptime64<<32;
    stoptime64 = stoptime64 + usecL;	
    JWE_LOG(mediaOpt_StopTime_setStopTime, ("duration64=%lld stoptime64=%lld",duration64,stoptime64));		
	/*need note:repreat problem*/
    if (stoptime64 > duration64)
	{
	 JWE_LOG(mediaOpt_StopTime_setStopTime, ("stoptime64 > duration64 no need operate"));		
        pPlayer->stoptime_low = pPlayer->duration_low;
        pPlayer->stoptime_high = pPlayer->duration_high;
        JWE_LOG(mediaOpt_StopTime_setStopTime, ("stopTime == duration Return JKMEDIA_RESULT_OK"));		
        return JKMEDIA_RESULT_OK;
    }

    /*
    If the stop time is short than the current play position
    THEN
        - 1. pause the player immediately
        - 2. notify the jvm with JKMEDIA_EVENT_STOPPED_AT_TIME==>toTime32.
    */
    if (pPlayer->playPos != 0)//has started
        elapsedTime = SCI_GetTickCount() - pPlayer->playPos;
    else
        elapsedTime  = 0;
	
    JWE_LOG(mediaOpt_StopTime_setStopTime, ("elapsedTime=%ld",elapsedTime));		

    if (stoptime64 < ((uint64)elapsedTime*1000))
	{
        JKT_MediaEvent mediaEvent;        

        ret = mediaCtrl_pause(id);

        mediaEvent.playerID = pPlayer->id;
        mediaEvent.mediaType = pPlayer->media_type;


        if (JKMEDIA_RESULT_OK == ret)
	{
            mediaEvent.eventType = JKMEDIA_EVENT_STOPPED_AT_TIME;
	     mediaEvent.usecH = pPlayer->stoptime_high;
            mediaEvent.usecL = pPlayer->stoptime_low;
        }
        else
            mediaEvent.eventType = JKMEDIA_EVENT_ERROR;

        AmMediaPostEvent(&mediaEvent);

        pPlayer->stoptime_high = JKMEDIA_TIME_MAX_HIGH;
	 pPlayer->stoptime_low = JKMEDIA_TIME_MAX_LOW;

        JWE_LOG(mediaOpt_StopTime_setStopTime, ("return :stoptime64 < currenttime64", ret));
        return ret;
    } 

    JWE_LOG(mediaOpt_StopTime_setStopTime, ("stoptime64 > currenttime64"));
    JWE_LOG(mediaOpt_StopTime_setStopTime, ("MSG_JAVA_MEDIA_STOPTIME_TIMER->MMIAPIJAVA_StartMediaStopTimer"));
    MMIAPIJAVA_SendSignal(MSG_JAVA_MEDIA_STOPTIME_TIMER,pPlayer->id);

    JWE_LOG(mediaOpt_StopTime_setStopTime, ("Return JKMEDIA_RESULT_OK"));
    return JKMEDIA_RESULT_OK;
}

PUBLIC void MMIAPIJAVA_StartMediaStopTimer(int32 player_id)
{
	T_mediaPlayer* pPlayer;
	JWE_LOG(MMIAPIJAVA_StartMediaStopTimer, ("enter player_id=%d",player_id));
	pPlayer = getPlayer( player_id );
	if ( pPlayer == JK_NULL ) 
	{
	    JWE_LOG(MMIAPIJAVA_StartMediaStopTimer, ("leave! null error "));
	    return;
	}
	//new add 20110124 -murphy.xie
	if(pPlayer->stoptime_timerid>0 && pPlayer->stoptime_timerid !=NULL)
	{
		MMK_StopTimer(pPlayer->stoptime_timerid);
	}
	pPlayer->stoptime_timerid = MMK_CreateTimerCallback(MEDIACONTROL_STOPTIME_DETECT_INTERVAL, MMIAPIJAVA_MediaStopTimer_Process,  NULL, SCI_TRUE);
	JWE_LOG(MMIAPIJAVA_StartMediaStopTimer, ("MMK_CreateWinTimer =  pPlayer->stoptime_timerid=%d",pPlayer->stoptime_timerid));
	//pPlayer->stoptime_timerid = KJAVA_SOCKET_LINK_POLLING_TIME_COUNT;
	
	MMK_StartTimerCallback(pPlayer->stoptime_timerid, MEDIACONTROL_STOPTIME_DETECT_INTERVAL, MMIAPIJAVA_MediaStopTimer_Process,  NULL,  SCI_TRUE);
}

PUBLIC void MMIAPIJAVA_MediaStopTimer_Process(uint8 timerID, uint32 param)
{
	// int32 i = 0;
	T_mediaPlayer* pPlayer;
	uint32 curTickTime_millisecond;
	uint32 elapsedTime_millisecond;
	uint64 stopTime_microsecond;


	JWE_LOG(MMIAPIJAVA_MediaStopTimer_Process, ("enter timerID=%d",timerID));
	
	pPlayer = getPlayerbyStopTimer( timerID );
	if ( pPlayer == JK_NULL ) 
	{
	    JWE_LOG(MMIAPIJAVA_MediaStopTimer_Process, ("leave! null error "));
	    return;
	}

	curTickTime_millisecond = SCI_GetTickCount();
	
	if (pPlayer->playPos != 0)
	{
		elapsedTime_millisecond = curTickTime_millisecond - pPlayer->playPos;
	}
	else
	{
		elapsedTime_millisecond = 0;
	}

	stopTime_microsecond = pPlayer->stoptime_high;
	stopTime_microsecond = stopTime_microsecond<<32;
	stopTime_microsecond = stopTime_microsecond + pPlayer->stoptime_low;
	
	if (((uint64)elapsedTime_millisecond*1000) < stopTime_microsecond) 
	{
        JWE_LOG(MMIAPIJAVA_MediaStopTimer_Process, ("continu timer return"));        
		return;
	}
	else
	{
		JKT_MediaEvent mediaEvent;

		mediaEvent.playerID = pPlayer->id;
		mediaEvent.mediaType = pPlayer->media_type;
		mediaEvent.usecH = 0;

        //new add 20110124 -murphy.xie
		JWE_LOG(MMIAPIJAVA_MediaStopTimer_Process, ("mediaCtrl_pause"));       
		MMK_StopTimer(timerID);
		mediaCtrl_pause(pPlayer->id);

		mediaEvent.eventType = JKMEDIA_EVENT_STOPPED_AT_TIME;
		mediaEvent.usecL = pPlayer->stoptime_low;
		mediaEvent.usecH = pPlayer->stoptime_high;

		AmMediaPostEvent(&mediaEvent);

		pPlayer->stoptime_high = JKMEDIA_TIME_MAX_HIGH;
		pPlayer->stoptime_low = JKMEDIA_TIME_MAX_LOW;

	}
}

static JKSint32 mediaOpt_StopTime_getStopTime(JKSint32 id, JKUint32 *usecH ,JKUint32 *usecL)
{
	T_mediaPlayer* pPlayer;

	JWE_LOG(mediaOpt_StopTime_getStopTime, ("enter id=%d", id));

	pPlayer = getPlayer( id );
	if ( pPlayer == JK_NULL ) 
	{
	    JWE_LOG(mediaOpt_StopTime_getStopTime, ("leave! null error "));
	    return JKMEDIA_RESULT_ERROR;
	}


	*usecH = pPlayer->stoptime_high;
	*usecL = pPlayer->stoptime_low;

	JWE_LOG(mediaOpt_StopTime_getStopTime, ("return JKMEDIA_RESULT_OK *pUsecH=%d *pUsecL=%d",*usecH,*usecL));    
	return JKMEDIA_RESULT_OK;
}

JKT_Media_StopControl mediaOpt_StopCtrl={
    mediaOpt_StopTime_setStopTime,
    mediaOpt_StopTime_getStopTime,  
};

/*---------------------------------------------------------------------------*/
/*                          FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
PUBLIC T_mediaPlayer* getPlayer( JKSint32 id )
{
    T_mediaPlayer *p = PNULL;

    JWE_LOG(getPlayer, ("enter id = %d", id));
    for ( p = m_pLinkTop; p != JK_NULL; p = p->pNext )
    {
        if ( p->id == id ) 
        {
            JWE_LOG(getPlayer, ("leave p->id=%d", p->id));
            return p;
        }
    }
    JWE_LOG(getPlayer, ("leave. can not get this player."));

    return JK_NULL;
}

PUBLIC T_mediaPlayer* createPlayer( int media_type ) 
{
    T_mediaPlayer *p = PNULL;  
    T_mediaPlayer *pPlayer = ( T_mediaPlayer* )SCI_ALLOC_APP( sizeof( T_mediaPlayer ) );

    JWE_LOG(createPlayer, ("enter"));

    if ( pPlayer == JK_NULL ) 
    {
        JWE_LOG(createPlayer, ("alloc T_mediaPlayer failed. return null"));
        return JK_NULL;
    }
    /* parameter initialization */
    SCI_MEMSET( pPlayer, 0, sizeof( T_mediaPlayer ) );
	
    pPlayer->media_type = media_type;
    pPlayer->duration_high = JKMEDIA_DURATION_UNKNOWN;
    pPlayer->duration_low = JKMEDIA_DURATION_UNKNOWN;
    pPlayer->loop = 1;
    pPlayer->status   = JKMEDIA_STATE_NODATA;
    pPlayer->volume   = DEFAULT_VOLUME;
    pPlayer->time_high   = JKMEDIA_TIME_UNKNOWN;
    pPlayer->time_low   = JKMEDIA_TIME_UNKNOWN;
    pPlayer->stoptime_high   = JKMEDIA_TIME_MAX_HIGH;
    pPlayer->stoptime_low   = JKMEDIA_TIME_MAX_LOW;
    pPlayer->id       = m_nNextPlayerID;
    pPlayer->tp       = PNULL;
    pPlayer->ac       = PNULL;
    m_nNextPlayerID++;
    if ( m_nNextPlayerID >= JKMEDIA_CHECK_ID_BASE ) 
    {
    	 JWE_LOG(createPlayer, ("Warning !The next player id %d mustn't beyound JKMEDIA_CHECK_ID_BASE",m_nNextPlayerID,JKMEDIA_CHECK_ID_BASE));
        m_nNextPlayerID = 1;
    }

    /* link */
    if ( m_pLinkTop == JK_NULL ) 
    {
        m_pLinkTop = pPlayer;
        pPlayer->ac = SCI_ALLOC_APP(sizeof( AudioClient ) );     
	 SCI_MEMSET( pPlayer->ac, 0, sizeof( AudioClient ) );
	 pPlayer->ac->m_hAudioHandle = PNULL;
	 #ifdef TONE_USE_JBLEND_PARSER
	 if(media_type==JKMEDIA_TYPE_TONE){
	 	 pPlayer->tp = SCI_ALLOC_APP(sizeof( ToneParser) );     
		 SCI_MEMSET( pPlayer->tp, 0, sizeof( ToneParser ) );
		 pPlayer->tp->duration = JKMEDIA_DURATION_UNKNOWN;
	 }
	 #endif
        JWE_LOG(createPlayer, ("leave"));
        return pPlayer;
    } 
    else 
    {
        for ( p = m_pLinkTop; ; p = p->pNext ) 
        {
            if ( p->pNext == JK_NULL ) 
            {
                p->pNext = pPlayer;
		  pPlayer->ac =SCI_ALLOC_APP(sizeof( AudioClient ) );     
		  SCI_MEMSET( pPlayer->ac, 0, sizeof( AudioClient ) );
		  pPlayer->ac->m_hAudioHandle = PNULL;
		  #ifdef TONE_USE_JBLEND_PARSER
		  if(media_type==JKMEDIA_TYPE_TONE){
	 	  pPlayer->tp = SCI_ALLOC_APP(sizeof( ToneParser) );     
		  SCI_MEMSET( pPlayer->tp, 0, sizeof( ToneParser ) );
		  pPlayer->tp->duration = JKMEDIA_DURATION_UNKNOWN;
	 	  }
		  #endif
		  JWE_LOG(createPlayer, ("leave"));
                return pPlayer;
            }
        }

    }

}

PUBLIC JKSint32 destroyPlayer( JKSint32 id )
{
    T_mediaPlayer *p = PNULL;
    T_mediaPlayer *q=m_pLinkTop;
    JWE_LOG(destroyPlayer,("enter,id=%d",id));
    
    for(p=m_pLinkTop ; p!=JK_NULL ; p=p->pNext){
        if(p->id==id){	
	     JWE_LOG(destroyPlayer, ("find this id in m_pLink,p->status =%d",p->status));	
	     switch( p->status ) 
             {
             	case JKMEDIA_STATE_STARTED:
             	case JKMEDIA_STATE_PAUSED:
					JWE_LOG(destroyPlayer, ("gc pPlayer=0x%.8x id=%d : STARTED or PAUSED", p,id));
					mediaCtrl_stop( p->id );
					mediaCtrl_unload( p->id );
					mediaCtrl_set( p->id,PNULL,0,0);	 	 
					/* not break */
					//lint -fallthrough
				case JKMEDIA_STATE_READY:
					JWE_LOG(destroyPlayer, ("gc pPlayer=0x%.8x id=%d : READY", p,id));
					mediaCtrl_unload( p->id );
					mediaCtrl_set( p->id,PNULL,0,0);	 
					break;
                case JKMEDIA_STATE_DATA_SET:
                    mediaCtrl_set( p->id,PNULL,0,0);     
                    break;
				default:
				    JWE_LOG(destroyPlayer, ("gc pPlayer=0x%.8x id=%d :OTHER STATE", p,id));
				    break;
             }
	     JWE_LOG(destroyPlayer,(" successfully free player %d",id));
	     SCI_FREE(p->ac);
	     #ifdef TONE_USE_JBLEND_PARSER
	     if(p->tp!=PNULL){
		SCI_FREE(p->tp);
	     }
	     #endif
		 
        if(p==m_pLinkTop)
        { /* try to free player_head*/
            if(p->pNext==JK_NULL)
            { /* only 1 player */  
                m_pLinkTop=JK_NULL;
            }
            else 
            {
                m_pLinkTop=p->pNext;
            }
        }
        else
        {
            q->pNext=p->pNext;
        }
			
        if(p->stoptime_timerid>0 && p->stoptime_timerid != NULL)
        {
            MMK_StopTimer(p->stoptime_timerid);
        }
                
        SCI_FREE(p);
        return JKMEDIA_RESULT_OK;
        
        }
        else{
            q=p;
        }
    }
   
    JWE_LOG(destroyPlayer,("fail to destroy player"));
    return JKMEDIA_RESULT_ERROR;
}

void mediaCtrl_callback(int32 event_type,int player_id,JKUint32 UsecH, JKUint32 UsecL,int32 needRepeat)
{
    T_mediaPlayer *pPlayer;
    JKSint32 result;
    JKT_MediaEvent event;
    JWE_LOG(mediaCtrl_callback,("enter,player_id=%d,event=%d,high=%u,low=%u",player_id,event_type,UsecH,UsecL))
   
    pPlayer=getPlayer(player_id);
    if(pPlayer==JK_NULL){
        JWE_LOG(mediaCtrl_callback,("null player"));
        return;
    }
    event.playerID=player_id;
    event.mediaType=pPlayer->media_type;
    event.usecL = UsecL;
    event.usecH = UsecH;

    if(event_type == MEDIA_CLIENT_EVENT_END)
    {   
        /*有的格式必须等到播放完之后才能知道总的持续时间，但展讯上当音乐播放结束后就得不到当前time了*/
       pPlayer->duration_low= UsecL;
       pPlayer->duration_high= UsecH;

	 if(needRepeat==1){
	     JWE_LOG(mediaCtrl_callback,("End of media,but will repeat"));
         #if 0
	     event.eventType=JKMEDIA_EVENT_REPEAT;
            if( (result=AmMediaPostEvent(&event)) ==JKMEDIA_RESULT_ERROR){
                JWE_LOG(mediaCtrl_callback,("Post JKMEDIA_EVENT_REPEAT Event error"));
                return ;
            }       
            else if(result==JKMEDIA_RESULT_OK){
                JWE_LOG(mediaCtrl_callback,("Post JKMEDIA_EVENT_REPEAT Event ok"));
            }
         #endif
            //mediaCtrl_startLoop(player_id);
			MMIAPIJAVA_SendSignal(MSG_JAVA_AUDIO_PLAY_AGAIN,PNULL);            
	        return;
	}
	else{
	    JWE_LOG(mediaCtrl_callback,("End of media,no more loop post JKMEDIA_EVENT_END_OF_MEDIA Event "));
        event.eventType=JKMEDIA_EVENT_END_OF_MEDIA;
        //MMIJAVA_unlock_device_multimedia(); NEWMS00108253
	    if( (result=AmMediaPostEvent(&event)) ==JKMEDIA_RESULT_ERROR){
	        JWE_LOG(mediaCtrl_callback,("Post JKMEDIA_EVENT_END_OF_MEDIA Event error"));
	        return ;
	    }       
	    else if(result==JKMEDIA_RESULT_OK){
		JWE_LOG(mediaCtrl_callback,("Post JKMEDIA_EVENT_END_OF_MEDIA Event ok"));
	    }
	}

        if(pPlayer->status==JKMEDIA_STATE_STARTED && pPlayer->media_type<JKMEDIA_TYPE_DEVICE_BASE){
		JWE_LOG(mediaCtrl_callback,("pPlayer->status=JKMEDIA_STATE_READY"));
              pPlayer->status=JKMEDIA_STATE_READY;
        }
    }
	
}


/*===========================================================================
 *
 *  Stream Control Functions
 *
 ==========================================================================*/
LOCAL int get_media_type_by_url(wchar *url)
{
    wchar suffix[MMIJAVA_MEDIA_SUFFIX_LEN] = {0};
    uint16 suffix_len = MMIJAVA_MEDIA_SUFFIX_LEN;    
    uint16 urlLength=0;
    urlLength = MMIAPICOM_Wstrlen( url);
    
    JWE_LOG(get_media_type_by_url,("url="));
    MMIAPICOM_WstrTraceOut(url,urlLength);

    
    MMIAPIFMM_SplitFileName(url, 
    urlLength, 
    PNULL, 
    PNULL, 
    suffix, 
    &suffix_len);     
 
    suffix_len = (suffix_len > MMIJAVA_MEDIA_SUFFIX_LEN/2)? suffix_len: MMIJAVA_MEDIA_SUFFIX_LEN/2;    
    MMIAPICOM_Wstrlower(suffix);  
    

    if (0 == MMIAPICOM_Wstrncmp(suffix, L"mid", suffix_len))
        return JKMEDIA_TYPE_MIDI;
    else if(0 == MMIAPICOM_Wstrncmp(suffix, L"jts", suffix_len))
        return JKMEDIA_TYPE_TONE;
    else if(0 == MMIAPICOM_Wstrncmp(suffix, L"wav", suffix_len))
        return JKMEDIA_TYPE_WAVE;
    else if(0 == MMIAPICOM_Wstrncmp(suffix, L"amr", suffix_len))
        return JKMEDIA_TYPE_AMR;
    else if(0 == MMIAPICOM_Wstrncmp(suffix, L"mp3", suffix_len))
        return JKMEDIA_TYPE_MP3;
    else if(0 == MMIAPICOM_Wstrncmp(suffix, L"aac", suffix_len))
        return JKMEDIA_TYPE_AAC;
#if 0	
    else if(ends_with(url_char,"m4a"))
        return JKMEDIA_TYPE_M4A;
    else if(ends_with(url_char,"mp4"))
        return JKMEDIA_TYPE_M4A;	
    else if(ends_with(url_char,"3gp"))
        return JKMEDIA_TYPE_MP2;
    else if(ends_with(url_char,"gif"))
        return JKMEDIA_TYPE_AVI;
#endif
    else {
	 JWE_LOG(get_media_type_by_url,("unknown media type!!!!"));
        return -1;
    }
}


StreamPlayer *createStreamPlayer(JKWChar *wchar_url)
{
    StreamPlayer* p;
    StreamPlayer* pStreamPlayer;
    int len;
    
    JWE_LOG( createStreamPlayer,( "enter") );
	
    pStreamPlayer = ( StreamPlayer* )SCI_ALLOC_APP( sizeof( StreamPlayer));
    if ( pStreamPlayer == JK_NULL ) {
        JWE_LOG(createStreamPlayer,("pStreamPlayer malloc returns  null!"));
        return JK_NULL;
    }
    /* parameter initialization */
    SCI_MEMSET( pStreamPlayer, 0, sizeof( StreamPlayer ) );

    len=MMIAPICOM_Wstrlen(wchar_url);
    pStreamPlayer->wchar_url=(char *)SCI_ALLOCA((len+1)*sizeof(JKWChar));/*lint !e737*/
    if(pStreamPlayer->wchar_url==JK_NULL)
     {
        JWE_LOG(createStreamPlayer,("pStreamPlayer->url malloc returns  null!"));
        SCI_FREE(pStreamPlayer);
        return JK_NULL;
     }

    pStreamPlayer->id=m_nNextStreamPlayerID;
    MMIAPICOM_Wstrcpy((wchar *)pStreamPlayer->wchar_url, (wchar *)wchar_url);
    pStreamPlayer->pNext=JK_NULL;
    pStreamPlayer->media_type=get_media_type_by_url(wchar_url);
    pStreamPlayer->stream_type=0;
    JWE_LOG(createStreamPlayer,("pStreamPlayer->media_type=%d",pStreamPlayer->media_type));
    m_nNextStreamPlayerID++;
    if ( m_nNextStreamPlayerID >= JKMEDIA_CHECK_ID_BASE ) 
    {
    	JWE_LOG(createStreamPlayer, ("Warning !The next player id %d mustn't beyound JKMEDIA_CHECK_ID_BASE",m_nNextPlayerID,JKMEDIA_CHECK_ID_BASE));
	m_nNextStreamPlayerID = 1;
    }
	
    /* link */
    p=stream_player_head;
    if ( p==JK_NULL ) {
        stream_player_head = pStreamPlayer;
        return pStreamPlayer;
    } 
    else {
        while ( p ) {
            if ( p->pNext == JK_NULL ) {
              
               p->pNext = pStreamPlayer;
               return pStreamPlayer;
            }
            p=p->pNext;
        }
        return pStreamPlayer;
    }

}

JKSint32 destroyStreamPlayer(JKSint32 streamId)
{
    StreamPlayer *p,*q=stream_player_head;
    JWE_LOG(destroyStreamPlayer,("enter"));

    for(p=stream_player_head ; p!=JK_NULL ; p=p->pNext){
        if(p->id==streamId){
            if(p==stream_player_head){ /* try to free player_head*/
                if(p->pNext==JK_NULL){ /* only 1 player */  
                    stream_player_head=JK_NULL;
                }
                else {
                    stream_player_head=p->pNext;
                }
            }
            else{
                q->pNext=p->pNext;
            }
            JWE_LOG(destroyStreamPlayer,(" successfully free player %d",streamId));
            SCI_FREE(p->wchar_url);
            SCI_FREE(p);
            return JKMEDIA_RESULT_OK;
        }
        else{
            q=p;
        }
    }

    JWE_LOG(destroyStreamPlayer,("fail to destroy player"));
    return JKMEDIA_RESULT_ERROR;
}


StreamPlayer *getStreamPlayer(JKSint32 streamId)
{
    StreamPlayer* p;  
    JWE_LOG(getStreamPlayer,("enter streamId=%d",streamId));
    for ( p = stream_player_head; p != JK_NULL; p = p->pNext ) {
        if ( p->id == streamId ) {
            return p;
        }
    }
    return JK_NULL;
}

static JKSint32 mediaCtrl_Extension_Stream_checkOpen(JKT_MediaStream  const *pStream,JKSint32 streamID)
{
    JWE_LOG(mediaCtrl_Extension_Stream_checkOpen,("enter,streanID=%d",streamID));
	
    return JKMEDIA_CHECK_ID_BASE + streamID;
}

static JKSint32 mediaCtrl_Extension_Stream_checkClose(JKSint32 checkID)
{
    JWE_LOG(mediaCtrl_Extension_Stream_checkClose,("enter,streanID=%d",checkID));

    return JKMEDIA_RESULT_OK;

}

static JKSint32 mediaCtrl_Extension_Stream_setStream( JKSint32 id,JKT_MediaStream const *pStream, JKSint32 streamID)
{
    T_mediaPlayer *pPlayer=JK_NULL;
    StreamPlayer *pStreamPlayer=JK_NULL;
    // int result;
	
    JWE_LOG(mediaCtrl_Extension_Stream_setStream,("enter,id=%d,streamID=%d",id,streamID));

    pStreamPlayer=getStreamPlayer(streamID);
    if(pStreamPlayer==JK_NULL){
        JWE_LOG(mediaCtrl_Extension_Stream_setStream,("null Stream player"));
        return JKMEDIA_RESULT_ERROR;
    }
	
    if(pStreamPlayer->media_type==-1){
        JWE_LOG(mediaCtrl_Extension_Stream_setStream,("unsupported media type"));
        return JKMEDIA_RESULT_ERROR;
    }

    pPlayer=getPlayer(id);
    if(pPlayer==JK_NULL){
        JWE_LOG(mediaCtrl_Extension_Stream_setStream,("null player"));
        return JKMEDIA_RESULT_ERROR;
    }
	
    //pPlayer->stream_type=pStreamPlayer->stream_type;
    pPlayer->stream_id = streamID;//用于记录filestream player的id

    if(pPlayer->media_type<JKMEDIA_TYPE_AUDIO){/*_audio_type_tag*/	
	   if(!AudioClient_player_open(pPlayer->id,pPlayer->media_type,PNULL,0,pStreamPlayer->wchar_url,pPlayer->ac))
	   {
		JWE_LOG(mediaCtrl_Extension_Stream_setStream,(" AudioClient_player_open failed!!!"));
		return JKMEDIA_RESULT_ERROR;
	   }
    }
    else{/*_video_types_tag*/
	    JWE_LOG(mediaCtrl_Extension_Stream_setStream,("Video not implement"));
	    return JKMEDIA_RESULT_ERROR;
    }
	
    return JKMEDIA_RESULT_OK;

}


static JKT_Media_StreamControl mediaCtrl_Extension_StreamCtrl={
    mediaCtrl_Extension_Stream_checkOpen,
    mediaCtrl_Extension_Stream_checkClose,
    mediaCtrl_Extension_Stream_setStream,
};


JKT_MediaExtension mediaCtrl_ExtensionCtrl={
    &mediaCtrl_Extension_StreamCtrl,  /* JKT_Media_StreamControl const * pStreamFuncs */        
    NULL,               /* void const * pMultiFuncs */
    NULL,               /* void const * pExtFuncs */
};
					
PUBLIC void MMIJAVA_MMAPI_init(void)
{
	JWE_LOG(MMIJAVA_MMAPI_init,("enter" ))
	AudioClient_player_initialize();

}
PUBLIC void MMIJAVA_MMAPI_fini(void)
{
	JWE_LOG(MMIJAVA_MMAPI_fini,("enter" ))
	AudioClient_player_finalize();

}
/*****************************************************************************/
//  Description :MMIJAVA_getPlayingPlayerVolume
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIJAVA_getPlayingPlayerVolume(int32* volume)
{
	T_mediaPlayer*  p = PNULL;
	int32           player_state;
	BOOLEAN         ret = FALSE;

	JWE_LOG(MMIJAVA_getPlayingPlayerVolume,("enter" ));

	for ( p = m_pLinkTop; p != JK_NULL; p = p->pNext )
	{
		player_state = mediaCtrl_getState(p->id);
		if(player_state==JKMEDIA_STATE_STARTED){
			*volume = p->volume;
			ret =TRUE;
			return ret;
		}
	}
	
	return ret;
}


/*****************************************************************************/
//  Description :MMIJAVA_PauseAllMediaPlayer
//  Global resource dependence : none
//  Author: 
//  Note: 此接口用于给MMI 调用，先关闭audio play
/*****************************************************************************/
PUBLIC void MMIJAVA_PauseAllMediaPlayer(void)
{
    JKSint32  active_playerID = MMIJAVA_AudioClientGetActivePlayerID();   
    JWE_LOG(MMIJAVA_PauseAllMediaPlayer,("enter,active_playerID =%d",active_playerID));
    s_is_paused_external = TRUE;
    mediaCtrl_pause(active_playerID);
    s_is_paused_external = FALSE;

    //release music play control
    //mediaCtrl_unlock_device_multimedia();
}


/*****************************************************************************/
//  Description :MMIJAVA_ResumeAllMediaPlayer
//  Global resource dependence : none
//  Author: 
//  Note: 此接口用于给MMI 调用，先恢复audio
/*****************************************************************************/
PUBLIC void MMIJAVA_ResumeAllMediaPlayer(void)
{
    JKSint32  active_playerID = MMIJAVA_AudioClientGetActivePlayerID();   
    JWE_LOG(MMIJAVA_ResumeAllMediaPlayer,("enter,active_playerID =%d",active_playerID));
    
    //mediaCtrl_restart(active_playerID);//不做jvm恢复，等待jvm自己恢复
}

/*****************************************************************************/
//  Description : close all playing media when JVM shutdown.
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MMIJAVA_CloseAllMediaPlayer()
{
    JKSint32  active_playerID = MMIJAVA_AudioClientGetActivePlayerID();   
    JWE_LOG(MMIJAVA_CloseAllMediaPlayer,("enter,active_playerID =%d",active_playerID));
    
    //mediaCtrl_stop(active_playerID);
}

#endif
