/*
 * Copyright 2003 Aplix Corporation. All Rights Reserved.
 *
 * JBlend and all Aplix- or JBlend-related trademarks and logos are
 * trademarks or registered trademarks of Aplix in the U.S., Japan and
 * other countries.
 */

/*****************************************************************************
** File Name:      media_recordAmrControl.c                                        *
** Description:    This file is used to implement record function             *
******************************************************************************/

#include "jblend_config.h"


#ifdef JAVA_SUPPORT_MEDIA_RECORD
/*-----------------------------------------------------------------------------
 * Include Files
 *----------------------------------------------------------------------------*/
#include "mmijava_internal.h"

/* header files for jblendia */
#include "jkmediacontrol.h"	    /* jkmediaxx */
#include "jkmediaoption.h"	    /* for option control */
#include "ammediacontrol.h"	    /* midp2.0 jbi funcs */

#include "mediacontrol.h"	    /* for struct player ,jkmedia_type_record_amr */

/* header files for ANSI C */
#include <string.h>		    /* for strncmp ,strcpy */
#include "jcutf8.h"         //for  jcutf8touni


/*-----------------------------------------------------------------------------
 * Marcos, Types, and Structures
 *----------------------------------------------------------------------------*/
#define BUFFER_LENGTH           10000 // for buffer size
#define RECODE_FILE_MAX_LENGTH  768
/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/
/* for RecordControl */
static JKUint8 *m_record_buff = NULL;   /*used by getBuffer() & discard()*/
static KJavaFileHandle m_file_handle;
static JKSint32 m_record_size = 0;
static JKSint32 m_record_buff_count = 0;
static int m_open_buff = 0;
static char DEFAULT_RECORD_FILE[RECODE_FILE_MAX_LENGTH+1]={0};
static char DEST_RECORD_FILE[RECODE_FILE_MAX_LENGTH+1] ={0};
static int m_camera_init_width = 0;
static int m_camera_init_height = 0;
static int is_location_set = 0;

/*-----------------------------------------------------------------------------
 * External variables
 *----------------------------------------------------------------------------*/
extern JKT_Media_RecordControl mediaOpt_RecordCtrl;
extern JKT_Media_EventControl mediaCtrl_EventCtrl;
/*-----------------------------------------------------------------------------
 * Local variables
 *----------------------------------------------------------------------------*/
//static const JKT_MediaOption recordAmrCtrl_OptionCtrl;
/*-----------------------------------------------------------------------------
 * External Function Prototypes
 *----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
 * Local Function Prototypes
 *----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
 * Global functions
 *----------------------------------------------------------------------------*/
/* JKT_Media_VolumeControl */
/**
 * Set current volume.
 */
static JKSint32 recordAmrVolCtrl_setVolume( JKSint32 id, JKSint32 value )
{
    JWE_LOG(recordAmrVolCtrl_setVolume, ("id=%d;value=%d",id,value))

    return JKMEDIA_RESULT_OK;
}

/**
 * Get current volume.
 */

static JKSint32 recordAmrVolCtrl_getVolume( JKSint32 id )
{
    JWE_LOG(recordAmrVolCtrl_getVolume, ("id=%d",id))

    return  0;
}

/**
 * Structure for volume control functions
 */

static const JKT_Media_VolumeControl volumeControl = {
    recordAmrVolCtrl_setVolume,
    recordAmrVolCtrl_getVolume
};

/**
 * Get MIME-type string.
 */
static JKSint32 recordAmrCtrl_getMIMEType( JKWChar** pType )
{
    static const JKWChar mimeType[] = {
        'a', 'u', 'd', 'i', 'o', '?', 'e', 'n','c','o','d','i','n','g','=','a','u','d','i','o','/','a','m','r',   
	/* for capture://audio?encoding=audio/amr */
    };
    *pType = ( JKWChar* )mimeType;
    JWE_LOG(recordAmrCtrl_getMIMEType, ("MIME is call"))
    return sizeof( mimeType ) / sizeof( JKWChar );
}

/**
 * Check signature in data.
 */
static JKBool recordAmrCtrl_isData( const JKUint8* pData, const JKSint32 length )
{
    JWE_LOG(recordAmrCtrl_isData,("enter"))
    return JK_TRUE;
}

/**
 * Open media for data validity check.
 */
JKSint32 recordAmrCtrl_checkOpen( const JKUint8* pData, const JKSint32 length )
{
    JWE_LOG(recordAmrCtrl_checkOpen,("enter JKMEDIA_TYPE_RECORD_AMR=%d",JKMEDIA_TYPE_RECORD_AMR))
    return mediaCtrl_checkOpen(pData,length,JKMEDIA_TYPE_RECORD_AMR);
}


/**
 * Create a player.
 */
JKSint32 recordAmrCtrl_create( void )
{
    JWE_LOG(recordAmrCtrl_create,("enter JKMEDIA_TYPE_RECORD_AMR=%d",JKMEDIA_TYPE_RECORD_AMR))
    return mediaCtrl_create(JKMEDIA_TYPE_RECORD_AMR);
}



