/*=================================================================================
====
	MODULE NAME : zip-zip.c
	MODULE NAME : zip

    GENERAL DESCRIPTION
        This File compress the stream

    Copyright (C) 1995-2004 Mark Adler
    For conditions of distribution and use, see copyright notice in common.h

===================================================================================
====
Revision History
Modification Tracking
	Date		Author			Number		Description of changes
   ---------- -------------- --------- --------------------------------------
   2006-10-29   duhuanbiao       1               porting

===================================================================================
==*/


/*******************************************************************************
* Include File Section
*******************************************************************************/

#include "zip.h"



/*******************************************************************************
* Type Define Section
*******************************************************************************/
typedef enum 
{
    E_ZIP_Need_More,      /* block not completed, need more input or more output */
    E_ZIP_Block_Done,     /* block flush performed */
    E_ZIP_Finish_Started, /* finish started, need only more output at next deflate */
    E_ZIP_Finish_Done,    /* finish done, accept no more input or output */
	E_ZIP_PARA_ERROR      /* error of the parameter */

} E_ZIP_Block_State;


typedef struct tagSt_Config 
{
	WE_UINT16 uLengthGood; /* reduce lazy search above this match length */
	WE_UINT16 uLengthNoLazy;/* do not perform lazy search above this match length */
	WE_UINT16 uLengthQuit; /* quit search above this match length */
	WE_UINT16 uMaxChain;

} St_Config, *P_St_Config;


/*******************************************************************************
* Macro Define Section
*******************************************************************************/

/* Tail of hash chains */
#define ZERO 0

/* Matches of length 3 are discarded if their distance exceeds TOO_FAR */
#ifndef TOO_FAR
#  define TOO_FAR 4096
#endif

static const St_Config s_Zip_Config_List[10] = 
{
	{0,    0,  0,    0},  /* store only */
	{4,    4,  8,    4},  /* max speed, no lazy matches */
	{4,    5, 16,    8},
	{4,    6, 32,   32},

	{4,    4, 16,   16},  /* lazy matches */
	{8,   16, 32,   32},
	{8,   16, 128, 128},
	{8,   32, 128, 256},
	{32, 128, 258, 1024},
	{32, 258, 258, 4096}  /* max compression */
}; 

/* result of memcmp for equal strings */
#define EQUAL 0

/* Update a hash value with the given input byte */
#define UPDATE_HASH(state,h,c) (h = (((h)<<state->uiHashShift) ^ (c)) & state->uiHashMask)


/* ===========================================================================
 * Insert string str in the dictionary and set match_head to the previous head
 * of the hash chain (the most recent string with same hash key). Return
 * the previous length of the hash chain.
 * If this file is compiled with -DFASTEST, the compression iLevel is forced
 * to 1, and no hash chains are maintained.
 */

#define INSERT_STRING(state, str, match_head) \
   ( UPDATE_HASH(state, state->uiHashInsert, state->pucWindow[(str) + (MIN_MATCH-1)]), \
    match_head = state->puPrevStr[(str) & state->uiWindowMask] = state->puHashHead[pstZipState->uiHashInsert], \
    state->puHashHead[state->uiHashInsert] = (WE_UINT16)(str))


/* ===========================================================================
 * Initialize the hash table (avoiding 64K overflow for 16 bit systems).
 * prev[] will be initialized on the fly.
 */
#define CLEAR_HASH(state) \
    state->puHashHead[state->uiHashSize-1] = ZERO; \
    state->puHashHead = (WE_UINT16*)MEMSET(state->puHashHead, 0, (state->uiHashSize-1) * sizeof(*state->puHashHead));

/* ===========================================================================
 * Flush the current block, with given end-of-file flag.
 * IN assertion: uiStrStart is set to the end of the current match.
 */
#define FLUSH_BLOCK_ONLY(pstZipState, eof) \
{ \
   Hfm_Flush_Mass(pstZipState, (pstZipState->lBlockStart >= 0L ? \
                   (WE_INT8 *)&pstZipState->pucWindow[(WE_UINT32)pstZipState->lBlockStart] : \
                   (WE_INT8 *)NULL), \
                (WE_ULONG)((WE_LONG)pstZipState->uiStrStart - pstZipState->lBlockStart), \
                (eof)); \
   pstZipState->lBlockStart = (WE_LONG)pstZipState->uiStrStart; \
   Zip_Pending_Flush(pstZipState->pstStream); \
}

/* Same but force premature exit if necessary. */
#define FLUSH_BLOCK(pstZipState, eof) \
{ \
   FLUSH_BLOCK_ONLY(pstZipState, eof); \
   if (pstZipState->pstStream->uiAvailOutput == 0) \
	{ \
		return (eof) ? E_ZIP_Finish_Started : E_ZIP_Need_More; \
	} \
}
/************************************************************************/
/* function declare */
/************************************************************************/
static WE_VOID Zip_Fill_BufWindow(P_St_ZIP_State pstZipState);
static WE_VOID Zip_Height_Check_Init(P_St_ZIP_State pstZipState);
static WE_VOID Zip_State_SetBuf(P_St_ZIP_State pstZipState, WE_UINT32 uiOffset);
static WE_VOID Zip_Pending_Flush(P_St_StreamState pstStream);

static E_ZIP_Block_State Zip_Filled(P_St_ZIP_State pstZipState, WE_INT32 iFlush);
static E_ZIP_Block_State Zip_Quickly(P_St_ZIP_State pstZipState, WE_INT32 iFlush);
static E_ZIP_Block_State Zip_Slowly(P_St_ZIP_State pstZipState, WE_INT32 iFlush);

static WE_INT32 Zip_Read_Buf(P_St_StreamState pstStream, WE_UINT8 *pucBufOut, WE_UINT32 uiBufSize);
static WE_INT32 Zip_HEIGHT_CHECK(P_St_ZIP_State pstZipState, WE_UINT32 uiCurMatch);
static WE_INT32 Zip_HEIGHT_CHECK_QUICKLY(P_St_ZIP_State pstZipState, WE_UINT32 uiCurMatch);

static WE_INT32 Zip_Done (P_St_StreamState pstStream);
static WE_INT32 Zip_Reset(P_St_StreamState pstStream);

