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
// ��д����
//��дʶ��Χ
#define IDENTIFY_LOWWER_CHAR            0x1     //СдӢ��
#define IDENTIFY_UPPER_CHAR             0x2     //��дӢ��
#define IDENTIFY_DIGIT                  0x04    //����
#define IDENTIFY_PUNCTUATION            0x08    //���ñ��, 8��, ����:, �� �� ? ! " : ;
#define IDENTIFY_PUNCTUATION_EXTEND     0x10    //��չ���, ����:    ' (���������� )-��
#define IDENTIFY_SYMBOL                 0x20    //���÷���, ����: �� # . = / > $ - < * @ &
#define IDENTIFY_SYMBOL_EXTEND          0x40    //��չ����, ����: ~ { } ^ ` \ ] _ [ |
#define IDENTIFY_GESTURE                0x80    //����4��, ����: �ո�(0x20)���س�(0x0d)����ɾ(0x08)��ɾ��(0x10)
#define IDENTIFY_COMPONMENT             0x100   //ƫ�Բ���
#define IDENTIFY_GB_LEVEL_ONE           0x200   //GB2312һ��������庺��
#define IDENTIFY_GB_LEVEL_TWO           0x400   //GB2312����������庺��
#define IDENTIFY_BIG5_LEVEL_ONE         0x800   //������һ��
#define IDENTIFY_BIG5_LEVEL_TWO         0x1000  //�����ֶ���
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
	uint32		dwWaveBlockSize;      // ���ο�Ĵ�С
	int8			tagWave[4];           // WAVE
	int8			tagFmt[4];            // fmt
	uint32		dwFmtBlockSize;       // ��ʽ��Ĵ�С
	uint16		wFormatTag;           // =1

	uint16		wChannels;			  // ͨ����
	uint32		dwSamplesPerSec;      // ������
	uint32		dwAvgBytesPerSec;     // ƽ��ÿ�벨����Ƶ����Ҫ�ļ�¼���ֽ���
	uint16		wBlockAlign;          // һ������������ֽ���
	uint16		wBitsPerSample;  

	int8			tagData[4];           // "data"
	uint32		dwWaveFileSize;       // �����ļ���С
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
                            uint16			       //in, ˫�ֽ�Ϊ��λ
                            );
/*****************************************************************************/
//  Discription:   
//  Global resource dependence: none 
//  Author:  
//  Note:
//
/*****************************************************************************/
PUBLIC void RituGps_OsFree( void );
// ��������

#endif





