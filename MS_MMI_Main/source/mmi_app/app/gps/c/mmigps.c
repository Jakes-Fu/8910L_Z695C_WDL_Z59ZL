/****************************************************************************
** File Name:      mmigps.c                                                *
** Author:         jian.ma                                                 *
** Date:           07/19/2007                                              *
** Copyright:      2005 Spreadtrum, Incorporated. All Rights Reserved.      *
** Description:    This file is used to describe the cryp display menu     *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 07/19/2007    jian.ma          Create                                   *
**                                                                         *
****************************************************************************/
//#ifndef _MMIGPS_C_
#define _MMIGPS_C_
#include "mmi_app_gps_trc.h"
#include "std_header.h"
#ifdef GPS_SUPPORT
#include "mmigps_internal.h"
#include "mmifmm_export.h"
#include "guifont.h"
#include "mmi_default.h"
/**---------------------------------------------------------------------------*
**                         DEPENDENCIES                                     *
**---------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/
#define MMI_GPS_ALLOC_SIZE  (4*1024)
/*---------------------------------------------------------------------------*/
/*                          CONSTANT ARRAY                                */
/*---------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------*
**                         EXTERNAL DECLARE                                 *
**--------------------------------------------------------------------------*/
extern void GPSMain(void);
extern int DelMessage( int im,unsigned long iP1,unsigned long ip2 );
extern uint32 MMI_SetFreq(PROD_FREQ_INDEX_E freq);
extern uint32 MMI_RestoreFreq(void);
/**--------------------------------------------------------------------------*
**                         STATIC DEFINITION                                *
**--------------------------------------------------------------------------*/
LOCAL uint32 		s_GPSisActive = 1;
LOCAL BLOCK_ID 	s_task_id = 0;
LOCAL BLOCK_ID	s_gpsTask = 0;
LOCAL BOOLEAN 	s_gps_is_permit_play = FALSE;

/**--------------------------------------------------------------------------*
**                          LOCAL FUNCTION DECLARE                           *
**---------------------------------------------------------------------------*/
/*****************************************************************************/
//	Description : close  GPS task
//	Global resource dependence : none
//	Author:
//	Note:
/*****************************************************************************/
LOCAL void CloseGPSTask( void );

/*****************************************************************************/
//  Discription:   gps task
//  Global resource dependence: none 
//  Author:  
//  Note:
//
/*****************************************************************************/
PUBLIC void GPSTask(uint32 argc, void* argv)
{
	do
	{
	
	MMIDEFAULT_AllowTurnOffBackLight(FALSE);

	MMIGPS_SetFreqInGPS(FREQ_INDEX_GPS_HIGH); //输入参数值选择限于（BOOT_CLK_HIGH，BOOT_CLK_MID,BOOT_CLK_LOW），参考chng_freq.h
	// 进入GPS
	s_GPSisActive = 1;
	s_task_id = 1;

	GPS_get_app_queue();
	
	GPSMain( );
	
	RituGps_OsFree();
	// 退出GPS
	s_GPSisActive = 0;
	s_task_id = 0;
	// 继续FM
	//MMIAUDIO_ResumeBgPlay(MMIBGPLAY_MODULE_MP3);
	GPS_get_app_queue();
	MMIGPS_RestoreFreqInGPS();

	// 终止线程
	MMIDEFAULT_AllowTurnOffBackLight(TRUE);
	SCI_SuspendThread( s_gpsTask );
	}while(1);
}

/*****************************************************************************/
//	Description : Is permit play
//	Global resource dependence : none
//	Author:jian.ma
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIGPS_IsPermitPlay(void )
{
	return s_gps_is_permit_play;
}

/*****************************************************************************/
//	Description : set permit play
//	Global resource dependence : none
//	Author:jian.ma
//	Note:
/*****************************************************************************/
PUBLIC void MMIGPS_SetPermitPlay(BOOLEAN is_permit_play )
{
	s_gps_is_permit_play = is_permit_play;
}

/*****************************************************************************/
//	Description : set GPS state
//	Global resource dependence : none
//	Author:jian.ma
//	Note:
/*****************************************************************************/
PUBLIC void MMIGPS_SetGpsState(uint32 state)
{
	s_GPSisActive = state;
}

/*****************************************************************************/
//	Description : get  GPS state
//	Global resource dependence : none
//	Author:jian.ma
//	Note:
/*****************************************************************************/
PUBLIC int32  MMIGPS_GetGpsState( void )
{
	return s_GPSisActive;
}

