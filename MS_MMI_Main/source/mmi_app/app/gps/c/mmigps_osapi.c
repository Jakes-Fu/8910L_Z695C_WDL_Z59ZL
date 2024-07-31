/****************************************************************************
** File Name:      mmigps_osapi.c                                         *
** Author:         jian.ma                                                 *
** Date:           07/19/2007                                              *
** Copyright:      2005 Spreadtrum, Incorporated. All Rights Reserved.     *
** Description:    This file is used to describe the cryp display menu     *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 07/19/2007    jian.ma          Create                                   *
**                                                                         *
****************************************************************************/


/**---------------------------------------------------------------------------*
**                         DEPENDENCIES                                      *
**---------------------------------------------------------------------------*/
#include "mmi_app_gps_trc.h"
#include "std_header.h"
#include "sci_types.h"
#ifdef GPS_SUPPORT
#include "mmigps_osapi.h"
#include "gps_drv.h"
#include "audio_api.h"
#include "audio_config.h"
#include "pcm_adp.h"
#include <stdarg.h>
//#include "audio_config.h"
//#include "sig_code.h"
#include "block_mem.h"
#include "mmigps_internal.h"
#include "dal_lcd.h"
#include "os_apiext.h"
#include "mmisrvaud_api.h"

/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/

typedef void (*ThreadType)( uint32,void * );
#define GPS_SOUND_TIMEOUT 				500 //20000
#define MMI_GPS_ALLOC_FILENAME_SIZE 	256
#define MMI_GPS_HANDWRITEING_BUFFER 	(1024*2)
#define MMI_GPS_MAX_PLAY_PCM_BUFFER (150*160)
#define MMI_GPS_MAX_THREAD_SIZE		6
#define MMI_GPS_MAX_CIPHER_SIZE           4
#define MMI_GPS_MAX_PLAY_BUFFER_SIZE  (500*1024)
/*---------------------------------------------------------------------------*/
/*                          CONSTANT ARRAY                                  */
/*---------------------------------------------------------------------------*/
LOCAL uint32 s_Volume_effect_table[5]={0,3,4,5,7};

 // 字体路径
char pFont_Path12[] = "D:\\GPS_MAP\\RtFonts\\Hzk12.rfon";
char pFont_Path16[] = "D:\\GPS_MAP\\RtFonts\\Hzk16.rfon";
char pFont_Path24[] = "D:\\GPS_MAP\\RtFonts\\Hzk24.rfon";
char pFont_PathHot[]= "D:\\GPS_MAP\\RtFonts\\Hzk16.rfon";
int8	pWirteBuffer[MMI_GPS_HANDWRITEING_BUFFER]  = {0};	// 手写缓冲
/**--------------------------------------------------------------------------*
**                         EXTERNAL DECLARE                                 *
**--------------------------------------------------------------------------*/
#ifdef IM_HANDWRITING_WINTONE
extern const uint8             handwriting_lib_access[];
#endif
extern uint32					g_uVolumeSize;

extern unsigned char * g_TTsSoundBuffer;	// 声音缓冲

 /**--------------------------------------------------------------------------*
**                         STATIC DEFINITION                                *
**--------------------------------------------------------------------------*/
LOCAL SFS_HANDLE 	s_CurrentOpenFile[20] = {0};	// 保存当前打开得文件

LOCAL uint32 			s_pcm_buffer[MMI_GPS_MAX_PLAY_PCM_BUFFER]	  = {0};
LOCAL BLOCK_ID		s_GpsThread[MMI_GPS_MAX_THREAD_SIZE] 		  = {0};
LOCAL uint16  		s_gps_cipher_result[MMI_GPS_MAX_CIPHER_SIZE]  = {0};
LOCAL PCM_STREAM_HEAD_T	s_header={0};
LOCAL int32				s_iCurrentFile = 0;				// 保存个数
LOCAL SFS_FIND_DATA_T 	s_find = {0};
LOCAL int32				s_GpsThreadNumber = 0;
LOCAL int8 * 				s_pSoundBuffer = NULL;
LOCAL uint32 				s_pSoundLen = 0;
LOCAL int32 				s_iPlayOver = 1;
LOCAL MMISRV_HANDLE_T 			s_audiohandle = NULL;
//LOCAL BLOCK_ID			s_PlaySoundId = 0;
LOCAL uint8* 				s_buf_ptr = PNULL;
LOCAL char 				s_gps_patch []=   "D:\\GPS_MAP\\tt.exe";
/**--------------------------------------------------------------------------*
**                          LOCAL FUNCTION DECLARE                           *
**---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Discription:   
//  Global resource dependence: none 
//  Author:  
//  Note:
//
/*****************************************************************************/
AUDIO_RESULT_E GPS_SoundCallback(HAUDIO hAudio,uint8 ** pBufferAdd,uint32 * plen);

/*****************************************************************************/
//  Description : Audio dummy notify callback function.
//  Global resource dependence : none
//  Author: xingdong.li
//  Note: 
/*****************************************************************************/
LOCAL void AudioNotifyCallback(
                HAUDIO hAudio, 
                uint32 notify_info, 
                uint32 affix_info
                )
{
    //do nothing
}

