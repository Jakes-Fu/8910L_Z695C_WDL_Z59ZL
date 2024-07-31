/*=================================================================================
====
	HEADER NAME : unzip.h
	MODULE NAME : zip

	PRE-INCLUDE FILES DESCRIPTION
	common.h

    GENERAL DESCRIPTION
        This file define the necessary data used by unzip.c

    Copyright (C) 1995-2004 Mark Adler
    For conditions of distribution and use, see copyright notice in common.h
===================================================================================
====
	Revision History
	Modification Tracking
	Date        Author      Number Description of changes
  ----------  ------------         ---------
--------------------------------------------
  2006-10-28  duhuanbiao         porting 

====================================================================================
==*/

/*******************************************************************************
* Multi-Include-Prevent Section
*******************************************************************************/

#ifndef UNZIP_H
#define UNZIP_H

/*******************************************************************************
* Include File Section
*******************************************************************************/

#include "common.h"


/*******************************************************************************
* Macro Define Section
*******************************************************************************/
#define ENOUGH 2048
#define MAXD 592


/*******************************************************************************
* Type Define Section
*******************************************************************************/
/* this enum define the state machine */
typedef enum 
{
    E_UNZIP_HEADER,     /* i: waiting for magic header */
    E_UNZIP_FLG,        /* i: waiting for method and flags (gzip) */
    E_UNZIP_DATE,       /* i: waiting for modification data (gzip) */
    E_UNZIP_OS,         /* i: waiting for extra flags and operating system (gzip) */
    E_UNZIP_EXLEN,      /* i: waiting for extra length (gzip) */
    E_UNZIP_EXTRA,      /* i: waiting for extra bytes (gzip) */
    E_UNZIP_NAME,		/* i: waiting for end of file name (gzip) */
    E_UNZIP_COMMENT,    /* i: waiting for end of comment (gzip) */
    E_UNZIP_HCRC,       /* i: waiting for header crc (gzip) */
    E_UNZIP_DICTCHECK,  /* i: waiting for dictionary check value */
    E_UNZIP_SETDICT,    /* i: waiting for Zip_SetDictionary() call */
    E_UNZIP_TYPEINI,    /* i: waiting for type bits, including last-flag bit */
    E_UNZIP_TYPESKIP,   /* i: same, but skip check to exit decode on new block */
    E_UNZIP_RESTORE,    /* i: waiting for stored size (length and complement) */
    E_UNZIP_TOCOPY,     /* i/o: waiting for input or output to copy stored block */
    E_UNZIP_DYNTABLE,   /* i: waiting for dynamic block table lengths */
    E_UNZIP_LENLENS,    /* i: waiting for code length code lengths */
    E_UNZIP_TREELENS,   /* i: waiting for length/lit and distance code lengths */
    E_UNZIP_LEN,        /* i: waiting for length/lit code */
    E_UNZIP_EXTREELEN,  /* i: waiting for length extra bits */
    E_UNZIP_DIST,       /* i: waiting for distance code */
    E_UNZIP_DISTEXT,    /* i: waiting for distance extra bits */
    E_UNZIP_MATCH,      /* o: waiting for output space to copy string */
    E_UNZIP_LIT,        /* o: waiting for output space to write literal */
    E_UNZIP_CHECK,      /* i: waiting for 32-bit check value */
    E_UNZIP_LENGTH,     /* i: waiting for 32-bit length (gzip) */
    E_UNZIP_DONE,       /* finished check, done -- remain here until reset */
    E_UNZIP_WRONG,      /* got a data error -- remain here until reset */
    E_UNZIP_MEMERR,     /* got an Unzip_stream() memory error -- remain here until reset */
    E_UNZIP_SYNC        /* looking for synchronization bytes to restart Unzip_stream() */

} E_UNZIP_ProcessMode;

/* Type of St_CodeCell to build for ziptable() */
typedef enum 
{
    E_UNZIP_CODES,
        E_UNZIP_LENS,
        E_UNZIP_DISTS
        
} E_UNZIP_CodeType;

typedef struct tagSt_CodeCell
{
    WE_UINT8 ucOperationBits;  /* operation, extra bits, table bits */
    WE_UINT8 ucCodeBits;       /* bits in this part of the code */
    WE_UINT16 uOffset;         /* offset in table or code value */
    
} St_CodeCell, *P_St_CodeCell;

typedef struct tagSt_UnzipState 
{
	WE_UINT16 auCodeLensTemp[320];   /* temporary storage for code lengths */
    WE_UINT16 auWorkSpace[288];		 /* work area for code table building */

	St_CodeCell const *pstLenCode;    /* starting table for length/literal codes */
    St_CodeCell const *pstDistCode;   /* starting table for distance codes */
    St_CodeCell *pstNextAvail;		  /* next available space in astCodes[] */
	St_CodeCell astCodes[ENOUGH];     /* space for St_CodeCell tables */
	
	P_St_GzipHeaderState pstGzipHead; /* where to save gzip header information */

	E_UNZIP_ProcessMode eMode;        /* current process mode */

    WE_INT32 iIsLastBlock;            /* true if processing last block */
    WE_INT32 iFormat;                 /* 1 for gzip */
    WE_INT32 iHaveDict;               /* true if dictionary provided */
    WE_INT32 iGzipFlg;                /* gzip header method and flags */
    WE_UINT32 uiMaxDistance;          /* header max distance */
    WE_ULONG ulCheckValue;			  /* protected copy of check value */
    WE_ULONG ulOutputCount;			  /* protected copy of output count */
               
				/* sliding window */
    WE_UINT32 uiWindowBitsReq;        /* log base 2 of requested window size */
    WE_UINT32 uiWindowSize;           /* window size or zero if not using window */
    WE_UINT32 uiWindowHave;           /* valid bytes in the window */
    WE_UINT32 uiWindowWrite;          /* window write index */
    WE_UINT8  *pucSlidWindow;		  /* allocated sliding window, if needed */
      
				/* bit accumulator */
    WE_ULONG ulDataHold;			  /* input bit accumulator */
    WE_UINT32 uiBitsHave;             /* number of bits in "in" */
      
				/* for string and stored block copying */
    WE_UINT32 uiCopyLen;			  /* literal or length of data to copy */
    WE_UINT32 uiCopyOffset;           /* distance back to copy string from */
       
				/* for table and code decoding */
    WE_UINT32 uiNeedExtra;            /* extra bits needed */

    WE_UINT32 uiLenBitsIndex;         /* index bits for pstLenCode */
    WE_UINT32 uiDistBitsIndex;        /* index bits for pstDistCode */
				
				/* dynamic table building */
    WE_UINT32 uiCodeNum;             /* number of code length code lengths */
    WE_UINT32 uiLenNum;              /* number of length code lengths */
    WE_UINT32 uiDistNum;             /* number of distance code lengths */
    WE_UINT32 uiHaveNum;             /* number of code lengths in auCodeLensTemp[] */

} St_UnzipState, *P_St_UnzipState;

/*******************************************************************************
* Prototype Declare Section
*******************************************************************************/

extern WE_INT32 Unzip_Table(E_UNZIP_CodeType eType, WE_UINT16  *puLens,
                             WE_UINT32 uiCodes, St_CodeCell  **ppstTable,
                             WE_UINT32  *puiBits, WE_UINT16  *puWork);

#endif /* UNZIP_H */