/*****************************************************************************/
//	Description : Is gps Opened
//	Global resource dependence : none
//	Author:
//	Note:
/*****************************************************************************/
PUBLIC int32 MMIAPIGPS_IsGpsOpened(void)
{
	return s_task_id;
}
/*****************************************************************************/
//	Description : Set freq in Gps
//	Global resource dependence : none
//	Author:
//	Note:
/*****************************************************************************/
PUBLIC uint32 MMIGPS_SetFreqInGPS(PROD_FREQ_INDEX_E index)
{
	return MMI_SetFreq(index);
}
/*****************************************************************************/
//	Description : restore Freq 
//	Global resource dependence : none
//	Author:
//	Note:
/*****************************************************************************/
PUBLIC uint32 MMIGPS_RestoreFreqInGPS(void)
{
	return MMI_RestoreFreq();	
}
/*****************************************************************************/
//	Description : get  GPS state
//	Global resource dependence : none
//	Author:
//	Note:
/*****************************************************************************/
LOCAL void CloseGPSTask( void )
{
	if( MMIAPIGPS_IsGpsOpened( ) )
	{
		MMIGPS_DelMessage( 0xFF,0,0 );
	}
}
/*****************************************************************************/
//	Description : close GPS task
//	Global resource dependence : none
//	Author:
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIGPS_CloseGPSTask(void)
{
	CloseGPSTask();
}


/*****************************************************************************/
//	Description : copy dir file
//	Global resource dependence : none
//	Author:
//	Note:
/*****************************************************************************/
PUBLIC SFS_ERROR_E MMIGPS_CopyDir(uint16 * sourceDir,uint16 * destDir) 
{ 
    
	HFS	hFind = NULL;

	uint16	psourceDir[100] ={0};		// ut-16 源文件
	uint16	pdestDir[100]={0};		// ut-16 目标文件

	uint16 	uLenSource = 0;
	uint16 uLenDest = 0;
	SFS_FIND_DATA_T sfsfind ={0};
	SFS_ERROR_E	     findRes = SFS_NO_ERROR;
	// 组合查找的文件
	uLenSource = MMIAPICOM_Wstrlen( sourceDir );
	uLenDest = MMIAPICOM_Wstrlen( destDir );

	if( 0 == uLenSource || uLenDest == 0 )
	{
		return SFS_ERROR_NOT_EXIST;
	}
	SCI_MEMCPY(psourceDir,sourceDir,uLenSource*2+2  );
	SCI_MEMCPY(pdestDir,destDir,uLenDest*2+2  );
	if( '\\' == psourceDir[uLenSource-1] || '/' == psourceDir[uLenSource-1] )
	{
		
	}
	else
	{
		psourceDir[uLenSource] = '\\';
		uLenSource ++;
	}
	psourceDir[uLenSource] = '*';
	psourceDir[uLenSource+1] = '\0';
	if( '\\' == pdestDir[uLenDest-1] || '/' == pdestDir[uLenDest-1] )
	{
		
	}
	else
	{
		pdestDir[uLenDest] = '\\';
		uLenDest ++;
	}
    
	pdestDir[uLenDest] = '\0';

	hFind = MMIAPIFMM_FindFirstFile( psourceDir,uLenDest,&sfsfind );
	//   SCI_TRACE_LOW( " find source file handle=%d",hFind );
	if( hFind == NULL )
	{
		return SFS_ERROR_NOT_EXIST;
	}
    // 开始拷贝
    do
    {
    	char pBuffer[100]={0};
    	uint uNameLen;	// 下一个文件夹的目录长度
    	GUI_UCS2GB( (unsigned char*)pBuffer,(unsigned char*)sfsfind.name,100 );
    //	SCI_TRACE_LOW( "file name = %s",pBuffer );
    //	SCI_TRACE_LOW( "file name len = %d", sfsfind.length );
    //	SCI_TRACE_LOW( "psourceDir[%d]=%d",uLenSource,psourceDir[uLenSource] );
    	uNameLen = MMIAPICOM_Wstrlen( sfsfind.name );
    	SCI_MEMCPY(&psourceDir[uLenSource],
    				sfsfind.name,uNameLen*2+2  );
    	psourceDir[uLenSource+uNameLen] = '\0';
    				
    	SCI_MEMCPY(&pdestDir[uLenDest],
    				sfsfind.name,uNameLen*2+2  );
    	pdestDir[uLenDest+uNameLen] = '\0';
    	// 是目录
    	if( sfsfind.attr & SFS_ATTR_DIR )
    	{
    		// 去掉'.'和'..'
    		if( '.' == sfsfind.name[0] )
    		{
    			findRes = SFS_FindNextFile( hFind,&sfsfind  );
    			continue;
    		}
    		// 拷贝目录
    		
    		
    		
    		SFS_CreateDirectory( pdestDir );
    		findRes = MMIGPS_CopyDir(psourceDir,pdestDir  );
    		if( SFS_NO_ERROR != findRes )
    		{
    			SFS_FindClose( hFind );
    			return findRes;
    		}
    	}
    	else
    	{
    		
    		findRes = MMIGPS_CopyOneFile(psourceDir,pdestDir  );
    		if( SFS_NO_ERROR != findRes )
    		{
    			SFS_FindClose( hFind );
    			//SCI_TRACE_LOW:"copy one file false"
    			SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIGPS_269_112_2_18_2_21_51_0,(uint8*)"" );
    			return findRes;
    		}
    	}
    	findRes = SFS_FindNextFile( hFind,&sfsfind  );
    }while( SFS_NO_ERROR == findRes  );
    SFS_FindClose( hFind );
    return SFS_NO_ERROR;
} 