/*****************************************************************************/
//  Description : Audio getting source data callback function, for streaming.
//  Global resource dependence : none
//  Author: xingdong.li
//  Note: 
/*****************************************************************************/
LOCAL void AudioGetSrcDataCallback(
                   /* HAUDIO hAudio, 
                    uint8 ** const ppucDataAddr, 
                    uint32* puiDataLength,
                    int32 offset
                    )*/
    HAUDIO hAudio, 
	AUDIO_GETSRCDATA_INFO_T * const ptSrcDataInfo,
	void *pvOtherInfo
	)
{
    //do nothing //Later
    GPS_SoundCallback(hAudio, (uint8 **)&(ptSrcDataInfo->pucDataAddr), (uint32 *)&(ptSrcDataInfo->uiDataLength));
}

// 打开文件
/*****************************************************************************/
//  Discription:   
//  Global resource dependence: none 
//  Author:  
//  Note:
//
/*****************************************************************************/
PUBLIC void* GPS_FileOpen(const char * filename, uint32 oflag)
{
	uint32 flag = 0;
	void * pRes = NULL;
	int32 iLen = 0;
	uint16 * pBuffer = NULL;
	
	if( (oflag & 0x02) || (oflag & 0x04) )
	{
		flag = SFS_MODE_WRITE;
	}
	else
	{
		flag = SFS_MODE_READ;
	}
	if( oflag & 0x0100 )
	{
		flag |=  SFS_MODE_SHARE_READ;
	}
	if( oflag & 0x0200 )
	{
		flag |= SFS_MODE_SHARE_WRITE;
	}
	if( oflag & 0x0400 )
	{
		flag |= SFS_MODE_SHARE_WRITE|SFS_MODE_SHARE_READ;
	}
	
	// 创建
	if( oflag & 0x1000 )
	{
		//if( oflag & 0x2000 )
			flag |= SFS_MODE_OPEN_ALWAYS;
		//else
		//	flag |= SFS_MODE_CREATE_NEW;
	}
	else
	{
		flag |= SFS_MODE_OPEN_EXISTING;
	}
	pBuffer = (uint16*)SCI_ALLOC_APP( MMI_GPS_ALLOC_FILENAME_SIZE );
	SCI_MEMSET( pBuffer,0,MMI_GPS_ALLOC_FILENAME_SIZE );
	// 文件名称转换
	iLen = GUI_GB2UCS( pBuffer,(unsigned char*)filename,strlen(filename ) );

	// 打开文件
	pRes = 	(void*)SFS_CreateFile( pBuffer,flag,0,0 );
		
	SCI_FREE( pBuffer );
	
	if( !pRes  )
	{
		//SCI_TRACE_LOW:"\n open file %s,%d false\n"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIGPS_OSAPI_181_112_2_18_2_21_52_3,(uint8*)"sd",filename,flag );
	}
	if( pRes && s_iCurrentFile < 20)
	{
		s_CurrentOpenFile[s_iCurrentFile] = (SFS_HANDLE)pRes;
		s_iCurrentFile ++;
	}
	
	return pRes;

}

/*****************************************************************************/
//  Discription:   
//  Global resource dependence: none 
//  Author:  
//  Note:
//
/*****************************************************************************/
// 移动文件指针
PUBLIC uint32 GPS_FileSeek(void* fp, uint32 offset, uint32 whence)
{
	int32 pos=0;
	if(PNULL==fp)
	{
	    //SCI_TRACE_LOW:"GPS_FileSeek  fp is NULL"
	    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIGPS_OSAPI_204_112_2_18_2_21_52_4,(uint8*)"");
	    return 0;
	}
	SFS_SetFilePointer( (SFS_HANDLE)fp,offset,whence );
	SFS_GetFilePointer( (SFS_HANDLE)fp,0,&pos );
	
	return pos;
}

/*****************************************************************************/
//  Discription:   
//  Global resource dependence: none 
//  Author:  
//  Note:
//
/*****************************************************************************/

// 读取文件
PUBLIC uint32 GPS_FileRead(void* fp, void* pBuffer, uint32 Len)
{
	uint32	res = 0;
	
	if(PNULL==fp)
	{
	    //SCI_TRACE_LOW:"GPS_FileRead  fp is NULL"
	    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIGPS_OSAPI_226_112_2_18_2_21_52_5,(uint8*)"");
	    return 0;
	}
	
	//SCI_TRACE_LOW:"[GPS_FileRead]:read Len:%x"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIGPS_OSAPI_230_112_2_18_2_21_52_6,(uint8*)"d",Len);
	if( SFS_ReadFile( (SFS_HANDLE)fp,pBuffer,Len,&res,NULL ) == SFS_ERROR_NONE )
	{
		return res;
	}
	else
	{
		return 0;
	}
}

/*****************************************************************************/
//  Discription:   
//  Global resource dependence: none 
//  Author:  
//  Note:
//
/*****************************************************************************/

// 写入文件
uint32 GPS_FileWrite(void* fp, void* pBuffer, uint32 len)
{
	uint32	res = 0;
	
	if(PNULL==fp)
	{
	    //SCI_TRACE_LOW:"GPS_FileWrite  fp is NULL"
	    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIGPS_OSAPI_254_112_2_18_2_21_52_7,(uint8*)"");
	    return 0;
	}
	if( SFS_WriteFile( (SFS_HANDLE)fp,pBuffer,len,&res,NULL ) == SFS_ERROR_NONE )
	{
		return res;
	}
	else
	{
		return 0;
	}
}

