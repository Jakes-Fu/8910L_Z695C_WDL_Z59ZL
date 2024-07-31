/*
Copyright (C) 1995-2005 Jean-loup Gailly and Mark Adler

  This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.

  Jean-loup Gailly        Mark Adler
  jloup@gzip.org          madler@alumni.caltech.edu
*/

/*=================================================================================
====
	HEADER NAME : common.h
	MODULE NAME : zip

	PRE-INCLUDE FILES DESCRIPTION
	typedef.h
	microdef.h

	GENERAL DESCRIPTION
		declare the common fun used as a interface

    Copyright (C) 1995-2004 Mark Adler
===================================================================================
====
	Revision History
	Modification Tracking
	Date        Author          Number    Description of changes
  ----------  ------------    ---------     ---------
---------------------------------------------------------------
  2006-11-3    duhuanbiao         1           create 

====================================================================================
==*/

/*******************************************************************************
* Multi-Include-Prevent Section
*******************************************************************************/

#ifndef COMMON_H
#define COMMON_H


/*******************************************************************************
* Include File Section
*******************************************************************************/

#include "microdef.h"


/*******************************************************************************
* Type Define Section
*******************************************************************************/

struct tagSt_ZIP_State;
typedef struct tagSt_StreamState 
{
    struct tagSt_ZIP_State *pstZipState; /* not visible by applications */
    
    WE_ULONG    ulTotalInput;	/* total nb of input bytes read so far */
    WE_ULONG    ulTotalOutput;	/* total nb of bytes output so far */
    WE_ULONG    ulAdlerCheck;    /* adler32 value of the uncompressed data */
    
    WE_UINT32   uiAvailInput;	/* number of bytes available at pucNextInput */
    WE_UINT32   uiAvailOutput;	/* remaining free space at pucNextOutput */
    
    WE_INT32	iDataType;		/* best guess about the data type: binary or text */
    
    WE_UINT8    *pucNextOutput; /* next output byte should be put there */
    WE_UINT8    *pucNextInput;  /* next input byte */
    WE_UINT8    *pucBufPtr;  /* store the input, used to free the backup data */
    
    
} St_StreamState, *P_St_StreamState;


typedef struct tagSt_GzipHeaderState 
{
    
    WE_ULONG   ulChangeDate;  /* modification date */
    
    WE_INT32   iIsText;       /* true if compressed data believed to be text */
    WE_INT32   iExtraFlgs;    /* extra flags (not used when writing a gzip file) */
    WE_INT32   iOs;           /* operating system */	    
    WE_INT32   iCrcHeadCheck; /* true if there was or will be a header crc */
    WE_INT32   iDownHeadRead; /* true when done reading gzip header */
    WE_UINT32  uiExtraDataLen;/* extra field length (valid if extra != Z_NULL) */		
    WE_UINT32  uiMaxExtraData;/* space at extra (only when reading header) */
    WE_UINT32  uiMaxName;     /* space at name (only when reading header) */
    WE_UINT32  uiMaxComment;  /* space at comment (only when reading header) */
    
    WE_UINT8   *pucExtraData; /* pointer to extra field or Z_NULL if none */
    WE_UINT8   *pucFileName;  /* pointer to zero-terminated file name or Z_NULL */
    WE_UINT8   *pucComment;   /* pointer to zero-terminated comment or Z_NULL */    
    
} St_GzipHeaderState, *P_St_GzipHeaderState;



/************************************************************************/
/* Function declare */
/************************************************************************/

					/* compress function */
extern WE_INT32 Zip_StreamWrite(
                               WE_UINT8* pucBufDest,  
                               WE_UINT32 *puiDestLen,
                               const WE_UINT8* pucSource, 
                               const WE_UINT32 uiSourceLen, 
                               WE_INT32 iLevel,
                               WE_INT32 iFormat
                               );