JKSint32 recordAmrCtrl_set( JKSint32 id, const JKUint8* pData, const JKSint32 length, JKSint32 extraID )
{
	T_mediaPlayer *pPlayer;

	JWE_LOG(mediaCtrl_set,("enter,player id=%d,pData=0x%08x length=%d extraID=%d",id,pData,length,extraID));

	pPlayer = getPlayer( id );
	if ( pPlayer == JK_NULL ) {
	    JWE_LOG(mediaCtrl_set,("set fail. null player id"));
	    return JKMEDIA_RESULT_ERROR;
	}

 
	if( pPlayer->media_type > JKMEDIA_TYPE_DEVICE_BASE)
    {
			 JWE_LOG(mediaCtrl_set,(" pPlayer->media_type =%x", pPlayer->media_type));
        /* for live media control , the pData is always NULL */
        if(pPlayer->status == JKMEDIA_STATE_DATA_SET){
            /* return JKMEDIA_RESULT_OK here */
            pPlayer->pending_unset=1;
            pPlayer->pending_unset=0;

            if(pPlayer->media_type<JKMEDIA_TYPE_RECORD_AUDIO){
                JWE_LOG(mediaCtrl_set,("close audio recorder "));
                if(jas_audio_recorder_close(id)==SCI_FALSE)
                {
                    JWE_LOG(mediaCtrl_set,("call jma_audio_recorder_close failed"));
                    pPlayer->status=JKMEDIA_STATE_ERROR;
                    return JKMEDIA_RESULT_ERROR;
                }
            }
            else if(pPlayer->media_type<JKMEDIA_TYPE_RECORD_VIDEO)
            {
                if(pPlayer->media_type==JKMEDIA_TYPE_CAMERA)
                {
                    JWE_LOG(mediaCtrl_set,("close camera "));
                }
                else
                {
                    JWE_LOG(mediaCtrl_set,("close video recorder "));
                }         
            }
            

            pPlayer->status= JKMEDIA_STATE_NODATA;    
            JWE_LOG(mediaCtrl_set,("change state to NODATA"))
            return JKMEDIA_RESULT_OK;
        }   /* DATA_SET */
        else if(pPlayer->status==JKMEDIA_STATE_NODATA)
        {
            int result; 
            if( (result=kjava_fs_remove( DEFAULT_RECORD_FILE )) == KJAVA_FILE_ERR_NOT_EXIST)
            {
                JWE_LOG(mediaCtrl_set , ("file not existed"))
            }
            else if(result==KJAVA_FILE_ERR_IO_ERR)
            {
                JWE_LOG(mediaCtrl_set,("remove file error, IO error"))
            }

            if( (pPlayer->media_type<JKMEDIA_TYPE_RECORD_AUDIO))
            {
                //wchar path_name_wchar[10];
                wchar file[RECODE_FILE_MAX_LENGTH+1];   

                char path_name[20];

                sprintf(path_name,"C:\\");

                if(pPlayer->media_type==JKMEDIA_TYPE_RECORD_WAVE)
                {
                    sprintf(DEFAULT_RECORD_FILE,"%stemp.wav",path_name);
                }
                else if(pPlayer->media_type==JKMEDIA_TYPE_RECORD_AMR)
                {
                    sprintf(DEFAULT_RECORD_FILE,"%stemp.amr",path_name);
                }
                JWE_LOG(mediaCtrl_set,("record_file=%s",DEFAULT_RECORD_FILE));
				is_location_set = 0;

                if( jas_audio_recorder_open(id) == SCI_FALSE)
                {
                    JWE_LOG(mediaCtrl_set,("jma_audio_recorder_open failed!"))   ;
                    return JKMEDIA_RESULT_ERROR;
                }

                memset(file,0,sizeof(file));

                JcUtf8ToUni((const JKUint8 *)DEST_RECORD_FILE, RECODE_FILE_MAX_LENGTH, (unsigned short *)file, RECODE_FILE_MAX_LENGTH, NULL, NULL, 0);
                if(jas_audio_recorder_set_record_location(id,file)==SCI_FALSE)
                {
                    JWE_LOG(mediaCtrl_set,("%s",DEFAULT_RECORD_FILE));
                    JWE_LOG(mediaCtrl_set,("call jas_audio_recorder_set_record_location failed"));
                }   

            }
            else if(pPlayer->media_type<JKMEDIA_TYPE_RECORD_VIDEO )
            {
                if(pPlayer->media_type==JKMEDIA_TYPE_CAMERA)
                {
                    JWE_LOG(mediaCtrl_set,("JKMEDIA_TYPE_CAMERA not implement"));
                }
                else
                {
		            JWE_LOG(mediaCtrl_set,("JKMEDIA_TYPE_RECORD_VIDEO not implement"));
                }
            }

           
            pPlayer->status = JKMEDIA_STATE_DATA_SET;
            JWE_LOG(mediaCtrl_set,("change state to DATA_SET"))
            return JKMEDIA_RESULT_OK;
        }   /* NO_DATA*/

	}

	
	pPlayer->status = JKMEDIA_STATE_DATA_SET;
	pPlayer->time_low = 0;
	pPlayer->time_high= 0;
	return JKMEDIA_RESULT_OK;
}

/*****************************************************************************/
//  Description : Get current play time..
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/

LOCAL JKSint32 mediaCtrl_no_support_getTime( JKSint32 id, JKUint32* pUsecH, JKUint32* pUsecL ) 
{    
    T_mediaPlayer *pPlayer = getPlayer( id );
    JWE_LOG(mediaCtrl_no_support_getTime,("enter,id=%d",id)) 
    if(JK_NULL != pPlayer)
    {
        *pUsecH = pPlayer->time_high;
        *pUsecL = pPlayer->time_low;      
        return JKMEDIA_RESULT_OK;
    }
    return JKMEDIA_RESULT_ERROR;
}


/*===========================================================================
 *
 *  Event Control Functions
 *
 ==========================================================================*/
static JKSint32 mediaCtrl_Event_getErrorCause(JKSint32 id,JKSint32 type,JKWChar *pStr,JKSint32 length)
{
    T_mediaPlayer *pPlayer=JK_NULL;
    JWE_LOG(mediaCtrl_Event_getErrorCause,("enter,id=%d,type=%d,length=%d",id,type,length))  
    
    pPlayer=getPlayer(id);
    if(pPlayer==JK_NULL ||pStr == JK_NULL)
    {
        JWE_LOG(mediaCtrl_Event_getErrorCause,("null player"))
        return 0;
    }
    if(type==JKMEDIA_EVENT_RECORD_ERROR)
    {   
        if(pPlayer->error_code==3)
         {
            JKWChar msg[]={'3'};
            JWE_LOG(mediaCtrl_Event_getErrorCause,("RECORD_ERROR, DISK FULL"))
            memcpy(pStr,msg,sizeof(msg));
            return sizeof(msg)/sizeof(JKWChar);
        }
        else if(pPlayer->error_code==6)
        {
            JKWChar msg[]={'6'};
            JWE_LOG(mediaCtrl_Event_getErrorCause,("RECORD_ERROR, MMI INTERRUPT"))
            memcpy(pStr,msg,sizeof(msg));
            return sizeof(msg)/sizeof(JKWChar);
        }
        else
        {
            JWE_LOG(mediaCtrl_Event_getErrorCause,("RECORD_ERROR, no error code!"))
            pStr=JK_NULL;//to do something in future
            return 0;
        }
    }
    else
    {
        pStr=JK_NULL;
        return 0;
    }
}