/*****************************************************************************/
//  Discription:   
//  Global resource dependence: none 
//  Author:  
//  Note:
//
/*****************************************************************************/
// 关闭文件
uint32 	 GPS_FileClose(void* fp)
{
	int32 i = 0;
	
	if(PNULL==fp)
	{
	    //SCI_TRACE_LOW:"GPS_FileWrite  fp is NULL"
	    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIGPS_OSAPI_279_112_2_18_2_21_52_8,(uint8*)"");
	    return 0;
	}
	
	for( i=0; i < s_iCurrentFile; i++ )
	{
		if( s_CurrentOpenFile[i] == (SFS_HANDLE)fp )
		{
			break;
		}
	}
	s_iCurrentFile --;
	for( ;i < s_iCurrentFile; i++ )
	{	
		s_CurrentOpenFile[i] = s_CurrentOpenFile[i+1];
	}
	s_CurrentOpenFile[s_iCurrentFile] = 0;
	return SFS_CloseFile( (SFS_HANDLE)fp );
}


/*****************************************************************************/
//  Discription:   
//  Global resource dependence: none 
//  Author:  
//  Note:
//
/*****************************************************************************/
uint32 GPS_FileStat(void*  fp, GPS_File_Stat *stat)
{
	if(PNULL==fp)
	{
	    //SCI_TRACE_LOW:"GPS_FileStat  fp is NULL"
	    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIGPS_OSAPI_309_112_2_18_2_21_52_9,(uint8*)"");
	    return 0;
	}
	
	SFS_GetFileSize( (SFS_HANDLE)fp,&(stat->st_size) );
	
	return 0;
}


/*****************************************************************************/
//  Discription:   
//  Global resource dependence: none 
//  Author:  
//  Note:
//
/*****************************************************************************/

// 查找文件

void* GPS_FindFile(int8 * filename)
{
	uint8 pBuffer[128];
	void * pRes = PNULL;
	
	if(PNULL==filename)
	{
	    //SCI_TRACE_LOW:"GPS_FindFile  filename is NULL"
	    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIGPS_OSAPI_334_112_2_18_2_21_52_10,(uint8*)"");
	    return PNULL;
	}
	pRes = (void*)SFS_FindFirstFile( (unsigned short*)filename,&s_find );
	SCI_MEMSET( pBuffer,0,128 );
	GUI_UCS2GB( (unsigned char*)pBuffer,(unsigned char*)filename,128 );
	
	return pRes;
}

/*****************************************************************************/
//  Discription:   
//  Global resource dependence: none 
//  Author:  
//  Note:
//
/*****************************************************************************/

int32 GPS_FindNextFile(void* pFind)
{
	if(PNULL==pFind)
	{
	    //SCI_TRACE_LOW:"GPS_FindNextFile  pFind is NULL"
	    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIGPS_OSAPI_354_112_2_18_2_21_52_11,(uint8*)"");
	    return PNULL;
	}
	return SFS_FindNextFile( (SFS_HANDLE)pFind,&s_find ) == SFS_ERROR_NONE;
}


/*****************************************************************************/
//  Discription:   
//  Global resource dependence: none 
//  Author:  
//  Note:
//
/*****************************************************************************/

int32 GPS_FindGetFile(void* pFind,  int8 *filename)
{
	filename = (int8*)s_find.name;
	return 1;
}


/*****************************************************************************/
//  Discription:   
//  Global resource dependence: none 
//  Author:  
//  Note:
//
/*****************************************************************************/

void GPS_FindClose(void* pFind)
{
	SFS_FindClose( (SFS_HANDLE)pFind );
}

/*****************************************************************************/
//  Discription:   
//  Global resource dependence: none 
//  Author:  
//  Note:
//
/*****************************************************************************/

int32 GPS_CreateThread( ThreadType run,void * pParam )
{
	BLOCK_ID tempThread = 0;
	int32 i = 0;
	//SCI_TRACE_LOW:"OSCreateThread"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIGPS_OSAPI_395_112_2_18_2_21_52_12,(uint8*)"" );
	
	tempThread = SCI_CreateAppThread( "GPS","GPS",run,0,pParam,0x8000,10,PRI_GPS_OS_TASK,SCI_PREEMPT,SCI_AUTO_START );
	for( i=0; i < MMI_GPS_MAX_THREAD_SIZE; i++ )
	{
		if( s_GpsThread[i] == 0 )
		{
			s_GpsThread[i] = tempThread;
			break;
		}
	}

	
	//SCI_TRACE_LOW:"s_GpsThread%x=%x"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIGPS_OSAPI_409_112_2_18_2_21_53_13,(uint8*)"dd",i,tempThread);
 	return tempThread;
}

/*****************************************************************************/
//  Discription:   
//  Global resource dependence: none 
//  Author:  
//  Note:
//
/*****************************************************************************/

void GPS_UpdateMainLCD(void)
{
	GUILCD_Invalidate((LCD_ID_E)0,0);
 	
}

/*****************************************************************************/
//  Discription:   
//  Global resource dependence: none 
//  Author:  
//  Note:
//
/*****************************************************************************/

void GPS_InvalidateRect( unsigned short left,unsigned short top,unsigned short right,
		unsigned short bottom )
{
    GUI_RECT_T  rect = {0};

    rect.left = left;
    rect.top = top;
    rect.right = right;
    rect.bottom = bottom;
	GUILCD_InvalidateRect(0,rect,0);
}

/*****************************************************************************/
//  Discription:   
//  Global resource dependence: none 
//  Author:  
//  Note:
//
/*****************************************************************************/