/* 
	this function is a buffer compress fun, using the deflation algorithm.if
you want to compress data, all you need to do is prepare two buffers; one for 
input containing the data you want to compress, the other for output, make sure
it is large enough to store the compressed data, you can also choice the compress
rate.

	parameter:
		pucBufDest: o:   the buffer output the compressed data;
		puiDestLen: i/o: when as input, it is the size of pucBufDest, as output, it is 
the real data been compressed.
		pucSource:  i:   the buffer contain the data you want to compress.
		uiSourceLen:i:   the size of the pucSource.
		iLevel:     i:   the comoress rate, the value is -1 -- 9;
				   -1:   it is the default compress level, it equal to level 6
				   0 :   no compress, it copies the real data and add the corresponding
				header.
				  1 - 9: 1 gives best speed, 9 gives best compression rate.
		iFormat:    i:   set 0 if you want to get a raw data format,
				1 if you want to get a gzip header. 

	return value:
			ZIP_OK:            success.
			ZIP_MEM_ERROR:     there was not enough room in the output buffer.
			ZIP_STREAM_ERROR:  the level is not a valid compression level.
			ZIP_PARA_ERR:      bad parameter
	comment:
			NULL

*/

				   /* decompress function */

/* we supply a set of decompressed functions,you can use these as follow:
	->Unzip_StreamSet( set the input stream )
		->Unzip_StreamRead( read the n decompressed data if you want)
			->Unzip_StreamClose( if there is no need to read, don't forget to close it )
	there also have an additional function Unzip_StreamResume,that is, if you want to restart
the stream or the stream is updated,just call it ,then you can start at the very beginning of
the stream.
*/

extern WE_HANDLE Unzip_StreamSet(
                                 const WE_UINT8 *pucStream, 
                                 const WE_UINT32 uiStreamLen, 
                                 const WE_BOOL bHaveHead
                                 );
/* this function set the input stream.
	parameter:
		pucStream   i: the stream contain the compressed data which you want to decode.
		uiStreamLen i: as it means, this is the stream length.
		bHaveHead   i: the property of the stream,0 for raw stream, 1 for gzip stream.
	
	  return value:	
		handle of the stream if success, NULL if failure.

	comment:
		NULL

*/

extern WE_INT32 Unzip_StreamRead(
                                 WE_HANDLE hStream,
                                 WE_UINT8 *pucBufDest,  
                                 WE_UINT32 *puiReadLen
                                 );
/* this function read n bytes from the stream.And the data is stored in the buffer you supply.

	parameter:
		hStream       i: the stream handle you get from the fun Unzip_StreamSet.
		pucBufDest    o: when output, it contains the decoded data.
		puiReadLen  i/o: when as input, it tells how many bytes you want to read, when as 
				output, it is the real data length in the pucBufDest.
	
	  return value:	
		ZIP_OK           : successfully read.
        ZIP_PARA_ERR     : bad parameter
		ZIP_STREAM_ERROR : stream error.
		ZIP_STREAM_END   : it is the format end of the data,not the buffer end sign.
		ZIP_MEM_ERROR    : there is not enough memory
		ZIP_BUF_ERROR    : there is not enough stream data,that is,it can only supply
				you the data less then you want.you must add new compressed data to the 
				stream.Been notice that the data rest have been decoded and output.
		ZIP_DATA_ERROR   :the input data was not as expected.
	comment:
		notice that the pucBufDest size is always larger than *puiReadLen,otherwise it 
		will be caused memory leak.

*/
extern WE_INT32 Unzip_StreamUpdate(
                                  WE_HANDLE hStream,
                                  const WE_UINT8 *pucStream, 
                                  const WE_UINT32 uiStreamLen
                                  );
/* this function update the stream.it is used when the first stream you supply is only
a part of the total compressed data.that is,because decode must used some first data as
reference,when data divide two or more parts,the second or later can't decode by itself,
then you can used this function.
	parameter:
		hStream     i: the stream handle you get from the fun Unzip_StreamSet.
		ucStream    i: new buffer contain the rest data,certainly it can be the original
		uiStreamLen i: as it means, this is the stream length.
	
	  return value:	
		ZIP_OK             :  success
		ZIP_STREAM_ERROR   :  stream error.
        ZIP_PARA_ERR       :  bad parameter  
	comment:
		NULL

*/