static JKSint32 mediaCtrl_Event_getUserEventType(JKSint32 id,JKSint32 type,JKWChar *pStr,JKSint32 length)
{
    JWE_LOG(mediaCtrl_Event_getUserEventType,("enter"))
    return 0;
}
static JKSint32 mediaCtrl_Event_getUserEventPrmType(JKSint32 id,JKSint32 type)
{
    JWE_LOG(mediaCtrl_Event_getUserEventPrmType,("enter"))
    return JKMEDIA_USEREVENTPRM_VOID;
}
static JKSint32 mediaCtrl_Event_getUserEventPrmInt(JKSint32 id, JKSint32 type)
{
    JWE_LOG(mediaCtrl_Event_getUserEventPrmInt,("enter"))
    return 0;
}
static JKSint32 mediaCtrl_Event_getUserEventPrmStr(JKSint32 id, JKSint32 type,JKWChar *pStr,JKSint32 length)
{
    JWE_LOG(mediaCtrl_Event_getUserEventPrmStr,("enter"))
    return 0;
}

JKT_Media_EventControl mediaCtrl_EventCtrl={
    mediaCtrl_Event_getErrorCause,
    mediaCtrl_Event_getUserEventType,
    mediaCtrl_Event_getUserEventPrmType,
    mediaCtrl_Event_getUserEventPrmInt,
    mediaCtrl_Event_getUserEventPrmStr,
};


JKSint32 mediaCtrl_getIncludedContents( JKSint32 id ) 
{
    JWE_LOG(mediaCtrl_getIncludedContents, ("enter"))
    return JKMEDIA_CONTENT_AUDIO;
}

JKSint32 mediaCtrl_getRecordTime( JKSint32 id, JKUint32* pUsecH, JKUint32* pUsecL ) 
{
    long time = 0;
    T_mediaPlayer* pPlayer = getPlayer( id );
    
    if( pPlayer == JK_NULL ) {
        JWE_LOG(mediaCtrl_getRecordTime,("null player"))
        return JKMEDIA_RESULT_ERROR;
    }
    
    JWE_LOG(mediaCtrl_getRecordTime,("enter,id=%d,mma_type=%d",id,pPlayer->media_type))
 
    if( pPlayer->status < JKMEDIA_STATE_READY ) {   
        JWE_LOG(mediaCtrl_getRecordTime,(" state < READY"))
        mediaCtrl_no_support_getTime(id,pUsecH,pUsecL);
        return JKMEDIA_RESULT_OK;
    }

    if( pPlayer->media_type==JKMEDIA_TYPE_CAMERA){
        JWE_LOG(mediaCtrl_getRecordTime,("media_type=CAMERA"))
        mediaCtrl_no_support_getTime(id,pUsecH,pUsecL);
        return JKMEDIA_RESULT_OK;
    }
    
    if(ajms_arc_getState() == AJSC_VMSTATE_SUSPENDED)
    {
        *pUsecH=0;
        *pUsecL=pPlayer->time;
    }
    

    *pUsecH = 0;
    *pUsecL  = pPlayer->time;
    JWE_LOG(mediaCtrl_getRecordTime,("pPlayer->time = %u",pPlayer->time))

    if( pPlayer->media_type<JKMEDIA_TYPE_RECORD_AUDIO )
    {
        time=jas_audio_recorder_get_record_time(id);
    }

    
    if(time==-1)
    {
        *pUsecH = 0;
        *pUsecL = 0;
    }
    else
    {
        *pUsecH = 0;
        *pUsecL=1000*time;
    }
    
    pPlayer->time=1000 * time;
    
    JWE_LOG(mediaCtrl_getRecordTime,("pUsecL=%u",*pUsecL))
    return JKMEDIA_RESULT_OK;
}

/*===========================================================================
 *
 *  Record Control Functions
 *
 ==========================================================================*/
static JKSint32 mediaOpt_Record_getEncoderCount(void)
{
    JWE_LOG(mediaOpt_Record_getEncoderCount,("enter"))
    return 5;
}
static JKSint32 mediaOpt_Record_getEncoderType(JKSint32 i , JKWChar **pType)
{
    static const JKWChar MIMEType_3gpp[] = {
        'v', 'i', 'd', 'e', 'o', '/', '3','g','p','p',
    };
    static const JKWChar MIMEType_Wave[] = {
        'a', 'u', 'd', 'i', 'o', '/', 'x','-','w','a','v',
    };
    static const JKWChar MIMEType_Amr[] = {
        'a', 'u', 'd', 'i', 'o', '/','a','m','r',
    };
    static const JKWChar MIMEType_Awb[] = {
        'a', 'u', 'd', 'i', 'o', '/','a','m','r','-','w','b',
    };
    static const JKWChar MIMEType_Avi[] = {
        'v', 'i', 'd', 'e', 'o', '/','x','-','m','s','v','i','d','e','o',
    };
    
    JWE_LOG(mediaOpt_Record_getEncoderType,("enter,i=%d",i))

    /* so far i is always 0 */
    if(i==0){
        *pType = ( JKWChar* )MIMEType_Wave;
        return sizeof( MIMEType_Wave ) / sizeof( JKWChar );
    }
    else if(i==1){
        *pType = ( JKWChar* )MIMEType_Amr;
        return sizeof( MIMEType_Amr ) / sizeof( JKWChar );
    }
    else if(i==2){
        *pType = ( JKWChar* )MIMEType_3gpp;
        return sizeof( MIMEType_3gpp ) / sizeof( JKWChar );
    }
    else if(i==3){
        *pType = ( JKWChar* )MIMEType_Awb;
        return sizeof( MIMEType_Awb ) / sizeof( JKWChar );
    }
    else if(i==4){
        *pType = ( JKWChar* )MIMEType_Avi;
        return sizeof( MIMEType_Avi ) / sizeof( JKWChar );
    }
    return NULL;
}