/************************************************************************/
/* Function prototype */
/************************************************************************/
/*=================================================================================
===
	FUNCTION: Zip_InitEx

	CREATE DATE: 2006-10-30
	AUTHOR: duhuanbiao

	DESCRIPTION:
		initial the zip state
		
	ARGUMENTS PASSED:

	RETURN VALUE:
		ZIP_OK               ; success
		ZIP_STREAM_ERROR	 : stream error
		ZIP_MEM_ERROR        : memory error

	CALL BY:

	IMPORTANT NOTES:
		NULL

===================================================================================
==*/
static WE_INT32  Zip_InitEx(
                            P_St_StreamState pstStream, 
                            WE_INT32 iLevel,
                            WE_INT32 iMethod,
                            WE_INT32 iWindowBits,
                            WE_INT32 iMemLevel,
                            WE_INT32 iStrategy
                            )
{
    P_St_ZIP_State pstZipState = NULL;
    WE_INT32 iFormat = 1;
 //   WE_INT32 iErr;
    
    WE_UINT16 *puOverLay = NULL;
    WE_UINT16 *puPrevStr = NULL;
    WE_UINT16 *puHashHead = NULL;
    WE_UINT8 *pucWindow = NULL;

    /* We overlay pucPendingbuf and puDBuf+pucLBuf. This works since the average
    * output size for (length,distance) codes is <= 24 bits.
    */
    
    if (pstStream == NULL) 
    {
        return ZIP_PARA_ERR;
    }
    
    if (iLevel == ZIP_DEFAULT_COMPRESSION) 
    {
        iLevel = 6;
    }
    if (iWindowBits < 0) 
    { 
        iFormat = 0;		/* raw format */
        iWindowBits = -iWindowBits;
    }
    else if (iWindowBits > 15)
    {
        iFormat = 2;       /* write gzip format instead */
        iWindowBits -= 16;
    }
    if (iMemLevel < 1 || 
        iMemLevel > MAX_MEM_LEVEL ||
        iMethod != ZIP_DEFLATED ||
        iWindowBits < 8 || 
        iWindowBits > 15 || 
        iLevel < 0 || 
        iLevel > 9 ||
        iStrategy < 0 ||
        iStrategy > ZIP_FIXED) 
    {
        return ZIP_STREAM_ERROR;
    }
    if (iWindowBits == 8) 
    {
        iWindowBits = 9;  /* until 256-byte window bug fixed */
    }
    
    pstZipState = (P_St_ZIP_State)TOALLOC(1U, sizeof(St_ZIP_State));
    if (pstZipState == NULL)
    {
        return ZIP_MEM_ERROR;
    }
    pstStream->pstZipState = pstZipState;
    pstZipState->pstStream = pstStream;
    
    pstZipState->iFormat = iFormat;
    pstZipState->pstGzipHeader = NULL;
    pstZipState->uiWindowBits = (WE_UINT32)iWindowBits;
    pstZipState->uiWindowSize = 1 << pstZipState->uiWindowBits;
    pstZipState->uiWindowMask = pstZipState->uiWindowSize - 1U;
    
    pstZipState->uiHashBits = (WE_UINT32)(iMemLevel + 7);
    pstZipState->uiHashSize = 1 << pstZipState->uiHashBits;
    pstZipState->uiHashMask = pstZipState->uiHashSize - 1U;
    pstZipState->uiHashShift = ((pstZipState->uiHashBits + MIN_MATCH - 1U)/MIN_MATCH);
    pstZipState->uiLBufSize = 1 << (iMemLevel + 6); /* 16K elements by default */
    
    pstZipState->pucWindow = NULL;
    pstZipState->puPrevStr = NULL;
    pstZipState->puHashHead =NULL;
    pstZipState->pucPendingBuf =NULL;

    pucWindow = (WE_UINT8 *)TOALLOC(pstZipState->uiWindowSize, 2U * sizeof(WE_UINT8));
    puPrevStr = (WE_UINT16 *)TOALLOC(pstZipState->uiWindowSize, sizeof(WE_UINT16));
    puHashHead = (WE_UINT16 *)TOALLOC(pstZipState->uiHashSize, sizeof(WE_UINT16));
    puOverLay = (WE_UINT16 *)TOALLOC(pstZipState->uiLBufSize, sizeof(WE_UINT16)+2);
    
    if (pucWindow == NULL || 
        puPrevStr == NULL || 
        puHashHead == NULL ||
        puOverLay == NULL)
    {
        pstZipState->iStatus = FINISH_STATE;
//        iErr = Zip_Done(pstStream);

        TOFREE(pstStream->pstZipState);

        TOFREE(pucWindow);
        TOFREE(puPrevStr);
        TOFREE(puHashHead);
        TOFREE(puOverLay);
        return ZIP_MEM_ERROR;
    }
    
    pstZipState->pucWindow = pucWindow;
    pstZipState->puPrevStr = puPrevStr;
    pstZipState->puHashHead = puHashHead;
    pstZipState->pucPendingBuf = (WE_UINT8 *)puOverLay;
    pstZipState->ulPendingBufSize = (WE_ULONG)pstZipState->uiLBufSize * (sizeof(WE_UINT16)+2L);
    
    pstZipState->puDBuf = puOverLay + pstZipState->uiLBufSize/sizeof(WE_UINT16);
    pstZipState->pucLBuf = pstZipState->pucPendingBuf + (1 + sizeof(WE_UINT16)) * pstZipState->uiLBufSize;
    
    pstZipState->iLevel = iLevel;
    pstZipState->iStrategy = iStrategy;
    pstZipState->ucMethod = (WE_UINT8)iMethod;
    
    return Zip_Reset(pstStream);   
}

/*=================================================================================
===
	FUNCTION: Zip_Init

	CREATE DATE: 2006-10-30
	AUTHOR: duhuanbiao

	DESCRIPTION:
		a simple init if you only used some default set 
		
	ARGUMENTS PASSED:

	RETURN VALUE:
		ZIP_OK               ; success
		ZIP_STREAM_ERROR	 : stream error
		ZIP_MEM_ERROR        : memory error

	CALL BY:

	IMPORTANT NOTES:
		NULL

===================================================================================
==*/
static WE_INT32 Zip_Init(
                         P_St_StreamState pstStream,
                         WE_INT32 iLevel, 
                         WE_INT32 iFormat
                         )
{	
    if (!iFormat)
    {
        return Zip_InitEx(pstStream, iLevel, ZIP_DEFLATED, -MAX_WINDOWBITS,
            DEF_MEM_LEVEL, ZIP_DEFAULT_STRATEGY);
    }
    else
    {
        return Zip_InitEx(pstStream, iLevel, ZIP_DEFLATED, MAX_WINDOWBITS + 16,
            DEF_MEM_LEVEL, ZIP_DEFAULT_STRATEGY);
    }
}

/*=================================================================================
===
	FUNCTION: Zip_Reset

	CREATE DATE: 2006-11-3
	AUTHOR: duhuanbiao

	DESCRIPTION:
		rest the state,but not free the struct
		
	ARGUMENTS PASSED:

	RETURN VALUE:
		ZIP_OK               ; success
		ZIP_STREAM_ERROR	 : stream error

	CALL BY:

	IMPORTANT NOTES:
		NULL

===================================================================================
==*/
static WE_INT32 Zip_Reset(
                          P_St_StreamState pstStream
                          )
{
    P_St_ZIP_State pstZipState = NULL;
    
    if (pstStream == NULL || pstStream->pstZipState == NULL)
    {
        return ZIP_PARA_ERR;
    }
    pstStream->ulTotalOutput = 0;
    pstStream->ulTotalInput = 0;
    pstStream->iDataType = ZIP_UNKNOWN;
    
    pstZipState = (P_St_ZIP_State)pstStream->pstZipState;
    pstZipState->uiPendingBufHave = 0;
    pstZipState->pucNextPendingOut = pstZipState->pucPendingBuf;
    
    if (pstZipState->iFormat < 0) 
    {
        pstZipState->iFormat = -pstZipState->iFormat; /* was made negative raw data */
    }
    pstZipState->iStatus = pstZipState->iFormat ? INIT_STATE : BUSY_STATE;
    pstStream->ulAdlerCheck = pstZipState->iFormat == 2 ? Zip_CrcCheck(0L, NULL, 0) :Zip_AdlerCheck(0L, NULL, 0);
    pstZipState->iPrevFlush = ZIP_NO_FLUSH;
    
    HFM_Init(pstZipState);
    Zip_Height_Check_Init(pstZipState);
    
    return ZIP_OK;
}

#if 0
/*=================================================================================
===
	FUNCTION: Zip_SetHeader

	CREATE DATE: 2006-11-3
	AUTHOR: duhuanbiao

	DESCRIPTION:
		add the gzip header to the stream, then if you want to 
	get a gzip format,this will as an header
		
	ARGUMENTS PASSED:

	RETURN VALUE:
		ZIP_OK               ; success
		ZIP_STREAM_ERROR	 : stream error

	CALL BY:

	IMPORTANT NOTES:
		NULL

===================================================================================
==*/
static WE_INT32 Zip_SetHeader (
                               P_St_StreamState pstStream, 
                               P_St_GzipHeaderState pstGzipHead
                               )
{
    if (pstStream == NULL || pstStream->pstZipState == NULL)
    {
        return ZIP_PARA_ERR;
    }
    if (pstStream->pstZipState->iFormat != 2) 
    {
        return ZIP_STREAM_ERROR;
    }
    pstStream->pstZipState->pstGzipHeader = pstGzipHead;
    return ZIP_OK;
}

#endif
/*=================================================================================
===
	FUNCTION: Zip_State_SetBuf

	CREATE DATE: 2006-11-3
	AUTHOR: duhuanbiao

	DESCRIPTION:
		Put a short in the pending buffer. The 16-bit value is put in MSB order.
		
	ARGUMENTS PASSED:

	RETURN VALUE:

	CALL BY:

	IMPORTANT NOTES:
		NULL

===================================================================================
==*/
static WE_VOID Zip_State_SetBuf(
                             P_St_ZIP_State pstZipState, 
                             WE_UINT32 uiOffset
                             )
{
    if (pstZipState)
    {
        SET_BUF_BYTE(pstZipState, (WE_UINT8)(uiOffset >> 8));
        SET_BUF_BYTE(pstZipState, (WE_UINT8)(uiOffset & 0xff));
    }
}

/*=================================================================================
===
	FUNCTION: Zip_Pending_Flush

	CREATE DATE: 2006-11-3
	AUTHOR: duhuanbiao

	DESCRIPTION:
		Flush as much pending output as possible.
		
	ARGUMENTS PASSED:

	RETURN VALUE:

	CALL BY:

	IMPORTANT NOTES:
		NULL

===================================================================================
==*/
static WE_VOID Zip_Pending_Flush(
                              P_St_StreamState pstStream
                              )
{
    WE_UINT32 uiLen = 0U;
    if (pstStream == NULL)
    {
        return;
    }
    uiLen = pstStream->pstZipState->uiPendingBufHave;
    
    if (uiLen > pstStream->uiAvailOutput) 
    {
        uiLen = pstStream->uiAvailOutput;
    }
    if (uiLen == 0)
    {
        return;
    }
    pstStream->pucNextOutput = (WE_UINT8*)MEMCPY(pstStream->pucNextOutput, pstStream->pstZipState->pucNextPendingOut, uiLen);
    pstStream->pucNextOutput  += uiLen;
    pstStream->pstZipState->pucNextPendingOut  += uiLen;
    pstStream->ulTotalOutput += uiLen;
    pstStream->uiAvailOutput  -= uiLen;
    pstStream->pstZipState->uiPendingBufHave -= uiLen;
    if (pstStream->pstZipState->uiPendingBufHave == 0) 
    {
        pstStream->pstZipState->pucNextPendingOut = pstStream->pstZipState->pucPendingBuf;
    }
}