void* GPS_GetMainLCDBuffer(void)
{
	return (void*)GUILCD_GetMainLcdBufPtr();
}

/*****************************************************************************/
//  Discription:   
//  Global resource dependence: none 
//  Author:  
//  Note:
//
/*****************************************************************************/

void TellThreadExit( void* thread )
{
	
}

/*****************************************************************************/
//  Discription:   
//  Global resource dependence: none 
//  Author:  
//  Note:
//
/*****************************************************************************/

// GPS退出调用函数
void ExitGPS( void )
{
	// 关闭文件
	int32 i = 0;
	//int8 		pName[SCI_MAX_NAME_SIZE];
	//int8 		pQuename[SCI_MAX_NAME_SIZE];
	//uint32      state =0;          // state of the thread.
    	//uint32      priority = 0;          // Priority of the thread.
    	//uint32      preempt = 0;  	
	//SCI_TRACE_LOW:"s_iCurrentFile=%d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIGPS_OSAPI_481_112_2_18_2_21_53_14,(uint8*)"d",s_iCurrentFile );
	for( i = 0; i < s_iCurrentFile; i++ )
	{
		SFS_CloseFile( s_CurrentOpenFile[i] );
	}
	s_iCurrentFile = 0;
	// 关闭线程
	for( i = 0; i < MMI_GPS_MAX_THREAD_SIZE; i++ )
	{
		
		//SCI_TerminateThread( s_GpsThread[i] );
		if(  s_GpsThread[i] )
		{
			//SCI_TRACE_LOW:"SCI_DeleteThread=%x"
			SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIGPS_OSAPI_494_112_2_18_2_21_53_15,(uint8*)"d",s_GpsThread[i] );
			SCI_DeleteThread( s_GpsThread[i] );
			s_GpsThread[i] = 0;
			//SCI_TRACE_LOW:"SCI_DeleteThread OK"
			SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIGPS_OSAPI_497_112_2_18_2_21_53_16,(uint8*)"" );
		}
		
	}
	s_GpsThreadNumber = 0;
}


/*****************************************************************************/
//  Discription:   
//  Global resource dependence: none 
//  Author:  
//  Note:
//
/*****************************************************************************/
int32 GPS_OpenWrite(void)
{
	//SCI_TRACE_LOW:"OpenWrite"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIGPS_OSAPI_512_112_2_18_2_21_53_17,(uint8*)"" );
#ifdef IM_HANDWRITING_WINTONE
	WTRecognizeInit( pWirteBuffer,MMI_GPS_HANDWRITEING_BUFFER,handwriting_lib_access );
	WTSetRange( IDENTIFY_CHN,pWirteBuffer );
#endif
	return 1;
	//WTSetInnerCode(0x2000,handwriting_lib_access );
}

/*****************************************************************************/
//  Discription:   
//  Global resource dependence: none 
//  Author:  
//  Note:
//
/*****************************************************************************/
int32 GPS_CloseWrite(void)
{
	//SCI_TRACE_LOW:"CloseWrite"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIGPS_OSAPI_528_112_2_18_2_21_53_18,(uint8*)"" );
#ifdef IM_HANDWRITING_WINTONE
	WTRecognizeEnd( pWirteBuffer );
#endif
	return 1;
}

/*****************************************************************************/
//  Discription:   
//  Global resource dependence: none 
//  Author:  
//  Note:
//
/*****************************************************************************/

AUDIO_RESULT_E GPS_SoundCallback( HAUDIO hAudio,uint8 ** pBufferAdd,uint32 * plen )
{
	int32 iCurrentLen = s_pSoundLen > 4096 ? 4096:s_pSoundLen;
	s_pSoundLen -= iCurrentLen;
	*plen = iCurrentLen;
	*plen >>= 1;
	*plen <<= 1;
	//SCI_TRACE_LOW:"GPSSoundCallback=%d,s_pSoundLen=%d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIGPS_OSAPI_549_112_2_18_2_21_53_19,(uint8*)"dd",iCurrentLen,s_pSoundLen );
	if( iCurrentLen == 0 )
	{
		
		//*pBufferAdd = NULL;
		return AUDIO_NO_ERROR;
	}
	else
	{
		
		
		SCI_MEMCPY(*pBufferAdd,s_pSoundBuffer,iCurrentLen );
		s_pSoundBuffer += iCurrentLen;
	}

	return AUDIO_NO_ERROR;
}


/*****************************************************************************/
//  Discription:   
//  Global resource dependence: none 
//  Author:  
//  Note:
//
/*****************************************************************************/
void GPS_get_app_queue(void)
{
	uint32 status = 0;
	uint32 count =0;
	uint32 enqueued = 0;
	uint32 store	= 0;
	char * queue_name_ptr = NULL;
	TX_THREAD	* suspended_thread = NULL;
	TX_QUEUE	* next_queue = NULL;
	SCI_THREAD_T * thread_bolock = NULL;
	xSignalHeader	pSigle = NULL;
	thread_bolock = (SCI_THREAD_T*)SCI_GetThreadBlockFromId( SCI_IdentifyThread( ) );

	status = tx_queue_info_get( (TX_QUEUE*)&thread_bolock->queue,
		&queue_name_ptr,
		(uint32*)&enqueued,
		(uint32*)&store,
		(TX_THREAD**)&suspended_thread,
		(uint32*)&count,
		(TX_QUEUE**)&next_queue );
	//SCI_TRACE_LOW:"ritusoundenqueued=%d,store=%d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIGPS_OSAPI_593_112_2_18_2_21_53_20,(uint8*)"dd",enqueued,store );
	
	while( enqueued )
	{
		
		pSigle = SCI_GetSignal( SCI_IdentifyThread( ) );
		
		SCI_FREE(pSigle);
		
		enqueued --;
	}
	
}

