/*****************************************************************************
** File Name:      kuro_depends.c                                            *
** Author:                                                                   *
** Date:           01/04/2008                                                *
** Description:    functions that kuro.lib needed                            *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 04/2008       cmzs              Create                                  *
******************************************************************************/
#define _KURO_DEPENDS_C
/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "std_header.h"
#ifdef KURO_SUPPORT
#include "klrfunction.h"
#include "mmifmm_export.h"
#include "mmikur_internal.h"

#ifndef WIN32
#include "upm_config.h"
#endif
/*****************************************************************************/
// 	Description :Get Kgc info by sonid,if not found return -1 
//	Global resource dependence :  
//  Author:cmzs 
//	Note:
/*****************************************************************************/	
PUBLIC int16 KUROLIB_FindKgcInfo(uint8 *songid,KGC_INFO* kgcinfo)
{
	return MMIKURO_FindKgcInfo(songid,kgcinfo);
}

/*****************************************************************************/
// 	Description : create file
//	Global resource dependence : 
//  Author: songbin.zeng
//	Note:
/*****************************************************************************/ 
PUBLIC MMIFILE_HANDLE KUROLIB_CreateFile(
								  const uint16 *file_name, 
								  uint32 access_mode,
								  uint32 share_mode,	//Must be NULL,File System have not realize it 
								  uint32 file_attri		//Must be NULL,File System have not realize it 
								  )
{
	return MMIAPIFMM_CreateFile(
		file_name, 
		access_mode,
		share_mode,	//Must be NULL,File System have not realize it 
		file_attri		//Must be NULL,File System have not realize it 
		);
}


/*****************************************************************************/
// 	Description : get file size
//	Global resource dependence : 
//  Author: songbin.zeng
//	Note:
/*****************************************************************************/ 
PUBLIC uint32 KUROLIB_GetFileSize(
						   MMIFILE_HANDLE file_handle
						   )
{
	return MMIAPIFMM_GetFileSize(file_handle);
}

/*****************************************************************************/
// 	Description : set file pointer
//	Global resource dependence : 
//  Author: songbin.zeng
//	Note:
/*****************************************************************************/ 
PUBLIC uint32 KUROLIB_SetFilePointer(
							  MMIFILE_HANDLE file_handle,
							  int32 offset,
							  uint32 origin
							  )
{
	return MMIAPIFMM_SetFilePointer(
		file_handle,
		offset,
		origin
		);

}


/*****************************************************************************/
// 	Description : read file
//	Global resource dependence : 
//  Author: songbin.zeng
//	Note:
/*****************************************************************************/ 
PUBLIC MMIFILE_ERROR_E KUROLIB_ReadFile(
								 MMIFILE_HANDLE file_handle,
								 void *buffer,
								 uint32 bytes_to_read,
								 uint32 *bytes_read,
								 MMIFILE_OVERLAPPED_T *overlapped
								 )
{

    if(SFS_INVALID_HANDLE == file_handle)
    {
        return SFS_ERROR_INVALID_PARAM;
    }
    else
    {
        return MMIAPIFMM_ReadFile(
                    file_handle,
                    buffer,
                    bytes_to_read,
                    bytes_read,
                    overlapped
                    );
    }
}

/*****************************************************************************/
// 	Description : close file
//	Global resource dependence : 
//  Author: songbin.zeng
//	Note:
/*****************************************************************************/ 
PUBLIC MMIFILE_ERROR_E KUROLIB_CloseFile(
								  MMIFILE_HANDLE file_handle
								  )
{
	return MMIAPIFMM_CloseFile(
		file_handle
							);
}

/*****************************************************************************/
// 	Description : get wchar length
//	Global resource dependence :                                
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC uint32 KUROLIB_GetWcharLength( //return length in unit of uint16
							  wchar const * pwchar
							  )
{
	return MMIAPICOM_Wstrlen(pwchar);
}
/*****************************************************************************/
// 	Description : check whether file is exist
//	Global resource dependence : 
//  Author:liqing.peng
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN KUROLIB_IsFileExist(
                                   const wchar			*full_path_ptr,     //in
                                   uint16				full_path_len       //in
                                   )
{
    return MMIAPIFMM_IsFileExist(
        full_path_ptr,     //in
        full_path_len       //in
        );
    
}
/*****************************************************************************/
// 	Description : write file
//	Global resource dependence : 
//  Author: songbin.zeng
//	Note:
/*****************************************************************************/ 
PUBLIC MMIFILE_ERROR_E KUROLIB_WriteFile(
								  MMIFILE_HANDLE file_handle,
								  const void *buffer,
								  uint32 bytes_to_write,
								  uint32 *bytes_written,
								  MMIFILE_OVERLAPPED_T *overlapped
								  )
{
	return MMIAPIFMM_WriteFile(
		file_handle,
		buffer,
		bytes_to_write,
		bytes_written,
		overlapped
		);
}
/*****************************************************************************/
// 	Description : delete file
//	Global resource dependence : 
//  Author: songbin.zeng
//	Note:
/*****************************************************************************/ 
MMIFILE_ERROR_E KUROLIB_DeleteFile(
								   const uint16 *name,
								   MMIFILE_OVERLAPPED_T *overlapped_ptr
								   )
{
	return MMIAPIFMM_DeleteFile(
		name,
		overlapped_ptr
		);
}