/*=================================================================================
===
	FUNCTION: Zip_Stream

	CREATE DATE: 2006-11-3
	AUTHOR: duhuanbiao

	DESCRIPTION:
		this is the decoding body,it always compress as much data as it can
		
	ARGUMENTS PASSED:
		iFlush: 
			ZIP_NO_FLUSH:allows zip_stream to decide how much data to accumualte before 
	producing output, in order to maximize compression.
			ZIP_SYNC_FLUSH: all pending output is flushed to the output buffer and the 
	output is aligned on a byte boundary
			ZIP_FULL_FLUSH:all output is flushed as with ZIP_SYNC_FLUSH, and the compression 
	state is reset 

	RETURN VALUE:
		ZIP_OK:           success.
		ZIP_MEM_ERROR:    there was not enough room in the output buffer.
		ZIP_STREAM_ERROR: the level is not a valid compression level.
        ZIP_PARA_ERR:     bad parameter
	CALL BY:

	IMPORTANT NOTES:
		stops when the input buffer becomes empty or the output buffer becomes full

===================================================================================
==*/
static WE_INT32 Zip_Stream(
                           P_St_StreamState pstStream, 
                           WE_INT32 iFlush
                           )
{
    WE_INT32 iPrevFlush = 0; /* value of flush param for previous deflate call */
    P_St_ZIP_State pstZipState = NULL;
    
    if (pstStream == NULL || 
        pstStream->pstZipState == NULL ||
        iFlush > ZIP_FINISH ||
        iFlush < 0)
    {
        return ZIP_STREAM_ERROR;
    }
    pstZipState = pstStream->pstZipState;
    
    if (pstStream->pucNextOutput == NULL ||
        (pstStream->pucNextInput == NULL && pstStream->uiAvailInput != 0) ||
        (pstZipState->iStatus == FINISH_STATE && iFlush != ZIP_FINISH)) 
    {
        return ZIP_STREAM_ERROR;
    }
    if (pstStream->uiAvailOutput == 0)
    {
        return ZIP_BUF_ERROR;
    }
    pstZipState->pstStream = pstStream; /* just in case */
    iPrevFlush = pstZipState->iPrevFlush;
    pstZipState->iPrevFlush = iFlush;
    
    /* Write the header */
    if (pstZipState->iStatus == INIT_STATE) 
    {
        if (pstZipState->iFormat == 2) 
        {
            pstStream->ulAdlerCheck = Zip_CrcCheck(0L, NULL, 0);
            SET_BUF_BYTE(pstZipState, 31);
            SET_BUF_BYTE(pstZipState, 139);
            SET_BUF_BYTE(pstZipState, 8);
            if (pstZipState->pstGzipHeader == NULL) 
            {
                SET_BUF_BYTE(pstZipState, 0);
                SET_BUF_BYTE(pstZipState, 0);
                SET_BUF_BYTE(pstZipState, 0);
                SET_BUF_BYTE(pstZipState, 0);
                SET_BUF_BYTE(pstZipState, 0);
                SET_BUF_BYTE(pstZipState, (WE_UINT8)(pstZipState->iLevel == 9 ? 2 :
                (pstZipState->iStrategy >= ZIP_HUFFMAN_ONLY || pstZipState->iLevel < 2 ?
                    4 : 0)));
                SET_BUF_BYTE(pstZipState, OS_CODE);
                pstZipState->iStatus = BUSY_STATE;
            }
            else 
            {
                SET_BUF_BYTE(pstZipState, (pstZipState->pstGzipHeader->iIsText ? 1 : 0) +
                    (pstZipState->pstGzipHeader->iCrcHeadCheck ? 2 : 0) +
                    (pstZipState->pstGzipHeader->uiExtraDataLen == 0 ? 0 : 4) +
                    (pstZipState->pstGzipHeader->pucFileName == NULL ? 0 : 8) +
                    (pstZipState->pstGzipHeader->pucComment == NULL ? 0 : 16)
                    );
                SET_BUF_BYTE(pstZipState, (WE_UINT8)(pstZipState->pstGzipHeader->ulChangeDate & 0xff));
                SET_BUF_BYTE(pstZipState, (WE_UINT8)((pstZipState->pstGzipHeader->ulChangeDate >> 8) & 0xff));
                SET_BUF_BYTE(pstZipState, (WE_UINT8)((pstZipState->pstGzipHeader->ulChangeDate >> 16) & 0xff));
                SET_BUF_BYTE(pstZipState, (WE_UINT8)((pstZipState->pstGzipHeader->ulChangeDate >> 24) & 0xff));
                SET_BUF_BYTE(pstZipState, pstZipState->iLevel == 9 ? 2 :
                (pstZipState->iStrategy >= ZIP_HUFFMAN_ONLY || pstZipState->iLevel < 2 ?
                    4 : 0));
                SET_BUF_BYTE(pstZipState, pstZipState->pstGzipHeader->iOs & 0xff);
                if (pstZipState->pstGzipHeader->uiExtraDataLen != 0) 
                {
                    SET_BUF_BYTE(pstZipState, pstZipState->pstGzipHeader->uiExtraDataLen & 0xff);
                    SET_BUF_BYTE(pstZipState, (pstZipState->pstGzipHeader->uiExtraDataLen >> 8) & 0xff);
                }
                if (pstZipState->pstGzipHeader->iCrcHeadCheck)
                {
                    pstStream->ulAdlerCheck = Zip_CrcCheck(pstStream->ulAdlerCheck, pstZipState->pucPendingBuf,
                        pstZipState->uiPendingBufHave);
                }
                pstZipState->uiGzipIndex = 0;
                pstZipState->iStatus = ADDITIONAL_STATE;
            }
        }
        else
        {
            WE_UINT32 uiHeader = (ZIP_DEFLATED + ((pstZipState->uiWindowBits-8)<<4)) << 8;
            WE_UINT32 uiLevelFlags;
            
            if (pstZipState->iStrategy >= ZIP_HUFFMAN_ONLY || pstZipState->iLevel < 2)
            {
                uiLevelFlags = 0;
            }
            else if (pstZipState->iLevel < 6)
            {
                uiLevelFlags = 1;
            }
            else if (pstZipState->iLevel == 6)
            {
                uiLevelFlags = 2;
            }
            else
            {
                uiLevelFlags = 3;
            }
            uiHeader |= (uiLevelFlags << 6);
            if (pstZipState->uiStrStart != 0)
            {
                uiHeader |= PRESET_DICT;
            }
            uiHeader += 31 - (uiHeader % 31);
            
            pstZipState->iStatus = BUSY_STATE;
            Zip_State_SetBuf(pstZipState, uiHeader);
            
            /* Save the adler checksum of the preset dictionary: */
            if (pstZipState->uiStrStart != 0) 
            {
                Zip_State_SetBuf(pstZipState, (WE_UINT32)(pstStream->ulAdlerCheck >> 16));
                Zip_State_SetBuf(pstZipState, (WE_UINT32)(pstStream->ulAdlerCheck & 0xffff));
            }
            pstStream->ulAdlerCheck = Zip_AdlerCheck(0L, NULL, 0);
        }
    }
    if (pstZipState->iStatus == ADDITIONAL_STATE) 
    {
        if (pstZipState->pstGzipHeader->uiExtraDataLen != 0)
        {
            WE_UINT32 uiBeg = pstZipState->uiPendingBufHave;  /* start of bytes to update crc */
            
            while (pstZipState->uiGzipIndex < (pstZipState->pstGzipHeader->uiExtraDataLen & 0xffff)) 
            {
                if (pstZipState->uiPendingBufHave == pstZipState->ulPendingBufSize) 
                {
                    if (pstZipState->pstGzipHeader->iCrcHeadCheck && pstZipState->uiPendingBufHave > uiBeg)
                    {
                        pstStream->ulAdlerCheck = Zip_CrcCheck(pstStream->ulAdlerCheck, pstZipState->pucPendingBuf + uiBeg,
                            pstZipState->uiPendingBufHave - uiBeg);
                    }
                    Zip_Pending_Flush(pstStream);
                    uiBeg = pstZipState->uiPendingBufHave;
                    if (pstZipState->uiPendingBufHave == pstZipState->ulPendingBufSize)
                    {
                        break;
                    }
                }
                SET_BUF_BYTE(pstZipState, pstZipState->pstGzipHeader->pucExtraData[pstZipState->uiGzipIndex]);
                pstZipState->uiGzipIndex++;
            }
            if (pstZipState->pstGzipHeader->iCrcHeadCheck && pstZipState->uiPendingBufHave > uiBeg)
            {
                pstStream->ulAdlerCheck = Zip_CrcCheck(pstStream->ulAdlerCheck, pstZipState->pucPendingBuf + uiBeg,
                    pstZipState->uiPendingBufHave - uiBeg);
            }
            if (pstZipState->uiGzipIndex == pstZipState->pstGzipHeader->uiExtraDataLen) 
            {
                pstZipState->uiGzipIndex = 0;
                pstZipState->iStatus = NAME_STATE;
            }
        }
        else
        {
            pstZipState->iStatus = NAME_STATE;
        }
    }
    if (pstZipState->iStatus == NAME_STATE) 
    {
        if (pstZipState->pstGzipHeader->pucFileName != NULL) 
        {
            WE_UINT32 uiBeg = pstZipState->uiPendingBufHave;  /* start of bytes to update crc */
            WE_INT32 iOffset;
            
            do
            {
                if (pstZipState->uiPendingBufHave == pstZipState->ulPendingBufSize) 
                {
                    if (pstZipState->pstGzipHeader->iCrcHeadCheck && pstZipState->uiPendingBufHave > uiBeg)
                    {
                        pstStream->ulAdlerCheck = Zip_CrcCheck(pstStream->ulAdlerCheck, pstZipState->pucPendingBuf + uiBeg,
                            pstZipState->uiPendingBufHave - uiBeg);
                    }
                    Zip_Pending_Flush(pstStream);
                    uiBeg = pstZipState->uiPendingBufHave;
                    if (pstZipState->uiPendingBufHave == pstZipState->ulPendingBufSize) 
                    {
                        iOffset = 1;
                        break;
                    }
                }
                iOffset = pstZipState->pstGzipHeader->pucFileName[pstZipState->uiGzipIndex++];
                SET_BUF_BYTE(pstZipState, iOffset);
            } while (iOffset != 0);
            
            if (pstZipState->pstGzipHeader->iCrcHeadCheck && pstZipState->uiPendingBufHave > uiBeg)
            {
                pstStream->ulAdlerCheck = Zip_CrcCheck(pstStream->ulAdlerCheck, pstZipState->pucPendingBuf + uiBeg,
                    pstZipState->uiPendingBufHave - uiBeg);
            }
            if (iOffset == 0) 
            {
                pstZipState->uiGzipIndex = 0;
                pstZipState->iStatus = COMMENT_STATE;
            }
        }
        else
        {
            pstZipState->iStatus = COMMENT_STATE;
        }
    }
    if (pstZipState->iStatus == COMMENT_STATE) 
    {
        if (pstZipState->pstGzipHeader->pucComment != NULL) 
        {
            WE_UINT32 uiBeg = pstZipState->uiPendingBufHave;  /* start of bytes to update crc */
            WE_INT32 iOffset = 0;
            
            do 
            {
                if (pstZipState->uiPendingBufHave == pstZipState->ulPendingBufSize) 
                {
                    if (pstZipState->pstGzipHeader->iCrcHeadCheck && pstZipState->uiPendingBufHave > uiBeg)
                    {
                        pstStream->ulAdlerCheck = Zip_CrcCheck(pstStream->ulAdlerCheck, pstZipState->pucPendingBuf + uiBeg,
                            pstZipState->uiPendingBufHave - uiBeg);
                    }
                    Zip_Pending_Flush(pstStream);
                    uiBeg = pstZipState->uiPendingBufHave;
                    if (pstZipState->uiPendingBufHave == pstZipState->ulPendingBufSize) 
                    {
                        iOffset = 1;
                        break;
                    }
                }
                iOffset = pstZipState->pstGzipHeader->pucComment[pstZipState->uiGzipIndex++];
                SET_BUF_BYTE(pstZipState, iOffset);
                
            } while (iOffset != 0);
            if (pstZipState->pstGzipHeader->iCrcHeadCheck && pstZipState->uiPendingBufHave > uiBeg)
            {
                pstStream->ulAdlerCheck = Zip_CrcCheck(pstStream->ulAdlerCheck, pstZipState->pucPendingBuf + uiBeg,
                    pstZipState->uiPendingBufHave - uiBeg);
            }
            if (iOffset == 0)
            {
                pstZipState->iStatus = HCRC_STATE;
            }
        }
        else
        {
            pstZipState->iStatus = HCRC_STATE;
        }
    }
    if (pstZipState->iStatus == HCRC_STATE) 
    {
        if (pstZipState->pstGzipHeader->iCrcHeadCheck) 
        {
            if (pstZipState->uiPendingBufHave + 2 > pstZipState->ulPendingBufSize)
            {
                Zip_Pending_Flush(pstStream);
            }
            if (pstZipState->uiPendingBufHave + 2 <= pstZipState->ulPendingBufSize) 
            {
                SET_BUF_BYTE(pstZipState, (WE_UINT8)(pstStream->ulAdlerCheck & 0xff));
                SET_BUF_BYTE(pstZipState, (WE_UINT8)((pstStream->ulAdlerCheck >> 8) & 0xff));
                pstStream->ulAdlerCheck = Zip_CrcCheck(0L, NULL, 0);
                pstZipState->iStatus = BUSY_STATE;
            }
        }
        else
        {
            pstZipState->iStatus = BUSY_STATE;
        }
    }
    
    /* Flush as much pending output as possible */
    if (pstZipState->uiPendingBufHave != 0) 
    {
        Zip_Pending_Flush(pstStream);
        if (pstStream->uiAvailOutput == 0) 
        {
        /* Since uiAvailOutput is 0, deflate will be called again with
        * more output space, but possibly with both pending and
        * uiAvailInput equal to zero. There won't be anything to do,
        * but this is not an error situation so make sure we
        * return OK instead of BUF_ERROR at next call of deflate:
            */
            pstZipState->iPrevFlush = -1;
            return ZIP_OK;
        }
        
    } 
    else if (pstStream->uiAvailInput == 0 && 
        iFlush <= iPrevFlush &&
        iFlush != ZIP_FINISH)
    {
        return ZIP_BUF_ERROR;
    }
    
    /* User must not provide more input after the first FINISH: */
    if (pstZipState->iStatus == FINISH_STATE && pstStream->uiAvailInput != 0) 
    {
        return ZIP_BUF_ERROR;
    }
    
    /* Start a new block or continue the current one.
    */
    if (pstStream->uiAvailInput != 0 || 
        pstZipState->uiBytesAhead != 0 ||
        (iFlush != ZIP_NO_FLUSH && pstZipState->iStatus != FINISH_STATE)
        ) 
    {
        E_ZIP_Block_State bstate;
        switch(pstZipState->iLevel) 
        {
        case 0:
            bstate = Zip_Filled(pstZipState, iFlush);
            break;
        case 1:
        case 2:
        case 3:
            bstate = Zip_Quickly(pstZipState, iFlush);
            break;
        default:
            bstate = Zip_Slowly(pstZipState, iFlush);
        }
        if (bstate == E_ZIP_Finish_Started || 
            bstate == E_ZIP_Finish_Done) 
        {
            pstZipState->iStatus = FINISH_STATE;
        }
        if (bstate == E_ZIP_Need_More || 
            bstate == E_ZIP_Finish_Started) 
        {
            if (pstStream->uiAvailOutput == 0)
            {
                pstZipState->iPrevFlush = -1; /* avoid BUF_ERROR next call, see above */
            }
            return ZIP_OK;
            /* If flush != Z_NO_FLUSH && uiAvailOutput == 0, the next call
            * of deflate should use the same flush parameter to make sure
            * that the flush is complete.
            */
        }
        if (bstate == E_ZIP_Block_Done) 
        {
            if (iFlush == ZIP_PARTIAL_FLUSH)
            {
                Unzip_Align(pstZipState);
            } 
            else 
            {	/* FULL_FLUSH or SYNC_FLUSH */
                Mass_SavedSend(pstZipState, (WE_INT8*)0, 0L, 0);
                /* For a full flush, this empty block will be recognized
                * as a special marker by Unzip_FindSyncPoint().*/
                
                if (iFlush == ZIP_FULL_FLUSH) 
                {
                    CLEAR_HASH(pstZipState);	/* forget history */
                }
            }
            Zip_Pending_Flush(pstStream);
            if (pstStream->uiAvailOutput == 0) 
            {
                pstZipState->iPrevFlush = -1;	/* avoid BUF_ERROR at next call */
                return ZIP_OK;
            }
        }
    }
    if (iFlush != ZIP_FINISH) 
    {
        return ZIP_OK;
    }
    if (pstZipState->iFormat <= 0) 
    {
        return ZIP_STREAM_END;
    }
    /* Write the trailer */
    if (pstZipState->iFormat == 2) 
    {
        SET_BUF_BYTE(pstZipState, (WE_UINT8)(pstStream->ulAdlerCheck & 0xff));
        SET_BUF_BYTE(pstZipState, (WE_UINT8)((pstStream->ulAdlerCheck >> 8) & 0xff));
        SET_BUF_BYTE(pstZipState, (WE_UINT8)((pstStream->ulAdlerCheck >> 16) & 0xff));
        SET_BUF_BYTE(pstZipState, (WE_UINT8)((pstStream->ulAdlerCheck >> 24) & 0xff));
        SET_BUF_BYTE(pstZipState, (WE_UINT8)(pstStream->ulTotalInput & 0xff));
        SET_BUF_BYTE(pstZipState, (WE_UINT8)((pstStream->ulTotalInput >> 8) & 0xff));
        SET_BUF_BYTE(pstZipState, (WE_UINT8)((pstStream->ulTotalInput >> 16) & 0xff));
        SET_BUF_BYTE(pstZipState, (WE_UINT8)((pstStream->ulTotalInput >> 24) & 0xff));
    }
    else
    {
        Zip_State_SetBuf(pstZipState, (WE_UINT32)(pstStream->ulAdlerCheck >> 16));
        Zip_State_SetBuf(pstZipState, (WE_UINT32)(pstStream->ulAdlerCheck & 0xffff));
    }
    Zip_Pending_Flush(pstStream);
    /* If uiAvailOutput is zero, the application will call Zip_stream again
    * to flush the rest.
    */
    if (pstZipState->iFormat > 0) pstZipState->iFormat = -pstZipState->iFormat; /* write the trailer only once! */
    return pstZipState->uiPendingBufHave != 0 ? ZIP_OK : ZIP_STREAM_END;
}
/*=================================================================================
===
	FUNCTION: Zip_Done

	CREATE DATE: 2006-11-3
	AUTHOR: duhuanbiao

	DESCRIPTION:
		All dynamically allocated data structures for this stream are freed
		
	ARGUMENTS PASSED:

	RETURN VALUE:

	CALL BY:

	IMPORTANT NOTES:
		NULL

===================================================================================
==*/
static WE_INT32 Zip_Done(
                         P_St_StreamState pstStream
                         )
{
    WE_INT32 iStatue = 0;
    
    if (pstStream == NULL || pstStream->pstZipState == NULL) 
    {
        return ZIP_PARA_ERR;
    }
    iStatue = pstStream->pstZipState->iStatus;

    /* Deallocate in reverse order of allocations: */
    TOFREE(pstStream->pstZipState->stLDest.pstStatTree);
    TOFREE(pstStream->pstZipState->pucPendingBuf);
    TOFREE(pstStream->pstZipState->puHashHead);
    TOFREE(pstStream->pstZipState->puPrevStr);
    TOFREE(pstStream->pstZipState->pucWindow);
    TOFREE(pstStream->pstZipState);

    if (iStatue != INIT_STATE &&
        iStatue != ADDITIONAL_STATE &&
        iStatue != NAME_STATE &&
        iStatue != COMMENT_STATE &&
        iStatue != HCRC_STATE &&
        iStatue != BUSY_STATE &&
        iStatue != FINISH_STATE) 
    {
        return ZIP_STREAM_ERROR;
    }
    
    return iStatue == BUSY_STATE ? ZIP_DATA_ERROR : ZIP_OK;
}

