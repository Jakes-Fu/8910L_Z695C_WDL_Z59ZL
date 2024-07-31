/*=================================================================================
====
	HEADER NAME : microdef.h
	MODULE NAME : zip

	PRE-INCLUDE FILES DESCRIPTION
	

	GENERAL DESCRIPTION
		define common micro which will be used in the module. 
	Contain the return value.

    Copyright (C) 1995-2004 Mark Adler
    For conditions of distribution and use, see copyright notice in common.h
===================================================================================
====
	Revision History
	Modification Tracking
	Date        Author      Number Description of changes
  ----------  ------------         ---------
--------------------------------------------
  2006-10-29  duhuanbiao         porting 

====================================================================================
==*/

/*******************************************************************************
* Multi-Include-Prevent Section
*******************************************************************************/

#ifndef WE_MICRODEF_H
#define WE_MICRODEF_H


/*******************************************************************************
* Include File Section
*******************************************************************************/
#include "we_def.h"
//#include "AEEStdlib.h"


/*******************************************************************************
* Macro Define Section
*******************************************************************************/

                 /* constant define */
/* tell the OS is unknow */
#define OS_CODE 0xFF	

/* the max unzip level */
#define MAX_MEM_LEVEL 9	

/* 32K for LZ77 window */
#define MAX_WINDOWBITS   15	
/* The three kinds of block type */
#define STORED_BLOCK 0
#define STATIC_TREES 1
#define DYN_TREES    2

/* The minimum and maximum match lengths */
#define MIN_MATCH  3
#define MAX_MATCH  258

/* preset dictionary flag in simple header */
#define PRESET_DICT 0x20

/* Allowed flush values; see deflate() and inflate() below for details */
#define ZIP_NO_FLUSH      0
#define ZIP_PARTIAL_FLUSH 1 /* will be removed, use Z_SYNC_FLUSH instead */
#define ZIP_SYNC_FLUSH    2
#define ZIP_FULL_FLUSH    3
#define ZIP_FINISH        4
#define ZIP_BLOCK         5

/* error define */
#define ZIP_OK            0		/* No error */
#define ZIP_STREAM_END    1		/* stream end */
#define ZIP_NEED_DICT     2		/* need dictionary */
#define ZIP_ERR_FILE      (-1)	/* file error */
#define ZIP_STREAM_ERROR (-2)	/* stream error */
#define ZIP_DATA_ERROR   (-3)	/* data error */
#define ZIP_MEM_ERROR    (-4)	/* insufficient memory */
#define ZIP_BUF_ERROR    (-5)	/* buffer error */
#define ZIP_PARA_ERR     (-6)
/* gzip error */
#define ZIP_FILE_WOULDBLOCK (-7) /* file read error, try later */
#define ZIP_END           (-8)   /* end of stream */

/* compression levels */
#define ZIP_NO_COMPRESSION         0
#define ZIP_BEST_SPEED             1
#define ZIP_BEST_COMPRESSION       9
#define ZIP_DEFAULT_COMPRESSION  (-1)

/* compression strategy; see deflateInit2() below for details */
#define ZIP_FILTERED            1
#define ZIP_HUFFMAN_ONLY        2
#define ZIP_RLE                 3
#define ZIP_FIXED               4
#define ZIP_DEFAULT_STRATEGY    0

/* Possible values of the data_type field (though see inflate()) */
#define ZIP_BINARY   0
#define ZIP_TEXT     1
#define ZIP_ASCII    ZIP_TEXT
#define ZIP_UNKNOWN  2

/* The deflate compression method (the only one supported in this version) */
#define ZIP_DEFLATED   8

#define ZIP_NULL  0  /* no error */

              /* Micro define */

#define TOALLOC(items, size) WE_SCL_MALLOC((WE_ULONG)(items * (size)))
#define TOFREE(p) {if (p) {WE_SCL_FREE(p); p = NULL;}}


/* default windowBits for decompression. MAX_WBITS is for compression only */
#ifndef DEF_WINDOWBITS
#  define DEF_WINDOWBITS MAX_WINDOWBITS
#endif

/* default memLevel */
#if MAX_MEM_LEVEL >= 8
#  define DEF_MEM_LEVEL 8
#else
#  define DEF_MEM_LEVEL  MAX_MEM_LEVEL
#endif


#endif /* WE_MICRODEF_H */