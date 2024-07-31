#ifdef KURO_SUPPORT
/*****************************************************************************
** File Name:      klrfunction.h                                            *
** Author:                                                                   *
** Date:           10/2007                                                   *
** Description:    This file is used to kuro functions                       *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 10/2007       cmzs           Create
******************************************************************************/

#ifndef _KLRFUNCTION_H_
#define _KLRFUNCTION_H_ 

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "ffs.h"
#include "decode.h"
 //#include "mmimultim.h"
#include "sci_types.h"
#include "mmifmm_export.h"
#include "mmi_common.h"
#include "mmi_nv.h"
/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif
/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#define GENDER_MALE	0
#define GENDER_FEMALE	1
#define GENDER_ALL		2
#define KURO_SYSDAT_NVSIZE 512
#define KGC_SONGID_LENGTH 32
#define KURO_FULL_PATH_MAX_LEN           60 //120 //byte
/**--------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                *
 **--------------------------------------------------------------------------*/
/**--------------------------------------------------------------------------*
 **                             Sys.Dat Information                          *
 **--------------------------------------------------------------------------*/
typedef struct {
	// Picture parameter
	uint16 ScreenWidth;		// picture width in pixels
	uint16 ScreenHeight;		// picture height in pixels
	BYTE LyricWidth; 			// lyric display width in Chinese Characters
	// Picture format
	BYTE PicFormat;  			// 0 = KURO proprietary, 1 = 8b Windows BITMAP, 2 = 16b Windows BITMAP, 3 = 24b Windows BITMAP, 4 = JPEG
	//Lyric Language
	BYTE LyricLanguage ;  		// bit 0 = 1, UNICODE simplified Chinese, bit 1 = 1, UNICODE traditional Chinese, bit 2 = 1, ANSI simplified Chinese, bit 3 =1, ANSI traditional Chinese
	// Music Format
	BYTE MusicFormat;  			// 0 = WMA, 1 = MP3
  	//1－代表高位字节在前，0－代表低位字节在前
  	BYTE BigEndian;
    wchar default_disk[KURO_FULL_PATH_MAX_LEN + 1];  //default system disk(like c: or d:);
}KURO_DEV_INFO;
typedef struct _KGC_INFO
{
	uint8 songid[KGC_SONGID_LENGTH];
	uint8 playcount;
}KGC_INFO;
/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
// 	Description : open kuru lyric
//	Global resource dependence : 
//  Author: cmzs
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIKLR_OpenLyric(uint16 *klr_full_path);
/*****************************************************************************/
// 	Description : release klr buffers(请在退出歌词显示功能时调用)
//  Author: cmzs
//	Note:
/*****************************************************************************/
PUBLIC void MMIKLR_ReleaseLrc(void);
/*****************************************************************************/
// 	Description :获取某行歌词
// @param [in]nLine		行号
// @param [out]szLyric	歌词
//  Author: cmzs
//*****************************************************************************/
PUBLIC char* MMIKLR_GetLyric(int32 nLine);
/*****************************************************************************/
/**
* 针对逐字显示
* 依据时间， 获取当前播放的行数 (二行)
*
* @param [in]lMSec		时间 (毫秒)
* @param [out]nLine1	第一行行号
* @param [out]nPercent1	第一行所在位置 (百分比: 0 - 100)
* @param [out]nSex1		第一行声音 (0-男声，1-女声， 2-合唱)
* @param [out]nLine2	第二行行号
* @param [out]nPercent2	第二行所在位置 (百分比: 0 - 100)
* @param [out]nSex2		第二行声音 (0-男声，1-女声， 2-合唱)
*/
//  Author:cmzs
/*****************************************************************************/
PUBLIC void MMIKLR_ShowLyric2(int32 lMSec, int32* pnLine1, int32* pnPercent1, int32* pnSex1, int32* pnLine2, int32* pnPercent2, int32* pnSex2);
/*****************************************************************************/
/**
* 针对全屏
* 依据时间， 获取当前播放的行数 (二行)
*
* @param [in]lMSec		时间 (毫秒)
* @param [out]nLine1	第一行行号
* @param [out]nPercent1	第一行所在位置 (百分比: 0 - 100)
* @param [out]nSex1		第一行声音 (0-男声，1-女声， 2-合唱)
* @param [out]nLine2	第二行行号
* @param [out]nPercent2	第二行所在位置 (百分比: 0 - 100)
* @param [out]nSex2		第二行声音 (0-男声，1-女声， 2-合唱)
*/
//  Author:cmzs
/*****************************************************************************/
void MMIKLR_ShowLyric3(uint32 lMSec, int32* pnLine1, int32* pnPercent1, int32* pnSex1, int32* pnLine2, int32* pnPercent2, int32* pnSex2);
/*****************************************************************************/
// 	Description : get the kuru picture index by music time
//	Global resource dependence : 
//  Author:cmzs
/*****************************************************************************/
PUBLIC uint32 MMIKLR_GetPicIndex(int32 lMSec,int32 *next_pic_index,uint32 *pic_all_count);

/*****************************************************************************/
// 	Description : get the kuru picture data block by pic index.
//	Global resource dependence : 
//  Author:cmzs
/*****************************************************************************/
PUBLIC BOOLEAN MMIKLR_GetPicData_BlockByIndex(uint32 npicindex,void *kur_bmp_buffer,int32 nBlockSize,uint32 nBufferSize,BOOLEAN *bAllLoaded);