/*=================================================================================
===
	FUNCTION: Zip_Read_Buf

	CREATE DATE: 2006-11-3
	AUTHOR: duhuanbiao

	DESCRIPTION:
		Read a new buffer from the current input stream, update the adler checksum
	and total number of bytes read.  All Zip_Stream() input goes through this function
	so some applications may wish to modify it to avoid allocating a large pstStream->pucNextInput 
	buffer and copying from it.
		
	ARGUMENTS PASSED:

	RETURN VALUE:

	CALL BY:

	IMPORTANT NOTES:
		NULL

===================================================================================
==*/
static WE_INT32 Zip_Read_Buf(
                             P_St_StreamState pstStream,
                             WE_UINT8 *pucReadbuf,
                             WE_UINT32 uiSize
                             )
{
    WE_UINT32 uiLen = 0U; 
    
    if (pstStream == NULL ||
        pucReadbuf == NULL)
    {
        return ZIP_PARA_ERR;
    }
    
    uiLen = pstStream->uiAvailInput;
    
    if (uiLen > uiSize)
    {
        uiLen = uiSize;
    }
    if (uiLen == 0U) 
    {
        return 0;
    }
    
    pstStream->uiAvailInput  -= uiLen;
    
    if (pstStream->pstZipState->iFormat == 1) 
    {
        pstStream->ulAdlerCheck = Zip_AdlerCheck(pstStream->ulAdlerCheck, pstStream->pucNextInput, uiLen);
    }
    else if (pstStream->pstZipState->iFormat == 2) 
    {
        pstStream->ulAdlerCheck = Zip_CrcCheck(pstStream->ulAdlerCheck, pstStream->pucNextInput, uiLen);
    }
    pucReadbuf = (WE_UINT8*)MEMCPY(pucReadbuf, pstStream->pucNextInput, uiLen);
    pstStream->pucNextInput  += uiLen;
    pstStream->ulTotalInput += uiLen;
    
    return (WE_INT32)uiLen;
}

