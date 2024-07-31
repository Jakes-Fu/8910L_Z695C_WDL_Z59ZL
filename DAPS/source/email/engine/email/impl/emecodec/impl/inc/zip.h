/*=================================================================================
====
	HEADER NAME : zip.h
	MODULE NAME : zip

	PRE-INCLUDE FILES DESCRIPTION
	    common.h

    GENERAL DESCRIPTION
        This File define the necessary data used by zip.c
  
    TechFaith Software Confidential Proprietary
    Copyright (c) 2006-2007 by TechFaith Software. All Rights Reserved.
===================================================================================
====
	Revision History
	Modification Tracking
	Date        Author      Number Description of changes
  ----------  ------------         ---------
--------------------------------------------
	2006-11-3 duhuanbiao         porting 

====================================================================================
==*/

/*******************************************************************************
* Multi-Include-Prevent Section
*******************************************************************************/

	#ifndef ZIP_H
	#define ZIP_H

/*******************************************************************************
* Include File Section
*******************************************************************************/

	#include "common.h"

/*******************************************************************************
* Macro Define Section
*******************************************************************************/

#define LENGTH_CODES 29
/* number of length codes, not counting the special END_BLOCK code */

#define LITERALS  256
/* number of literal bytes 0..255 */

#define CODES_LENGTH (LITERALS+1+LENGTH_CODES)
/* number of Literal or Length codes, including the END_BLOCK code */

#define D_CODES   30
/* number of distance codes */

#define BL_CODES  19
/* number of codes used to transfer the bit lengths */

#define HEAP_MAXSIZE (2 * CODES_LENGTH+1)
/* maximum heap size */

#define MAX_BITS 15
/* All codes must not exceed MAX_BITS bits */

/* Stream status */
#define INIT_STATE    42
#define ADDITIONAL_STATE 69
#define NAME_STATE    73
#define COMMENT_STATE 91
#define HCRC_STATE   103
#define BUSY_STATE   113
#define FINISH_STATE 666

#define SET_BUF_BYTE(state, c) {state->pucPendingBuf[state->uiPendingBufHave++] = (WE_UINT8)(c);}
/* Output a byte on the stream.
 * IN assertion: there is enough room in pucPendingBuf.
 */

#define MIN_LOOKAHEAD (MAX_MATCH+MIN_MATCH+1)
/* Minimum amount of uiBytesAhead, except at the end of the input file.*/

#define MAX_DIST(state)  ((state)->uiWindowSize - MIN_LOOKAHEAD)
/* In order to simplify the code, particularly on 16 bit machines, match
 * distances are limited to MAX_DIST instead of WSIZE.
 */

#define DIST_CODE(dist) \
   ((dist) < 256 ? iaDistCode[dist] : iaDistCode[256+((dist)>>7)])
/* Mapping from a distance to a distance code. dist is the distance - 1 and
 * must not have side effects.*/

/* Inline versions of Hfm_Save for speed: */
  extern const WE_UINT8 iaDistCode[];
  extern const WE_UINT8 iaLength[];


#define TREE_PUSH_L(state, c, flush) \
  { WE_UINT8 cc = (c); \
    state->puDBuf[state->uiLLastIndex] = 0; \
    state->pucLBuf[state->uiLLastIndex++] = cc; \
    state->astDynLTree[cc].Freq++; \
    flush = (state->uiLLastIndex == state->uiLBufSize-1); \
   }

#define TREE_PUSH_D(state, distance, length, flush) \
  { WE_UINT8 len = (WE_UINT8)(length); \
    WE_UINT16 dist = (WE_UINT16)(distance); \
    state->puDBuf[state->uiLLastIndex] = dist; \
    state->pucLBuf[state->uiLLastIndex++] = len; \
    dist--; \
    state->astDynLTree[iaLength[len]+LITERALS+1].Freq++; \
    state->astDynDTree[DIST_CODE(dist)].Freq++; \
    flush = (state->uiLLastIndex == state->uiLBufSize-1); \
  }
  
/*******************************************************************************
* Type Define Section
*******************************************************************************/