/*****************************************************************************/
//  Discription:   
//  Global resource dependence: none 
//  Author:  
//  Note:
//
/*****************************************************************************/
void GPS_StopPlay( void )
{
	//SCI_TRACE_LOW:"GPS_stopplay,s_audiohandle:%x,thread_id:%x"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIGPS_OSAPI_614_112_2_18_2_21_53_21,(uint8*)"dd",s_audiohandle,SCI_IdentifyThread( ));
	if(s_audiohandle)
	{
	    //uint32 get_ms_out; 
	   // uint32 get_ms_in; 
		//xSignalHeader		pSigle = NULL;
		//uint16 				signal_code = 0xFFFF;
	 	MMISRV_HANDLE_T hBack = s_audiohandle;
	 	BLOCK_ID currentID;
	 	s_audiohandle = NULL;
	 	currentID = SCI_IdentifyThread( );
		MMISRVAUD_Stop(hBack);
	    
	    //SCI_TRACE_LOW:"ritusoundAUDIO_CloseHandle"
	    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIGPS_OSAPI_627_112_2_18_2_21_53_22,(uint8*)"" );
	    MMISRVMGR_Free( hBack );
/*		get_ms_in = SCI_GetTickCount();
	    do
    	{
	    	
	    	pSigle =(xSignalHeader ) SCI_PeekSignal( SCI_IdentifyThread( ) );
	    	
	    	if(SCI_NULL == pSigle)
	    	{
	    	  	SCI_Sleep(200);
	    	  	signal_code = 0xFFFF;
	    	}
	    	else
	    	{
	    		signal_code = pSigle->SignalCode;
	    		SCI_FREE(pSigle);
	    		//SCI_TRACE_LOW:"ritusoundSignalCode=%d"
	    		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIGPS_OSAPI_643_112_2_18_2_21_53_23,(uint8*)"d", signal_code );
	    	}
	    	
	    	  
	    	get_ms_out = SCI_GetTickCount();
	    	if(((get_ms_out - get_ms_in) > GPS_SOUND_TIMEOUT)||(get_ms_in > get_ms_out))
	    	{
	    	  break;  
	    	}   	  
	    	
	    	
	    }while((signal_code != AUDIO_CLOSE_HANDLE_CNF)||(signal_code != AUDIO_CLOSE_HANDLE_NEG_CNF));
		*/
	}
	SCI_SLEEP(50);
	// 清空消息队列
	GPS_get_app_queue( );
}

/*****************************************************************************/
//  Discription:   
//  Global resource dependence: none 
//  Author:  
//  Note:
//
/*****************************************************************************/
void MMIGPS_StopPlay(void)
{
	//SCI_TRACE_LOW:"MMIGPS_StopPlay,s_audiohandle:%x,thread_id:%x"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIGPS_OSAPI_669_112_2_18_2_21_53_24,(uint8*)"dd",s_audiohandle,SCI_IdentifyThread( ));
	if(s_audiohandle)
	{
	 	HAUDIO hBack = s_audiohandle;
	 	
	 	s_audiohandle = NULL;
	 	
		MMISRVAUD_Stop( hBack );
	    MMISRVMGR_Free( hBack );
	}

}

/*****************************************************************************/
//  Discription:   
//  Global resource dependence: none 
//  Author:  
//  Note:
//
/*****************************************************************************/

void GPS_PlayPCMData( int8 * pBuffer,long len )
{
	
	WaveFileHeader *	pcmHead =PNULL;
	AUDIO_DEVICE_MODE_TYPE_E      mode = AUDIO_DEVICE_MODE_HANDHOLD;
	uint32 * volume_table_ptr =(uint32 *) s_Volume_effect_table;
	uint32 volume_index =0;
    
        MMISRVMGR_SERVICE_REQ_T req = {0};
        MMISRVAUD_TYPE_T audio_srv = {0};

	if((0==g_uVolumeSize)||!MMIGPS_IsPermitPlay())
	{
		return;
	}
	
	if(PNULL==pBuffer)
	{
	    //SCI_TRACE_LOW:"GPS_FindNextFile  pFind is NULL"
	    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIGPS_OSAPI_706_112_2_18_2_21_53_25,(uint8*)"");
	    return PNULL;
	}
	pcmHead = (WaveFileHeader*)(pBuffer-44);

	s_header.sampleRate 	= pcmHead->dwSamplesPerSec;
	s_header.channel		= 1;
	s_header.is_big_endian		= 0;
	s_iPlayOver = 0;
	len >>= 1;
	len <<= 1;


    volume_index =g_uVolumeSize>>1;
    GPS_StopPlay( );
         
    req.pri = MMISRVAUD_PRI_NORMAL;

    audio_srv.duation = 0;
    audio_srv.eq_mode = 0;
    audio_srv.is_mixing_enable = FALSE;
    audio_srv.play_times = 1;
    audio_srv.volume = volume_index;
    
    audio_srv.info.streaming.type = MMISRVAUD_STREAMING;
    audio_srv.info.streaming.data = s_header;
    audio_srv.info.streaming.data_len = sizeof(s_header);
    audio_srv.info.streaming.fmt = MMISRVAUD_RING_FMT_WAVE;
    audio_srv.info.streaming.cb = (void *)AudioGetSrcDataCallback;
    audio_srv.route = MMISRVAUD_ROUTE_AUTO;
    s_audiohandle = MMISRVMGR_Request(STR_SRV_AUD_NAME, &req, &audio_srv);
     
    //SCI_TRACE_LOW:"s_audiohandle=%d,thread:%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIGPS_OSAPI_738_112_2_18_2_21_53_26,(uint8*)"dd",(long)s_audiohandle,SCI_IdentifyThread( ) );
	if( s_audiohandle == NULL )
	{
		return ;
	}
    	
    GPS_get_app_queue( );
    	
    // 播放