/*=================================================================================
===
	FUNCTION: Zip_Height_Check_Init

	CREATE DATE: 2006-11-3
	AUTHOR: duhuanbiao

	DESCRIPTION:
		Initialize the "longest match" routines for a new zip stream.
		
	ARGUMENTS PASSED:

	RETURN VALUE:

	CALL BY:

	IMPORTANT NOTES:
		NULL

===================================================================================
==*/
static WE_VOID Zip_Height_Check_Init(
                                  St_ZIP_State *pstZipState
                                  )
{
    if (pstZipState == NULL)
    {
        return;
    }
    pstZipState->ulWindowSizeAll = (WE_ULONG)2L * pstZipState->uiWindowSize;
    
    CLEAR_HASH(pstZipState);
    
    /* Set the default configuration parameters: */
    pstZipState->uiMaxLazyMatch = s_Zip_Config_List[pstZipState->iLevel].uLengthNoLazy;
    pstZipState->uiGoodMatch = s_Zip_Config_List[pstZipState->iLevel].uLengthGood;
    pstZipState->iMaxMatch = s_Zip_Config_List[pstZipState->iLevel].uLengthQuit;
    pstZipState->uiMaxChainLens = s_Zip_Config_List[pstZipState->iLevel].uMaxChain;
    
    pstZipState->uiStrStart = 0U;
    pstZipState->lBlockStart = 0L;
    pstZipState->uiBytesAhead = 0U;
    pstZipState->uiMatchLens = pstZipState->uiPrevMatchLens = MIN_MATCH - 1U;
    pstZipState->iIsPrevMatchAvail = 0;
    pstZipState->uiHashInsert = 0U;
}

