/****************************************************************************
** File Name:      mmigps_osapi.h                                          *
** Author:                                                                 *
** Date:                                                                   *
** Copyright:      2005 Spreadtrum, Incorporated. All Rights Reserved.     *
** Description:    This file is used to describe the cryp display menu     *
****************************************************************************/
#ifndef OSAPI_H
#define OSAPI_H
#include "os_api.h"
//#include "guilcd.h"
//#include "sfs.h"
//#include "lcd.h"
#include "tx_api_thumb.h"
#include "threadx_os.h"
#include "chng_freq.h"
// shengyin
// 手写定义
//手写识别范围
#define IDENTIFY_LOWWER_CHAR            0x1     //小写英文
#define IDENTIFY_UPPER_CHAR             0x2     //大写英文
#define IDENTIFY_DIGIT                  0x04    //数字
#define IDENTIFY_PUNCTUATION            0x08    //常用标点, 8个, 包括:, 。 、 ? ! " : ;
#define IDENTIFY_PUNCTUATION_EXTEND     0x10    //扩展标点, 包括:    ' (…〈·〉《 )-》
#define IDENTIFY_SYMBOL                 0x20    //常用符号, 包括: ￥ # . = / > $ - < * @ &
#define IDENTIFY_SYMBOL_EXTEND          0x40    //扩展符号, 包括: ~ { } ^ ` \ ] _ [ |
#define IDENTIFY_GESTURE                0x80    //手势4个, 包括: 空格(0x20)、回车(0x0d)、回删(0x08)、删除(0x10)
#define IDENTIFY_COMPONMENT             0x100   //偏旁部首
#define IDENTIFY_GB_LEVEL_ONE           0x200   //GB2312一级国标简体汉字
#define IDENTIFY_GB_LEVEL_TWO           0x400   //GB2312二级国标简体汉字
#define IDENTIFY_BIG5_LEVEL_ONE         0x800   //繁体字一级
#define IDENTIFY_BIG5_LEVEL_TWO         0x1000  //繁体字二级
#define IDENTIFY_GBK                    0x2000  //GBK

#define IDENTIFY_CHAR           (IDENTIFY_LOWWER_CHAR | IDENTIFY_UPPER_CHAR)
#define IDENTIFY_CHN            (IDENTIFY_GB_LEVEL_ONE | IDENTIFY_GB_LEVEL_TWO)
#define IDENTIFY_CHS            (IDENTIFY_BIG5_LEVEL_ONE | IDENTIFY_BIG5_LEVEL_TWO)

// GPS MAP memory used size
#define MMIGPS_MAX_SIZE			0x1000000			// 16MB

typedef enum 
{

 	BOOT_CLK_LOW  = ARM_48_AHB_24,
 	BOOT_CLK_MID  = ARM_96_AHB_48,
 	BOOT_CLK_HIGH = ARM_192_AHB_96,

}CHNG_FREQ_GPS_E;



typedef unsigned long uint32;
typedef unsigned short uint16;
typedef uint32               SFS_HANDLE;
typedef unsigned char uint8;
#define  SFS_MAX_PATH 255

typedef struct tagGPS_File_Stat {
	uint16	st_mode;
	uint32 	st_size;
}GPS_File_Stat;

typedef struct tagWaveFileHeader
{
	int8			tagRiff[4];           // RIFF
	uint32		dwWaveBlockSize;      // 波形块的大小
	int8			tagWave[4];           // WAVE
	int8			tagFmt[4];            // fmt
	uint32		dwFmtBlockSize;       // 格式块的大小
	uint16		wFormatTag;           // =1

	uint16		wChannels;			  // 通道数
	uint32		dwSamplesPerSec;      // 采样率
	uint32		dwAvgBytesPerSec;     // 平均每秒波形音频所需要的记录的字节数
	uint16		wBlockAlign;          // 一个采样所需的字节数
	uint16		wBitsPerSample;  

	int8			tagData[4];           // "data"
	uint32		dwWaveFileSize;       // 波形文件大小
}WaveFileHeader;

/*
extern unsigned short GUI_GB2UCS(					
								unsigned short 				*ucs2_ptr, 	//destination: string pointer to UCS2 string.
								const unsigned char 		*gb_ptr,    //source: string pointer to GB string
								unsigned short 				len			//the length to convert from GB to UCS2.
						);
 extern unsigned short GUI_UCS2GB(					
    							unsigned char  				*gb_ptr,	 // store the gb2312 string code
					    		const unsigned char  		*ucs_ptr,    // the ucs2 stringcode that need to be converted.
					    		unsigned short   			length       //the unicode length
    					);
extern void * SCI_Malloc(   // Returns a void pointer to the allocated space, 
                            // or SCI_NULL if there is insufficient memory
                            // available.
    unsigned long size,        // Bytes to allocate.
    const char * file,  // The name of the file which call this function.
    unsigned long line         // The line number in the file which call this function.
    );

extern unsigned long SCI_Free(     // SCI_SUCCESS or SCI_ERROR
    void *memory_ptr        // Pointer to memory block to be deleted.
    );


SFS_HANDLE SFS_CreateFile(
const uint16 *file_name, 
uint32 access_mode,
uint32 share_mode,	//Must be NULL,File System have not realize it 
uint32 file_attri		//Must be NULL,File System have not realize it 
);
*/
/*****************************************************************************/
//  Discription:   
//  Global resource dependence: none 
//  Author:  
//  Note:
//
/*****************************************************************************/
PUBLIC void GPS_StopPlay( void );
/*****************************************************************************/
//  Discription:   
//  Global resource dependence: none 
//  Author:  
//  Note:
//
/*****************************************************************************/
PUBLIC void GPS_get_app_queue(void);

/*****************************************************************************/
//  Discription:   
//  Global resource dependence: none 
//  Author:  
//  Note:
//
/*****************************************************************************/
PUBLIC BOOLEAN GPS_IsFolderExist(
                            const uint16		*,     //in
                            uint16			       //in, 双字节为单位
                            );
/*****************************************************************************/
//  Discription:   
//  Global resource dependence: none 
//  Author:  
//  Note:
//
/*****************************************************************************/
PUBLIC void RituGps_OsFree( void );
// 函数声明

#endif





