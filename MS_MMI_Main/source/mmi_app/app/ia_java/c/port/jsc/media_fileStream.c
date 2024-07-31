
/*-----------------------------------------------------------------------------
 * Include Files
 *----------------------------------------------------------------------------*/


#include "jkmediastream.h"		
#include "jkmediacontrol.h"
#include "jwe_log.h"
#include "mediacontrol.h"		


/*-----------------------------------------------------------------------------
 * Local functions
 *---------------------------------------------------------------------------*/
 
/* this function will remove the "file:///" prefix and convert '/' to '\' */
static void wchar_convert_path(JKWChar *src,JKWChar *dst,int length)
{
    int i,j=0;
    for(i=8,j=0;i<length;i++,j++){
        if(src[i]==L'/'){
//            dst[j++]=L'\\';
            dst[j]=L'\\';
        }
        else{
            dst[j]=src[i];
        }
    }
    dst[j]=0x0;
}

/*-----------------------------------------------------------------------------
 * Global functions
 *----------------------------------------------------------------------------*/
static JKSint32 fileStream_getProtocolName(JKWChar **pName)
{
    static const JKWChar name[] = { 'f', 'i', 'l', 'e' };
    JWE_LOG(fileStream_getProtocolName,("enter"))
    *pName = ( JKWChar* )name;
    return sizeof(name)/sizeof(JKWChar);
}

/*  in JKT_MediaStream.create()
 *  we return a reader instance id
 *  this is different from JKT_MediaControl.create()
 */
static JKSint32 fileStream_create(JKWChar *pLocation,JKSint32 length)
{
   // char file[MAX_MEDIA_FILE_NAME*2];
    StreamPlayer *pStreamPlayer=JK_NULL;
    JKWChar * location;

    JWE_LOG(fileStream_create,("enter pLocation length=%d",length));

    location= SCI_ALLOC_APP((length -8 + 1)*sizeof(JKWChar));/*lint !e737*/
    if(location==NULL)
    {
	    JWE_LOG(fileStream_create,("location alloc failed!return -1"));
	    return -1;
    }

    /* this function removes 'file:///' ,and translate '/' into '\' */
    wchar_convert_path(pLocation,location,length);
    
    pStreamPlayer=createStreamPlayer(location);

    SCI_FREE(location);
	 
    if(pStreamPlayer==JK_NULL){
	    JWE_LOG(fileStream_create,("no stream player available"));
	    return -1; /* return negative value means error */
    }
    pStreamPlayer->stream_type=FILE_STREAM;
    JWE_LOG(fileStream_create,("return id=%d",pStreamPlayer->id));
    return pStreamPlayer->id;    
}

static JKSint32 fileStream_connect(JKSint32 streamID)
{
    /* since JBlend didnt have API for disconnect
     * the connection is connected after JKT_MediaStream.setStream*/
    return JKMEDIA_RESULT_OK;
}
static JKSint32 fileStream_getPermission(JKSint32 streamID,JKWChar *pBuffer,JKSint32 length)
{
    /* L"javax.microedition.Connector.file.read" */
    const JKWChar namePermissionFileStream[] =
    {   'j', 'a', 'v', 'a', 'x', '.', 'm', 'i', 'c', 'r',
        'o', 'e', 'd', 'i', 't', 'i', 'o', 'n', '.', 'i',
        'o', '.', 'C', 'o', 'n', 'n', 'e', 'c', 't', 'o',
        'r', '.', 'f', 'i', 'l', 'e', '.', 'r', 'e', 'a', 'd' };
	
    JWE_LOG(fileStream_getPermission,("enter,streamID=%d,length=%d,strlen()=%d",streamID,length,sizeof(namePermissionFileStream)/sizeof(JKWChar)))
    
    memcpy(pBuffer,namePermissionFileStream,sizeof(namePermissionFileStream));
    //XXX ,should check permission here
    return sizeof(namePermissionFileStream)/sizeof(JKWChar); /* means no need to check permission so far */
}