extern WE_INT32 Unzip_StreamResume(
                                  WE_HANDLE hStream,
                                  const WE_UINT8 *pucStream,
                                  const WE_UINT32 uiStreamLen
                                  );
/* this function resume the stream.that is, if the stream filled with new data,and it 
is the same format,you can call this fun.compare with the Unzip_StreamUpdate,it is a
new compressed data and Unzip_StreamUpdate is with same compressed data.

	parameter:
		hStream     i: the stream handle you get from the fun Unzip_StreamSet.
		uiStreamLen i: the new stream data avail.
	return value:	
		ZIP_OK            : success
		ZIP_STREAM_ERROR  ; stream error.
        ZIP_PARA_ERR      : bad parameter

	comment:
		you can used the basic fun to restart a new stream, but most likely
	it will be faster.

*/

extern WE_INT32 Unzip_StreamClose(
                                 WE_HANDLE hStream
                                 );
/* All dynamically allocated data structures for this stream are freed.

	parameter:
		hStream     i: the stream handle you get from the fun Unzip_StreamSet.
	
	return value:	
		ZIP_OK            : success
		ZIP_STREAM_ERROR  ; stream error.
        ZIP_PARA_ERR      : bad parameter

	comment:
		all the unpending data will been lost.

*/


                 /* gzip file function */
#if 0 
WE_HANDLE Gzip_Open(IShell* pIShell, const WE_CHAR* pcFileName, WE_INT32 iMode);
#endif

/* this function open .gz file, and return the handle which will needed
   in other gzip process function.
	parameter:
		pIShell     i: shell point
        pcFileName: i: file location
        iMode:      i: _OFM_READ, only this support
	
	return value:	
		NULL            : fail to open
		hHandle         : success
*/
#if 0
WE_INT32 Gzip_Read(WE_HANDLE hHnd, WE_UINT8* pcBuf, WE_UINT32 uiLen);
#endif
/* this function like the file read function, can read as much data as you want.
	parameter:
		hHnd        i: get from Gzip_open
        pcBuf:    i/o: data buffer
        uiLen:      i: how many bytes you need
	
	return value:
		0       : there is an error
        >0      : success,if it is less than you want,means you are in the end of 
                  the file
*/
#if 0
WE_VOID Gzip_Readable(WE_HANDLE hHnd, PFNNOTIFY pfn, void * pUser);
#endif /*Cancel by MTK*/

/* This function is used for registering a callback function which tries to read 
   from the gzip file at a later time
	parameter:
		hHnd        i: get from Gzip_open
        pfn         i: Address of the callback function. 
        pUser       i: User defiend data that will be passed to the callback function 
                       when it is invoked. 

	
	return value:
                    NONE
*/

WE_INT32 Gzip_GetLastErr(WE_HANDLE hHnd);
/* This function would typically be called when a gzip file operation 
   has failed and caller needs to know about the reason for the failure
	parameter:
		hHnd        i: get from Gzip_open
	
	return value:	
    ZIP_OK           : successfully read.
    ZIP_PARA_ERR     : bad parameter
    ZIP_STREAM_ERROR : stream error.
    ZIP_STREAM_END   : it is the format end of the data,not the buffer end sign.
    ZIP_MEM_ERROR    : there is not enough memory
    ZIP_BUF_ERROR    : there is not enough stream data,that is,it can only supply
				you the data less then you want.you must add new compressed data to the 
                stream.Been notice that the data rest have been decoded and output.
    ZIP_DATA_ERROR   :the input data was not as expected.
    ZIP_FILE_WOULDBLOCK: file error, call Gzip_Readable to register a callback.

*/

WE_INT32 Gzip_Close(WE_HANDLE hHnd);
/* All dynamically allocated data structures for this stream are freed.

	parameter:
		hHnd     i: the handle you get from the fun Gzip_open.
	
	return value:	
		ZIP_OK            : success

	comment:
		all the unpending data will been lost.

*/

#endif /* COMMON_H */