/*=================================================================================
===
	FUNCTION: Zip_HEIGHT_CHECK

	CREATE DATE: 2006-11-3
	AUTHOR: duhuanbiao

	DESCRIPTION:
		Set match_start to the longest match starting at the given string and
	return its length. Matches shorter or equal to prev_length are discarded
		
	ARGUMENTS PASSED:

	RETURN VALUE:

	CALL BY:

	IMPORTANT NOTES:
		NULL

===================================================================================
==*/
static WE_INT32 Zip_HEIGHT_CHECK(
                                 P_St_ZIP_State pstZipState, 
                                 WE_UINT32 uiCurMatch    /* current match */
                                 )                             
{
    WE_UINT32 uiHashChainLen = 0U;
    WE_UINT8 *pucScan = NULL;
    WE_UINT8 *pucMatchStr = NULL; /* matched string */
    WE_INT32 iLen = 0;			  /* length of current match */
    WE_INT32 iBestLen = 0;
    WE_INT32 iNiceMatch = 0;
    WE_UINT32 uiLimit = 0U;
    
    WE_UINT16 *puPrev = NULL;
    WE_UINT32 uiWmask = 0U;
    
    
    WE_UINT8 *pucStrend = NULL;
    WE_UINT8 ucScanEnd1 = 0;
    WE_UINT8 ucScanEnd = 0;
    
    if (pstZipState == NULL)
    {
        return ZIP_PARA_ERR;
    }
    
    uiHashChainLen = pstZipState->uiMaxChainLens;/* max hash chain length */
    
    pucScan = pstZipState->pucWindow + pstZipState->uiStrStart; /* current string */
    
    iBestLen = (WE_INT32)pstZipState->uiPrevMatchLens;	/* best match length so far */
    
    iNiceMatch = pstZipState->iMaxMatch;		/* stop if match long enough */
    
    uiLimit = pstZipState->uiStrStart > (WE_UINT32)MAX_DIST(pstZipState) ?
        pstZipState->uiStrStart - (WE_UINT32)MAX_DIST(pstZipState) : ZERO;
        /* Stop when uiCurMatch becomes <= limit. To simplify the code,
        * we prevent matches with the string of pucWindow index 0.
    */
    
    puPrev = pstZipState->puPrevStr;
    uiWmask = pstZipState->uiWindowMask;
    
    pucStrend = pstZipState->pucWindow + pstZipState->uiStrStart + MAX_MATCH;
    ucScanEnd1 = pucScan[iBestLen-1];
    ucScanEnd = pucScan[iBestLen];
    
    /* Do not waste too much time if we already have a good match: */
    if (pstZipState->uiPrevMatchLens >= pstZipState->uiGoodMatch) 
    {
        uiHashChainLen >>= 2;
    }
    /* Do not look for matches beyond the end of the input. This is necessary
    * to make deflate deterministic.
    */
    if ((WE_UINT32)iNiceMatch > pstZipState->uiBytesAhead) 
    {
        iNiceMatch = (WE_INT32)pstZipState->uiBytesAhead;
    }
    
    do {
        pucMatchStr = pstZipState->pucWindow + uiCurMatch;
        
        /* Skip to next match if the match length cannot increase
        * or if the match length is less than 2.
        */
        
        
        if (pucMatchStr[iBestLen]   != ucScanEnd  ||
            pucMatchStr[iBestLen-1] != ucScanEnd1 ||
            *pucMatchStr            != *pucScan   ||
            *++pucMatchStr          != pucScan[1])     
        {
            continue;
        }
        
        /* The check at iBestLen-1 can be removed because it will be made
        * again later.
        */
        pucScan += 2;
        pucMatchStr++;
        
        /* We check for insufficient uiBytesAhead only every 8th comparison;
        * the 256th check will be made at uiStrStart + 258.
        */
        while (*++pucScan == *++pucMatchStr && *++pucScan == *++pucMatchStr &&
            *++pucScan == *++pucMatchStr && *++pucScan == *++pucMatchStr &&
            *++pucScan == *++pucMatchStr && *++pucScan == *++pucMatchStr &&
            *++pucScan == *++pucMatchStr && *++pucScan == *++pucMatchStr &&
            pucScan < pucStrend);
        
        iLen = (WE_INT32)(MAX_MATCH - (WE_INT32)(pucStrend - pucScan));
        pucScan = pucStrend - MAX_MATCH;
        
        if (iLen > iBestLen) 
        {
            pstZipState->uiMatchStart = uiCurMatch;
            iBestLen = iLen;
            if (iLen >= iNiceMatch) 
            {
                break;
            }
            ucScanEnd1  = pucScan[iBestLen-1];
            ucScanEnd   = pucScan[iBestLen];
            
        }
    } while ((uiCurMatch = puPrev[uiCurMatch & uiWmask]) > uiLimit && --uiHashChainLen != 0);
    
    if ((WE_UINT32)iBestLen <= pstZipState->uiBytesAhead) 
    {
        return iBestLen;
    }
    return (WE_INT32)pstZipState->uiBytesAhead;
}

/*=================================================================================
===
	FUNCTION: Zip_HEIGHT_CHECK_QUICKLY

	CREATE DATE: 2006-11-3
	AUTHOR: duhuanbiao

	DESCRIPTION:
		Optimized version for iLevel == 1 or iStrategy == Z_RLE only
		
	ARGUMENTS PASSED:

	RETURN VALUE:

	CALL BY:

	IMPORTANT NOTES:
		NULL

===================================================================================
==*/
static WE_INT32 Zip_HEIGHT_CHECK_QUICKLY(
                                         P_St_ZIP_State pstZipState,
                                         WE_UINT32 uiCurMatch
                                         )                             /* current match */
{
    WE_UINT8 *pucScan = NULL;
    WE_UINT8 *pucMatchStr = NULL;        /* matched string */
    WE_INT32 iLen = 0;                   /* length of current match */
    WE_UINT8 *pucStrend = NULL;
    
    if (pstZipState == NULL)
    {
        return ZIP_PARA_ERR;
    }
    pucScan = pstZipState->pucWindow + pstZipState->uiStrStart; /* current string */
    pucStrend = pstZipState->pucWindow + pstZipState->uiStrStart + MAX_MATCH;
    
    pucMatchStr = pstZipState->pucWindow + uiCurMatch;
    
    /* Return failure if the match length is less than 2: */
    if (pucMatchStr[0] != pucScan[0] || pucMatchStr[1] != pucScan[1]) 
    {
        return MIN_MATCH-1;
    }
    
    /* The check at iBestLen-1 can be removed because it will be made
    * again later. 
    */
    pucScan += 2;
    pucMatchStr += 2;
    
    /* We check for insufficient uiBytesAhead only every 8th comparison;
    * the 256th check will be made at uiStrStart+258.
    */
    while (*++pucScan == *++pucMatchStr && 
        *++pucScan == *++pucMatchStr &&
        *++pucScan == *++pucMatchStr && 
        *++pucScan == *++pucMatchStr &&
        *++pucScan == *++pucMatchStr && 
        *++pucScan == *++pucMatchStr &&
        *++pucScan == *++pucMatchStr && 
        *++pucScan == *++pucMatchStr &&
        pucScan < pucStrend);
    
    
    iLen = MAX_MATCH - (WE_INT32)(pucStrend - pucScan);
    
    if (iLen < MIN_MATCH)
    {
        return MIN_MATCH - 1;
    }
    pstZipState->uiMatchStart = uiCurMatch;
    return ((WE_UINT32)iLen) <= pstZipState->uiBytesAhead ? iLen : (WE_INT32)pstZipState->uiBytesAhead;
}


/*=================================================================================
===
	FUNCTION: Zip_Fill_BufWindow

	CREATE DATE: 2006-11-3
	AUTHOR: duhuanbiao

	DESCRIPTION:
		Fill the pucWindow when the uiBytesAhead becomes insufficient.
	Updates uiStrStart and uiBytesAhead.
		
	ARGUMENTS PASSED:

	RETURN VALUE:

	CALL BY:

	IMPORTANT NOTES:
		NULL

===================================================================================
==*/
static WE_VOID Zip_Fill_BufWindow(
                               P_St_ZIP_State pstZipState
                               )
{
    WE_UINT32 uiSize = 0U;
    WE_UINT32 uiValue = 0U;
    WE_UINT16 *puHash = NULL;
    WE_UINT32 uiMore = 0U;    /* Amount of free space at the end of the pucWindow. */
    WE_UINT32 uiWindowSize = 0U;
    
    if (pstZipState == NULL)
    {
        return;
    }
    
    uiWindowSize = pstZipState->uiWindowSize;
    
    do 
    {
        uiMore = (WE_UINT32)(pstZipState->ulWindowSizeAll - (WE_ULONG)pstZipState->uiBytesAhead 
            - (WE_ULONG)pstZipState->uiStrStart);
        
            /* If the pucWindow is almost full and there is insufficient uiBytesAhead,
            * move the upper half to the lower one to make room in the upper half.
        */
        if (pstZipState->uiStrStart >= uiWindowSize + MAX_DIST(pstZipState)) 
        {
            
            pstZipState->pucWindow = (WE_UINT8*)MEMCPY(pstZipState->pucWindow, pstZipState->pucWindow + uiWindowSize, (WE_UINT32)uiWindowSize);
            pstZipState->uiMatchStart -= uiWindowSize;
            pstZipState->uiStrStart -= uiWindowSize; /* we now have uiStrStart >= MAX_DIST */
            pstZipState->lBlockStart -= (WE_LONG) uiWindowSize;
            
            /* Slide the hash table . We slide even when iLevel == 0
            * to keep the hash table consistent if we switch back to iLevel > 0
            * later. 
            */
            
            /* %%% avoid this when ZIP_RLE */
            uiSize = pstZipState->uiHashSize;
            puHash = &pstZipState->puHashHead[uiSize];
            do 
            {
                uiValue = *--puHash;
                *puHash = (WE_UINT16)(uiValue >= uiWindowSize ? (uiValue - uiWindowSize) : ZERO);
            } while (--uiSize);
            
            uiSize = uiWindowSize;
            puHash = &pstZipState->puPrevStr[uiSize];
            do 
            {
                uiValue = *--puHash;
                *puHash = (WE_UINT16)(uiValue >= uiWindowSize ? (uiValue-uiWindowSize) : ZERO);
                /* If uiSize is not on any hash chain, puPrev[uiSize] is garbage but
                * its value will never be used.
                */
            } while (--uiSize);
            uiMore += uiWindowSize;
        }
        if (pstZipState->pstStream->uiAvailInput == 0) 
        {
            return;
        }
        
        uiSize = (WE_UINT32)Zip_Read_Buf(pstZipState->pstStream, 
            pstZipState->pucWindow + pstZipState->uiStrStart + pstZipState->uiBytesAhead, uiMore);
        pstZipState->uiBytesAhead += uiSize;
        
        /* Initialize the hash value now that we have some input: */
        if (pstZipState->uiBytesAhead >= MIN_MATCH) 
        {
            pstZipState->uiHashInsert = pstZipState->pucWindow[pstZipState->uiStrStart];
            UPDATE_HASH(pstZipState, pstZipState->uiHashInsert, pstZipState->pucWindow[pstZipState->uiStrStart+1]);
        }
        /* If the whole input has less than MIN_MATCH bytes, ins_h is garbage,
        * but this is not important since only literal bytes will be emitted.
        */
        
    } while (pstZipState->uiBytesAhead < MIN_LOOKAHEAD && pstZipState->pstStream->uiAvailInput != 0);
}