static JKSint32 mediaOpt_Record_setEncodeParam( JKSint32 id,
                             JKT_Media_Param* pParam,
                             JKSint32 paramLen,
                            JKWChar** pType )
{
#if 0
    static const JKWChar MIMEType_3gpp[] = {
        'v', 'i', 'd', 'e', 'o', '/', '3','g','p','p',
    };
#endif
    static const JKWChar MIMEType_Wave[] = {
        'a', 'u', 'd', 'i', 'o', '/', 'x','-','w','a','v',
    };
    static const JKWChar MIMEType_Amr[] = {
        'a', 'u', 'd', 'i', 'o', '/','a','m','r',
    };
#if 0
    static const JKWChar MIMEType_Awb[] = {
        'a', 'u', 'd', 'i', 'o', '/','a','m','r','-','w','b',
    };
    static const JKWChar MIMEType_Avi[] = {
        'v', 'i', 'd', 'e', 'o', '/','x','-','m','s','v','i','d','e','o',
    };
#endif
    
    T_mediaPlayer *pPlayer;
    
    JWE_LOG(mediaOpt_Record_setEncodeParam,("enter id=%d",id))
    
    pPlayer=getPlayer(id);
    if(pPlayer==JK_NULL){
        JWE_LOG(mediaOpt_Record_setEncodeParam,("null player"))
        return 0;
    }

    if(pPlayer->media_type==JKMEDIA_TYPE_RECORD_WAVE){
        JWE_LOG(mediaOpt_Record_setEncodeParam,("Record Type: audio/x-wav"))
        *pType = ( JKWChar* )MIMEType_Wave;
        return sizeof( MIMEType_Wave ) / sizeof( JKWChar );
    }
    else if(pPlayer->media_type==JKMEDIA_TYPE_RECORD_AMR){
        JWE_LOG(mediaOpt_Record_setEncodeParam,("Record Type: audio/amr"))
        *pType = ( JKWChar* )MIMEType_Amr;
        return sizeof( MIMEType_Amr ) / sizeof( JKWChar );
    }
#if 0
    else if(pPlayer->media_type==JKMEDIA_TYPE_RECORD_AWB){
        JWE_LOG(mediaOpt_Record_setEncodeParam,("Record Type: audio/amr-wb"))
        *pType = ( JKWChar* )MIMEType_Awb;
        return sizeof( MIMEType_Awb ) / sizeof( JKWChar );
    }
    else if(pPlayer->media_type==JKMEDIA_TYPE_RECORD_H263){
        JWE_LOG(mediaOpt_Record_setEncodeParam,("Record Type: video/3gpp"))
        *pType = ( JKWChar* )MIMEType_3gpp;
        return sizeof( MIMEType_3gpp ) / sizeof( JKWChar );
    }
    else if(pPlayer->media_type==JKMEDIA_TYPE_RECORD_AVI){
        JWE_LOG(mediaOpt_Record_setEncodeParam,("Record Type: video/x-msvideo"))
        *pType = ( JKWChar* )MIMEType_Avi;
        return sizeof( MIMEType_Avi ) / sizeof( JKWChar );
    }
#endif	
    else{
        JWE_LOG(mediaOpt_Record_setEncodeParam,("Record Type: null"))
        *pType=JK_NULL;
        return 0;
    }
}

/*
 *  return JK_TRUE, means user native system buffer, and those functions will be called :
 *      initBuffer()
 *      getBufferCount()
 *      getBuffer()
 *
 *  return JK_FALSE ,means user JBlend JAVA_HEAP as buffer , those functions will be called:
 *      getBufferUnitSize()
 *      addBuffer()
 */
static JKBool mediaOpt_Record_usePlatformBuffer( JKSint32 id )
{
    JWE_LOG(mediaOpt_Record_usePlatformBuffer,("enter,id=%d",id))
    return JK_TRUE;
}
static JKSint32 mediaOpt_Record_getBufferUnitSize(JKSint32 id)
{
    JWE_LOG(mediaOpt_Record_getBufferUnitSize,("enter,id=%d",id))
    return 0;

}
static JKSint32 mediaOpt_Record_addBuffer( JKSint32 id, JKUint8* pBuffer, JKSint32 size )
{
    JWE_LOG(mediaOpt_Record_addBuffer,("enter,id=%d",id))
    pBuffer=NULL;
    return JKMEDIA_RESULT_OK;
}


static JKSint32 mediaOpt_Record_setBufferLimit( JKSint32 id, JKSint32 size )
{
    T_mediaPlayer *pPlayer=JK_NULL;
    JWE_LOG(mediaOpt_Record_setBufferLimit,("enter,id=%d,size=%d",id,size))
    pPlayer=getPlayer(id);
    if(pPlayer==JK_NULL){
        JWE_LOG(mediaOpt_Record_setBufferLimit,("null player"))
        return JKMEDIA_RESULT_ERROR;
    }
    
    if(size<=0){
        JWE_LOG(mediaOpt_Record_setBufferLimit, ("IllegalArgument Exception! size=%d",size))
        return JKMEDIA_RESULT_ERROR;
    }   
    pPlayer->record_size=size;

    return JKMEDIA_RESULT_OK;
}


