/*****************************************************************************
** File Name:      kurodepends.h                                                  *
** Author:                                                                   *
** Date:           04/2008                                                *
** Description:    define all functions that kuro.lib need                 *
*****************************************************************************/
#ifndef KURO_DEPENDS_H
#define KURO_DEPENDS_H
/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/

#ifdef __cplusplus
    extern   "C"
    {
#endif
/*****************************************************************************/
// 	Description : create file
//	Global resource dependence : 
//  Author: cmzs
//	Note:
/*****************************************************************************/ 
PUBLIC MMIFILE_HANDLE KUROLIB_CreateFile(
								  const uint16 *file_name, 
								  uint32 access_mode,
								  uint32 share_mode,	//Must be NULL,File System have not realize it 
								  uint32 file_attri		//Must be NULL,File System have not realize it 
									);
/*****************************************************************************/
// 	Description : get file size
//	Global resource dependence : 
//  Author: cmzs
//	Note:
/*****************************************************************************/ 
PUBLIC uint32 KUROLIB_GetFileSize(
						   MMIFILE_HANDLE file_handle
						   );

/*****************************************************************************/
// 	Description : set file pointer
//	Global resource dependence : 
//  Author: cmzs
//	Note:
/*****************************************************************************/ 
PUBLIC uint32 KUROLIB_SetFilePointer(
							  MMIFILE_HANDLE file_handle,
							  int32 offset,
							  uint32 origin
							  );
/*****************************************************************************/
// 	Description : read file
//	Global resource dependence : 
//  Author: cmzs
//	Note:
/*****************************************************************************/ 
PUBLIC MMIFILE_ERROR_E KUROLIB_ReadFile(
								 MMIFILE_HANDLE file_handle,
								 void *buffer,
								 uint32 bytes_to_read,
								 uint32 *bytes_read,
								 MMIFILE_OVERLAPPED_T *overlapped
								 );

/*****************************************************************************/
// 	Description : close file
//	Global resource dependence : 
//  Author: cmzs
//	Note:
/*****************************************************************************/ 
PUBLIC MMIFILE_ERROR_E KUROLIB_CloseFile(
								  MMIFILE_HANDLE file_handle
								  );
/*****************************************************************************/
// 	Description : close file
//	Global resource dependence : 
//  Author: cmzs
//	Note:
/*****************************************************************************/ 
PUBLIC MMIFILE_ERROR_E KUROLIB_CloseFile(
								  MMIFILE_HANDLE file_handle
								  );
/*****************************************************************************/
// 	Description : get wchar length
//	Global resource dependence :                                
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC uint32 KUROLIB_GetWcharLength( //return length in unit of uint16
							 wchar const * pwchar
							 );
/*****************************************************************************/
// 	Description : check whether file is exist
//	Global resource dependence : 
//  Author:cmzs
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN KUROLIB_IsFileExist(
                                   const wchar			*full_path_ptr,     //in
                                   uint16				full_path_len       //in
                                   );
/*****************************************************************************/
// 	Description : write file
//	Global resource dependence : 
//  Author: cmzs
//	Note:
/*****************************************************************************/ 
PUBLIC MMIFILE_ERROR_E KUROLIB_WriteFile(
								  MMIFILE_HANDLE file_handle,
								  const void *buffer,
								  uint32 bytes_to_write,
								  uint32 *bytes_written,
								  MMIFILE_OVERLAPPED_T *overlapped
								  );
/*****************************************************************************/
// 	Description : delete file
//	Global resource dependence : 
//  Author: cmzs
//	Note:
/*****************************************************************************/ 
PUBLIC MMIFILE_ERROR_E KUROLIB_DeleteFile(
								   const uint16 *name,
								   MMIFILE_OVERLAPPED_T *overlapped_ptr
								   );
/*****************************************************************************/
// 	Description :从NV中将kuro加密所需要的参数(512byte)取出
//	Global resource dependence :  
//  Author:cmzs 
//	Note:
/*****************************************************************************/	
PUBLIC BOOLEAN KUROLIB_ReadSysDat(uint8 *psysdat);
/*****************************************************************************/
// 	Description :将kuro加密所需要的参数(512byte)写入系统保护区
//	Global resource dependence :  
//  Author:cmzs 
//	Note:
/*****************************************************************************/	
PUBLIC BOOLEAN KUROLIB_WriteSysDat(uint8 *psysdat);
/*****************************************************************************/
// 	Description :将kuro Device ID从NV中读出
//	Global resource dependence :  
//  Author:cmzs 
//	Note:
/*****************************************************************************/	
PUBLIC BOOLEAN KUROLIB_ReadIgrsKey(uint8 *igrskey);
/*****************************************************************************/
// 	Description :将kuro Device ID写入NV中
//	Global resource dependence :  
//  Author:cmzs 
//	Note:
/*****************************************************************************/	
PUBLIC BOOLEAN KUROLIB_WriteIgrsKey(uint8 *pigrskey);
/*****************************************************************************/
// 	Description :返回客户自行定义的kuro device id(一般是与Imei号绑定)
//	Global resource dependence :  
//  Author:cmzs 
//	Note:
/*****************************************************************************/	
PUBLIC BOOLEAN KUROLIB_GetKuroID(char *a_str,uint8 a_length);

/*****************************************************************************/
// 	Description :Get Kgc info by sonid,if not found return -1 
//	Global resource dependence :  
//  Author:cmzs 
//	Note:
/*****************************************************************************/	
PUBLIC int16 KUROLIB_FindKgcInfo(uint8 *songid,KGC_INFO *kgcinfo);
/*****************************************************************************/
// 	Description :Change Kgc info
//	Global resource dependence :  
//  Author:cmzs 
//	Note:
/*****************************************************************************/	
PUBLIC BOOLEAN KUROLIB_ChangeKgcInfo(uint16 nIndex,KGC_INFO *kgcinfo);
/*****************************************************************************/
// 	Description :Add Kgc info
//	Global resource dependence :  
//  Author:cmzs 
//	Note:
/*****************************************************************************/	
PUBLIC BOOLEAN KUROLIB_AddKgcInfo(KGC_INFO *kgcinfo);
// 	Description :Get Kgc info's Count From NV
//	Global resource dependence :  
//  Author:cmzs 
//	Note:
/*****************************************************************************/	
PUBLIC uint16 KUROLIB_GetKgcInfoCount();
/*****************************************************************************/
// 	Description :Get Kgc info
//	Global resource dependence :  
//  Author:cmzs 
//	Note:
/*****************************************************************************/	
PUBLIC BOOLEAN KUROLIB_GetKgcInfo(int16 nIndex,KGC_INFO *kgcinfo);

/*****************************************************************************/
// 	Description :Set Usb Vender String And ProductStr
//	Global resource dependence :  
//  Author:cmzs 
//	Note:
/*****************************************************************************/	
PUBLIC BOOLEAN KUROLIB_SetUsbInfo(char *usbinfo,uint32 ninfolength);

/*****************************************************************************/
// 	Description : gb to wstr
//	Global resource dependence : none
//  Author:
//	Note: 
/*****************************************************************************/
PUBLIC uint16 KUROLIB_GB2UCS(					//return the number of UCS2 code that have been converted.
                   wchar *wstr_ptr, 	//destination: string pointer to UCS2 string.
                   const uint8 *gb_ptr, //source: string pointer to GB string
                   uint16 len			//the length to convert from GB to UCS2.
                   );

#ifdef __cplusplus
}
#endif
#endif