/*****************************************************************************/
// 	Description :Change Kgc info
//	Global resource dependence :  
//  Author:cmzs 
//	Note:
/*****************************************************************************/	
PUBLIC BOOLEAN KUROLIB_ChangeKgcInfo(uint16 nIndex,KGC_INFO *kgcinfo)
{
	return MMIKURO_ChangeKgcInfo(nIndex,kgcinfo);
}

/*****************************************************************************/
// 	Description :Add Kgc info
//	Global resource dependence :  
//  Author:cmzs 
//	Note:
/*****************************************************************************/	
PUBLIC BOOLEAN KUROLIB_AddKgcInfo(KGC_INFO *kgcinfo)
{
	return MMIKURO_AddKgcInfo(kgcinfo);
}
/*****************************************************************************/
// 	Description :Get Kgc info's Count From NV
//	Global resource dependence :  
//  Author:cmzs 
//	Note:
/*****************************************************************************/	
PUBLIC uint16 KUROLIB_GetKgcInfoCount()
{
	return MMIKURO_GetKgcInfoCount();
}

/*****************************************************************************/
// 	Description :Get Kgc info
//	Global resource dependence :  
//  Author:cmzs 
//	Note:
/*****************************************************************************/	
PUBLIC BOOLEAN KUROLIB_GetKgcInfo(int16 nIndex,KGC_INFO *kgcinfo)
{
	return MMIKURO_GetKgcInfo(nIndex,kgcinfo);
}

/*****************************************************************************/
// 	Description :Set Usb Vender String And ProductStr
//	Global resource dependence :  
//  Author:cmzs 
//	Note:
/*****************************************************************************/	
PUBLIC BOOLEAN KUROLIB_SetUsbInfo(char *usbinfo,uint32 ninfolength)
{
	//arvinzhang modify:delete assert
	//SCI_ASSERT(ninfolength >= 23);
	if(ninfolength < 23)
	{
		return FALSE;
	}
	
#ifndef WIN32
	if(UPM_MASS_STORAGE != UPM_GetService())
	{
		UBOT_SetVenderStr(usbinfo,8);
		UBOT_SetProductIDStrNand(&usbinfo[7],16);
		UBOT_SetProductIDStrSD(&usbinfo[7],16);
	}
#endif
	return TRUE;
}


/*****************************************************************************/
// 	Description :从NV中将kuro加密所需要的参数(512byte)取出
//	Global resource dependence :  
//  Author:cmzs 
//	Note:
/*****************************************************************************/	
PUBLIC BOOLEAN KUROLIB_ReadSysDat(uint8 *psysdat)
{
	return FALSE;//ReadSysDatFromNV(psysdat);
}

/*****************************************************************************/
// 	Description :将kuro加密所需要的参数(512byte)写入系统保护区
//	Global resource dependence :  
//  Author:cmzs 
//	Note:
/*****************************************************************************/	
PUBLIC BOOLEAN KUROLIB_WriteSysDat(uint8 *psysdat)
{
	return WriteSysDatToNV(psysdat);
}

/*****************************************************************************/
// 	Description :将kuro Device ID从NV中读出(23 byte)
//	Global resource dependence :  
//  Author:cmzs 
//	Note:
/*****************************************************************************/	
PUBLIC BOOLEAN KUROLIB_ReadIgrsKey(uint8 *igrskey)
{
	return ReadIgrsKeyFromNV(igrskey);
}

/*****************************************************************************/
// 	Description :将kuro Device ID写入NV中(23 byte)
//	Global resource dependence :  
//  Author:cmzs 
//	Note:
/*****************************************************************************/	
PUBLIC BOOLEAN KUROLIB_WriteIgrsKey(uint8 *pigrskey)
{
	return WriteIgrsKeyToNV(pigrskey);
}

/*****************************************************************************/
// 	Description :返回客户自行定义的kuro device id(一般是与Imei号绑定)
//	Global resource dependence :  
//  Author:cmzs 
//	Note:
/*****************************************************************************/	
PUBLIC BOOLEAN KUROLIB_GetKuroID(char *a_str,uint8 a_length)
{
	return MMIKUR_GetKuroID(a_str,a_length);
}

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
                   )
{
    return GUI_GBToWstr(wstr_ptr, gb_ptr, len);
}

#endif