static JKSint32 mediaOpt_Record_initBuffer( JKSint32 id )
{
    T_mediaPlayer *pPlayer=JK_NULL;
    //int result; 
    wchar file[RECODE_FILE_MAX_LENGTH+1];   

    JWE_LOG(mediaOpt_Record_initBuffer,("enter,id=%d",id))
  
    pPlayer=getPlayer(id);
    if(pPlayer==JK_NULL){
        JWE_LOG(mediaOpt_Record_initBuffer,("null player"))
        return JKMEDIA_RESULT_ERROR;
    }
    JWE_LOG(mediaOpt_Record_initBuffer,("enter,state=%d",pPlayer->status))

    memset(file,0,sizeof(file));

    if(pPlayer->status == JKMEDIA_STATE_STARTED || pPlayer->status == JKMEDIA_STATE_PAUSED){
        if( pPlayer->media_type>JKMEDIA_TYPE_DEVICE_BASE && pPlayer->media_type<JKMEDIA_TYPE_RECORD_AUDIO){
            JWE_LOG(mediaOpt_Record_initBuffer,("Try to reset audio Record buffer"))
            if(jas_audio_recorder_reset(id) == SCI_FALSE){
                JWE_LOG(mediaOpt_Record_initBuffer,("Reset Record fail")) 
            }

            if(is_location_set){
                JcUtf8ToUni((const JKUint8 *)DEST_RECORD_FILE, strlen(DEST_RECORD_FILE), file, RECODE_FILE_MAX_LENGTH, NULL, NULL, 0);
            }
            else{
                JcUtf8ToUni((const JKUint8 *)DEFAULT_RECORD_FILE, strlen(DEFAULT_RECORD_FILE), file, RECODE_FILE_MAX_LENGTH, NULL, NULL, 0);
            }
            if(jas_audio_recorder_set_record_location(id,file)==SCI_FALSE){
                JWE_LOG(mediaOpt_Record_initBuffer,("%s",DEFAULT_RECORD_FILE));
                JWE_LOG(mediaOpt_Record_initBuffer,("call jma_audio_recorder_set_record_location failed"));
                if(jas_audio_recorder_close(id)==SCI_FALSE){
                    JWE_LOG(mediaOpt_Record_initBuffer,("call jma_audio_recorder_clsoe failed!"))   
                }
                return JKMEDIA_RESULT_ERROR;
            }   

        }

            if(is_location_set){
                JWE_LOG(mediaOpt_Record_initBuffer,("DEST_RECORD_FILE is %s",DEST_RECORD_FILE))
                JcUtf8ToUni((const JKUint8 *)DEST_RECORD_FILE, strlen(DEST_RECORD_FILE), file, RECODE_FILE_MAX_LENGTH, NULL, NULL, 0);
            }
            else{
                JWE_LOG(mediaOpt_Record_initBuffer,("DEFAULT_RECORD_FILE is %s",DEFAULT_RECORD_FILE))
                JcUtf8ToUni((const JKUint8 *)DEFAULT_RECORD_FILE, strlen(DEFAULT_RECORD_FILE), file, RECODE_FILE_MAX_LENGTH, NULL, NULL, 0);
            }
               
            pPlayer->is_previewing = 0;
        }

    
        return JKMEDIA_RESULT_OK;
}

/*
 *  this returns the number of buffers
 */
static JKSint32 mediaOpt_Record_getBufferCount( JKSint32 id )
{
    if(is_location_set){
        JWE_LOG(mediaOpt_Record_getBufferCount,("enter,id=%d,buffer_count=%d",id,m_record_buff_count))
	 return 0;
    }
    m_record_buff_count = m_record_size/BUFFER_LENGTH + 1 ;
    JWE_LOG(mediaOpt_Record_getBufferCount,("enter,id=%d,buffer_count=%d",id,m_record_buff_count))
    return m_record_buff_count;
}

static JKUint8* mediaOpt_Record_getBuffer( JKSint32 id, JKSint32 count, JKSint32* pLength )
{
    int err;
    int retval;

    JWE_LOG(mediaOpt_Record_getBuffer,("enter,id=%d,count=%d",id,count))

    if(is_location_set){
        JWE_LOG(mediaOpt_Record_getBufferCount,("enter,id=%d,buffer_count=%d",id,m_record_buff_count))
	 return NULL;
    }

    if ( !m_open_buff ) {
        if( m_record_buff == NULL ){
            m_record_buff = (JKUint8 *)SCI_ALLOCA( BUFFER_LENGTH );
            if ( m_record_buff == NULL ) {
                JWE_LOG(mediaOpt_Record_getBuffer, ("jvm_malloc Fail."))
                return NULL;
            }
        }

        m_file_handle = kjava_fs_open(DEFAULT_RECORD_FILE,KJAVA_FILE_OPEN_READ,0,&err);
        if ( m_file_handle == NULL ) {
            JWE_LOG(mediaOpt_Record_getBuffer, ("kjava_fs_open Fail. retval = %d", err))
            return NULL;
        }

        m_open_buff = 1;
    }

    retval = kjava_fs_read( m_record_buff, BUFFER_LENGTH, (signed long *)pLength, m_file_handle, 0 );
    if ( retval == KJAVA_FILE_ERR_IO_ERR ) {
        JWE_LOG(mediaOpt_Record_getBuffer,("kjava_fs_read Fail. (KJAVA_FILE_IO_ERR)"))
        return NULL;
    }

    if ( retval == KJAVA_FILE_ERR_END_OF_FILE ) {
        if ( (count+1) != m_record_buff_count ) {
            JWE_LOG(mediaOpt_Record_getBuffer,("buffer count not consist!"))
        //XXX ???
        }
    }

    return m_record_buff;
}

static JKSint32 mediaOpt_Record_startRecord( JKSint32 id )
{
    T_mediaPlayer *pPlayer=JK_NULL;
    JWE_LOG(mediaOpt_Record_startRecord,("enter, id=%d",id))
    pPlayer=getPlayer(id);
    if(pPlayer==JK_NULL){
        JWE_LOG(mediaOpt_Record_startRecord,("null player"))
        return JKMEDIA_RESULT_ERROR;
    }

    if(pPlayer->media_type<JKMEDIA_TYPE_RECORD_AUDIO){
        if (jas_audio_recorder_start_record(id) == SCI_FALSE){
            JWE_LOG(mediaOpt_Record_startRecord,("start failed"))
            return JKMEDIA_RESULT_ERROR;
        }
    }

    
    /* set record size limit */
    //比较的类型不匹配！！！！
    //if(pPlayer->record_size!=JKMEDIA_TIME_MAX_LOW){
    JWE_LOG(mediaOpt_Record_startRecord,("set record size limit!"))
    if(pPlayer->media_type<JKMEDIA_TYPE_RECORD_AUDIO){
        if(jas_audio_recorder_set_record_size_limit(id,pPlayer->record_size)==0){
            JWE_LOG(mediaOpt_Record_startRecord,("set size limit error!"))
            return JKMEDIA_RESULT_ERROR;
        }
    }
    //}
    pPlayer->is_recording=1;
    return JKMEDIA_RESULT_OK;
}

