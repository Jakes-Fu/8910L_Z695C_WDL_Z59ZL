/****************************************************************************
** File Name:      _AMR_STREAM_C_.c                                             *
** Author:                                                                   *
** Date:           11/10/2010                                                *
** Copyright:      2003 Spreadtrum, Incorporated. All Rights Reserved.         *
** Description:    the file AMR STREM PLAY     
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 10/2010        hai.li          Create
******************************************************************************/
#if (defined SPREAD_PLAT_SC6600L_1025) || (defined SPREAD_PLAT_SC6600L_0937)||(defined SPREAD_PLAT_SC6620)
#define _AMR_STREAM_C_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "std_header.h"
#include "mmiaudio.h"
#include "mmiaudio_ctrl.h" 
#include "mmirecord.h"
#include "mmimp3.h"
#include "mmi_appmsg.h"
#include "mmi_common.h"
#include "mmi_nv.h"
#include "sig_code.h"
#include "mmi_signal.h"
#include "mmibt_export.h"
#ifdef KURO_SUPPORT
#include "mmikur.h"
#endif
#include "sfs.h"
#include "ffs.h"
#include "dsp_codec_adp.h"
#include "dal_audio.h"

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#define MAX_AMR_TESTBUF  32

/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/
#ifdef WIN32
#define __align(x)
#endif

#define AMR_FILE_MAX_SIZE (50*1024+6)

uint16  amr_count =6;
static const uint8 AMR_HEADER2[6] = {'#', '!', 'A', 'M', 'R', '\n'};
//__align(4) unsigned char amr_buffer[AMR_FILE_MAX_SIZE];
unsigned char* amr_buffer = PNULL;
unsigned char *amr_datastream, *amr_datastream_init;
uint16 frame_size[16]={ 12, 13, 15, 17, 19, 20, 26, 31, 5, 0, 0, 0, 0, 0, 0, 0 };
uint16 samr_temp[40];
wchar s_file_name[512] = {0}; 
/**--------------------------------------------------------------------------*
 **                         EXTERNAL DECLARE                                 *
 **--------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : Create amr record file.
//  Global resource dependence : none
//  Author:
//  Note: 
/*****************************************************************************/
static void AMRREOCED_File(uint8 *data_ptr, uint32 data_len)
{  
    uint32 tmp = 0;     
    SFS_HANDLE file_handle_r = NULL;
    file_handle_r = SFS_CreateFile(s_file_name, FFS_MODE_CREATE_ALWAYS|FFS_MODE_WRITE,NULL,NULL);
    if(file_handle_r > 0)
    {
        SFS_WriteFile(file_handle_r, data_ptr, data_len, (uint32*)&tmp, NULL);
    } 		     
    SFS_CloseFile(file_handle_r);
    memset(s_file_name,0,sizeof(s_file_name));
    return;
}

/*****************************************************************************/
//  Description : Create amr record callback funcation.
//                for read arm_reoced data from codec.
//  Global resource dependence : none
//  Author:
//  Note: 
/*****************************************************************************/

static void amrenc_CallBack(uint16 i_len)
{	
	uint16* p16_tmp = PNULL;
	int i = 0;    
	SCI_TRACE_LOW("123=%d", i_len); 
	#ifndef WIN32
	AUD_ReadVoiceData((uint16 *)samr_temp, (uint16)i_len);
	#endif
	SCI_TRACE_LOW("456=%d", (uint8)samr_temp[0]); 
	p16_tmp = (uint16 *)samr_temp;
	for (i=0; i<i_len; i++)
	{
		*(amr_datastream+i) = (uint8)(p16_tmp[i]);
	}
	amr_datastream += i_len;
	if ((amr_datastream - amr_datastream_init) > (AMR_FILE_MAX_SIZE - 6))
	{
		amr_datastream = amr_datastream_init;
	}
}

/*****************************************************************************/
//  Description : API of start amr record 
//  Global resource dependence : none
//  Author:
//  Note: 
/*****************************************************************************/
static BOOLEAN AMR_Recode_Start(void)
{
    int i = 0;
    SCI_TRACE_LOW("AMR_Record_Test: start reoced");	
    // Add amr head for saving recode amr_data to amr_file
    amr_buffer = (unsigned char*)MAPI_SCI_ALLOCA_ByZero(AMR_FILE_MAX_SIZE);
	if(PNULL == amr_buffer)
	{
		return FALSE;
	}    
    amr_datastream=amr_buffer+6;
    amr_datastream_init=amr_buffer+6;    
    for(i=0;i<6;i++)
       amr_buffer[i]=AMR_HEADER2[i];
	//参数说明：
	//amr_mode：AMR4.75 only  channels：mono channel only sample_rate：8k only
	//AUD_CfgAmrParam(MR475,1,8000);//mono channel 
    #ifndef WIN32
	AUD_CfgAmrParam(MR475,1,8000);//mono channel 
    //start recoeding
	AUD_EnableVoiceRecord(SCI_TRUE, AUD_REC_AMR, amrenc_CallBack);    	
	#endif
	return TRUE;
}

/*****************************************************************************/
//  Description : API of stop amr record and save record data to a file
//  Global resource dependence : none
//  Author:
//  Note: 
/*****************************************************************************/
static BOOLEAN AMR_Recode_Stop(void)
{
	SCI_TRACE_LOW("AMR_Record_Test: stop reoced");
    #ifdef WIN32
    if(1)
    #else
	if(ERR_AUD_NONE == AUD_EnableVoiceRecord(SCI_FALSE, AUD_REC_AMR, NULL))
	#endif
	{
		//save reoced data to file	
		SCI_TRACE_LOW("AMR_Recode_Stop file_size = %d", (amr_datastream - amr_datastream_init));
		if(amr_datastream >= amr_datastream_init)
		{
			AMRREOCED_File((uint8 *)amr_buffer, (amr_datastream - amr_datastream_init)); 
		}
		else
		{
			AMRREOCED_File((uint8 *)amr_buffer, 6); 
		}
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

/*****************************************************************************/
//  Description : API of start record
//  Global resource dependence : none
//  Author: wancan.you
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN M_MMIAUDIO_StartRecord(const wchar* name_ptr, uint32 size, BOOLEAN is_in_call)
{
    BOOLEAN  aud_result = FALSE;    
    //SCI_ASSERT(PNULL != name_ptr);
    if(PNULL == name_ptr)
    {
        return aud_result;
    }
    
    MMIAPICOM_Wstrcpy(s_file_name, name_ptr);
    aud_result = AMR_Recode_Start();
	SCI_TRACE_LOW("M_MMIAUDIO_StartRecord size = %d, aud_result = %d", size, aud_result);
    return aud_result;
}

/*****************************************************************************/
//  Description : API of start record
//  Global resource dependence : none
//  Author: wancan.you
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN M_MMIAUDIO_StopRecord(void)
{
    BOOLEAN result = TRUE;
    result = AMR_Recode_Stop();
    return result;
}

/*****************************************************************************/
//  Description : API of end record (close handle)
//  Global resource dependence : none
//  Author: wancan.you
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN M_MMIAUDIO_EndRecord(void)
{
    BOOLEAN result = TRUE;    
    SCI_TRACE_LOW("[MMIAUDIO]:MMIAUDIO_EndRecord:result = %d", result);
    return result;
}

#endif