//	s_pSoundBuffer = pBuffer;
//	s_pSoundLen = len;
	// 计算播放的大小,并且把声音放到播放的Buffer中
	s_pSoundLen = MMI_GPS_MAX_PLAY_BUFFER_SIZE >= len ? len : MMI_GPS_MAX_PLAY_BUFFER_SIZE ;
	SCI_MEMCPY( &g_TTsSoundBuffer[0],pBuffer,s_pSoundLen );
	//SCI_TRACE_LOW:"s_pSoundLen=%d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIGPS_OSAPI_752_112_2_18_2_21_53_27,(uint8*)"d",s_pSoundLen  );
	s_pSoundBuffer = (int8*)&g_TTsSoundBuffer[0];
    
    MMISRVAUD_Play( s_audiohandle,0);
}


/*****************************************************************************/
//  Discription:   
//  Global resource dependence: none 
//  Author:  
//  Note:
//
/*****************************************************************************/
void GPS_PlayPCM_TTS( int8 * pBuffer,long len )
{
	AUDIO_DEVICE_MODE_TYPE_E      mode = AUDIO_DEVICE_MODE_HANDHOLD;
	//SFS_HANDLE				pFile = 0;
	//uint32					res = 0;
       uint32 					volume_index =0;
       uint32 * 					volume_table_ptr =(uint32 *) s_Volume_effect_table;
        MMISRVMGR_SERVICE_REQ_T req = {0};
        MMISRVAUD_TYPE_T audio_srv = {0};

// 测试将声音文件写入文件
    GPS_StopPlay( );
    if(0==MMIGPS_GetGpsState() || 0 == g_uVolumeSize ) 
    {
    	return;
    }
   
    s_header.sampleRate 	= 8000;//16000;
    s_header.channel		= 1;
    s_header.is_big_endian		= 1;
    s_iPlayOver = 0;
  	len >>= 1;
  	len <<= 1;
  	    
    volume_index =g_uVolumeSize>>1;
      
    req.pri = MMISRVAUD_PRI_NORMAL;

    audio_srv.duation = 0;
    audio_srv.eq_mode = 0;
    audio_srv.is_mixing_enable = FALSE;
    audio_srv.play_times = 1;
    audio_srv.volume = volume_index;
    
    audio_srv.info.streaming.type = MMISRVAUD_STREAMING;
    audio_srv.info.streaming.data = s_header;
    audio_srv.info.streaming.data_len = sizeof(s_header);
    audio_srv.info.streaming.fmt = MMISRVAUD_RING_FMT_WAVE;
    audio_srv.info.streaming.cb = (void *)AudioGetSrcDataCallback;
    audio_srv.route = MMISRVAUD_ROUTE_AUTO;
    s_audiohandle = MMISRVMGR_Request(STR_SRV_AUD_NAME, &req, &audio_srv);    
    s_audiohandle = NULL;
/*     
    s_audiohandle = AUDIO_CreateStreamBufferHandle(
                        hWAVCodec,NULL,hARMVB,NULL,
                        (unsigned char*)&s_header,sizeof(s_header),s_pcm_buffer,sizeof(s_pcm_buffer)/4 ,
                        AudioNotifyCallback,
                        AudioGetSrcDataCallback
                        );
*/                        
    //SCI_TRACE_LOW:"GPS_PlayPCM_TTS,s_audiohandle=%d,thread:%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIGPS_OSAPI_813_112_2_18_2_21_53_28,(uint8*)"dd",(long)s_audiohandle,SCI_IdentifyThread( ) );
    if( s_audiohandle == NULL )
    {	
    	return ;
    }

    // 播放
	s_pSoundBuffer = pBuffer;
//	s_pSoundLen = len;
	// 计算播放的大小,并且把声音放到播放的Buffer中
	s_pSoundLen = MMI_GPS_MAX_PLAY_BUFFER_SIZE >= len ? len : MMI_GPS_MAX_PLAY_BUFFER_SIZE;
	// TTS传入的Buffer就是g_TTsSoundBuffer
//	s_pcm_buffer( &g_TTsSoundBuffer[0],pBuffer,s_pSoundLen );
//	s_pSoundBuffer = (int8*)&g_TTsSoundBuffer[0];
    
    MMISRVAUD_Play( s_audiohandle,0);
    
}

/*****************************************************************************/
//  Discription:   
//  Global resource dependence: none 
//  Author:  
//  Note:
//
/*****************************************************************************/

uint32 GPS_GetTickCount( )
{
	return SCI_GetTickCount( );
}
/*****************************************************************************/
//  Discription:   
//  Global resource dependence: none 
//  Author:  
//  Note:
//
/*****************************************************************************/