/*=================================================================================
===
	FUNCTION: Zip_Filled

	CREATE DATE: 2006-11-3
	AUTHOR: duhuanbiao

	DESCRIPTION:
		Copy without compression as much as possible from the input stream, return
	the current block state.
		
	ARGUMENTS PASSED:

	RETURN VALUE:

	CALL BY:

	IMPORTANT NOTES:
		This function does not insert new strings in the dictionary since
	uncompressible data is probably not useful. This function is used only 
	for the iLevel=0 compression option.

===================================================================================
==*/
static E_ZIP_Block_State Zip_Filled(
                                    P_St_ZIP_State pstZipState, 
                                    WE_INT32 iFlush
                                    )
{
/* Stored blocks are limited to 0xffff bytes, pending_buf is limited
    to ulPendingBufSize, and each stored block has a 5 byte header:*/
    WE_ULONG ulMaxBlockSize = 0xffffUL;
    WE_ULONG ulMaxStart = 0UL;
    
    if (pstZipState == NULL)
    {
        return E_ZIP_PARA_ERROR;
    }
    if (ulMaxBlockSize > pstZipState->ulPendingBufSize - 5) 
    {
        ulMaxBlockSize = pstZipState->ulPendingBufSize - 5;
    }
    
    /* Copy as much as possible from input to output: */
    for (;;) 
    {
        /* Fill the window as much as possible: */
        if (pstZipState->uiBytesAhead <= 1) 
        {
            Zip_Fill_BufWindow(pstZipState);
            if (pstZipState->uiBytesAhead == 0 && iFlush == ZIP_NO_FLUSH) 
            {
                return E_ZIP_Need_More;
            }
            if (pstZipState->uiBytesAhead == 0)
            {
                break; /* flush the current block */
            }
        }
        
        pstZipState->uiStrStart += pstZipState->uiBytesAhead;
        pstZipState->uiBytesAhead = 0;
        
        /* Emit a stored block if pending_buf will be full: */
        ulMaxStart = (WE_ULONG)pstZipState->lBlockStart + ulMaxBlockSize;
        if (pstZipState->uiStrStart == 0 || ((WE_ULONG)pstZipState->uiStrStart) >= ulMaxStart) 
        {
            /* uiStrStart == 0 is possible when wraparound on 16-bit machine */
            pstZipState->uiBytesAhead = (WE_UINT32)(pstZipState->uiStrStart - ulMaxStart);
            pstZipState->uiStrStart = (WE_UINT32)ulMaxStart;
            FLUSH_BLOCK(pstZipState, FALSE);
        }
        /* Flush if we may have to slide, otherwise block_start may become
        * negative and the data will be gone:
        */
        if (pstZipState->uiStrStart - ((WE_UINT32)pstZipState->lBlockStart) >= MAX_DIST(pstZipState)) 
        {
            FLUSH_BLOCK(pstZipState, FALSE);
        }
    }
    FLUSH_BLOCK(pstZipState, iFlush == ZIP_FINISH);
    return iFlush == ZIP_FINISH ? E_ZIP_Finish_Done : E_ZIP_Block_Done;
}