static JKSint32 mediaOpt_Record_stopRecord( JKSint32 id )
{
    T_mediaPlayer *pPlayer=JK_NULL;
    JWE_LOG(mediaOpt_Record_stopRecord,("enter,id=%d",id))
    
    pPlayer=getPlayer(id);
    if(pPlayer==JK_NULL){
        JWE_LOG(mediaOpt_Record_stopRecord,("null player"))
        return JKMEDIA_RESULT_ERROR;
    }
    

    if(pPlayer->media_type<JKMEDIA_TYPE_RECORD_AUDIO){
        jas_audio_recorder_pause_record(id);
    }


    return JKMEDIA_RESULT_OK;
}

static JKSint32 mediaOpt_Record_getState( JKSint32 id )
{
    T_mediaPlayer *pPlayer=JK_NULL;
    JWE_LOG(mediaOpt_Record_getState,("enter,id=%d",id))
    pPlayer=getPlayer(id);
    if(pPlayer==JK_NULL){
        JWE_LOG(mediaOpt_Record_getState,("NULL player"))
        return JKMEDIA_RECSTATE_ERROR;
    }

    if (pPlayer->is_recording){
        JWE_LOG(mediaOpt_Record_getState,("JKMEDIA_RECSTATE_RECORDING"))
        return JKMEDIA_RECSTATE_RECORDING;
    }
    else{
        JWE_LOG(mediaOpt_Record_getState,("JKMEDIA_RECSTATE_READY"))
        return JKMEDIA_RECSTATE_READY;
    }
}

static JKSint32 mediaOpt_Record_getRecordedSize( JKSint32 id )
{
    KJavaStat stat;
    int result;
    JWE_LOG(mediaOpt_Record_getRecordedSize,("enter,id=%d",id))

    if(is_location_set == 1){
        JWE_LOG(mediaOpt_Record_getRecordedSize,("DEST_RECORD_FILE is %s",DEST_RECORD_FILE))
        result=kjava_fs_stat2(DEST_RECORD_FILE,&stat);
    }
    else
    {
        JWE_LOG(mediaOpt_Record_getRecordedSize,("DEFAULT_RECORD_FILE is %s",DEFAULT_RECORD_FILE))
        result=kjava_fs_stat2(DEFAULT_RECORD_FILE,&stat);
    }
    if(result!=KJAVA_FILE_ERR_NONE){
        JWE_LOG(mediaOpt_Record_getRecordedSize,("fs_stat2 error!!!!!!!, result=%d",result))
        return 0;
    }
    JWE_LOG(mediaOpt_Record_getRecordedSize,("return size %d",stat.size.low))
    m_record_size = (JKSint32)stat.size.low;
    return m_record_size;
}

static JKSint32 mediaOpt_Record_complete( JKSint32 id )
{
    T_mediaPlayer *pPlayer=JK_NULL;
    int result;

    JWE_LOG(mediaOpt_Record_complete,("enter,id=%d",id))
    pPlayer=getPlayer(id);
    if(pPlayer==JK_NULL){
        JWE_LOG(mediaOpt_Record_complete,("null player"))
        return JKMEDIA_RESULT_ERROR;
    }
    
    if(pPlayer->media_type<JKMEDIA_TYPE_RECORD_AUDIO){
        if(jas_audio_recorder_commit(id) == SCI_FALSE){
            JWE_LOG(mediaOpt_Record_complete,("jma_audio_recorder_commit failed!"))
            return JKMEDIA_RESULT_ERROR;
        }
    }
    else if(pPlayer->media_type<JKMEDIA_TYPE_RECORD_VIDEO){
    if( (result=kjava_fs_remove( DEST_RECORD_FILE )) == KJAVA_FILE_ERR_NOT_EXIST){
        JWE_LOG(mediaOpt_Record_complete , ("file not existed"))
    }
    else if(result==KJAVA_FILE_ERR_IO_ERR){
        JWE_LOG(mediaOpt_Record_complete,("remove file error, IO error"))
    }

#ifdef JBLENDIA_SUPPORT_BACKGROUND_VM
        if(ajms_arc_getState()!=AJSC_VMSTATE_STARTED){
            if(ajms_arc_getState()!=AJSC_VMSTATE_FOREGROUND){
                JWE_LOG(mediaCtrl_Extension_Stream_checkOpen,(" VM is suspending now"))
                return JKMEDIA_RESULT_ERROR;
            }
        }
#else
        if(ajms_arc_getState()!=AJSC_VMSTATE_STARTED){
            JWE_LOG(mediaCtrl_Extension_Stream_checkOpen,(" VM is suspending now"))
            return JKMEDIA_RESULT_ERROR;
        }
#endif

        JWE_LOG(mediaOpt_Record_complete,("jma_video_recorder_record_merged returns true"))

        pPlayer->is_previewing=1;
    }
    
    return JKMEDIA_RESULT_OK;
}

static JKSint32 mediaOpt_Record_discard( JKSint32 id )
{
    T_mediaPlayer *pPlayer=JK_NULL;
    int result;

    JWE_LOG(mediaOpt_Record_discard,("enter,id=%d",id))
    pPlayer=getPlayer(id);
    if(pPlayer==JK_NULL){
        JWE_LOG(mediaOpt_Record_discard,("null player"))
        return JKMEDIA_RESULT_ERROR;
    }
    pPlayer->is_recording=0;
    if ( m_record_buff ) {
        SCI_FREE(m_record_buff);
        m_record_buff = NULL;   // merlin
    }
    if ( m_file_handle ) {
        kjava_fs_close(m_file_handle, 0);
        m_file_handle = NULL;   // merlin
    }

    m_record_size = 0;
    m_record_buff_count = 0;
    m_open_buff = 0;

    if(pPlayer->media_type >= JKMEDIA_TYPE_RECORD_WAVE  && pPlayer->media_type < JKMEDIA_TYPE_RECORD_VIDEO){
        if( (result=kjava_fs_remove( DEFAULT_RECORD_FILE )) == KJAVA_FILE_ERR_NOT_EXIST){
            JWE_LOG(mediaOpt_Record_discard , ("remove DEFAULT_RECORD_FILE file error , file not existed"))
        }
        else if(result==KJAVA_FILE_ERR_IO_ERR){
            JWE_LOG(mediaOpt_Record_discard,("DEFAULT_RECORD_FILE file error, IO error"))
        }
    }

    return JKMEDIA_RESULT_OK;
}