static JKSint32 fileStream_getMimeType(JKSint32 streamID,JKWChar *pBuffer,JKSint32 length)
{
    StreamPlayer *pStreamPlayer=JK_NULL;

    static const JKWChar midi_mimeType[]={
	    'a', 'u', 'd', 'i', 'o', '/', 'm', 'i', 'd', 'i',
    };
    static const JKWChar spmidi_mimeType[] = {
        'a', 'u', 'd', 'i', 'o', '/', 's','p','-','m', 'i', 'd', 'i'
    };
    static const JKWChar tone_seq_mimeType[] = {
        'a', 'u', 'd', 'i', 'o', '/', 'x', '-', 't' ,'o', 'n', 'e', '-', 's' ,'e' ,'q' 
    };
    
    static const JKWChar wave_mimeType[]={
	    'a', 'u', 'd', 'i', 'o', '/', 'x', '-', 'w', 'a', 'v',
    };
//    static const JKWChar imy_mimeType[] = {
//        'a', 'u', 'd', 'i', 'o', '/', 'i', 'm', 'e' ,'l' ,'o','d','y',   
//    };
    static const JKWChar amr_mimeType[] = {
        'a', 'u', 'd', 'i', 'o', '/', 'a', 'm' , 'r',
    };
//    static const JKWChar amb_mimeType[] = {
//        'a', 'u', 'd', 'i', 'o', '/', 'a', 'm' , 'r', '-' , 'w', 'b'
//    };
    static const JKWChar aac_mimeType[] = {
        'a', 'u', 'd', 'i', 'o', '/', 'a', 'a' , 'c',
    };
  //   static const JKWChar au_mimeType[] = {
  //       'a', 'u', 'd', 'i', 'o', '/', 'b', 'a' , 's', 'i' , 'c',
  //   };
    static const JKWChar mp3_mimeType[]={
	    'a', 'u', 'd', 'i', 'o', '/', 'm', 'p', 'e', 'g'
    };
//    static const JKWChar wma_mimeType[]={
//       'a', 'u', 'd', 'i', 'o', '/', 'w', 'm', 'a',
//    };

    JWE_LOG(fileStream_getMimeType,("enter,streanID=%d,length=%d",streamID,length));
    pStreamPlayer=getStreamPlayer(streamID);
    if(pStreamPlayer==JK_NULL){
	    JWE_LOG(fileStream_getMimeType,("null player"));
	    return 0;
    }

    if(pStreamPlayer->media_type==JKMEDIA_TYPE_MIDI){
	    JWE_LOG(fileStream_getMimeType,("mime type :audio/midi"));
	    SCI_MEMCPY(pBuffer,midi_mimeType,  10 * sizeof( JKWChar ) );
	    return sizeof( midi_mimeType ) / sizeof( JKWChar );
    }
    else if(pStreamPlayer->media_type==JKMEDIA_TYPE_SPMIDI){
	    JWE_LOG(fileStream_getMimeType,("mime type :audio/sp-midi"));
	    SCI_MEMCPY(pBuffer,spmidi_mimeType,  13 * sizeof( JKWChar ) );
	    return sizeof( spmidi_mimeType ) / sizeof( JKWChar );
    }        
    else if(pStreamPlayer->media_type==JKMEDIA_TYPE_TONE){
	    JWE_LOG(fileStream_getMimeType,("mime type :audio/x-tone-seq"));
	    SCI_MEMCPY(pBuffer,tone_seq_mimeType,  16 * sizeof( JKWChar ) );
	    return sizeof( tone_seq_mimeType ) / sizeof( JKWChar );
    }
    else if(pStreamPlayer->media_type==JKMEDIA_TYPE_WAVE){
	    JWE_LOG(fileStream_getMimeType,("mime type :audio/x-wav"));
	    SCI_MEMCPY(pBuffer,wave_mimeType,11 * sizeof( JKWChar ) );
	    return sizeof( wave_mimeType ) / sizeof( JKWChar );
    }
    else if(pStreamPlayer->media_type==JKMEDIA_TYPE_AMR){
	    JWE_LOG(fileStream_getMimeType,("mime type :audio/amr"));
	    SCI_MEMCPY(pBuffer,amr_mimeType,9 * sizeof( JKWChar ) );
	    return sizeof( amr_mimeType ) / sizeof( JKWChar );
    }
    else if(pStreamPlayer->media_type==JKMEDIA_TYPE_MP3){
	    JWE_LOG(fileStream_getMimeType,("mime type :audio/mpeg"));
	    SCI_MEMCPY(pBuffer,mp3_mimeType, 10 * sizeof( JKWChar ) );
	    return sizeof( mp3_mimeType ) / sizeof( JKWChar );
    }
    else if(pStreamPlayer->media_type==JKMEDIA_TYPE_AAC){
	    JWE_LOG(fileStream_getMimeType,("mime type :audio/aac"));
	    SCI_MEMCPY(pBuffer,aac_mimeType,9 * sizeof( JKWChar ) );
	    return sizeof( aac_mimeType ) / sizeof( JKWChar );
    }
    else {
	    JWE_LOG(fileStream_getMimeType,(" non-supported mime type,use midi as default"));
	    SCI_MEMCPY(pBuffer,midi_mimeType,  10 * sizeof( JKWChar ) );
	    return sizeof( midi_mimeType ) / sizeof( JKWChar );
    }
    
}



static JKSint32 fileStream_destroy(JKSint32 streamID)
{   
    JWE_LOG(fileStream_destroy,("enter,streamID=%d",streamID))
    return destroyStreamPlayer(streamID);
}


JKT_MediaStream fileStream={
    fileStream_getProtocolName,		/* getProtocolName(JKWChar **pName) */
    fileStream_create,			/* create(JKWChar *pLocation,JKSint32 length) */
    fileStream_connect,			/* connect(JKSint32 streamID) */
    fileStream_getPermission,		/* getPermission(JKSint32 streamID, JKWChar *pBuffer,JKSint32 length) */
    fileStream_getMimeType,		/* getMimeType(JKSint32 streamID, JKWChar *pBuffer, JKSint32 length) */
    fileStream_destroy,			/* destroy(JKSint32 streamID) */
};
	