/*=================================================================================
===
	FUNCTION: Zip_Quickly

	CREATE DATE: 2006-11-3
	AUTHOR: duhuanbiao

	DESCRIPTION:
		Compress as much as possible from the input stream, return the current
	block state.
		
	ARGUMENTS PASSED:

	RETURN VALUE:

	CALL BY:

	IMPORTANT NOTES:
		This function does not perform lazy evaluation of matches and inserts
	new strings in the dictionary only for unmatched strings or for short matches. 
	It is used only for the fast compression options.

===================================================================================
==*/
static E_ZIP_Block_State Zip_Quickly(
                                     St_ZIP_State *pstZipState,
                                     WE_INT32 iFlush
                                     )
{
    WE_UINT32 uiHashHead = ZERO; /* head of the hash chain */
    WE_INT32 iNeedFlush = 0;     /* set if current block must be flushed */
    
    if (pstZipState == NULL)
    {
        return E_ZIP_PARA_ERROR;
    }
    
    for (;;) 
    {
    /* Make sure that we always have enough uiBytesAhead, except
        at the end of the input file.*/
        if (pstZipState->uiBytesAhead < MIN_LOOKAHEAD) 
        {
            Zip_Fill_BufWindow(pstZipState);
            if (pstZipState->uiBytesAhead < MIN_LOOKAHEAD && iFlush == ZIP_NO_FLUSH) 
            {
                return E_ZIP_Need_More;
            }
            if (pstZipState->uiBytesAhead == 0) 
            {
                break; /* flush the current block */
            }
        }
        
        /* Insert the string window[uiStrStart .. uiStrStart+2] in the
        dictionary, and set uiHashHead to the head of the hash chain:*/
        if (pstZipState->uiBytesAhead >= MIN_MATCH) 
        {
            INSERT_STRING(pstZipState, pstZipState->uiStrStart, uiHashHead);
        }
        
        /* Find the longest match, discarding those <= uiPrevMatchLens.
        At this point we have always uiMatchLens < MIN_MATCH */
        if (uiHashHead != ZERO && pstZipState->uiStrStart - uiHashHead <= MAX_DIST(pstZipState)) 
        {
        /* To simplify the code, we prevent matches with the string
            of window index 0 */
            if (pstZipState->iStrategy != ZIP_HUFFMAN_ONLY && pstZipState->iStrategy != ZIP_RLE) 
            {
                pstZipState->uiMatchLens = (WE_UINT32)Zip_HEIGHT_CHECK(pstZipState, uiHashHead);
            }
            else if (pstZipState->iStrategy == ZIP_RLE && pstZipState->uiStrStart - uiHashHead == 1) 
            {
                pstZipState->uiMatchLens = (WE_UINT32)Zip_HEIGHT_CHECK_QUICKLY (pstZipState, uiHashHead);
            }
        }
        if (pstZipState->uiMatchLens >= MIN_MATCH) 
        {
            
            TREE_PUSH_D(pstZipState, pstZipState->uiStrStart - pstZipState->uiMatchStart,
                pstZipState->uiMatchLens - MIN_MATCH, iNeedFlush);
            
            pstZipState->uiBytesAhead -= pstZipState->uiMatchLens;
            
            /* Insert new strings in the hash table only if the match length
            * is not too large. This saves time but degrades compression.
            */
            
            if (pstZipState->uiMatchLens <= pstZipState->MAX_INSERT_LENS &&
                pstZipState->uiBytesAhead >= MIN_MATCH) 
            {
                pstZipState->uiMatchLens--; /* string at uiStrStart already in table */
                do 
                {
                    pstZipState->uiStrStart++;
                    INSERT_STRING(pstZipState, pstZipState->uiStrStart, uiHashHead);
                    /* uiStrStart never exceeds WSIZE-MAX_MATCH, so there are
                    * always MIN_MATCH bytes ahead.
                    */
                } while (--pstZipState->uiMatchLens != 0);
                pstZipState->uiStrStart++;
            } 
            else
            {
                pstZipState->uiStrStart += pstZipState->uiMatchLens;
                pstZipState->uiMatchLens = 0;
                pstZipState->uiHashInsert = pstZipState->pucWindow[pstZipState->uiStrStart];
                UPDATE_HASH(pstZipState, pstZipState->uiHashInsert, pstZipState->pucWindow[pstZipState->uiStrStart+1]);
                /* If uiBytesAhead < MIN_MATCH, uiHashInsert is garbage, but it does not
                matter since it will be recomputed at next deflate call.*/
            }
        } 
        else 
        {
            /* No match, output a literal byte */
            TREE_PUSH_L(pstZipState, pstZipState->pucWindow[pstZipState->uiStrStart], iNeedFlush);
            pstZipState->uiBytesAhead--;
            pstZipState->uiStrStart++;
        }
        if (iNeedFlush) 
        {
            FLUSH_BLOCK(pstZipState, FALSE);
        }
    }
    FLUSH_BLOCK(pstZipState, iFlush == ZIP_FINISH);
    return iFlush == ZIP_FINISH ? E_ZIP_Finish_Done : E_ZIP_Block_Done;
}

 /*=================================================================================
===
	FUNCTION: Zip_Slowly

	CREATE DATE: 2006-11-3
	AUTHOR: duhuanbiao

	DESCRIPTION:
		Same as above, but achieves better compression. We use a lazy
	evaluation for matches: a match is finally adopted only if there is
	no better match at the next window position.
		
	ARGUMENTS PASSED:

	RETURN VALUE:

	CALL BY:

	IMPORTANT NOTES:
		NULL

===================================================================================
==*/
static E_ZIP_Block_State Zip_Slowly(
                                    P_St_ZIP_State pstZipState, 
                                    WE_INT32 iFlush
                                    )
{
    WE_UINT32 uiHashHead = ZERO;           /* head of hash chain */
    WE_INT32 iNeedFlush = 0;               /* set if current block must be flushed */
    
    if (pstZipState == NULL)
    {
        return E_ZIP_PARA_ERROR;
    }
    
    /* Process the input block. */
    for (;;) 
    {
    /* Make sure that we always have enough uiBytesAhead, except
    * at the end of the input file.
        */
        if (pstZipState->uiBytesAhead < MIN_LOOKAHEAD) 
        {
            Zip_Fill_BufWindow(pstZipState);
            if (pstZipState->uiBytesAhead < MIN_LOOKAHEAD && iFlush == ZIP_NO_FLUSH)
            {
                return E_ZIP_Need_More;
            }
            if (pstZipState->uiBytesAhead == 0) break; /* flush the current block */
        }
        
        /* Insert the string window[uiStrStart .. uiStrStart+2] in the
        dictionary, and set uiHashHead to the head of the hash chain: */
        if (pstZipState->uiBytesAhead >= MIN_MATCH) 
        {
            INSERT_STRING(pstZipState, pstZipState->uiStrStart, uiHashHead);
        }
        
        /* Find the longest match, discarding those <= uiPrevMatchLens.*/
        pstZipState->uiPrevMatchLens = pstZipState->uiMatchLens, pstZipState->uiPrevMatch = pstZipState->uiMatchStart;
        pstZipState->uiMatchLens = MIN_MATCH-1;
        
        if (uiHashHead != ZERO && pstZipState->uiPrevMatchLens < pstZipState->uiMaxLazyMatch &&
            pstZipState->uiStrStart - uiHashHead <= MAX_DIST(pstZipState))
        {
            if (pstZipState->iStrategy != ZIP_HUFFMAN_ONLY && pstZipState->iStrategy != ZIP_RLE) 
            {
                pstZipState->uiMatchLens = (WE_UINT32)Zip_HEIGHT_CHECK (pstZipState, uiHashHead);
            } 
            else if (pstZipState->iStrategy == ZIP_RLE && pstZipState->uiStrStart - uiHashHead == 1) 
            {
                pstZipState->uiMatchLens = (WE_UINT32)Zip_HEIGHT_CHECK_QUICKLY (pstZipState, uiHashHead);
            }
            
            if (pstZipState->uiMatchLens <= 5 && 
                (pstZipState->iStrategy == ZIP_FILTERED || (pstZipState->uiMatchLens == MIN_MATCH &&
                pstZipState->uiStrStart - pstZipState->uiMatchStart > TOO_FAR)
                )) 
            {
                
            /* If uiPrevMatch is also MIN_MATCH, uiMatchStart is garbage
                but we will ignore the current match anyway.*/
                pstZipState->uiMatchLens = MIN_MATCH-1;
            }
        }
        /* If there was a match at the previous step and the current
        * match is not better, output the previous match:
        */
        if (pstZipState->uiPrevMatchLens >= MIN_MATCH && pstZipState->uiMatchLens <= pstZipState->uiPrevMatchLens) 
        {
            WE_UINT32 max_insert = pstZipState->uiStrStart + pstZipState->uiBytesAhead - MIN_MATCH;
            /* Do not insert strings in hash table beyond this. */
            
            
            TREE_PUSH_D(pstZipState, pstZipState->uiStrStart -1 - pstZipState->uiPrevMatch,
                pstZipState->uiPrevMatchLens - MIN_MATCH, iNeedFlush);
            
                /* Insert in hash table all strings up to the end of the match.
                uiStrStart-1 and uiStrStart are already inserted. If there is not
                enough uiBytesAhead, the last two strings are not inserted in
            the hash table.*/
            pstZipState->uiBytesAhead -= pstZipState->uiPrevMatchLens-1;
            pstZipState->uiPrevMatchLens -= 2;
            do 
            {
                if (++pstZipState->uiStrStart <= max_insert)
                {
                    INSERT_STRING(pstZipState, pstZipState->uiStrStart, uiHashHead);
                }
            } while (--pstZipState->uiPrevMatchLens != 0);
            
            pstZipState->iIsPrevMatchAvail = 0;
            pstZipState->uiMatchLens = MIN_MATCH-1;
            pstZipState->uiStrStart++;
            
            if (iNeedFlush) 
            {
                FLUSH_BLOCK(pstZipState, FALSE);
            }
            
        } 
        else if (pstZipState->iIsPrevMatchAvail) 
        {
        /* If there was no match at the previous position, output a
        single literal. If there was a match but the current match
            is longer, truncate the previous match to a single literal.*/
            TREE_PUSH_L(pstZipState, pstZipState->pucWindow[pstZipState->uiStrStart-1], iNeedFlush);
            if (iNeedFlush) 
            {
                FLUSH_BLOCK_ONLY(pstZipState, FALSE);
            }
            pstZipState->uiStrStart++;
            pstZipState->uiBytesAhead--;
            if (pstZipState->pstStream->uiAvailOutput == 0)
            {
                return E_ZIP_Need_More;
            }
        }
        else 
        {
        /* There is no previous match to compare with, wait for
            the next step to decide.*/
            pstZipState->iIsPrevMatchAvail = 1;
            pstZipState->uiStrStart++;
            pstZipState->uiBytesAhead--;
        }
    }
    if (pstZipState->iIsPrevMatchAvail) 
    {
        TREE_PUSH_L(pstZipState, pstZipState->pucWindow[pstZipState->uiStrStart-1], iNeedFlush);
        pstZipState->iIsPrevMatchAvail = 0;
    }
    FLUSH_BLOCK(pstZipState, iFlush == ZIP_FINISH);
    return iFlush == ZIP_FINISH ? E_ZIP_Finish_Done : E_ZIP_Block_Done;
}

		/*  follow fun used as an interface, see detail in the common.h */

 /*=================================================================================
===
	FUNCTION: Zip_StreamWrite

	CREATE DATE: 2006-11-3
	AUTHOR: duhuanbiao

	DESCRIPTION:
		compress the buffer data to another buffer
		
	ARGUMENTS PASSED:

	RETURN VALUE:

	CALL BY: 
		UI

	IMPORTANT NOTES:
		NULL

===================================================================================
==*/
WE_INT32 Zip_StreamWrite(
                        WE_UINT8* pucBufDest,  
                        WE_UINT32 *puiDestLen,
                        const WE_UINT8 *pucSource, 
                        const WE_UINT32 uiSourceLen, 
                        WE_INT32 iLevel,
                        WE_INT32 iFormat
                        )
{
    P_St_StreamState pstStream = NULL;
    WE_INT32 iErr = ZIP_STREAM_ERROR;
    
    if (pucBufDest == NULL ||
        puiDestLen == NULL ||
        pucSource == NULL)
    {
        return ZIP_PARA_ERR;
    }
    
    pstStream = (P_St_StreamState)TOALLOC(1, sizeof(St_StreamState));
    if (pstStream == NULL)
    {
        return ZIP_MEM_ERROR;
    }
    pstStream->pucNextInput = (WE_UINT8*)pucSource;
    pstStream->uiAvailInput = (WE_UINT32)uiSourceLen;
    pstStream->pucNextOutput = pucBufDest;
    pstStream->uiAvailOutput = *puiDestLen;
    
    iErr = Zip_Init(pstStream, iLevel, iFormat);
    if (iErr != ZIP_OK)
    {
        Zip_Done(pstStream);
        TOFREE(pstStream);
        return iErr;
    }
    
    iErr = Zip_Stream(pstStream, ZIP_FINISH);
    
    if (iErr != ZIP_STREAM_END)
    {
        Zip_Done(pstStream);
        TOFREE(pstStream);
        return iErr == ZIP_OK ? ZIP_BUF_ERROR : iErr;
    }
    
    *puiDestLen = pstStream->ulTotalOutput;
    
    iErr = Zip_Done(pstStream);
    TOFREE(pstStream);
    
    return iErr;
    
}