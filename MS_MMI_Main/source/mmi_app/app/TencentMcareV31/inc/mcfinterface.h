
/******************************************************************************/
// File Name:       McfInterface.h                                           
// Author:          wilsonzhu wiltan                                 
// DATE:            10/01/2011                                  
// Modifed:         09/26/2011
// Copyright:       2011 Tencent, Incoporated. All Rights Reserved.
// Description:     ��������SIM����ؽӿ�                      
/******************************************************************************/
#ifndef _MCFINTERFACE_H
#define _MCFINTERFACE_H

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern  "C"
    {
#endif

/******************************************************************************/
//                      include files
/******************************************************************************/
////////////////////////////spread .h include /////////////////////
#if (defined SPREAD_PLAT_SC6600L_1025) || (defined SPREAD_PLAT_SC6600L_0937)
#include "image_proc.h"
#else

#ifdef SPREAD_PLAT_SC6530
#include "mmiconnection_export.h"
#include "mmipdp_export.h"
#else//#ifndef SPREAD_PLAT_SC6530
#include "mmipdp_api.h"
#include "mmiconnection.h"
#endif//#ifndef SPREAD_PLAT_SC6530

#if (defined SPREAD_PLAT_SC6620)
#include "image_proc.h"
#else
#include "img_res_decoder.h"
#endif
#include "mmiaudio_ctrl.h"
#endif

#include "dal_img.h"

#include "mmifmm_interface.h"
#include "mmipub.h"
////////////////////////////porting .h include /////////////////////
#include "mcfzlib.h"
#include "mcffs.h"
#include "mcfeditor.h"
#include "mcfscreen.h"
#include "mcfsys.h"
#if (defined SPREAD_PLAT_SC6620)
#include "mmk_timer.h"
#endif
/******************************************************************************/
//                      Macro Declaration
/******************************************************************************/
#define MCARE_VIBRATE_TIME      (100)

#ifdef MCR_ASSERT
#ifndef MCF_SCI_PASSERT
#define MCF_SCI_PASSERT(_EXP,PRINT_STR)   (_EXP) ? (void)0 : (void )SCI_PAssert(#_EXP, __FILE__, __LINE__, SCI_SaveAssertInfo##PRINT_STR)
#endif

#ifndef MCF_SCI_ASSERT
#define MCF_SCI_ASSERT(_EXP)    (_EXP) ? (void)0 : SCI_Assert(#_EXP, __FILE__, __LINE__)
#endif
#else
#ifndef MCF_SCI_PASSERT
#define MCF_SCI_PASSERT(_EXP,PRINT_STR)   
#endif
#ifndef MCF_SCI_ASSERT
#define MCF_SCI_ASSERT(_EXP)
#endif
#endif//#if MCR_ASSERT



#ifndef MCARE_BACKLIGHT_OPEN_FOREVER
//Ĭ�ϰ汾�رոú�
//#define MCARE_BACKLIGHT_OPEN_FOREVER//��������������ƽ���MCARE�Ƿ���
#endif
/******************************************************************************/
//                      Type and constant
/******************************************************************************/
/******************************************************************************/
//                      Local variables Declaration
/******************************************************************************/

/******************************************************************************/
//                      Local function extern
/******************************************************************************/

/******************************************************************************/
//                      function declare
/******************************************************************************/

/******************************************************************************/


/*=======================McfAudio.c Begin==========================*/

#ifdef SPREAD_NEW_AUDIO_UPDATE
MUINT McfAud_McareToMocorVol(MBYTE volume);
//---------------------------------------------------------------------------
//	FUNCTION:       McfAud_GetAudioType
//	Descrption:     ����MCARE����Ƶ���ͻ�ȡƽ̨����Ƶ����
//	Param:
//	   	IN          MBYTE playStyle :MCARE����Ƶ����    
//	Return:         ƽ̨����Ƶ����
//  Note:
//----------------------------------------------------------------------------
MMISRVAUD_RING_FMT_E McfAud_GetAudioType(MBYTE format);
MUINT McfAud_GetRingVol(MVOID);
MBOOL McfAud_IsSilentMode(MBYTE format);
MBYTE McfAud_GetFileFormat(MSHORT* pFile);


MINT McfIF_Aud_PlayStream(MVOID* pUser, MVOID* pAudioData, MUINT len, MBYTE format, 
					   MBYTE playStyle, pfnMcfAudFunc handler);
MINT McfIF_Aud_StopStream(MINT audHd);
MINT McfIF_Aud_PlayFile(MVOID* pUser, MSHORT* pFile, MBYTE playStyle, pfnMcfAudFunc handler);
MINT McfIF_Aud_StopFile(MINT audHd);
MINT McfIF_Aud_Pause(MINT audHd, MVOID* pUser, pfnMcfAudFunc handler);
MINT McfIF_Aud_Resume(MINT audHd, MVOID* pUser, pfnMcfAudFunc handler);
MBOOL MAPI_MMISRVMGR_Free(MUINT audio_handle);
MBOOL McfIF_Aud_CallBack(MUINT mocor_hanlde, MVOID* pParam);

MBOOL McfIF_Aud_IsPermit(MVOID);

#else//#ifdef SPREAD_NEW_AUDIO_UPDATE


MBOOL MAPI_MMIAPISET_IsPermitPlayRing(MINT ring_type);

MBOOL MAPI_MMIAUDIO_AudioPlay(
                            MMIAUDIO_HTYPE   audio_handle_type,
                            MUINT           uiOffset,
                            MUINT           play_times,
                            MMIAUDIO_END_CALLBACK_PFUNC end_callback_func
                            );   

MVOID MAPI_MMIAUDIO_AudioStop(MMIAUDIO_HTYPE audio_handle_type);

MBOOL MAPI_MMIAUDIO_AudioPause(MMIAUDIO_HTYPE audio_handle_type);

MBOOL MAPI_MMIAUDIO_AudioResume(MMIAUDIO_HTYPE audio_handle_type);

AUDIO_RESULT_E MAPI_AUDIO_SetVolume(MUINT uiVol);

AUDIO_RESULT_E MAPI_AUDIO_GetVolume(MUINT *uiVol);

MMIAUDIO_HTYPE McfAud_GetAppType(MBYTE format);

MUINT McfAud_McareToMocorVol(MBYTE volume);
//---------------------------------------------------------------------------
//	FUNCTION:       McfAud_GetAudioType
//	Descrption:     ����MCARE����Ƶ���ͻ�ȡƽ̨����Ƶ����
//	Param:
//	   	IN          MBYTE playStyle :MCARE����Ƶ����    
//	Return:         ƽ̨����Ƶ����
//  Note:
//----------------------------------------------------------------------------

MMIAUDIO_TYPE_E McfAud_GetAudioType(MBYTE playStyle);

MMISET_ALL_RING_TYPE_E McfAud_GetRingTypeOther(MBYTE format);

MUINT McfAud_GetRingVol(MVOID);

MVOID McfAud_AudioCallBack(MMIAUDIO_RESULT result, DPARAM param);

MBOOL McfAud_IsSilentMode(MBYTE format);

MBOOL McfIF_Aud_IsPermit(MVOID);

MBOOL McfIF_Aud_IsPlaying(MVOID);
//---------------------------------------------------------------------------
//FUNCTION: MAPI_MMIAUDIO_CreateAudioStreamingHandle
//Descrption:��Ƶ���ŵ�ƽ̨���Ļص�����
//Param:
//  IN: 
//  OUT:audio_hand ��������Ƶ���
//Return: MTRUE :�ɹ� MFALSE:ʧ��
//Note:
//----------------------------------------------------------------------------
MBOOL MAPI_CreateAudioBufHandle(MBYTE playStyle,MVOID* pAudioData,
                                                    MUINT len);
//---------------------------------------------------------------------------
//FUNCTION: MAPI_CreateAudioFileHandle
//Descrption:�����ļ�������Ӧ����Ƶ
//Param:
//  IN:     ������Ƶ�ļ�������
//  OUT:audio_hand ��������Ƶ���
//Return: MTRUE :�ɹ� MFALSE:ʧ��
//Note:
//----------------------------------------------------------------------------
MBOOL MAPI_CreateAudioFileHandle( MSHORT* pFile);
MBYTE McfAud_GetFileFormat(MSHORT* pFile);
MBOOL MAPI_MMIAUDIO_CloseAudioHandle(MMIAUDIO_HTYPE audio_handle_type);
MVOID McfAud_EnableKeyRing(MBOOL is_enable);
MMIAUDIO_HTYPE MAPI_MMIAUDIO_GetAudioHtype(MVOID);


MBOOL McfIF_MMIAUDIO_GetIsPlaying(MVOID);
MBOOL McfIF_MMIAUDIO_AudioResume(MVOID);
MBOOL McfIF_MMIAUDIO_AudioPause(MVOID);

MVOID MAPI_MMIAPISET_PlayRingByPtr(
                                    MMISET_ALL_RING_TYPE_E      ring_type,
                                    uint8                       ring_vol,   // the ring volume
                                    uint8                       *ring_data_ptr,
                                    uint32                      ring_data_len,
                                    uint32                      play_times,
                                    MMIAUDIO_TYPE_E             audio_type,
                                    MMISET_RING_CALLBACK_PFUNC  callback_func
                                    );
#endif//#ifdef SPREAD_NEW_AUDIO_UPDATE

/*=======================McfAudio.c End==========================*/

/*=======================McfBrowser.c API BEGIN=======================*/ 

//---------------------------------------------------------------------------
//	FUNCTION:       McfIF_Brw_StartWapReq
//	Descrption:     ����ϵͳ������ĺ���
//	Param:
//	   	IN          url:�����url    
//	Return:         
//                  0 : success
//                  -1 : fail
//  Note:
//----------------------------------------------------------------------------
MINT McfIF_Brw_StartWapReq(MCHAR* url);

//---------------------------------------------------------------------------
//	FUNCTION:       McfBrw_StartWapReq
//	Descrption:     ��ȡ����� USER AGENT         ͷ����Ϣ
//	Param:
//	   	OUT         pUaHeader:��ȡ����UA ��ַ
//	   	IN          len:����buffer �ĳ���
//	Return:         
//                  success : ����ͷ����Ϣ
//                  fail   : ����MNULL
//  Note:
//----------------------------------------------------------------------------
MCHAR* McfIF_Brw_GetUaHeader(MCHAR* pUaHeader, MINT len);

/*=======================McfBrowser.c API END========================*/ 

/*=======================McfChset.c API Begin==========================*/
//---------------------------------------------------------------------------
//	FUNCTION:       MAPI_GUI_UTF8ToWstr
//	Descrption:     ��UTF8ת����WCHAR
//	Param:
//	   	OUT         wstr_ptr:����ת���ַ��������buff
//	   	IN          wstr_len:buffer �ĳ���
//	   	IN          utf8_ptr:��Ҫת�����ַ����ĵ�ַ
//	   	IN          utf8_len:��ת���ĳ���

//	Return:         
//                  len : ����ת���ɺ���ַ�������(�ַ�����)
//  Note:
//----------------------------------------------------------------------------
MINT MAPI_GUI_UTF8ToWstr(MWCHAR* wstr_ptr, MUINT wstr_mem_size, const MCHAR* utf8_ptr, MUINT utf8_len);

//---------------------------------------------------------------------------
//	FUNCTION:       MAPI_GUI_WstrToUTF8
//	Descrption:     ��WCHARת����UTF8
//	Param:
//	   	OUT         utf8_ptr:����ת���ַ��������buff
//	   	IN          utf8_buf_len:buffer �ĳ���
//	   	IN          wstr_ptr:��Ҫת�����ַ����ĵ�ַ
//	   	IN          wstr_len:��ת���ĳ���

//	Return:         
//                  len : ����ת���ɺ���ַ�������(�ַ�����)
//  Note:
//----------------------------------------------------------------------------
MINT MAPI_GUI_WstrToUTF8(MCHAR* utf8_ptr, MUINT utf8_buf_len, const MWCHAR* wstr_ptr, MUINT wstr_len);


//mcfsms.c
MUSHORT MAPI_GUI_GB2UCS(					//return the number of UCS2 code that have been converted.
                         MWCHAR*ucs2_ptr, 	//destination: string pointer to UCS2 string.
                         const MCHAR*gb_ptr,//source: string pointer to GB string
                         MUSHORT len			//the length to convert from GB to UCS2.
                         );
/*=======================McfChset.c API End==========================*/  


/*=======================McfDtcnt.c API BEGIN=========================*/

/*----------------------------------------------------------------------------*/
/*                          FUNCTION DECLARE                            */
/*----------------------------------------------------------------------------*/

MUSHORT McfCmn_GetAppMnSignal(MUSHORT appMnSignal);
MUSHORT McfCmn_GetMnGprsErr(MUSHORT mnGprsErr);
ERR_MNGPRS_CODE_E McfCmn_MNGPRS_No_error_code(MVOID);
MN_GPRS_ERR_E McfCmn_MNGPRS_ERR_SUC(MVOID);
MUSHORT McfDcnt_GetAppMnSignal(MUSHORT appMnSignal);
MINT McfIF_GetModuleID(MVOID);

#if (defined SPREAD_PLAT_SC6800H) || (defined SPREAD_PLAT_SC6610) || (defined SPREAD_PLAT_SC8800G) || (defined SPREAD_PLAT_SC6600L) || (defined SPREAD_PLAT_SC6620) || (defined SPREAD_PLAT_SC6530)|| (defined SPREAD_PLAT_SC8800G_PDA)
MBYTE McfIF_GetNetSettingIndex(MN_DUAL_SYS_E dual_sys);
MINT McfIF_GetPDPInterfaceWIFI(MVOID);
MINT McfIF_GetPDPInterfaceGPRS(MVOID);
MINT McfIF_GetPSServiceRat(MVOID);

ERR_MNGPRS_CODE_E MAPI_MNGPRS_ReadStorageExtReqQosEx(
					MN_DUAL_SYS_E dual_sys,
					MN_GPRS_STORAGE_E storage,
					MN_PS_SERVICE_TYPE_E ps_service_type,
					MN_GPRS_EXT_QOS_INFO_T* qos_info);

MN_GPRS_STORAGE_E McfDcnt_GetGprsType();

MMICONNECTION_LINKSETTING_DETAIL_T* MAPI_CONNECTION_GetLinkSettingItemByIndex(MN_DUAL_SYS_E dual_sys,uint8 setting_index);

BOOLEAN MAPI_MMIAPIPDP_Active(MMIPDP_ACTIVE_INFO_T *app_info_ptr);

BOOLEAN MAPI_MMIAPIPDP_Deactive(MUINT app_handler);

#endif//#if (defined SPREAD_PLAT_SC6800H) || (defined SPREAD_PLAT_SC6610) || (defined SPREAD_PLAT_SC8800G) || (defined SPREAD_PLAT_SC6600L) || (defined SPREAD_PLAT_SC6620) || (defined SPREAD_PLAT_SC6530)


#if (defined SPREAD_PLAT_SC6600L_1025) || (defined SPREAD_PLAT_SC6600L_0937)
ERR_MNGPRS_CODE_E MAPI_MNGPRS_SetAndActivePdpContextEx(MN_DUAL_SYS_E dual_sys, MBYTE* APNcn, MN_PS_SERVICE_TYPE_E ps_service_type);
ERR_MNGPRS_CODE_E MAPI_MNGPRS_ResetAndDeactivePdpContextEx(MN_DUAL_SYS_E dual_sys);
MINT McfCmn_GetMCareGprsID(MVOID);
#endif//#ifdef SPREAD_PLAT_SC6600L_1025

MN_PS_SERVICE_TYPE_E McfDcnt_GetSerivceType(MVOID);

/*=======================McfDtcnt.c API END==========================*/  


/*=====================McfEditor.c API BEGIN=======================*/

/*----------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                            */
/*----------------------------------------------------------------------------*/
//------------------------------------------------------------------------------
//	FUNCTION:       McfEditor_CreateEditBox
//	Descrption:     �����༭��Ŀؼ�
//	Param:
//	   	 IN         text_ptr:����༭��ؼ����ַ�
//	   	 IN         max_alpha_len:�༭����󳤶�
//	   	 IN         max_ucs2_len:�༭������ַ���
//	   	 IN         input_type:��������뷽��
//	   	 IN         win_id:����ID
//	Return:         
//                  MVOID
//  Note:           
//------------------------------------------------------------------------------
MINT McfEditor_CreateEditBox(MMI_STRING_T* text_ptr,
							 MUSHORT max_alpha_len,
							 MUSHORT max_ucs2_len,
							 MBYTE input_type,
							 MMI_WIN_ID_T win_id);

MMI_HANDLE_T MAPI_MMK_CreateWin(MUINT* win_table_ptr,ADD_DATA add_data_ptr);

MVOID McfEditor_SetCallBack(MVOID* pUser, pfnMcfInputCb func);

MUINT McfEditor_GetMEditorWinTab(MVOID);

MUINT McfEditor_GetMEditorWinID(MVOID);

MBOOL McfEditor_GetMEditorType(MBYTE input_type, MBYTE* edit_type, 
                                                  MBYTE* all_im, MBYTE* first_im);
/*======================McfEditor.c API END========================*/  

/*=======================McfFs.c Begin==========================*/
SFS_ERROR_E MAPI_SFS_CloseFile(SFS_HANDLE handle);

SFS_ERROR_E MAPI_SFS_ReadFile(
						SFS_HANDLE 			handle,
						MVOID* 				buffer,
						MUINT 				bytes_to_read,
						MUINT* 				bytes_read,
						SFS_OVERLAPPED_T* 	overlapped
						);

SFS_ERROR_E MAPI_SFS_WriteFile(
					  SFS_HANDLE 			handle,
					  const MVOID* 			buffer,
					  MUINT 					bytes_to_write,
					  MUINT* 				bytes_written,
					 SFS_OVERLAPPED_T* 		overlapped
					 );

SFS_ERROR_E MAPI_SFS_DeleteFile(const MWCHAR* name,SFS_OVERLAPPED_T* overlapped_ptr);

SFS_ERROR_E MAPI_SFS_FlushFile(SFS_HANDLE handle,SFS_OVERLAPPED_T* overlapped);

SFS_ERROR_E MAPI_SFS_GetFileSize(SFS_HANDLE sfs_handle, MUINT* size);

SFS_ERROR_E MAPI_SFS_SetFileSize(SFS_HANDLE sfs_handle, MUINT size);

SFS_ERROR_E MAPI_SFS_SetFilePointer(SFS_HANDLE handle, MLONG offset,MUINT origin);

MBOOL MAPI_MMIFILE_IsFileExist(const MWCHAR* full_path_ptr,MUSHORT full_path_len);

MBOOL MAPI_MMIFILE_IsFolderExist(const MWCHAR* full_path_ptr,MUSHORT full_path_len);

SFS_HANDLE MAPI_SFS_FindFirstFile(const MWCHAR* find, SFS_FIND_DATA_T* fd);

SFS_ERROR_E MAPI_SFS_FindNextFile(SFS_HANDLE sfs_handle, SFS_FIND_DATA_T* fd);

SFS_ERROR_E MAPI_SFS_FindClose(SFS_HANDLE sfs_handle);

SFS_ERROR_E MAPI_SFS_GetFilePointer(SFS_HANDLE handle,MUINT origin,MINT* currentpos);

SFS_ERROR_E MAPI_SFS_GetDeviceFreeSpace(const MWCHAR* device_name, MUINT* high_word, MUINT* low_word);

SFS_ERROR_E MAPI_SFS_GetDeviceStatus(const MWCHAR* dev_name);

SFS_ERROR_E MAPI_SFS_CreateDirectory(const MWCHAR* path);

SFS_ERROR_E MAPI_SFS_RenameFile(const MWCHAR* sour,const MWCHAR* dest,SFS_OVERLAPPED_T* overlapped_ptr);

MMIFMM_OPENWIN_RET_E MAPI_MMIFMM_OpenFileWinByLayer(
                                MWCHAR* 				path_ptr,      //Ĭ�ϴ�Ŀ¼�������NULL����Ӹ�Ŀ¼��
                                MUSHORT 				path_len,
                                MMIFMM_FILTER_T*		filter_ptr,    //ɸѡ����
                                MMIFMM_FUNC_TYPE_E       func_type,      //����
                                MBOOL                 		is_multi_sel,   //�Ƿ��ѡ
                                PROCESSMSG_FUNC         	handle_func,    //�Զ��崦����
                                FILEARRAY               		ret_array,      //�������
                                MMIFMM_OpenFileCallBack 	callback        //�ص�����
                                );

MVOID MAPI_MMIFILEARRAY_Destroy(FILEARRAY* file_array_ptr);

MUINT MAPI_MMIFILEARRAY_GetArraySize(FILEARRAY file_array);

 MBOOL MAPI_MMIFILEARRAY_Read(FILEARRAY file_array, MUINT index, FILEARRAY_DATA_T* data_ptr);

MBOOL MAPI_MMIAPICOM_CombinePath(
                              const MWCHAR*	dir_ptr,           //in
                              MUSHORT           	dir_len,            //in, ˫�ֽ�Ϊ��λ
                              const MWCHAR*	file_name_ptr,     //in, can be NULL
                              MUSHORT           	file_name_len,      //in, ˫�ֽ�Ϊ��λ
                              MBOOL          		is_extname,         //in, �Ƿ�Ϊ��չ��
                              MWCHAR*			full_path_ptr,     //out
                              MUSHORT*		full_path_len_ptr  //out, ˫�ֽ�Ϊ��λ
                              );

SFS_ERROR_E MAPI_SFS_DeleteDirectory(const MWCHAR* path);
FILEARRAY MAPI_MMIFILEARRAY_Create(void) ;//���ط����ָ��

MWCHAR McfCmn_GetCardDrv(MVOID);

MWCHAR McfCmn_GetPublicDrv(MVOID);

MINT McfFile_return_error_code(SFS_ERROR_E ret);

MINT McfFile_GetSeekOrigin(MINT SeekType);

MINT McfFile_GetMaxPath(MVOID);

MINT McfFile_GetAttrDir(MVOID);

MMIFMM_FUNC_TYPE_E McfFile_GetFuncType(MVOID);

MMIFMM_OPENWIN_RET_E McfFile_GetOpenWinRet(MVOID);

MINT McfFile_SetFileAttrEx(const MWCHAR* pcwszFileName, MBYTE nAttr);

MFd McfFile_OpenEx(const MWCHAR* pcwszFileName, MUINT nOpenFlag);

MBOOL McfIF_IsUsePublicDrv(MVOID);

MVOID McfIF_SetUsePublicDrv(MBOOL is_udisk);

MUINT McfIF_GetFsSizeReq(MVOID);

MBOOL McfIF_IsStorageReady(MVOID);

//----------------------------------------------------------------------------
//FUNCTION: McfIF_GetRootDrv
//Descrption:���Mcare���еĸ�Ŀ¼
//Param:
//  IN: MVOID
//  OUT:MWCHAR*
//Return:Mcare���еĸ�Ŀ¼
//Note:
//----------------------------------------------------------------------------
MWCHAR* McfIF_GetRootDrv(MVOID);

/*=======================McfFs.c End==========================*/

/***************************************McfGdi.c API Begin*****************************************/


MBOOL McfIF_Gdi_bNeedDecode(MINT img_type);

GUI_LCD_DEV_INFO McfCmn_GetDevInfo(MVOID);

IMG_RES_ERROR_E McfCmn_GetResError(MUINT type);

GUI_FONT_T McfCmn_GetFontId(MINT fontSize);

MINT McfIF_GetCharWidth(GUI_FONT_T font);

MINT McfIF_GetCharHeight(GUI_FONT_T font);

GUI_ALIGN_E McfCmn_GetAligenType(MINT aligen_enum);

#if (defined SPREAD_PLAT_SC6800H) || (defined SPREAD_PLAT_SC8800G) || (defined SPREAD_PLAT_SC6530)|| (defined SPREAD_PLAT_SC8800G_PDA)
MINT McfIF_GetScrWidth(MVOID);

MINT McfIF_GetScrHeight(MVOID);

#else
MINT McfIF_GetScrWidth(MVOID);

MINT McfIF_GetScrHeight(MVOID);
#endif

MUINT McfCmn_GetLCdID(MVOID);

MUINT McfCmn_GetBlockID(MVOID);

#if (defined SPREAD_PLAT_SC6800H) || (defined SPREAD_PLAT_SC8800G) || (defined SPREAD_PLAT_SC6530)|| (defined SPREAD_PLAT_SC8800G_PDA)
MINT __Gdi_DrawImageBuffer(MINT x, MINT y, const MBYTE* pImgData, MUINT nDataSize);
#endif //#if (defined(SPREAD_PLAT_SC6800H)||defined(SPREAD_PLAT_SC8800G)) || (defined SPREAD_PLAT_SC6530)

MBOOL McfCmn_IsSupportGradRect(MVOID);

#if (defined SPREAD_PLAT_SC6800H) || (definedSPREAD_PLAT_SC8800G) || (defined SPREAD_PLAT_SC6530)
MINT McfIF_Gdi_DrawText(MINT x, MINT y, const MWCHAR* pcwszStr);
MINT McfIF_Gdi_DrawTextN(MINT x, MINT y, const MWCHAR* pcwszStr, MINT n);
#endif//#if (defined SPREAD_PLAT_SC6800H) || (definedSPREAD_PLAT_SC8800G) || (defined SPREAD_PLAT_SC6530)

MVOID McfIF_Gdi_DrawLine(GUI_LCD_DEV_INFO const *dev_info_ptr, MINT x1, MINT y1, MINT x2, MINT y2, MColor line_color);

MUINT McfIF_GetImgResBMP(MVOID);

IMG_RES_ERROR_E MAPI_IMG_RES_GetImageInfo(const IMG_RES_SRC_T* img_src_ptr, IMG_RES_INFO_T* img_info_ptr);

IMG_RES_ERROR_E MAPI_IMG_RES_Display_Img(const IMG_RES_SRC_T* img_src_ptr, MVOID* img_disp_ptr, IMG_RES_INFO_T* img_info_ptr);

MBOOL MAPI_MMITHEME_StoreUpdateRect(GUI_LCD_DEV_INFO const* lcd_dev_ptr, GUI_RECT_T update_rect);


#if (defined SPREAD_PLAT_SC6800H) || (defined SPREAD_PLAT_SC8800G) || (defined SPREAD_PLAT_SC6530)|| (defined SPREAD_PLAT_SC8800G_PDA)
MBOOL MAPI_GUILCD_GetInfo(LCD_ID_E lcd_id, LCD_INFO_T* lcd_info_ptr);
MBOOL MAPI_GUILCD_GetLogicWidthHeight(LCD_ID_E lcd_id, uint16* logic_width_ptr, uint16* logic_height_ptr);
#endif

MUINT MAPI_GUI_GetStringWidth(GUI_FONT_T font_type, const wchar* str_ptr, MUINT str_len);

MVOID MAPI_LCD_DrawRect(GUI_LCD_DEV_INFO const* dev_info_ptr, GUI_RECT_T box, GUI_COLOR_T color);

MVOID MAPI_LCD_FillRect(GUI_LCD_DEV_INFO const* dev_info_ptr, GUI_RECT_T box, GUI_COLOR_T color);

MBOOL MAPI_GUILCD_InvalidateRect(LCD_ID_E lcd_id, GUI_RECT_T rect,  MUINT update_block_set);

GUI_COLOR_T* MAPI_GUIBLOCK_GetBuf(GUI_LCD_DEV_INFO* lcd_dev_ptr);
/***************************************McfGdi.c API end*****************************************/

/*=======================McfIdle.c Begin==========================*/

GUIWIN_STBDATA_ICON_STYLE_E McfIdle_GetIconStyleNormal(MVOID);

MUINT McfIdle_GetIdleIconID(MVOID);

MBOOL McfIdle_IsPhoneLocked(MVOID);

MBOOL McfIdle_IsIdleQQMsgBlinkLCD(MVOID);

MBOOL McfIF_IsIdleQQMsgLightLCD(MVOID);

MBOOL McfIF_IsOnlyStaterBarIconMsg(MVOID);

MUINT McfIdle_GetMocorImageIDNull(MVOID);

MUINT McfIdle_GetIdleIconImageIDNull(MVOID);

MUINT McfIdle_GetIdleIconImageIDMcare(MVOID);

MUINT McfIdle_GetIdleIconImageIDQqOnline(MVOID);

MUINT McfIdle_GetIdleIconImageIDQqOffline(MVOID);

MUINT McfIdle_GetIdleIconImageIDQqAway(MVOID);

MUINT McfIdle_GetIdleIconImageIDQqInvisible(MVOID);

MUINT McfIdle_GetIdleIconImageIDQqIncomeMsg(MVOID);

MUINT McfIdle_GetIdleIconImageIDContact(MVOID);
MVOID McfIdle_UpdateStaterBarIcon(MVOID);
MVOID McfIdle_DropDownNotifyRecord(MBOOL is_add, MINT icon_id);

MBOOL MAPI_MMIDEFAULT_GetBackLightState(MVOID);

MVOID MAPI_MMIDEFAULT_TurnOffBackLight(MVOID);

MVOID MAPI_MMIDEFAULT_TurnOnBackLight(MVOID);

MBOOL MAPI_GUIWIN_SetStbItemIconId(MUINT item_index, MMI_IMAGE_ID_T icon_id);

MBOOL MAPI_GUIWIN_SetStbItemIconCurFrame(MUINT item_index, MUINT cur_frame);

MBOOL MAPI_GUIWIN_GetStbItemIconStyle(MUINT item_index, GUIWIN_STBDATA_ICON_STYLE_E* icon_style_ptr);

MBOOL MAPI_GUIWIN_SetStbItemVisible(MUINT item_index, MBOOL is_visible);

MBOOL MAPI_GUIWIN_AppendStbItemData(GUIWIN_STBDATA_ITEM_DATA_T *item_data_ptr);

MINT McfIdle_Get_GUIWIN_STBDATA_ICON_NORMAL(MVOID);

#ifdef PDA_UI_DROPDOWN_WIN
MUINT McfIF_GetIdleIconImageID(MINT idle_icon);
#endif

MVOID McfIF_SetStatuBarInfo(MVOID);

MBOOL McfIdle_IsShowContanct(MVOID);

/*=======================McfIdle.c End==========================*/

/***************************************McfKey.c API begin**********************************************/
MVOID McfKey_GetkeyInfo(MUINT* keyCode, MUINT* keyType, MUINT msg_id);
/***************************************McfKey.c API end**********************************************/


/***************************************McfMemory.c API begin**********************************************/
MVOID* MAPI_SCI_ALLOCA(MUINT size);
MVOID* MAPI_SCI_ALLOCA_ByZero(MUINT size);
MVOID MAPI_SCI_FREE(MVOID* pMem);
/***************************************McfMemory.c API end**********************************************/

/*=======================McfMisc.c API Begin==========================*/   
MBOOL  MAPI_MNNV_GetIMEIEx(MBYTE simId,MN_IMEI_T imei);  // TRUE if read IMEI success

MN_IMSI_T MAPI_MNSIM_GetImsiEx(MBYTE simId);

//------------------------------------------------------------------------------
//	FUNCTION: McfMisc_GetLanguageCodeIndex
//	Descrption:��ȡ��Ӧ������CODE�洢�������е�����ֵ
//	Param:
//      IN    languagetype:��Ӧ����������
//	Return: 
//            ��Ӧ���Ե�code�洢�������е�����ֵ
//  Note:
//------------------------------------------------------------------------------

MUINT McfMisc_GetLanguageCodeIndex(MINT languagetype);

/*=======================McfMisc.c API End==========================*/ 

/***************************************McfPen.c API begin**********************************************/
MUINT McfPen_GetPenPressed(MVOID);

MUINT McfPen_GetPenReleased(MVOID);

MUINT McfPen_GetPenMoved(MVOID);
/***************************************McfPen.c API end**********************************************/



/*=======================McfPhonebook.c API Begin==========================*/ 

MBOOL McfPB_IsStorageReady(MINT nStorage);

#ifdef SPREAD_NEW_PB_UPDATE
//------------------------------------------------------------------------------
//	FUNCTION: McfPhb_McareToMocroId
//	Descrption:��MCAREƽ̨�ļ�¼IDת����չѶ�ļ�¼ID
//	Param:
//      IN    mcare_id:MCARE ƽ̨���߼��洢ID
//      OUT   mocro_id:��ȡ�������չѶƽ̨��ʵ�ʴ洢��ID
//	Return: 
//      MTRUE : success
//      MFALSE : fail
//  Note:
//------------------------------------------------------------------------------
MBOOL McfPhb_McareToMocroId(MINT mcare_id, MUSHORT* mocro_id);
//------------------------------------------------------------------------------
//	FUNCTION: McfPhb_MocroToMcareId
//	Descrption:��չѶƽ̨�ļ�¼IDת����MCARE�ļ�¼ID
//	Param:
//      IN    mocro_id:չѶ ƽ̨��ʵ�ʴ洢ID
//      OUT   mcare_id:MCAREƽ̨���߼��洢��ID
//	Return: 
//      MTRUE : success
//      MFALSE : fail
//  Note:
//------------------------------------------------------------------------------
MBOOL McfPhb_MocroToMcareId(MUSHORT mocro_id, MINT* mcare_id );

//------------------------------------------------------------------------------
//	FUNCTION: McfPhb_McareToMocroStorage
//	Descrption:��MCAREƽ̨�ļ�¼�洢����ת����չѶ�Ĵ洢����
//	Param:
//      IN    storage:MCARE ƽ̨���߼��洢����
//      OUT   group_id:��ȡ�������չѶƽ̨��ʵ�ʴ洢��ID
//      OUT   mocro_storage:��ȡ�������չѶƽ̨��ʵ�ʴ洢����
//	Return: 
//      MTRUE : success
//      MFALSE : fail
//  Note:
//------------------------------------------------------------------------------
MBOOL McfPhb_McareToMocroStorage(MINT storage,MBYTE* group_id,MUSHORT* mocro_storage);

//------------------------------------------------------------------------------
//	FUNCTION: McfPhb_MocroToMcareStorage
//	Descrption:��չѶƽ̨�ļ�¼�洢����ת����MCARE�Ĵ洢����
//	Param:
//      IN    storage:MCARE ƽ̨���߼��洢����
//      OUT   group_id:չѶƽ̨��ʵ�ʴ洢��ID
//	Return: 
//      MTRUE : success
//      MFALSE : fail
//  Note:
//------------------------------------------------------------------------------
MBOOL McfPhb_MocroToMcareStorage(MUSHORT storage_id,MINT* storage);
//------------------------------------------------------------------------------
//	FUNCTION: McfPhb_ReadEntryByID
//	Descrption:������Ϣ������
//	Param:
//      IN   group_id:չѶƽ̨��ʵ�ʴ洢��ID
//      IN   entry_id:չѶƽ̨��ʵ�ʴ洢ID
//      IN   mocro_storage:չѶƽ̨��ʵ�ʴ洢����
//      OUT  pEntry:MCARE ƽ̨�ļ�¼��Ϣ
//	Return: 
//      MTRUE : success
//      MFALSE : fail
//  Note:
//------------------------------------------------------------------------------
MBOOL McfPhb_ReadEntryByID(MINT entry_id,MINT  Storage_id,McfPhbEntry* pEntry);
//------------------------------------------------------------------------------
//	FUNCTION: McfPhb_GetSpareEntryID
//	Descrption:MCARE�Ļ�ȡչѶͨѶ¼������е�ID
//	Param:
//      IN  group_id:չѶƽ̨����ID
//	Return: 
//      ��Ӧ�Ŀ���ID
//  Note:
//------------------------------------------------------------------------------

MUSHORT McfPhb_GetSpareEntryID(MUSHORT         storage_id);
//------------------------------------------------------------------------------
//	FUNCTION: McfPhb_UpdateEntry
//	Descrption:���¼�¼
//	Param:
//      IN   pUser:�ص������Ĳ���
//      IN   pEntry:����ӵ�MCARE��¼
//      IN   fnAddCb:�ص�����
//	Return: 
//      MPHB_SUCCESS : success
//      MPHB_ERROR : fail
//  Note:
//------------------------------------------------------------------------------

MINT McfPhb_UpdateEntry(MVOID* pUser, McfPhbEntry* pEntry, pfnPhbNotifyCb fnUpdateCb);
//------------------------------------------------------------------------------
//	FUNCTION: McfPhb_AddEntry
//	Descrption:��Ӽ�¼
//	Param:
//      IN   pUser:�ص������Ĳ���
//      IN   pEntry:����ӵ�MCARE��¼
//      IN   fnAddCb:�ص�����
//	Return: 
//      MPHB_SUCCESS : success
//      MPHB_ERROR : fail
//  Note:
//------------------------------------------------------------------------------

MINT McfPhb_AddEntry(MVOID* pUser, McfPhbEntry* pEntry, pfnPhbNotifyCb fnAddCb);

//------------------------------------------------------------------------------
//	FUNCTION: McfPhb_DelEntry
//	Descrption:ɾ����¼
//	Param:
//      IN   pUser:�ص������Ĳ���
//      IN   nId: ɾ����ID
//      IN   nStorage: ɾ����¼�洢������
//      IN   fnAddCb:�ص�����
//	Return: 
//      MPHB_SUCCESS : success
//      MPHB_ERROR : fail
//  Note:
//------------------------------------------------------------------------------
MINT McfPhb_DelEntry(MVOID* pUser, MINT nId, MINT nStorage, pfnPhbNotifyCb fnDelCb);
//------------------------------------------------------------------------------
//	FUNCTION: McfPhb_QueryProperty
//	Descrption:��ȡ��صĵ绰���е�����
//	Param:
//      IN   pProp:�����ȡ��������ֵ
//	Return: 
//      MPHB_SUCCESS : success
//      MPHB_ERROR : fail
//  Note:
//------------------------------------------------------------------------------

MINT McfPhb_QueryProperty(McfPhbProp* pProp);


MINT McfIF_Misc_GetCharPinyin(MUSHORT ucs2Char, MCHAR* pPinyin, MINT nSize);

//------------------------------------------------------------------------------
//	FUNCTION: McfPhb_GetNextIdByStorage
//	Descrption:������Ϣ������
//	Param:
//      IN    storage:MCARE ƽ̨���߼��洢����
//      OUT   group_id:��ȡ�������չѶƽ̨��ʵ�ʴ洢��ID
//      OUT   mocro_storage:MCAREƽ̨��ʵ�ʴ洢����
//	Return: 
//      MTRUE : success
//      MFALSE : fail
//  Note:
//------------------------------------------------------------------------------
MBOOL McfPhb_GetNextIdByStorage(MINT *entry_id,MUINT *m_index, MINT storage_id);

MBOOL MAPI_MMIPB_GetPBIsSorting(MVOID);

MUSHORT MAPI_MMIPB_GetEntryCountInNv(MVOID);
MUSHORT MAPI_MMIPB_GetMaxContactNumOfNV(MVOID);
MUSHORT Mcfphb_GetMaxRecordNumByStorage(MINT storge_id);
MUSHORT Mcfphb_GetUsedRecordNumByStorage(MINT storge_id);


#else//#ifdef SPREAD_NEW_PB_UPDATE

MINT McfIF_GetMcarePbList(MVOID);
MINT McfIF_GetPbListType(MVOID);

#if (defined SPREAD_PLAT_SC6800H) ||(defined SPREAD_PLAT_SC8800G) || (defined SPREAD_PLAT_SC6530)|| (defined SPREAD_PLAT_SC8800G_PDA)
MINT McfIF_GetPbSearchType(MVOID);
#endif

MBOOL  MAPI_MMIAPIPB_OpenListAllGroupWin(MVOID);

MVOID McfPB_OpenNumberList(MVOID) ;//Note: nicklei  add  McfPB_OpenNumberList
//------------------------------------------------------------------------------
//	FUNCTION: McfPhb_McareToMocroId
//	Descrption:��MCAREƽ̨�ļ�¼IDת����չѶ�ļ�¼ID
//	Param:
//      IN    mcare_id:MCARE ƽ̨���߼��洢ID
//      OUT   mocro_id:��ȡ�������չѶƽ̨��ʵ�ʴ洢��ID
//	Return: 
//      MTRUE : success
//      MFALSE : fail
//  Note:
//------------------------------------------------------------------------------
MBOOL McfPhb_McareToMocroId(MINT mcare_id, MUSHORT* mocro_id);
//------------------------------------------------------------------------------
//	FUNCTION: McfPhb_MocroToMcareId
//	Descrption:��չѶƽ̨�ļ�¼IDת����MCARE�ļ�¼ID
//	Param:
//      IN    mocro_id:չѶ ƽ̨��ʵ�ʴ洢ID
//      OUT   mcare_id:MCAREƽ̨���߼��洢��ID
//	Return: 
//      MTRUE : success
//      MFALSE : fail
//  Note:
//------------------------------------------------------------------------------
MBOOL McfPhb_MocroToMcareId(MUSHORT mocro_id, MINT* mcare_id );

//------------------------------------------------------------------------------
//	FUNCTION: McfPhb_McareToMocroStorage
//	Descrption:��MCAREƽ̨�ļ�¼�洢����ת����չѶ�Ĵ洢����
//	Param:
//      IN    storage:MCARE ƽ̨���߼��洢����
//      OUT   group_id:��ȡ�������չѶƽ̨��ʵ�ʴ洢��ID
//      OUT   mocro_storage:��ȡ�������չѶƽ̨��ʵ�ʴ洢����
//	Return: 
//      MTRUE : success
//      MFALSE : fail
//  Note:
//------------------------------------------------------------------------------
MBOOL McfPhb_McareToMocroStorage(MINT storage,MBYTE* group_id,MUSHORT* mocro_storage);
//------------------------------------------------------------------------------
//	FUNCTION: McfPhb_MocroToMcareStorage
//	Descrption:��չѶƽ̨�ļ�¼�洢����ת����MCARE�Ĵ洢����
//	Param:
//      IN    storage:MCARE ƽ̨���߼��洢����
//      OUT   group_id:չѶƽ̨��ʵ�ʴ洢��ID
//	Return: 
//      MTRUE : success
//      MFALSE : fail
//  Note:
//------------------------------------------------------------------------------
MBOOL McfPhb_MocroToMcareStorage(MBYTE group_id,MINT* storage);
//------------------------------------------------------------------------------
//	FUNCTION: McfPhb_ReadEntryByID
//	Descrption:������Ϣ������
//	Param:
//      IN   group_id:չѶƽ̨��ʵ�ʴ洢��ID
//      IN   entry_id:չѶƽ̨��ʵ�ʴ洢ID
//      IN   mocro_storage:չѶƽ̨��ʵ�ʴ洢����
//      OUT  pEntry:MCARE ƽ̨�ļ�¼��Ϣ
//	Return: 
//      MTRUE : success
//      MFALSE : fail
//  Note:
//------------------------------------------------------------------------------
MBOOL McfPhb_ReadEntryByID(MINT entry_id, MINT  Storage_id,McfPhbEntry* pEntry);
//------------------------------------------------------------------------------
//	FUNCTION: McfPhb_GetSpareEntryID
//	Descrption:MCARE�Ļ�ȡչѶͨѶ¼������е�ID
//	Param:
//      IN  group_id:չѶƽ̨����ID
//	Return: 
//      ��Ӧ�Ŀ���ID
//  Note:
//------------------------------------------------------------------------------

MUSHORT McfPhb_GetSpareEntryID(MBYTE         group_id);
//------------------------------------------------------------------------------
//	FUNCTION: McfPhb_UpdateEntry
//	Descrption:���¼�¼
//	Param:
//      IN   pUser:�ص������Ĳ���
//      IN   pEntry:����ӵ�MCARE��¼
//      IN   fnAddCb:�ص�����
//	Return: 
//      MPHB_SUCCESS : success
//      MPHB_ERROR : fail
//  Note:
//------------------------------------------------------------------------------

MINT McfPhb_UpdateEntry(MVOID* pUser, McfPhbEntry* pEntry, pfnPhbNotifyCb fnUpdateCb);
//------------------------------------------------------------------------------
//	FUNCTION: McfPhb_AddEntry
//	Descrption:��Ӽ�¼
//	Param:
//      IN   pUser:�ص������Ĳ���
//      IN   pEntry:����ӵ�MCARE��¼
//      IN   fnAddCb:�ص�����
//	Return: 
//      MPHB_SUCCESS : success
//      MPHB_ERROR : fail
//  Note:
//------------------------------------------------------------------------------

MINT McfPhb_AddEntry(MVOID* pUser, McfPhbEntry* pEntry, pfnPhbNotifyCb fnAddCb);

//------------------------------------------------------------------------------
//	FUNCTION: McfPhb_DelEntry
//	Descrption:ɾ����¼
//	Param:
//      IN   pUser:�ص������Ĳ���
//      IN   nId: ɾ����ID
//      IN   nStorage: ɾ����¼�洢������
//      IN   fnAddCb:�ص�����
//	Return: 
//      MPHB_SUCCESS : success
//      MPHB_ERROR : fail
//  Note:
//------------------------------------------------------------------------------
MINT McfPhb_DelEntry(MVOID* pUser, MINT nId, MINT nStorage, pfnPhbNotifyCb fnDelCb);
//------------------------------------------------------------------------------
//	FUNCTION: McfPhb_QueryProperty
//	Descrption:��ȡ��صĵ绰���е�����
//	Param:
//      IN   pProp:�����ȡ��������ֵ
//	Return: 
//      MPHB_SUCCESS : success
//      MPHB_ERROR : fail
//  Note:
//------------------------------------------------------------------------------

MINT McfPhb_QueryProperty(McfPhbProp* pProp);


MINT McfIF_Misc_GetCharPinyin(MUSHORT ucs2Char, MCHAR* pPinyin, MINT nSize);

//------------------------------------------------------------------------------
//	FUNCTION: McfPhb_GetNextIdByGroup
//	Descrption:������Ϣ������
//	Param:
//      IN    storage:MCARE ƽ̨���߼��洢����
//      OUT   group_id:��ȡ�������չѶƽ̨��ʵ�ʴ洢��ID
//      OUT   mocro_storage:��ȡ�������չѶƽ̨��ʵ�ʴ洢����
//	Return: 
//      MTRUE : success
//      MFALSE : fail
//  Note:
//------------------------------------------------------------------------------
MBOOL McfPhb_GetNextIdByStorage(MINT *entry_id,MUINT *m_index, MINT storage_id);
MBOOL MAPI_MMIPB_GetPBIsSorting(MVOID);
MUSHORT MAPI_MMIPB_GetEntryCountInNv(MVOID);
MUSHORT Mcfphb_GetMaxRecordNumByStorage(MINT storge_id);
MUSHORT Mcfphb_GetUsedRecordNumByStorage(MINT storge_id);

#endif////#ifdef SPREAD_NEW_PB_UPDATE


#ifdef SPREAD_NEW_AUDIO_UPDATE
MINT McfIF_Aud_GetDuration(MUINT* pDuration, MINT nType, MVOID* pSource, MINT nFormat, MINT nLen);
MVOID MAPI_MNPHONE_GetPlmnInfoEx(MN_DUAL_SYS_E dual_sys, MSG_CAMP_STATUS_T* plmn_info_ptr);
MBOOL McfIF_Aud_StartRecord(MVOID* pUser, McfAudRecord* pRecord, pfnAudFunc fnAudRecd, MUINT* record_hanlde);
MINT McfIF_Aud_StopRecord(MINT nHd);
MVOID McfIF_Aud_TimerCallBack(MINT nHd);
MBOOL McfIF_Record_callback(MUINT mocor_hanlde, MVOID* pParam);

#else//#ifdef SPREAD_NEW_AUDIO_UPDATE
//------------------------------------------------------------------------------
//	FUNCTION: McfPb_IsRcording
//	Descrption:�ж��Ƿ���¼��
//	Param:

//	Return: 
//      MTRUE : 
//      MFALSE
//  Note:
//------------------------------------------------------------------------------

MBOOL McfPb_IsRcording(MVOID);
MINT McfRecord_ChangeRet(MMIAUDIO_RESULT result);
MBOOL MAPI_MMIAUDIO_StopRecord(MVOID);
MBOOL MAPI_MMIAUDIO_EndRecord(MVOID);
MVOID MAPI_MMIAUDIO_ResumeBgPlay(MVOID);
MVOID MAPI_MMIAPISET_ClearCurRingType(MVOID);
MVOID MAPI_MMIAPISET_InitAudioDevice(MVOID);
MVOID MAPI_MMIAUDIO_PauseBgPlay(MVOID);
MVOID MAPI_MMIAPISET_SetCurRingType(MVOID);
#if (defined SPREAD_PLAT_SC6600L_1025) || (defined SPREAD_PLAT_SC6600L_0937)||(defined SPREAD_PLAT_SC6620)
#else
MBOOL MAPI_MMIAUDIO_StartRecord(
                    const wchar     *name_ptr,
                    MMIFILE_HANDLE  file_handle,    //file handle created, can be null
                    MMIAUDIO_RECORD_FORMAT_T format,
                    MMIAUDIO_RECORD_FRAME_LENGTH_T frame_length,
                    MMIAUDIO_RECORD_TYPE_E      record_type,
                    MMIAUDIO_END_CALLBACK_PFUNC end_callback_func
                    );
MMIAUDIO_RECORD_FORMAT_T McfRecord_GetFormatAMR(MVOID);
MMIAUDIO_RECORD_FRAME_LENGTH_T McfRecord_GetFrameLenthAMR(MVOID);
MMIAUDIO_RECORD_TYPE_E McfRecord_GetRecordTypeAPP(MVOID);
#endif
MINT McfIF_Aud_GetDuration(MUINT* pDuration, MINT nType, MVOID* pSource, MINT nFormat, MINT nLen);
MVOID MAPI_MNPHONE_GetPlmnInfoEx(MN_DUAL_SYS_E dual_sys, MSG_CAMP_STATUS_T* plmn_info_ptr);

#endif//#ifdef SPREAD_NEW_AUDIO_UPDATE

/*=======================McfPhonebook.c API End==========================*/ 

/*======================McfScreen.c API Begin========================*/  
/*----------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                            */
/*----------------------------------------------------------------------------*/

MBOOL McfCmn_IsTouchScreen(MVOID);



MUINT McfCmn_GetWindowMsg(MUINT msg_id);

MBOOL McfIF_IsMcareSupport3D(MVOID);

MVOID McfIF_SetMcareSupport3D(MBOOL is_support);

MVOID McfSrceen_Enable3DMMI(MBOOL is_enable); //nicklei add

MUINT McfCmn_GetWarningImgId(MVOID);

MUINT McfIF_GetSoftkeyOne(MVOID);

MBOOL MAPI_MMK_OpenGsensor(MMI_HANDLE_T handle);

MBOOL MAPI_MMK_CloseGsensor(MMI_HANDLE_T handle);

MBOOL MAPI_MMK_IsOpenWin(MMI_HANDLE_T win_handle);

MMI_HANDLE_T MAPI_MMK_GetFirstAppletHandle( MVOID);

MMI_HANDLE_T MAPI_MMK_CreateWindow(const MMI_WINDOW_CREATE_T* create_ptr);

MBOOL MAPI_MMK_CloseWin(MMI_HANDLE_T win_handle);

MBOOL MAPI_MMK_IsFocusWin(MMI_HANDLE_T win_handle);

MBOOL MAPI_MMK_ReturnIdleWin(MVOID);

MVOID MAPI_MMIPUB_OpenAlertWinByTextPtr(
                                         MUINT  *time_period_ptr,   //time period,default is 3s
                                         MMI_STRING_T              *text1_ptr,         //text 1
                                         MMI_STRING_T              *text2_ptr,         //text 2
                                         MMI_IMAGE_ID_T            image_id,           //alert image id
                                         MMI_WIN_ID_T              *win_id_ptr,        //win id,default is MMIPUB_ALERT_WIN_ID
                                         MMI_WIN_PRIORITY_E        *win_priority_ptr,  //win priority,default is WIN_ONE_LEVEL
                                         MMIPUB_SOFTKEY_STYLE_E    softkey_style,  //softkey style
                                         MMIPUB_HANDLE_FUNC        handle_func         //handle function
                                         );
MINT McfScreen_GetAvailScreen(MVOID);

MBOOL  MAPI_MMK_SendMsg(MMI_HANDLE_T  handle,MMI_MESSAGE_ID_E msg_id,DPARAM param_ptr);

MVOID McfIF_ShowEnterMcareTip(MVOID);

//---------------------------------------------------------------------------
//	FUNCTION: EntryNewScreen
//	Descrption:����һ���´���
//	Param:
//      IN    win_id:����ID  
//      IN    exitFunc:�˳����ڵĻص�����
//      IN    entryFunc:���봰�ڵĻص�����
//	Return: 
//      VOID
//    Note:
//----------------------------------------------------------------------------
MVOID McfScreen_EntryNewScreen(MINT win_id, pfnFunc exitFunc, pfnFunc entryFunc,MVOID* flag);

/*======================McfScreen.c API END========================*/  

/***************************************McfSim.c API begin**********************************************/

/*
*��ȡһ���Ѿ�������SIM��������EMSimCard
*
*/
MINT McfSim_GetSimReadyFirst(MVOID);

/******************************************************************************/
//	FUNCTION: McfIF_Sim_GetSimOperator
//	Descrption: �õ���Ч��sim��������
//	Param:  
//	IN:         simId:SIM��ID
//	OUT:        
//	Return:     ��Ӫ�̵�ID
//  Note: 
/******************************************************************************/
MINT McfIF_Sim_GetSimOperator(MINT simId);
/*
*�ж�ĳ��SIM�Ƿ�֧��
*/
MBOOL McfSim_IsVerify(MINT select_card);

/*
*�ж�ĳ��SIM���Ƿ��Ѿ�����
*/
MBOOL McfSim_IsReady(MINT select_card);

MN_DUAL_SYS_E McfSim_GetSimID(MINT simId);

MINT McfSim_GetCardID(MN_DUAL_SYS_E dual_sys);

MINT McfIF_Sim_GetValidSim(MVOID);

/***************************************McfSim.c API end**********************************************/


/***************************************McfSms.c API begin************************************************/
/*************************************
* McfIF_Sms_WriteMsg
* @pNumber --[in]destination number
* @pContent --[in]msg default content,not support now@0909,arlo
* @nSize --[in]size of msg default content,not support now
* #return none
*/
MVOID McfIF_Sms_WriteMsg(MCHAR* pNumber, MINT nNumberCnt, MWCHAR* pContent, MINT nSize);

#ifdef SPREAD_NEW_SMS_UPDATE
MINT McfIF_Sms_SendSms(MVOID* pUser, MINT simId, MINT nOption, const MCHAR* pNumber, const MCHAR* pData,
					MINT dataSize, MINT encodeType, pfnMcfSmsSendCb sendSmsCb);
#else//#ifdef SPREAD_NEW_SMS_UPDATE
MVOID  MAPI_MMISMS_SetNewMsgTOSend(MVOID);

MBOOL MAPI_MMISMS_AppSendSmsForMSD(MINT simId, MWCHAR* sms_content, MINT sms_len,
                                                    MBYTE* dest_addr_ptr, MBYTE dest_addr_len);
#endif//#ifdef SPREAD_NEW_SMS_UPDATE

#ifdef SPREAD_NEW_SMS_UPDATE
MVOID McfIF_SmsCallback(MBOOL result, MVOID* pUser);
#else
MVOID McfIF_SmsCallback(MBOOL result);
#endif


MBOOL McfIF_SmsGetMcareStatus(MVOID);

MVOID McfIF_SmsSetMcareStatus(MBOOL is_mcare);

/***************************************McfSms.c API end************************************************/


/*=======================McfSocket.c API BEGIN==========================*/ 

MBOOL McfIF_IsFlyMode(MVOID);
MINT McfSoc_ErrorNo(MVOID);
MINT McfSoc_WouldBlock(MVOID);
/*=======================McfSocket.c API End==========================*/ 

/***************************************McfStdlib.c API begin************************************************/
MUINT MAPI_strlen(const MCHAR* pStr);
MINT MAPI_strcmp(const MCHAR* pStr0, const MCHAR* pStr1);
MINT MAPI_stricmp(const MCHAR* pStr0,  const MCHAR* pStr1);
MCHAR* MAPI_strcat(MCHAR* pStrDest,  const MCHAR* pStrSrc);
MCHAR* MAPI_strcpy(MCHAR* pStrDest,  const MCHAR* pStrSrc);
MCHAR* MAPI_strncpy(MCHAR* pStrDest, const MCHAR* pStrSrc, MUINT nSize);
MCHAR* MAPI_strchr(const MCHAR* pStr,  MINT chr);//����ΪMINT
MCHAR* MAPI_strstr(const MCHAR* pStr, const MCHAR* pStrSearch);
MINT MAPI_atoi(const MCHAR* pStr);
MCHAR* MAPI_MMIAPICOM_Strlower(MCHAR* pStr);
MCHAR* MAPI_MMIAPICOM_Strupper(MCHAR* pStr);
MUINT MAPI_MMIAPICOM_Wstrlen(const MWCHAR* pwstr);
MINT MAPI_MMIAPICOM_Wstrcmp(const MWCHAR* pwstr0,const MWCHAR* pwstr1);
MWCHAR* MAPI_MMIAPICOM_Wstrlower(MWCHAR* pwstr);
MWCHAR* MAPI_MMIAPICOM_Wstrcat(MWCHAR* dest_str,const MWCHAR* src_str);
MWCHAR* MAPI_MMIAPICOM_Wstrcpy(MWCHAR* pwstrDest, const MWCHAR* pwstrSrc);
MWCHAR* MAPI_McfStdC_wcschr(const MWCHAR* str1_ptr, MUINT str1_len, MWCHAR* str2_ptr, MUINT str2_len);
MINT MAPI_MMIAPICOM_Wstrncmp(const MWCHAR* pwstr0,const MWCHAR* pwstr1,MINT icount);
MWCHAR* MAPI_MMIAPICOM_Wstrncpy(MWCHAR* pwstrDest,const MWCHAR* pwstrSrc,MINT icount);
MWCHAR* MAPI_MMIAPICOM_Wstrncat(MWCHAR* pwstrDest,const MWCHAR* pwstrSrc,MINT icount);
MWCHAR* MAPI_MMIAPICOM_Wstrupper(MWCHAR* pwstr);
MWCHAR* MAPI_MMIAPICOM_StrToWstr(const MCHAR* pstrSrc, MWCHAR* pwstrDest);
MCHAR* MAPI_MMIAPICOM_WstrToStr(const MWCHAR* pwstrSrc,MCHAR* pstrDest);
MINT MAPI_memcmp(const MVOID* buf1, const MVOID* buf2, MUINT  count);
MVOID* MAPI_memcpy(MVOID* pDst, const MVOID* pSrc, MUINT count);
MVOID* MAPI_memset(MVOID* buf1, MINT set_value, MUINT  count);
MVOID* MAPI_memchr(const MVOID* buf1, MINT chr, MUINT  count);
MVOID* MAPI_memmove(MVOID* pDst, const MVOID* pSrc, MUINT count);

MVOID MAPI_srand (MUINT seed);
MINT MAPI_rand(MVOID);
MINT MAPI__vsnprintf(MCHAR* pstrDest,MUINT nSize, const MCHAR* pstrFmt, va_list aList);

/***************************************McfStdlib.c API end************************************************/

/***************************************McfTelephone.c API beging****************************************/

MINT McfIF_GetCallNormalCall(MVOID);
MINT MAPI_MMIAPICC_MakeCall(MINT simId,const MCHAR* pNumber,MINT call_sim_type);  //nicklei add;
MINT McfCmn_GetCallType(MINT simId);

/***************************************McfTelephone.c API end*****************************************/


/***************************************McfTime.c API end************************************************/
ERR_TM_E McfTm_GetError(MVOID);
ERR_TM_E MAPI_TM_GetSysDate(SCI_DATE_T  *date_ptr );
ERR_TM_E MAPI_TM_GetSysTime(SCI_TIME_T  *time_ptr);

MUINT MAPI_SCI_GetTickCount(MVOID)    ;
/***************************************McfTime.c API end************************************************/


/*=======================McfTimer.c API Begin==========================*/ 
typedef MVOID (*MCF_TIMER_FUNC)(MBYTE  timer_id, MUINT param);

MBOOL McfIF_HandleTimer(MVOID* param);

MBYTE McfTm_CreateTimer(MUINT interval, MCF_TIMER_FUNC func, MBOOL is_period);

MMI_HANDLE_T McfTm_GetIdleWinId(MVOID);

/*=======================McfTimer.c API End==========================*/ 


//--------------------------------Layer-begin----------------------------------

#ifdef MCARE_LAYER_SUPPORT
GUI_LCD_DEV_INFO McfCmn_GetMcareLayerInfo(MVOID);

MUINT McfCmn_GetMcareLayerBlockID(MVOID);
#endif

MVOID McfIF_CreateMcareLayer(MMI_WIN_ID_T win_id);

MVOID McfIF_ReleaseMcareLayer(MVOID);
 
MBOOL McfIF_IsLayerActive(MVOID);
       
MBOOL McfIF_GetIsMcareLayerExit(MVOID);

MVOID McfIF_SetIsMcareLayerExit(MBOOL is_exit);
//--------------------------------Layer-end------------------------------------
/***************************************McfUsb.c API begin************************************************/
MBOOL MAPI_MMIAPIUDISK_UdiskIsRun(MVOID);
/***************************************McfUsb.c API end*************************************************/


/***************************************McfVibrate.c API begin *********************************************/
#ifdef SPREAD_NEW_AUDIO_UPDATE

MVOID McfIF_Vib_PlayVibratorOnce(MVOID);

#else//#ifdef SPREAD_NEW_AUDIO_UPDATE

MINT MAPI_GetOtherSetRingType(MVOID);
MBOOL MAPI_MMIAPISET_IsVibratTimerActive(MVOID);
MVOID MAPI_MMIAPISET_StopVibratTimer(MVOID);
MBOOL MAPI_MMIAUDIO_Vibrate(MUINT vibrate_time);
MINT McfVib_GetVibTime(MVOID);
MVOID McfVib_StopInvalidVibrator(MVOID);

#endif//#ifdef SPREAD_NEW_AUDIO_UPDATE

/***************************************McfVibrate.c API end***********************************************/


/***************************************McfZlib.c API begin************************************************/
/*
MINT MAPI_inflateInit_(z_streamp z, const MCHAR* version, MINT stream_size);

MINT MAPI_inflateInit2_(z_streamp z,MINT w,const MCHAR* version,MINT stream_size);

MINT MAPI_inflate(z_streamp z, MINT f);

MINT MAPI_inflateReset(z_streamp z);

MINT MAPI_inflateEnd(z_streamp z);

MINT MAPI_appzlib_register(_alloc_func alloc_hdlr, _free_func free_hdlr);*/
/***************************************McfZlib.c API end************************************************/
/***************************************Lib API end************************************************/
MUINT McfIF_GetDataBufferSize(MVOID);
/***************************************Lib API end************************************************/

#if (defined SPREAD_PLAT_SC6800H_PDA) || (defined SPREAD_PLAT_SC6530_PDA)
MUINT McfIF_GetSoftbarHeight(MVOID);
MUINT McfIF_GetFullSoftbarHeight(MVOID);
MVOID McfIF_ScreenDestryCtl(MVOID);
#else
MUINT McfIF_GetSoftbarHeight(MVOID);
MUINT McfIF_GetFullSoftbarHeight(MVOID);
MVOID McfIF_ScreenDestryCtl(MVOID);
#endif

MBOOL McfCmn_IsKeyMsg(MUINT msg_id);
MBOOL McfCmn_IsTPMsg(MUINT msg_id);
MUINT McfIF_GetMmiResult(MBOOL is_true);

MBOOL __McfIF_GetKeyDownSlide(MVOID);
MVOID __McfIF_SetKeyDownSlide(MBOOL isKeyDownSlide);
MBOOL McfIF_GetKeyDownSlide(MVOID);
MVOID McfIF_SetKeyDownSlide(MBOOL isKeyDownSlide);

#if (defined SPREAD_PLAT_SC6600L_1025) || (defined SPREAD_PLAT_SC6600L_0937)||(defined SPREAD_PLAT_SC6620)
MVOID McfIF_NotifyGifDecodeOneFrame(IMGPROC_GIF_DISPLAY_INFO_T* display_info_ptr);
MBOOL McfIF_IsContinueDecodeGif(MVOID);
MBOOL McfIF_IsAppDecodeGif(MVOID);
#endif

MVOID McfIF_InvalidelCACHEALL(MVOID);

MVOID Mcare_Udisk_Start(MVOID);

MBOOL MCareV31_Close_ExitConfirm_PubWin(void);


#ifdef MMI_TASK_MANAGER
MVOID MCare_StartApplet(MINT applet_type);
MVOID McfIF_RegAppletInfo(MVOID);
MVOID MCare_StopApplet(MVOID);
#endif

/*****************************************************************************/
//  Description : get the type of language 
//  Global resource dependence : none
//  Author: Jassmine.Meng
//  Note:
/*****************************************************************************/
MVOID MAPI_MMIAPISET_GetLanguageType(MINT* language_type_ptr);


MVOID McfIF_Init(MVOID);

MVOID McfIF_Deinit(MVOID);

MVOID Mcare_Exit_Exception(MVOID);

MBOOL McfRecord_CheckPermit(MVOID);

MINT McfIF_GetCurAngle(MVOID);

MVOID McareV31_PreEntry(MVOID);

MVOID McareV31_DataSyn(MVOID);

MVOID McareV31_DataSyn_FileCopy(const MWCHAR* pwcDesPath, const MWCHAR* pwcSrcPath, const MWCHAR* pwcFileType);

MVOID McareV31_DoEndKey(MVOID);

MVOID McfIF_ShowActiveGPRSScreen(MVOID);

MVOID McfIF_CloseActiveGPRSScreenAndEntry(MVOID);
MVOID McfIF_CloseActiveGPRSScreenAndWarnnig(MVOID);

MVOID McfIF_CloseActiveGPRSScreen(MVOID);

MVOID McfIF_ApnInit(MVOID);

MINT McfIF_ReadApnFromFile(MINT *pSimId, MINT *pApnType,MCHAR *pApnName);
MINT McfIF_WriteApnToFile(MINT nSimId, MINT nApnType,MCHAR *pApnName);
MVOID McfIF_CloseGPRS(MVOID);
MINT McfIF_GetGPRSSIMID(MVOID);

MBOOL McfIF_DisplayOKCancle(MUSHORT* wstr);
MBOOL McfIF_CloseOKCancle(void);
MBOOL Mcf_SupportMultiBG(MVOID);
MINT McfIF_GetInvalidFSHandle(MVOID);
MINT McfIF_GetFSErrorNone(MVOID);


MBOOL McfIF_SupportQCBG(MVOID);
MBOOL McfIF_SupportQQBG(MVOID);

MBOOL McfIF_SetSIMPriority(MVOID);
MVOID McfIF_IdleSatuBarInit(MINT iconFlag);

MVOID McfIF_UpdateStatusIcons(MINT iconFlag, MINT iconID);

#if (defined SPREAD_PLAT_SC6800H) || (defined SPREAD_PLAT_SC8800G) || (defined SPREAD_PLAT_SC6530) || (defined SPREAD_PLAT_SC8800G_PDA)
MBOOL McfIF_Dtcnt_ActiveWifi(pfnHandleFunc pfn);
#endif

#if (defined SPREAD_PLAT_SC6800H) || (defined SPREAD_PLAT_SC6610) || (defined SPREAD_PLAT_SC8800G)\
|| (defined SPREAD_PLAT_SC6600L) || (defined SPREAD_PLAT_SC6620) || (defined SPREAD_PLAT_SC6530) || (defined SPREAD_PLAT_SC8800G_PDA)
MBOOL McfIF_Dtcnt_ActiveGprs(MINT select_card, MBYTE* APNcn, pfnHandleFunc pfn);
#endif


/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/

#ifdef __cplusplus
    }
#endif


#endif//_MCFINTERFACE_H
