#ifndef _EME_CODEC_FILE_H_
#define _EME_CODEC_FILE_H_

#include "eme_file_api.h"

#define EME_FILE_READ_ONE_TIME      (1024 * 48)

typedef  WE_VOID* PARTINDEXTB;

FILE_HANDLE EMEFile_GetHeadData(FILE_HANDLE pIFile, WE_VOID *pvMemHdl, WE_UINT32 uiBlockSize, WE_INT32 *piHeadSize);

WE_INT32    EMEFile_ReadDataFromFile(FILE_HANDLE pIFile, WE_VOID *pvData, WE_UINT32 uiSize);

WE_VOID     EMFile_FreePartIndexTable(PARTINDEXTB pvPartIndexTable, WE_VOID *pvMemHdl);

PARTINDEXTB EMEFile_GetPartIndexTable(FILE_HANDLE pIFile, WE_VOID *pvMemHdl, WE_CHAR *pcBoundary, WE_UINT32 uiBodySize, WE_INT32 *piPartCount);

FILE_HANDLE EMEFile_GetOnePartByIndex(FILE_HANDLE pIFile, PARTINDEXTB pvPartIndexTable, WE_INT32 iIndex, WE_CHAR *pcBoundary, WE_INT32 *piOffset);

WE_INT32    EMEFile_GetOnePartInfo(PARTINDEXTB pvPartIndexTable, WE_INT32 iIndex,  WE_CHAR *pcBoundary, WE_INT32 *piOffset, WE_INT32 *piDataSize);
#endif