/* Data structure describing a single value and its code string. */
typedef struct tagSt_Tree_Data 
{
    union 
	{
        WE_UINT16  uFreq;       /* frequency count */
        WE_UINT16  uCode;       /* bit string */
    } FreqStr;
    union 
	{
        WE_UINT16  uDadNode;    /* father node in Huffman tree */
        WE_UINT16  uStrLen;     /* length of bit string */
		
    } DadLen;

}  St_Tree_Data, *P_St_Tree_Data;

#define Freq FreqStr.uFreq
#define Code FreqStr.uCode
#define Dad  DadLen.uDadNode
#define Len  DadLen.uStrLen

typedef struct s_Tree  s_Tree;

typedef struct tagSt_Tree_Define 
{  
    s_Tree *pstStatTree;	   /* the corresponding static tree */
    P_St_Tree_Data pstDynTree; /* the dynamic tree */
    WE_INT32  iMaxCode;        /* largest code with non zero frequency */

} St_Tree_Define, *P_St_Tree_Define;

typedef struct tagSt_ZIP_State 
{
	WE_INT32 aiHeap[2 * CODES_LENGTH + 1];    
	/* heap used to build the Huffman trees */

	St_Tree_Data astDynLTree[HEAP_MAXSIZE];
	/* literal and length tree */

	WE_UINT8 aucSubTreeDepth[ 2 * CODES_LENGTH + 1];
    /* Depth of each subtree used as tie breaker for trees of equal frequency */
	
	WE_UINT8  ucMethod;        
	/* STORED (for zip only) or DEFLATED */
	
	St_Tree_Data astDynDTree[2 * D_CODES + 1];
	/* distance tree */
	
    St_Tree_Data astBLTree[2 * BL_CODES + 1];
	/* Huffman tree for bit lengths */

	WE_UINT16 auBLCount[MAX_BITS + 1];
    /* number of codes at each bit length for an optimal tree */

	WE_UINT16 uOutBufOffset;
    /* Output buffer. bits are inserted starting at the bottom 
	 *(least significant bits).
     */
	
	St_Tree_Define stLDest;          
	/* desc. for literal tree */
	
    St_Tree_Define stDDest;             
	/* desc. for distance tree */
	
    St_Tree_Define stBLDest;             
	/* desc. for bit length tree */

    P_St_StreamState pstStream;      
	/* pointer back to this raw stream */

    P_St_GzipHeaderState  pstGzipHeader;  
	/* gzip header information to write */	
	
	WE_INT32 iStatus;        
	/* as the name implies */

    WE_UINT8 *pucPendingBuf;  
	/* output still pending */

    WE_ULONG ulPendingBufSize; 
	/* size of pucPendingBuf */

    WE_UINT8 *pucNextPendingOut;  
	/* next pending byte to output to the stream */

    WE_UINT32   uiPendingBufHave;     
	/* nb of bytes in the pending buffer */

    WE_INT32   iFormat;          
	/* bit 0 for raw, bit 1 for gzip */

    WE_UINT32   uiGzipIndex;      
	/* where in extra, name, or comment */

    WE_INT32   iPrevFlush;    
	/* value of flush param for previous zip call */

                /* used by zip.c: */

    WE_UINT32  uiWindowSize;        
	/* LZ77 window size (32K by default) */

    WE_UINT32  uiWindowBits;        
	/* log2(uiWindowSize)  (8..16) */

    WE_UINT32  uiWindowMask;       
	/* uiWindowSize - 1 */

    WE_UINT8 *pucWindow;
    /* Sliding window. Input bytes are read into the second half of the window,
     * and move to the first half later to keep a dictionary of at least wSize
     * bytes.*/

    WE_ULONG ulWindowSizeAll;
    /* Actual size of window: 2 * wSize, except when the user input buffer
     * is directly used as sliding window.
     */

    WE_UINT16 *puPrevStr;
    /* Link to older string with same hash index.*/

    WE_UINT16 *puHashHead; 
	/* Heads of the hash chains or NIL. */

    WE_UINT32  uiHashInsert;          
	/* hash index of string to be inserted */

    WE_UINT32  uiHashSize;      
	/* number of elements in hash table */

    WE_UINT32  uiHashBits;      
	/* log2(uiHashSize) */

    WE_UINT32  uiHashMask;      
	/* uiHashSize-1 */

    WE_UINT32  uiHashShift;
    /* Number of bits by which uiHashInsert must be shifted at each input
     * step.*/

    WE_LONG lBlockStart;
    /* Window position at the beginning of the current output block. Gets
     * negative when the window is moved backwards.*/

    WE_UINT32 uiMatchLens;          
	/* length of best match */

    WE_UINT32 uiPrevMatch;             
	/* previous match */

    WE_INT32 iIsPrevMatchAvail;         
	/* set if previous match exists */

    WE_UINT32 uiStrStart;              
	/* start of string to insert */

    WE_UINT32 uiMatchStart;           
	/* start of matching string */

    WE_UINT32 uiBytesAhead;            
	/* number of valid bytes ahead in window */

    WE_UINT32 uiPrevMatchLens;
    /* Length of the best match at previous step. Matches not greater than this
     * are discarded. This is used in the lazy match evaluation.*/

    WE_UINT32 uiMaxChainLens;
    /* To speed up zip, hash chains are never searched beyond this
     * length.*/

    WE_UINT32 uiMaxLazyMatch;
    /* Attempt to find a better match only when the current match is strictly
     * smaller than this value. This mechanism is used only for compression
     * levels >= 4.*/

    WE_INT32 iLevel;   
	/* compression level (1..9) */
	
    WE_INT32 iStrategy; 
	/* favor or force Huffman coding*/

    WE_UINT32 uiGoodMatch;
    /* Use a faster search when the previous match is longer than this */

    WE_INT32 iMaxMatch;
	/* Stop searching when current match exceeds this */

    WE_INT32 iHeapLen;               
	/* number of elements in the heap */

    WE_INT32 iHeapMax;               
	/* element of largest frequency */

    WE_UINT8 *pucLBuf;
	/* buffer for literals or lengths */

    WE_UINT32  uiLBufSize;
	/* Size of match buffer for literals/lengths.*/
    
    WE_UINT32 uiLLastIndex;      
	/* running index in pucLBuf */

    WE_UINT16 *puDBuf;
    /* Buffer for distances. To simplify the code, puDBuf and pucLBuf have
     * the same number of elements. To use different lengths, an extra flag
     * array would be necessary.
     */

    WE_ULONG ulOptBlockLens;       
	/* bit length of current block with optimal trees */

    WE_ULONG ulStaBlockLens;     
	/* bit length of current block with static trees */

    WE_UINT32 uiMatchSize;      
	/* number of string matches in current block */

    WE_INT32 iLastEobLens;   
	/* bit length of EOB code for last block */

    WE_INT32 iOutBufAvail;
    /* Number of valid bits in uOutBufOffset.  All bits above the last valid bit
     * are always zero.
     */

#define MAX_INSERT_LENS  uiMaxLazyMatch
    /* Insert new strings in the hash table only if the match length is not
     * greater than this length. This saves time but degrades compression.
     * max_insert_length is used only for compression levels <= 3.*/
	 
} St_ZIP_State, *P_St_ZIP_State;


/*******************************************************************************
* Prototype Declare Section
*******************************************************************************/
extern WE_VOID HFM_Init(St_ZIP_State *pstState);

extern WE_VOID Hfm_Flush_Mass(St_ZIP_State *pstState, WE_INT8 * buf,
					WE_ULONG stored_len,  WE_INT32 eof);

extern WE_VOID Mass_SavedSend(St_ZIP_State *pstState, WE_INT8 *buf,
					WE_ULONG stored_len,WE_INT32 eof);

extern WE_VOID Unzip_Align(St_ZIP_State *pstState);

extern WE_ULONG  Zip_AdlerCheck(WE_ULONG ulAdlerCheck, const WE_UINT8 *pucBufBeChecked, WE_UINT32 uiBufferLen);

extern WE_ULONG Zip_CrcCheck(WE_ULONG ulCrcCheck,const WE_UINT8 *pucCheckBuff,WE_UINT32 uiBufferLen);

#endif /* DEFLATE_H */