/*****************************************************************************/
// 	Description : get the kuru picture data block by time of music.
//	Global resource dependence : 
//  Author:cmzs
/*****************************************************************************/
PUBLIC BOOLEAN MMIKLR_GetPicData_Block(int32 lMSec,BOOLEAN *ispicchanged,void *kur_bmp_buffer,int32 nBlockSize,uint32 nBufferSize);

/*****************************************************************************/
// 	Description : get the kuru picture data by the index of picture.
//	Global resource dependence : 
//  Author:cmzs
/*****************************************************************************/
PUBLIC BOOLEAN MMIKLR_GetPicData(int32 lMSec,BOOLEAN *ispicchanged,T_GUI_Bitmap *kur_bmp_info);
/*****************************************************************************/
// 	Description : 设备启动时进行Kuro相关的初始化动作
//	Global resource dependence : none
//  Author: cmzs
//	Note: 
/******************** *********************************************************/
PUBLIC void MMIKUR_ConfigInit(KURO_DEV_INFO *psysdat);
/*****************************************************************************/
// 	Description : 启动到U盘时写入设备相关配置信息
//	Global resource dependence : none
//  Author: cmzs
//	Note: 
/******************** *********************************************************/
PUBLIC void MMIKUR_ExportSysInfo(void);
/*****************************************************************************/
// 	Description : Get Kuro Device ID
//	Global resource dependence : none
//  Author: cmzs
//	Note: 
/******************** *********************************************************/
PUBLIC BOOLEAN MMIKUR_GetDeviceID(char *devicdidbuffer,uint16 nbufferlen);

/**
* 解密数据
*
* @param pBuf	指向需要解密的数据buffer
* @param nBufLength	需要解密的数据长度(要求nBufLength是256的整数倍数)
* @param nOffsetInFile	指出该解密数据在文件中的位置（以便优化，比如：只有部分的buffer才加密，节省PC计算时间）
* @param nFormat			给出是WMA还是MP3格式 0:Mp3 1:Wma
*
* @return  大于0表示操作成功
*/
int KuroDecode(unsigned char* pBuf, int nBufLength, unsigned long nOffsetInFile, int nFormat);

/*****************************************************************************/
// 	Description : MMIKUR_GetKurHeaderSize
//	Global resource dependence : none
//  Author: cmzs
//	Note: 
/******************** *********************************************************/
PUBLIC uint32 MMIKUR_GetKurHeaderSize(uint16 *kur_file_path);
/*****************************************************************************/
// 	Description : add kur play count
//	Global resource dependence : none
//  Author: cmzs
//	Note: 
/*****************************************************************************/
PUBLIC void MMIKUR_AddKgcCount(void);
/*****************************************************************************/
// 	Description : export kgc info to kgc file
//	Global resource dependence : none
//  Author: cmzs
//	Note: 
/*****************************************************************************/
PUBLIC void MMIKUR_ExportKgcCount(void);

/*****************************************************************************/
// 	Description : 获取kur的采样率(to be called after MMIKUR_GetKurHeaderSize)
//	Global resource dependence : none
//  Author: cmzs
//	Note: 
/******************** *********************************************************/
PUBLIC uint32 MMIKUR_GetKurSampleRate(void);
/*****************************************************************************/
// 	Description : 获取kur的位速(to be called after MMIKUR_GetKurHeaderSize)
//	Global resource dependence : none
//  Author: cmzs
//	Note: 
/******************** *********************************************************/
PUBLIC uint32 MMIKUR_GetKurBitRate(void);
/*****************************************************************************/
// 	Description : 获取kur的id3 tag大小(to be called after MMIKUR_GetKurHeaderSize)
//	Global resource dependence : none
//  Author: cmzs
//	Note: 
/******************** *********************************************************/
PUBLIC uint32 MMIKUR_GetKurID3TagSize(void);


/*****************************************************************************/
// 	Description :Get Kgc info's Count From NV
//	Global resource dependence :  
//  Author:cmzs 
//	Note:
/*****************************************************************************/	
PUBLIC uint16 MMIKURO_GetKgcInfoCount(void);

/*****************************************************************************/
// 	Description :Get Kgc info
//	Global resource dependence :  
//  Author:cmzs 
//	Note:
/*****************************************************************************/	
PUBLIC BOOLEAN MMIKURO_GetKgcInfo(int16 nIndex,KGC_INFO *kgcinfo);

/*****************************************************************************/
// 	Description :Get Kgc info by sonid,if not found return -1 
//	Global resource dependence :  
//  Author:cmzs 
//	Note:
/*****************************************************************************/	
PUBLIC int16 MMIKURO_FindKgcInfo(uint8 *songid,KGC_INFO *kgcinfo);

/*****************************************************************************/
// 	Description :Add Kgc info
//	Global resource dependence :  
//  Author:cmzs 
//	Note:
/*****************************************************************************/	
PUBLIC BOOLEAN MMIKURO_AddKgcInfo(KGC_INFO *kgcinfo);

/*****************************************************************************/
// 	Description :Change Kgc info
//	Global resource dependence :  
//  Author:cmzs 
//	Note:
/*****************************************************************************/	
PUBLIC BOOLEAN MMIKURO_ChangeKgcInfo(uint16 nIndex,KGC_INFO *kgcinfo);

/*****************************************************************************/
// 	Description : clear kgc info fromnv
//	Global resource dependence : none
//  Author: cmzs
//	Note: 
/*****************************************************************************/
PUBLIC void MMIKUR_ClearKgcCount(void);

#ifdef   __cplusplus
    }
#endif
#endif
#endif