/*************************************************************************
*                                                             
*  Function name:   mediaOpt_Record_setLocation
*                                                             
*  Author: Lo.G          Version: 1.0             Date: 2008-3-5
*                                                             
*  Description:
*              1.                                                          
*              2.                                                          
*  Input:      
*              1.   JKSint32 id 
*              2. JKWChar* locator
*  return:    
*  set the locator ,if the directory did not exist return -1            
*  if success return 1 else return -1                                                          
**************************************************************************/
static JKSint32 mediaOpt_Record_setLocation( JKSint32 id ,JKWChar* locator)
{
    int pathLen;
    T_mediaPlayer *pPlayer;
    char filePath[RECODE_FILE_MAX_LENGTH+1];
    wchar wFilePath[RECODE_FILE_MAX_LENGTH+1];  
    int result;
    int extLen=0;
    int dest_pathlen = 0;

    if(locator==NULL){
        return -1;
    }

    memset(filePath,0x00,RECODE_FILE_MAX_LENGTH);
    pathLen=MMIAPICOM_Wstrlen(locator);
    JcUniToUtf8( locator, pathLen+1, (unsigned char*)filePath, RECODE_FILE_MAX_LENGTH, NULL);

    pathLen=strlen(filePath);

    memset(DEST_RECORD_FILE,0x00,RECODE_FILE_MAX_LENGTH);
    memcpy(DEST_RECORD_FILE,(const void *)filePath,RECODE_FILE_MAX_LENGTH);

    if(DEST_RECORD_FILE[0] > 'a' && DEST_RECORD_FILE[0] < 'z'){
        DEST_RECORD_FILE[0] = DEST_RECORD_FILE[0] - 0x20;
    }

    while(*( filePath+pathLen-1-extLen)!='.'){
        extLen++; 
        if(extLen>6||extLen==pathLen){
            return JKMEDIA_RESULT_ERROR;
        }
    }

    while(*( filePath+pathLen-1-dest_pathlen)!='\\'){
        dest_pathlen++; 
        if(dest_pathlen==pathLen){
            return JKMEDIA_RESULT_ERROR;
        }
    }

    memset(wFilePath,0,sizeof(wFilePath));
    JcUtf8ToUni((const JKUint8 *)DEST_RECORD_FILE,  strlen(DEST_RECORD_FILE), wFilePath, RECODE_FILE_MAX_LENGTH, NULL, NULL, 0);

    JWE_LOG(mediaOpt_Record_setLocation,("enter,id=%d,Locator=%s,tempfile:%s",id,filePath,DEST_RECORD_FILE));
    pPlayer = getPlayer( id );   
    if( (result=kjava_fs_remove( DEST_RECORD_FILE )) == KJAVA_FILE_ERR_NOT_EXIST){
        JWE_LOG(mediaOpt_Record_setLocation , ("file not existed"))
    }
    else if(result==KJAVA_FILE_ERR_IO_ERR){
        JWE_LOG(mediaOpt_Record_setLocation,("remove file error, IO error"))
    }
    if(JK_NULL == pPlayer)
    {
        return JKMEDIA_RESULT_ERROR;
    }           
    if( (pPlayer->media_type<JKMEDIA_TYPE_RECORD_AUDIO)){
        if(jas_audio_recorder_set_record_location(id,wFilePath)==SCI_FALSE){
            JWE_LOG(mediaOpt_Record_setLocation,("call jma_audio_recorder_set_record_location failed"));
            return JKMEDIA_RESULT_ERROR;
        }
    }
 
    is_location_set =1;
    return 1;
}


/*===========================================================================
 *
 *  Input Device Control
 *
 ==========================================================================*/
static JKSint32 recordAmrOpt_InputDevice_getMIMEType(JKWChar** pType)
{
    static const JKWChar MIMEType[] = {
        'a', 'u', 'd', 'i', 'o', '/', 'a','m','r',
    };
    
    JWE_LOG(recordAmrOpt_InputDevice_getMIMEType,("enter"))
    *pType = ( JKWChar* )MIMEType;

    return sizeof( MIMEType ) / sizeof( JKWChar );
}
static JKSint32 recordAmrOpt_InputDevice_setParam( JKT_Media_Param* pParam, JKSint32 paramLen )
{
    JWE_LOG(recordAmrOpt_InputDevice_setParam,("enter paramLen=%d",paramLen))
    return JKMEDIA_RESULT_OK;
}
static JKSint32 recordAmrOpt_InputDevice_open( void )
{
    JWE_LOG(recordAmrOpt_InputDevice_open,("enter"))
    /*
     *  jma_audio_recorder_open needs java_recorder id as arg
     *  postpone it until load() is called
     */
    return JKMEDIA_RESULT_OK;
}
static JKSint32 recordAmrOpt_InputDevice_close( void )
{
    JWE_LOG(recordAmrOpt_InputDevice_close,("enter"))
    return JKMEDIA_RESULT_OK;
}