void GPS_Sleep( uint32 time_in_ms)
{
	SCI_SLEEP( time_in_ms  );
}

/*****************************************************************************/
//  Discription:   
//  Global resource dependence: none 
//  Author:  
//  Note:
//
/*****************************************************************************/
void GPS_SetCPUFrequency( int32 irequency )
{
	if( irequency == 50 )
	{
		MMIGPS_SetFreqInGPS( BOOT_CLK_MID  );
	}
	else if( irequency == 192 )
	{
		MMIGPS_SetFreqInGPS( BOOT_CLK_HIGH  );
	}
	else
	{
		MMIGPS_SetFreqInGPS(ARM_48_AHB_24);
	}
}

/*****************************************************************************/
//  Discription:   
//  Global resource dependence: none 
//  Author:  
//  Note:
//
/*****************************************************************************/
int8 * RituGps_OsMalloc( uint32 ulSize )
{

	//SCI_TRACE_LOW:"RituGps_OsMalloc ulSize = %d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIGPS_OSAPI_882_112_2_18_2_21_53_29,(uint8*)"d",ulSize);
	if (s_buf_ptr==PNULL)
	{
		if ((NOT_USE == BL_GetStatus((BLOCK_MEM_ID_E)BLOCK_MEM_POOL_GPS)) &&
			(ulSize <= BL_GetSize((BLOCK_MEM_ID_E)BLOCK_MEM_POOL_GPS)))
		{
			s_buf_ptr = (int8*)MMI_BL_MALLOC(BLOCK_MEM_POOL_GPS);
		}
	}
	return s_buf_ptr;
}

/*****************************************************************************/
//  Discription:   
//  Global resource dependence: none 
//  Author:  
//  Note:
//
/*****************************************************************************/
void RituGps_OsFree( void )
{
	//SCI_TRACE_LOW:"RituGps_OsFree s_buf_ptr =%x"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIGPS_OSAPI_901_112_2_18_2_21_53_30,(uint8*)"d",s_buf_ptr);
	
	if (PNULL!=s_buf_ptr)
	{
		MMI_BL_FREE(BLOCK_MEM_POOL_GPS);
		s_buf_ptr = PNULL;
	}
	
}

/*****************************************************************************/
//  Discription:   
//  Global resource dependence: none 
//  Author:  
//  Note:
//
/*****************************************************************************/
/// 与平台无关函数
const char * GPS_GetModePath( void )
{
	return s_gps_patch;
}

/*****************************************************************************/
//  Discription:   
//  Global resource dependence: none 
//  Author:  
//  Note:
//
/*****************************************************************************/
// 调试用代码
void VOSPrint( const char *pArg,... )
{

	va_list argList = {0};
	char pBuffer[255];
	va_start(argList, pArg);

	SCI_MEMSET( pBuffer,0,255 );
	vsprintf( pBuffer,pArg,argList );
	//SCI_TRACE_LOW:"ritu:%s\n"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIGPS_OSAPI_937_112_2_18_2_21_54_31,(uint8*)"s",pBuffer );
	
	va_end(argList);
	
}

/*****************************************************************************/
//  Discription:   
//  Global resource dependence: none 
//  Author:  
//  Note:
//
/*****************************************************************************/
void ShowError( const char *pArg,... )
{
	va_list argList = {0};
	char pBuffer[255];
	va_start(argList, pArg);
	SCI_MEMSET( pBuffer,0,255 );
	vsprintf( pBuffer,pArg,argList );
	va_end(argList);
	SCI_TRACE_LOW( pBuffer );
	SCI_ASSERT( 0 );    /*assert verified*/   
}

/**************************************
		code for test
***************************************/
//@David.Jia 2007.8.2   begin


/*****************************************************************************/
//  FUNCTION:     COM_Init
//  Description:    call GPS_Open
//  return:         0--ok, others--error.
//  Author:         David.Jia
//  date:           
//	Note:           
/*****************************************************************************/
int COM_Init(void)
{

   //return 0;
    return GPS_ERR_NONE == GPS_Open(GPS_MODE_NORMAL)? 0 : -1;
}

/*****************************************************************************/
//  FUNCTION:     COM_Close
//  Description:    call GPS_Close
//  return:         0--ok, others--error.
//  Author:         David.Jia
//  date:           
//	Note:           
/*****************************************************************************/
int COM_Close(void)
{
   // return 0;
    return GPS_ERR_NONE == GPS_Close() ? 0 : -1;
}


/*****************************************************************************/
//  FUNCTION:     Map_Read
//  Description:   call GPS_ReadData
//  INPUT:          uint8* buf--data buffer alocated by caller, uint32 len--bytes number wish to read.
//  OUTPUT:         read byte number in fact.
//  Author:         
//  date:           
//	Note:          
/*****************************************************************************/
int Map_Read(uint8 *buf, uint32 len)
{
    // return 0;
    int iRes = 0;
    iRes = GPS_ReadData(buf, len);
    /*buf[iRes] = '\0';
    if( iRes > 0 )
    {
    	int ii = 0;
    	char pBuffer[256];
    	while( ii < iRes )
    	{
    		int iSize = (iRes-ii) > 255?255:iRes-ii;
    		if( iSize == 0 )
    			break;
    		s_pcm_buffer( pBuffer,&buf[ii],iSize );
    		pBuffer[255] = '\0';
    		SCI_TRACE_LOW( pBuffer );
    		ii += iSize;
    	}
    }*/
    return iRes;
}