/*****************************************************************************/
//	Description : copy one file
//	Global resource dependence : none
//	Author:
//	Note:
/*****************************************************************************/
PUBLIC  SFS_ERROR_E MMIGPS_CopyOneFile( uint16 * sourceDir,uint16 * destDir )
{
	SFS_HANDLE hsource =PNULL;
	SFS_HANDLE hdest=PNULL;
	uint32 iFileSize = 0;
	uint32 iOneLen = 0;
	SFS_ERROR_E res = SFS_NO_ERROR;
	char * pBuffer = PNULL ;
	pBuffer = SCI_ALLOC_APP( MMI_GPS_ALLOC_SIZE );
	hsource = SFS_CreateFile( sourceDir,SFS_MODE_READ|SFS_MODE_OPEN_EXISTING,0,0 );
	if(hsource == NULL )
	{
		SCI_MEMSET(pBuffer,0,MMI_GPS_ALLOC_SIZE );
		GUI_UCS2GB( (unsigned char*)pBuffer,(unsigned char*)sourceDir,MMI_GPS_ALLOC_SIZE/4 );
		//SCI_TRACE_LOW:"open false :%s"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIGPS_297_112_2_18_2_21_51_1,(uint8*)"s",pBuffer );
		SCI_FREE( pBuffer );
		return SFS_ERROR_NOT_EXIST;
	}
	hdest = SFS_CreateFile( destDir,SFS_MODE_WRITE|SFS_MODE_CREATE_NEW,NULL,NULL );
	if(hdest == NULL )
	{
		SFS_CloseFile( hsource );
		SCI_MEMSET(pBuffer,0,MMI_GPS_ALLOC_SIZE );
		GUI_UCS2GB( (unsigned char*)pBuffer,(unsigned char*)destDir,MMI_GPS_ALLOC_SIZE/4 );
		//SCI_TRACE_LOW:"open false :%s"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIGPS_307_112_2_18_2_21_51_2,(uint8*)"s",pBuffer );
		SCI_FREE( pBuffer );
		return SFS_ERROR_IO_PENDING;
	}
	// 
	SFS_GetFileSize( hsource,&iFileSize );
	//SCI_TRACE_LOW( "copy file size=%d",iFileSize );
	while( iFileSize > 0 )
	{
		iOneLen = iFileSize > MMI_GPS_ALLOC_SIZE ? MMI_GPS_ALLOC_SIZE:iFileSize;
		iFileSize -= iOneLen;
		res = SFS_ReadFile( hsource,pBuffer,iOneLen,&iOneLen,NULL  );
		//SCI_TRACE_LOW( "read file=%d",iOneLen );
		
		res = SFS_WriteFile(hdest,pBuffer,iOneLen,&iOneLen,NULL  );
		//SCI_TRACE_LOW( "write file=%d",iOneLen );	
	}
	SFS_CloseFile( hsource );
	SFS_CloseFile( hdest );
	SCI_FREE( pBuffer );
	return SFS_NO_ERROR;
}

/*****************************************************************************/
//	Description : delete message
//	Global resource dependence : none
//	Author:jian.ma
//	Note:
/*****************************************************************************/
PUBLIC int32 MMIGPS_DelMessage( int32 im,unsigned long iP1,unsigned long ip2 )
{
	return DelMessage(im,iP1,ip2);
}

/*****************************************************************************/
//	Description : get gps task id
//	Global resource dependence : none
//	Author:juan.zhang
//	Note:
/*****************************************************************************/
PUBLIC BLOCK_ID MMIGPS_GetGPSTaskID(void)
{
	return s_gpsTask;
}

/*****************************************************************************/
//	Description : set gps task id
//	Global resource dependence : none
//	Author:juan.zhang
//	Note:
/*****************************************************************************/
PUBLIC void MMIGPS_SetGPSTaskID(BLOCK_ID gps_task)
{
	s_gpsTask = gps_task;
}

/*****************************************************************************/
//  Discription:   
//  Global resource dependence: none 
//  Author:  
//  Note:
//
/*****************************************************************************/
PUBLIC void MMIGPS_OsFree( void )
{
	RituGps_OsFree();
}
#else
/*****************************************************************************/
//	Description : close GPS task
//	Global resource dependence : none
//	Author:
//	Note:
/*****************************************************************************/
void MMIAPIGPS_CloseGPSTask(void)
{
	
}

/*****************************************************************************/
//	Description : open gps main win
//	Global resource dependence : none
//	Author: 
//	Note:
/*****************************************************************************/
void MMIAPIGPS_OpenMainWin(void)
{

}
/*****************************************************************************/
//	Description : Is gps Opened
//	Global resource dependence : none
//	Author:
//	Note:
/*****************************************************************************/
int MMIAPIGPS_IsGpsOpened(void)
{
	return 0;
}
#endif