JKT_Media_RecordControl mediaOpt_RecordCtrl = {
    mediaOpt_Record_getEncoderCount,   /* getEncoderCount(void) */
    mediaOpt_Record_getEncoderType,    /* getEncoderType(JKSint32 i,JKWChar **pType) */
    mediaOpt_Record_setEncodeParam,    /* setEncodeParam(JKSint32 id,JKT_Media_Param *pParam,JKSint32 paramLen,JKWChar **pType) */
    mediaOpt_Record_usePlatformBuffer, /* usePlatformBuffer(JKSint32 id) */
    mediaOpt_Record_getBufferUnitSize, /* getBufferUnitSize(JKSint32 id) */
    mediaOpt_Record_addBuffer,         /* addBuffer(JKSint32 id,JKUint8 *pBuffer,JKSint32 size) */
    mediaOpt_Record_setBufferLimit,    /* setBufferLimit(JKSint32 id,JKSint32 size) */
    mediaOpt_Record_initBuffer,        /* initBuffer(JKSint32 id) */
    mediaOpt_Record_getBufferCount,    /* getBufferCount(JKSint32 id) */
    mediaOpt_Record_getBuffer,         /* getBuffer(JKSint32 id,JKSint32 count,JKSint32 *pLengtg) */
    mediaOpt_Record_startRecord,       /* startRecord(JKSint32 id) */
    mediaOpt_Record_stopRecord,        /* stopRecord(JKSint32 id) */
    mediaOpt_Record_getState,          /* getState(JKSint32 id) */
    mediaOpt_Record_getRecordedSize,   /* getRecordedSize(JKSint32 id) */
    mediaOpt_Record_complete,          /* complete(JKSint32 id) */
    mediaOpt_Record_discard,           /* discard(JKSint32 id) */
 //   mediaOpt_Record_setLocation /* JKSint32    setRecordLocation (JKSint32 id ,JKWChar* locator); */
};
static const JKT_Media_InputDeviceControl recordAmrOpt_InputDeviceCtrl = {
    recordAmrOpt_InputDevice_getMIMEType, /* JKSint32 ( *getMIMEType )( JKWChar** pType ) */
    recordAmrOpt_InputDevice_setParam,    /* JKSint32 ( *setParam )( JKT_Media_Param* pParam, JKSint32 paramLen ) */
    recordAmrOpt_InputDevice_open,	    /* JKSint32 ( *open )( void ); */
    recordAmrOpt_InputDevice_close	    /* JKSint32 ( *close )( void ); */
};


/****************************************************************************
 *
 *    Optional Control Function
 *
 ****************************************************************************/
/**
 * Structure for option control functions
 */

static const JKT_MediaOption recordAmrCtrl_OptionCtrl ={
    NULL,				/* JKT_Media_TempoControl const * pTempoFuncs; */
    NULL,				/* JKT_Media_SpeedControl const * PSpeedFuncs; */
    NULL,				/* JKT_Media_StopControl const * pStopFuncs;   */
    NULL,				/* JKT_Media_PitchControl const * pPitchFuncs; */
    NULL,				/* JKT_Media_MeteDataControl const * pMetaDataFuncs; */
    NULL,				/* JKT_Media_MIDIControl const * pMIDIFuncs;   */
    NULL,				/* JKT_Media_SyncControl const * pSyncFuncs;   */
    NULL,				/* JKT_Media_VideoControl const * pVideoFuncs; */
    NULL,				/* JKT_Media_FrameControl const * pFrameFuncs; */
    NULL,				/* void const * pSnapShotFuncs;*/
    &recordAmrOpt_InputDeviceCtrl,    /* void const * pInputDeviceFuncs;         */
    &mediaOpt_RecordCtrl,		/* void const * pRecordFuncs;          */
    NULL,				/* void const * pExtFuncs;             */
};
/**
 * Structure for SMF playback control functions
 */

const JKT_MediaControl recordAmrControl = {
    JKMEDIA_TYPE_RECORD_AMR,		/* JKSint32 mediaType */
    recordAmrCtrl_getMIMEType,		/* JKSint32 ( *getMIMEType )( JKWChar** pType ); */
    recordAmrCtrl_isData,		    /* JKBool ( *isData )( const JKUint8* pData, const JKSint32 length ); */
    recordAmrCtrl_checkOpen,		/* JKSint32 ( *checkOpen )( const JKUint8* pData, const JKSint32 length ); */
    mediaCtrl_check,			    /* JKSint32 ( *check )( JKSint32 id, JKSint32* pExtraID ); */
    mediaCtrl_checkClose,		    /* JKSint32 ( *checkClose )( JKSint32 id ); */
    mediaCtrl_getMediaSize,		    /* JKSint32 ( *getMediaSize )( JKSint32 id, JKSint32* pWidth, JKSint32* pHeight ); */
    recordAmrCtrl_create,		    /* JKSint32 ( *create )( void ); */
    mediaCtrl_destroy,			    /* JKSint32 ( *destroy )( JKSint32 id ); */
    recordAmrCtrl_set,			/* JKSint32 ( *set )( JKSint32 id, const JKUint8* pData, const JKSint32 length, JKSint32 extraID ); */
    mediaCtrl_load,			        /* JKSint32 ( *load )( JKSint32 id ); */
    mediaCtrl_start,			    /* JKSint32 ( *start )( JKSint32 id, JKSint32 loop ); */
    mediaCtrl_stop,			        /* JKSint32 ( *stop )( JKSint32 id ); */
    mediaCtrl_pause,			    /* JKSint32 ( *pause )( JKSint32 id ); */
    mediaCtrl_restart,			    /* JKSint32 ( *restart )( JKSint32 id ); */
    mediaCtrl_unload,			    /* JKSint32 ( *unload )( JKSint32 id ); */
    mediaCtrl_getState,			    /* JKSint32 ( *getState )( JKSint32 id ); */
    mediaCtrl_getDuration,	        /* JKSint32 ( *getDuration )( JKSint32 id, JKUint32* pUsecH, JKUint32* pUsecL ); */
    mediaCtrl_setTime,	            /* JKSint32 ( *setTime )( JKSint32 id, JKUint32 usecH, JKUint32 usecL ); */
    mediaCtrl_getRecordTime,		/* JKSint32 ( *getTime )( JKSint32 id, JKUint32* pUsecH, JKUint32* pUsecL ); */
    mediaCtrl_getIncludedContents,	/* JKSint32 ( *getIncludedContents )( JKSint32 id ); */
    mediaCtrl_suspendVM,		    /* JKSint32 ( *suspend )( JKSint32 id ); */
    mediaCtrl_resumeVM,			    /* JKSint32 ( *resume )( JKSint32 id ); */

    &mediaCtrl_EventCtrl,		    /* JKT_Media_EventControl */
    JK_NULL,				        /* JKT_Media_VolumeControl */
    JK_NULL,				        /* JKT_Media_UserControl */
    JK_NULL,				        /* void const* pExtFuncs */
    &recordAmrCtrl_OptionCtrl,      /* void const* pOptionFuncs */
};

#endif	/* SUPPORT_MMAPI */