/*****************************************************************************/
//  FUNCTION:     Map_Write
//  Description:    call GPS_WriteData
//  INPUT:          uint8* buf--data buffer alocated by caller, uint32 len--bytes number wish to read.
//  OUTPUT:         
//  Author:         
//  date:           
//	Note:          if len > Output_Q.size, data will overwrite.
/*****************************************************************************/
int Map_Write(uint8 *buf, uint32 len)
{
    // return 0;
    return GPS_WriteData(buf, len);
}

//@David.Jia 2007.8.2   end


/*****************************************************************************/
//  Discription:   
//  Global resource dependence: none 
//  Author:  
//  Note:
//
/*****************************************************************************/
void gps_cipher (
					uint16 random1,
					uint16 random2
				)
{
	GPS_VALIDATION_CIPHER_T output_validation_cipher = {0};
	uint32 result =0;
	result = GPS_GetValidationCipher(((random2<<16)+random1), &output_validation_cipher);
	 
	SCI_SLEEP(20);
	if (0==result)
	{
	  	s_gps_cipher_result [1] =output_validation_cipher.low_cipher&0xffff;
	  	s_gps_cipher_result [2] =(output_validation_cipher.low_cipher>>16)&0xffff;
	  	s_gps_cipher_result [3] =output_validation_cipher.high_cipher&0xffff;
	  	s_gps_cipher_result [0] =0;
	  	//SCI_TRACE_LOW:"s_gps_cipher_result [0]:%x,"
	  	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIGPS_OSAPI_1055_112_2_18_2_21_54_32,(uint8*)"d",s_gps_cipher_result [0]);
	  	//SCI_TRACE_LOW:"s_gps_cipher_result [1]:%x,"
	  	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIGPS_OSAPI_1056_112_2_18_2_21_54_33,(uint8*)"d",s_gps_cipher_result [1]);
	  	//SCI_TRACE_LOW:"s_gps_cipher_result [2]:%x,"
	  	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIGPS_OSAPI_1057_112_2_18_2_21_54_34,(uint8*)"d",s_gps_cipher_result [2]);
	  	//SCI_TRACE_LOW:"s_gps_cipher_result [3]:%x,"
	  	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIGPS_OSAPI_1058_112_2_18_2_21_54_35,(uint8*)"d",s_gps_cipher_result [3]);
	} 
	else
	{
		SCI_MEMSET(s_gps_cipher_result,0,sizeof(s_gps_cipher_result));
	} 
}


/*****************************************************************************/
//  Discription:   
//  Global resource dependence: none 
//  Author:  
//  Note:
//
/*****************************************************************************/
SFS_ERROR_E GPS_CopyOneFile( uint16 * sourceDir,uint16 * destDir )
{
	return MMIGPS_CopyOneFile(sourceDir, destDir);
}

/*****************************************************************************/
//  Discription:   
//  Global resource dependence: none 
//  Author:  
//  Note:
//
/*****************************************************************************/
BOOLEAN GPS_IsFolderExist(
                            const uint16		*full_path_ptr,     //in
                            uint16				full_path_len       //in, 双字节为单位
                            )
{
	BOOLEAN         	result = FALSE;
	SFS_HANDLE      	sfs_handle = 0;
	SFS_FIND_DATA_T s_find_data = {0};

	if(PNULL==full_path_ptr)
	{
	    //SCI_TRACE_LOW:"GPS_IsFolderExist  full_path_ptr is NULL"
	    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIGPS_OSAPI_1093_112_2_18_2_21_54_36,(uint8*)"");
	    return PNULL;
	}

	sfs_handle = MMIAPIFMM_FindFirstFile(full_path_ptr,full_path_len, &s_find_data);

	//SCI_TRACE_LOW:": s_find first sfs_handle = %d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIGPS_OSAPI_1099_112_2_18_2_21_54_37,(uint8*)"d", sfs_handle);

	if(0 != sfs_handle)
	{
		if(SFS_ATTR_DIR & s_find_data.attr)
		{
			result = TRUE;
		}
		SFS_FindClose(sfs_handle);
	}

    return result;//MMIAPIFMM_IsFileExist(full_path_ptr,full_path_len);
}
/*****************************************************************************/
//  Discription:   
//  Global resource dependence: none 
//  Author:  
//  Note:
//
/*****************************************************************************/
SFS_ERROR_E GPS_CopyDir(uint16 * sourceDir,uint16 * destDir) 
{ 
   return  MMIGPS_CopyDir(sourceDir,destDir);

} 
/*****************************************************************************/
//  Discription:   
//  Global resource dependence: none 
//  Author:  
//  Note:
//
/*****************************************************************************/
void CloseWind( void )
{
	 MMIGPS_CloseWind();
}
/*****************************************************************************/
//  Discription:   
//  Global resource dependence: none 
//  Author:  
//  Note:
//
/*****************************************************************************/
int IsGpsActive( void )
{
	return MMIGPS_GetGpsState();
}
#else
uint32	g_uVolumeSize = 0;
uint32	g_TTsSoundBuffer = 0;
int COM_Close(void)
{
    return -1;
}
void GPS_ReadData(void){}
void GPS_Open(void){}
void DelMessage(void){}
void GPSMain(void){}

void GPS_WriteData(void){}
void GPS_GetValidationCipher(void){}
#endif
