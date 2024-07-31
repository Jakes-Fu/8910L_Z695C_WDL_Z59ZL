/*=================================================================================
====
	MODULE NAME : zip-unzip.c
	MODULE NAME : zip

    GENERAL DESCRIPTION
        This File uncompress the stream or file.

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

#include "unzip.h"
#include "zip.h"
#include "AEEFile.h"
#include "AEEShell.h"

/*******************************************************************************
* Macro Define Section
*******************************************************************************/
	
#define GZIP_BUF_LEN 2048U
typedef struct Gzip_State
{
    P_St_StreamState pStream;
    IFile* pIFile;
    IFileMgr* pIFileMgr;
    WE_UINT8* pucBuf;
    WE_UINT32 uiBufLen;
    WE_UINT32 uiDataLeft;

    /* used to check error in file reading */
    WE_UINT32 uiFileRead;

    /* record the last error */
    WE_INT32 iErr;

} Gzip_State, *P_Gzip_State;


			/* Macros for Unzip_Stream(): */
/* check function to use adler_check() or crc_check() */
#  define UPDATE(check, buf, len) \
    (pstUnzipState->iGzipFlg ? Zip_CrcCheck(check, buf, len) : Zip_AdlerCheck(check, buf, len))


/* check macros for header crc */
#  define CRC2(check, word) \
    { \
        ucaHeadCheckBuf[0] = (WE_UINT8)(word); \
        ucaHeadCheckBuf[1] = (WE_UINT8)((word) >> 8); \
        check = Zip_CrcCheck(check, ucaHeadCheckBuf, 2); \
    }

#define CRC4(check, word) \
	{ \
        ucaHeadCheckBuf[0] = (WE_UINT8)(word); \
        ucaHeadCheckBuf[1] = (WE_UINT8)((word) >> 8); \
        ucaHeadCheckBuf[2] = (WE_UINT8)((word) >> 16); \
        ucaHeadCheckBuf[3] = (WE_UINT8)((word) >> 24); \
        check = Zip_CrcCheck(check, ucaHeadCheckBuf, 4); \
    }

/* Load registers with state in Unzip_Stream() for speed */
#define LOAD() \
    { \
        pucNextOutput = pstStream->pucNextOutput; \
        uiOutLeft = pstStream->uiAvailOutput; \
        pucNextIn = pstStream->pucNextInput; \
        uiInHave = pstStream->uiAvailInput; \
        ulDataHold = pstUnzipState->ulDataHold; \
        uiHoldBits = pstUnzipState->uiBitsHave; \
    }

/* Restore state from registers in Unzip_Stream() */
#define RESTORE() \
    { \
        pstStream->pucNextOutput = pucNextOutput; \
        pstStream->uiAvailOutput = uiOutLeft; \
        pstStream->pucNextInput = pucNextIn; \
        pstStream->uiAvailInput = uiInHave; \
        pstUnzipState->ulDataHold = ulDataHold; \
        pstUnzipState->uiBitsHave = uiHoldBits; \
    }

/* Clear the input bit accumulator */
#define INITBITS() \
   { \
        ulDataHold = 0; \
        uiHoldBits = 0; \
    }

/* Get a byte of input into the bit accumulator, or return from Unzip_Stream()
   if there is no input available. */
#define PULLBYTE() \
   { \
        if (uiInHave == 0) goto Unzip_Down; \
        uiInHave--; \
        ulDataHold += (WE_ULONG)(*pucNextIn++) << uiHoldBits; \
        uiHoldBits += 8; \
    }

/* Assure that there are at least n bits in the bit accumulator.  If there is
   not enough available input to do that, then return from Unzip_Stream(). */
#define NEEDBITS(n) \
    { \
        while (uiHoldBits < (WE_UINT32)(n)) \
			PULLBYTE(); \
    }

/* Return the low n bits of the bit accumulator (n < 16) */
#define BITS(n) \
    ((WE_UINT32)ulDataHold & ((1U << (n)) - 1))

/* Remove n bits from the bit accumulator */
#define DROPBITS(n) \
   { \
        ulDataHold >>= (n); \
        uiHoldBits -= (WE_UINT32)(n); \
    }

/* Remove zero to seven bits as needed to go to a byte boundary */
#define BYTEBITS() \
    { \
        ulDataHold >>= (uiHoldBits & 7); \
        uiHoldBits -= (uiHoldBits & 7); \
    }

/* Reverse the bytes in a 32-bit value */
#define REVERSE(q) \
    ((((q) >> 24) & 0xff) + (((q) >> 8) & 0xff00) + \
(((q) & 0xff00) << 8) + (((q) & 0xff) << 24))

/************************************************************************/
/* Fixed Data Define */
/************************************************************************/
static const St_CodeCell s_staLenFix[512] = {
    {96,7,0},{0,8,80},{0,8,16},{20,8,115},{18,7,31},{0,8,112},{0,8,48},
    {0,9,192},{16,7,10},{0,8,96},{0,8,32},{0,9,160},{0,8,0},{0,8,128},
    {0,8,64},{0,9,224},{16,7,6},{0,8,88},{0,8,24},{0,9,144},{19,7,59},
    {0,8,120},{0,8,56},{0,9,208},{17,7,17},{0,8,104},{0,8,40},{0,9,176},
    {0,8,8},{0,8,136},{0,8,72},{0,9,240},{16,7,4},{0,8,84},{0,8,20},
    {21,8,227},{19,7,43},{0,8,116},{0,8,52},{0,9,200},{17,7,13},{0,8,100},
    {0,8,36},{0,9,168},{0,8,4},{0,8,132},{0,8,68},{0,9,232},{16,7,8},
    {0,8,92},{0,8,28},{0,9,152},{20,7,83},{0,8,124},{0,8,60},{0,9,216},
    {18,7,23},{0,8,108},{0,8,44},{0,9,184},{0,8,12},{0,8,140},{0,8,76},
    {0,9,248},{16,7,3},{0,8,82},{0,8,18},{21,8,163},{19,7,35},{0,8,114},
    {0,8,50},{0,9,196},{17,7,11},{0,8,98},{0,8,34},{0,9,164},{0,8,2},
    {0,8,130},{0,8,66},{0,9,228},{16,7,7},{0,8,90},{0,8,26},{0,9,148},
    {20,7,67},{0,8,122},{0,8,58},{0,9,212},{18,7,19},{0,8,106},{0,8,42},
    {0,9,180},{0,8,10},{0,8,138},{0,8,74},{0,9,244},{16,7,5},{0,8,86},
    {0,8,22},{64,8,0},{19,7,51},{0,8,118},{0,8,54},{0,9,204},{17,7,15},
    {0,8,102},{0,8,38},{0,9,172},{0,8,6},{0,8,134},{0,8,70},{0,9,236},
    {16,7,9},{0,8,94},{0,8,30},{0,9,156},{20,7,99},{0,8,126},{0,8,62},
    {0,9,220},{18,7,27},{0,8,110},{0,8,46},{0,9,188},{0,8,14},{0,8,142},
    {0,8,78},{0,9,252},{96,7,0},{0,8,81},{0,8,17},{21,8,131},{18,7,31},
    {0,8,113},{0,8,49},{0,9,194},{16,7,10},{0,8,97},{0,8,33},{0,9,162},
    {0,8,1},{0,8,129},{0,8,65},{0,9,226},{16,7,6},{0,8,89},{0,8,25},
    {0,9,146},{19,7,59},{0,8,121},{0,8,57},{0,9,210},{17,7,17},{0,8,105},
    {0,8,41},{0,9,178},{0,8,9},{0,8,137},{0,8,73},{0,9,242},{16,7,4},
    {0,8,85},{0,8,21},{16,8,258},{19,7,43},{0,8,117},{0,8,53},{0,9,202},
    {17,7,13},{0,8,101},{0,8,37},{0,9,170},{0,8,5},{0,8,133},{0,8,69},
    {0,9,234},{16,7,8},{0,8,93},{0,8,29},{0,9,154},{20,7,83},{0,8,125},
    {0,8,61},{0,9,218},{18,7,23},{0,8,109},{0,8,45},{0,9,186},{0,8,13},
    {0,8,141},{0,8,77},{0,9,250},{16,7,3},{0,8,83},{0,8,19},{21,8,195},
    {19,7,35},{0,8,115},{0,8,51},{0,9,198},{17,7,11},{0,8,99},{0,8,35},
    {0,9,166},{0,8,3},{0,8,131},{0,8,67},{0,9,230},{16,7,7},{0,8,91},
    {0,8,27},{0,9,150},{20,7,67},{0,8,123},{0,8,59},{0,9,214},{18,7,19},
    {0,8,107},{0,8,43},{0,9,182},{0,8,11},{0,8,139},{0,8,75},{0,9,246},
    {16,7,5},{0,8,87},{0,8,23},{64,8,0},{19,7,51},{0,8,119},{0,8,55},
    {0,9,206},{17,7,15},{0,8,103},{0,8,39},{0,9,174},{0,8,7},{0,8,135},
    {0,8,71},{0,9,238},{16,7,9},{0,8,95},{0,8,31},{0,9,158},{20,7,99},
    {0,8,127},{0,8,63},{0,9,222},{18,7,27},{0,8,111},{0,8,47},{0,9,190},
    {0,8,15},{0,8,143},{0,8,79},{0,9,254},{96,7,0},{0,8,80},{0,8,16},
    {20,8,115},{18,7,31},{0,8,112},{0,8,48},{0,9,193},{16,7,10},{0,8,96},
    {0,8,32},{0,9,161},{0,8,0},{0,8,128},{0,8,64},{0,9,225},{16,7,6},
    {0,8,88},{0,8,24},{0,9,145},{19,7,59},{0,8,120},{0,8,56},{0,9,209},
    {17,7,17},{0,8,104},{0,8,40},{0,9,177},{0,8,8},{0,8,136},{0,8,72},
    {0,9,241},{16,7,4},{0,8,84},{0,8,20},{21,8,227},{19,7,43},{0,8,116},
    {0,8,52},{0,9,201},{17,7,13},{0,8,100},{0,8,36},{0,9,169},{0,8,4},
    {0,8,132},{0,8,68},{0,9,233},{16,7,8},{0,8,92},{0,8,28},{0,9,153},
    {20,7,83},{0,8,124},{0,8,60},{0,9,217},{18,7,23},{0,8,108},{0,8,44},
    {0,9,185},{0,8,12},{0,8,140},{0,8,76},{0,9,249},{16,7,3},{0,8,82},
    {0,8,18},{21,8,163},{19,7,35},{0,8,114},{0,8,50},{0,9,197},{17,7,11},
    {0,8,98},{0,8,34},{0,9,165},{0,8,2},{0,8,130},{0,8,66},{0,9,229},
    {16,7,7},{0,8,90},{0,8,26},{0,9,149},{20,7,67},{0,8,122},{0,8,58},
    {0,9,213},{18,7,19},{0,8,106},{0,8,42},{0,9,181},{0,8,10},{0,8,138},
    {0,8,74},{0,9,245},{16,7,5},{0,8,86},{0,8,22},{64,8,0},{19,7,51},
    {0,8,118},{0,8,54},{0,9,205},{17,7,15},{0,8,102},{0,8,38},{0,9,173},
    {0,8,6},{0,8,134},{0,8,70},{0,9,237},{16,7,9},{0,8,94},{0,8,30},
    {0,9,157},{20,7,99},{0,8,126},{0,8,62},{0,9,221},{18,7,27},{0,8,110},
    {0,8,46},{0,9,189},{0,8,14},{0,8,142},{0,8,78},{0,9,253},{96,7,0},
    {0,8,81},{0,8,17},{21,8,131},{18,7,31},{0,8,113},{0,8,49},{0,9,195},
    {16,7,10},{0,8,97},{0,8,33},{0,9,163},{0,8,1},{0,8,129},{0,8,65},
    {0,9,227},{16,7,6},{0,8,89},{0,8,25},{0,9,147},{19,7,59},{0,8,121},
    {0,8,57},{0,9,211},{17,7,17},{0,8,105},{0,8,41},{0,9,179},{0,8,9},
    {0,8,137},{0,8,73},{0,9,243},{16,7,4},{0,8,85},{0,8,21},{16,8,258},
    {19,7,43},{0,8,117},{0,8,53},{0,9,203},{17,7,13},{0,8,101},{0,8,37},
    {0,9,171},{0,8,5},{0,8,133},{0,8,69},{0,9,235},{16,7,8},{0,8,93},
    {0,8,29},{0,9,155},{20,7,83},{0,8,125},{0,8,61},{0,9,219},{18,7,23},
    {0,8,109},{0,8,45},{0,9,187},{0,8,13},{0,8,141},{0,8,77},{0,9,251},
    {16,7,3},{0,8,83},{0,8,19},{21,8,195},{19,7,35},{0,8,115},{0,8,51},
    {0,9,199},{17,7,11},{0,8,99},{0,8,35},{0,9,167},{0,8,3},{0,8,131},
    {0,8,67},{0,9,231},{16,7,7},{0,8,91},{0,8,27},{0,9,151},{20,7,67},
    {0,8,123},{0,8,59},{0,9,215},{18,7,19},{0,8,107},{0,8,43},{0,9,183},
    {0,8,11},{0,8,139},{0,8,75},{0,9,247},{16,7,5},{0,8,87},{0,8,23},
    {64,8,0},{19,7,51},{0,8,119},{0,8,55},{0,9,207},{17,7,15},{0,8,103},
    {0,8,39},{0,9,175},{0,8,7},{0,8,135},{0,8,71},{0,9,239},{16,7,9},
    {0,8,95},{0,8,31},{0,9,159},{20,7,99},{0,8,127},{0,8,63},{0,9,223},
    {18,7,27},{0,8,111},{0,8,47},{0,9,191},{0,8,15},{0,8,143},{0,8,79},
    {0,9,255}
};

static const St_CodeCell s_staDistFix[32] = {
    {16,5,1},{23,5,257},{19,5,17},{27,5,4097},{17,5,5},{25,5,1025},
    {21,5,65},{29,5,16385},{16,5,3},{24,5,513},{20,5,33},{28,5,8193},
    {18,5,9},{26,5,2049},{22,5,129},{64,5,0},{16,5,2},{23,5,385},
    {19,5,25},{27,5,6145},{17,5,7},{25,5,1537},{21,5,97},{29,5,24577},
    {16,5,4},{24,5,769},{20,5,49},{28,5,12289},{18,5,13},{26,5,3073},
    {22,5,193},{64,5,0}
};

/************************************************************************/
/* Function declare */
/************************************************************************/

static WE_VOID Lz77tablesFixed(struct tagSt_UnzipState *pstUnzipState);
static WE_INT32 OutStoreForContinue(P_St_StreamState pstStream, WE_UINT32 uiOut);
//static WE_INT32 SyncCheck(WE_UINT32 *puiNumHave,WE_UINT8 *pucCheckBuf,WE_UINT32 uiBufferLen);


/************************************************************************/
/* function prototypes */
/************************************************************************/
/*=================================================================================
===
	FUNCTION: Unzip_Reset

	CREATE DATE: 2006-11-3
	AUTHOR: duhuanbiao

	DESCRIPTION:
		reset the state but not free the memory.
		
	ARGUMENTS PASSED:

	RETURN VALUE:
		ZIP_OK               ; success
		ZIP_STREAM_ERROR	 : stream error

	CALL BY:
		Unzip_Init, Unzip_StreamResume

	IMPORTANT NOTES:
		NULL

===================================================================================
==*/
WE_INT32 Unzip_Reset(
								P_St_StreamState pstStream
							)
{
    P_St_UnzipState pstUnzipState = NULL;

    if (pstStream == NULL || pstStream->pstZipState == NULL) 
    {
        return ZIP_PARA_ERR;
    }
    pstUnzipState = (P_St_UnzipState)pstStream->pstZipState;
    pstUnzipState->ulOutputCount = 0;
    pstStream->ulTotalOutput = 0;
    pstStream->ulTotalInput = 0;
    pstStream->ulAdlerCheck = 1;  /* to support ill-conceived Java test suite */
    pstUnzipState->eMode = E_UNZIP_HEADER;
    pstUnzipState->iIsLastBlock = 0;
    pstUnzipState->iHaveDict = 0;
    pstUnzipState->uiMaxDistance = 32768U;
    pstUnzipState->pstGzipHead = NULL;
    pstUnzipState->uiWindowSize = 0;
    pstUnzipState->uiWindowHave = 0;
    pstUnzipState->uiWindowWrite = 0;
    pstUnzipState->ulDataHold = 0;
    pstUnzipState->uiBitsHave = 0;
	pstUnzipState->pstNextAvail = pstUnzipState->astCodes;
    pstUnzipState->pstDistCode = pstUnzipState->astCodes;
	pstUnzipState->pstLenCode = pstUnzipState->astCodes;
    return ZIP_OK;
}
/*=================================================================================
===
	FUNCTION: Unzip_Init

	CREATE DATE: 2006-11-3
	AUTHOR: duhuanbiao

	DESCRIPTION:
		initial the unzip state, in this fun, you can set some unzip
	characteristic
		
	ARGUMENTS PASSED:

	RETURN VALUE:
		ZIP_OK               ; success
		ZIP_STREAM_ERROR	 : stream error
        ZIP_PARA_ERR         : parameter error

	CALL BY:
		Unzip_StreamSet

	IMPORTANT NOTES:
		NULL

===================================================================================
==*/

WE_INT32 Unzip_Init(
                           P_St_StreamState pstStream,
                           WE_INT32 iWindowBits
                           )
{
    P_St_UnzipState pstUnzipState = NULL;
    
    if (pstStream == NULL)
    {
        return ZIP_PARA_ERR;
    }
    
    pstUnzipState = (P_St_UnzipState)TOALLOC(1, sizeof(St_UnzipState));
    if (pstUnzipState == NULL) 
    {
        return ZIP_MEM_ERROR;
    }
    pstStream->pstZipState = (P_St_ZIP_State)pstUnzipState;
    if (iWindowBits < 0) 
    {
        pstUnzipState->iFormat = 0;
        iWindowBits = -iWindowBits;
    }
    else 
    {
        pstUnzipState->iFormat = ((WE_UINT32)iWindowBits >> 4) + 1;
        if (iWindowBits < 48) 
        {
            iWindowBits &= 15;
        }
    }
    if (iWindowBits < 8 || iWindowBits > 15)
    {
        FREE(pstUnzipState);
        pstStream->pstZipState = NULL;
        return ZIP_STREAM_ERROR;
    }
    pstUnzipState->uiWindowBitsReq = (WE_UINT32)iWindowBits;
    pstUnzipState->pucSlidWindow = NULL;
    return Unzip_Reset(pstStream);
}
/*=================================================================================
===
	FUNCTION: Lz77tablesFixed

	CREATE DATE: 2006-11-3
	AUTHOR: duhuanbiao

	DESCRIPTION:
		fixed the Lz77 table
		
	ARGUMENTS PASSED:

	RETURN VALUE:

	CALL BY:
		Unzip_Stream

	IMPORTANT NOTES:
		we used the static table

===================================================================================
==*/
static WE_VOID Lz77tablesFixed(
								P_St_UnzipState pstUnzipState
							)
{
    if (pstUnzipState)
    {
        pstUnzipState->pstLenCode = s_staLenFix;
        pstUnzipState->uiLenBitsIndex = 9;
        pstUnzipState->pstDistCode = s_staDistFix;
        pstUnzipState->uiDistBitsIndex = 5;
    }
}

/*=================================================================================
===
	FUNCTION: OutStoreForContinue

	CREATE DATE: 2006-11-3
	AUTHOR: duhuanbiao

	DESCRIPTION:
		store the last decoding data which will be used in the next, usually it is when a full
	compressed data divide two or more part.
		
	ARGUMENTS PASSED:

	RETURN VALUE:

	CALL BY:
		Unzip_Stream

	IMPORTANT NOTES:
		we used the static table

===================================================================================
==*/
static WE_INT32 OutStoreForContinue(
                                    P_St_StreamState pstStream, 
                                    WE_UINT32 uiOut //out lens
                                    )
{
    P_St_UnzipState pstUnzipState = NULL;
    WE_UINT32 uiCopy = 0U;
    WE_UINT32 uiDist = 0U;
    WE_UINT8 *pucTemp = NULL;
    
    if (pstStream == NULL)
    {
        return ZIP_PARA_ERR;
    }
    pstUnzipState = (P_St_UnzipState)pstStream->pstZipState;
    
    /* if it hasn't been done already, allocate space for the window */
    if (pstUnzipState->pucSlidWindow == NULL) 
    {
        pstUnzipState->pucSlidWindow = (WE_UINT8*)TOALLOC((1U << pstUnzipState->uiWindowBitsReq), sizeof(WE_UINT8));
        if (pstUnzipState->pucSlidWindow == NULL) 
        {
            return 1;
        }
    }
    
    /* if window not in use yet, initialize */
    if (pstUnzipState->uiWindowSize == 0) 
    {
        pstUnzipState->uiWindowSize = 1U << pstUnzipState->uiWindowBitsReq;
        pstUnzipState->uiWindowWrite = 0U;
        pstUnzipState->uiWindowHave = 0U;
    }
    
    /* copy state->uiWindowSize or less output bytes into the circular window */
    uiCopy = uiOut - pstStream->uiAvailOutput;
    if (uiCopy >= pstUnzipState->uiWindowSize) 
    {
        pstUnzipState->pucSlidWindow = (WE_UINT8*)MEMCPY(pstUnzipState->pucSlidWindow, (pstStream->pucNextOutput - pstUnzipState->uiWindowSize), pstUnzipState->uiWindowSize);
        pstUnzipState->uiWindowWrite = 0U;
        pstUnzipState->uiWindowHave = pstUnzipState->uiWindowSize;
    }
    else 
    {
        uiDist = pstUnzipState->uiWindowSize - pstUnzipState->uiWindowWrite;
        if (uiDist > uiCopy) 
        {
            uiDist = uiCopy;
        }
        pucTemp = pstUnzipState->pucSlidWindow + pstUnzipState->uiWindowWrite;
        pucTemp = (WE_UINT8*)MEMCPY(pucTemp, (pstStream->pucNextOutput - uiCopy), uiDist);
        uiCopy -= uiDist;
        if (uiCopy) 
        {
            pstUnzipState->pucSlidWindow = (WE_UINT8*)MEMCPY(pstUnzipState->pucSlidWindow, pstStream->pucNextOutput - uiCopy, uiCopy);
            pstUnzipState->uiWindowWrite = uiCopy;
            pstUnzipState->uiWindowHave = pstUnzipState->uiWindowSize;
        }
        else 
        {
            pstUnzipState->uiWindowWrite += uiDist;
            if (pstUnzipState->uiWindowWrite == pstUnzipState->uiWindowSize) pstUnzipState->uiWindowWrite = 0;
            if (pstUnzipState->uiWindowHave < pstUnzipState->uiWindowSize) pstUnzipState->uiWindowHave += uiDist;
        }
    }
    return 0;
}


/*=================================================================================
===
	FUNCTION: Unzip_Stream

	CREATE DATE: 2006-11-3
	AUTHOR: duhuanbiao

	DESCRIPTION:
		this fun is the real decoding fun, using the state machine to decompress the data
	as much as it can
		
	ARGUMENTS PASSED:
		flush: can be ZIP_NO_FLUSH, ZIP_SYNC_FLUSH,ZIP_FINISH, or ZIP_BLOCK. ZIP_SYNC_FLUSH 
	requests that ZIP_Stream() flush as much output as possible to the output buffer. ZIP_BLOCK 
	requests that ZIP_Stream() stop if and when it gets to the next decoding block boundary

	RETURN VALUE:
		ZIP_OK           : successfully decoding.
		ZIP_STREAM_ERROR : stream error.
		ZIP_STREAM_END   : it is the format end of the data,not the buffer end sign.
		ZIP_MEM_ERROR    : there is not enough memory
		ZIP_BUF_ERROR    : there is not enough stream data,that is,it can only supply
        ZIP_PARA_ERR     : parameter error
	you the data less then you want.you must add new compressed data to the 
	stream.Notice that the less data have been decoded and output.
		ZIP_DATA_ERROR   :the input data was not as expected.

	CALL BY:
		Unzip_StreamRead

	IMPORTANT NOTES:
		stops when the input buffer becomes empty or the output buffer becomes full.

===================================================================================
==*/
WE_INT32 Unzip_Stream(
                             P_St_StreamState pstStream,
                             WE_INT32 flush
                             )
{
    P_St_UnzipState pstUnzipState = NULL;
    WE_UINT8 *pucTemp = NULL;
    WE_UINT8 *pucNextIn = NULL;         /* next input */
    WE_UINT8 *pucNextOutput = NULL;		/* next output */
    WE_UINT32 uiInHave = 0U;            /* available input */
    WE_UINT32 uiOutLeft = 0U;           /* available output */
    WE_ULONG ulDataHold = 0L;			/* bit buffer */
    WE_UINT32 uiHoldBits = 0U;			/* bits in bit buffer */
    WE_UINT32 uiInOffset = 0U;          /* save starting available input */
    WE_UINT32 uiOutOffset = 0U;         /* save starting available input and output */
    WE_UINT32 uiNeedCopy = 0U;          /* number of stored or match bytes to copy */
    WE_UINT8 *uiCopyFrom = NULL;	    /* where to copy match bytes from */
    St_CodeCell stTableNow = {0};	    /* current decoding table entry */
    St_CodeCell stTableLast= {0};       /* parent table entry */
    WE_UINT32 uiLenCopyMore = 0U;       /* length to copy for repeats, bits to drop */
    WE_INT32 ret = 0;                   /* return code */
    
    WE_UINT8 ucaHeadCheckBuf[4] = {0};  /* buffer for gzip header crc calculation */
    
    static const WE_UINT16 s_uaLz77Order[19] =    /* permutation of code lengths */
    {16, 17, 18, 0, 8, 7, 9, 6, 10, 5, 11, 4, 12, 3, 13, 2, 14, 1, 15};
    
    if (pstStream == NULL ||
        pstStream->pstZipState == NULL ||
        pstStream->pucNextOutput == NULL ||
        (pstStream->pucNextInput == NULL && pstStream->uiAvailInput != 0))
    {
        return ZIP_PARA_ERR;
    }
    pstUnzipState = (P_St_UnzipState)pstStream->pstZipState;
    
    if (pstUnzipState->eMode == E_UNZIP_TYPEINI)
    {
        pstUnzipState->eMode = E_UNZIP_TYPESKIP;      /* skip check */
    }
    LOAD();
    uiInOffset = uiInHave;
    uiOutOffset = uiOutLeft;
    ret = ZIP_OK;
    for (;;)
    {
        switch (pstUnzipState->eMode) 
        {
        case E_UNZIP_HEADER:
            if (pstUnzipState->iFormat == 0) 
            {
                pstUnzipState->eMode = E_UNZIP_TYPESKIP;
                break;
            }
            NEEDBITS(16);
            
            if ((pstUnzipState->iFormat & 2) && ulDataHold == 0x8b1f) 
            {  /* gzip header */
                pstUnzipState->ulCheckValue = Zip_CrcCheck(0L, NULL, 0);
                CRC2(pstUnzipState->ulCheckValue, ulDataHold);
                INITBITS();
                pstUnzipState->eMode = E_UNZIP_FLG;
                break;
            }
            pstUnzipState->iGzipFlg = 0;           /* expect simple header */
            if (pstUnzipState->pstGzipHead != NULL)
                pstUnzipState->pstGzipHead->iDownHeadRead = -1;
            if (!(pstUnzipState->iFormat & 1) ||((BITS(8) << 8) + (ulDataHold >> 8)) % 31) 
            {
                pstUnzipState->eMode = E_UNZIP_WRONG;
                break;
            }
            if (BITS(4) != ZIP_DEFLATED) 
            {
                pstUnzipState->eMode = E_UNZIP_WRONG;
                break;
            }
            DROPBITS(4);
            uiLenCopyMore = BITS(4) + 8;
            if (uiLenCopyMore > pstUnzipState->uiWindowBitsReq) 
            {
                pstUnzipState->eMode = E_UNZIP_WRONG;
                break;
            }
            pstUnzipState->uiMaxDistance = 1U << uiLenCopyMore;
            pstStream->ulAdlerCheck = pstUnzipState->ulCheckValue = Zip_AdlerCheck(0L, NULL, 0);
            pstUnzipState->eMode = ulDataHold & 0x200 ? E_UNZIP_DICTCHECK : E_UNZIP_TYPEINI;
            INITBITS();
            break;
            
        case E_UNZIP_FLG:
            NEEDBITS(16);
            pstUnzipState->iGzipFlg = (WE_INT32)(ulDataHold);
            if ((pstUnzipState->iGzipFlg & 0xff) != ZIP_DEFLATED) 
            {
                pstUnzipState->eMode = E_UNZIP_WRONG;
                break;
            }
            if (pstUnzipState->iGzipFlg & 0xe000)
            {
                pstUnzipState->eMode = E_UNZIP_WRONG;
                break;
            }
            if (pstUnzipState->pstGzipHead != NULL)
            {
                pstUnzipState->pstGzipHead->iIsText = (WE_INT32)((ulDataHold >> 8) & 1);
            }
            if (pstUnzipState->iGzipFlg & 0x0200)
            {
                CRC2(pstUnzipState->ulCheckValue, ulDataHold);
            }
            INITBITS();
            pstUnzipState->eMode = E_UNZIP_DATE;
        case E_UNZIP_DATE:
            NEEDBITS(32);
            if (pstUnzipState->pstGzipHead != NULL)
            {
                pstUnzipState->pstGzipHead->ulChangeDate = ulDataHold;
            }
            if (pstUnzipState->iGzipFlg & 0x0200) 
            {
                CRC4(pstUnzipState->ulCheckValue, ulDataHold);
            }
            INITBITS();
            pstUnzipState->eMode = E_UNZIP_OS;
        case E_UNZIP_OS:
            NEEDBITS(16);
            if (pstUnzipState->pstGzipHead != NULL) 
            {
                pstUnzipState->pstGzipHead->iExtraFlgs = (WE_INT32)(ulDataHold & 0xff);
                pstUnzipState->pstGzipHead->iOs = (WE_INT32)(ulDataHold >> 8);
            }
            if (pstUnzipState->iGzipFlg & 0x0200)
            {
                CRC2(pstUnzipState->ulCheckValue, ulDataHold);
            }
            INITBITS();
            pstUnzipState->eMode = E_UNZIP_EXLEN;
        case E_UNZIP_EXLEN:
            if (pstUnzipState->iGzipFlg & 0x0400) 
            {
                NEEDBITS(16);
                pstUnzipState->uiCopyLen = (WE_UINT32)(ulDataHold);
                if (pstUnzipState->pstGzipHead != NULL)
                {
                    pstUnzipState->pstGzipHead->uiExtraDataLen = (WE_UINT32)ulDataHold;
                }
                if (pstUnzipState->iGzipFlg & 0x0200)
                {
                    CRC2(pstUnzipState->ulCheckValue, ulDataHold);
                }
                INITBITS();
            }
            else if (pstUnzipState->pstGzipHead != NULL)
            {
                pstUnzipState->pstGzipHead->pucExtraData = NULL;
            }
            pstUnzipState->eMode = E_UNZIP_EXTRA;
        case E_UNZIP_EXTRA:
            if (pstUnzipState->iGzipFlg & 0x0400)
            {
                uiNeedCopy = pstUnzipState->uiCopyLen;
                if (uiNeedCopy > uiInHave) 
                {
                    uiNeedCopy = uiInHave;
                }
                if (uiNeedCopy) 
				{
                    if (pstUnzipState->pstGzipHead != NULL &&
                        pstUnzipState->pstGzipHead->pucExtraData != NULL) 
                    {
                        uiLenCopyMore = pstUnzipState->pstGzipHead->uiExtraDataLen - pstUnzipState->uiCopyLen;
                        pucTemp = pstUnzipState->pstGzipHead->pucExtraData + uiLenCopyMore;
                        pucTemp = (WE_UINT8*)MEMCPY(pucTemp, pucNextIn, uiLenCopyMore + uiNeedCopy > pstUnzipState->pstGzipHead->uiMaxExtraData ?
                            pstUnzipState->pstGzipHead->uiMaxExtraData - uiLenCopyMore : uiNeedCopy);
                    }
                    if (pstUnzipState->iGzipFlg & 0x0200)
                    {
                        pstUnzipState->ulCheckValue = Zip_CrcCheck(pstUnzipState->ulCheckValue, pucNextIn, uiNeedCopy);
                    }
                    uiInHave -= uiNeedCopy;
                    pucNextIn += uiNeedCopy;
                    pstUnzipState->uiCopyLen -= uiNeedCopy;
                }
                if (pstUnzipState->uiCopyLen)
                {
                    goto Unzip_Down;
                }
            }
            pstUnzipState->uiCopyLen = 0;
            pstUnzipState->eMode = E_UNZIP_NAME;
        case E_UNZIP_NAME:
            if (pstUnzipState->iGzipFlg & 0x0800)
            {
                if (uiInHave == 0) 
                {
                    goto Unzip_Down;
                }
                uiNeedCopy = 0;
                do 
                {
                    uiLenCopyMore = (WE_UINT32)(pucNextIn[uiNeedCopy++]);
                    if (pstUnzipState->pstGzipHead != NULL &&
                        pstUnzipState->pstGzipHead->pucFileName != NULL &&
                        pstUnzipState->uiCopyLen < pstUnzipState->pstGzipHead->uiMaxName)
                    {
                        pstUnzipState->pstGzipHead->pucFileName[pstUnzipState->uiCopyLen++] = (WE_UINT8)uiLenCopyMore;
                    }
                } while (uiLenCopyMore && uiNeedCopy < uiInHave);
                if (pstUnzipState->iGzipFlg & 0x0200)
                {
                    pstUnzipState->ulCheckValue = Zip_CrcCheck(pstUnzipState->ulCheckValue, pucNextIn, uiNeedCopy);
                }
                uiInHave -= uiNeedCopy;
                pucNextIn += uiNeedCopy;
                if (uiLenCopyMore) 
                {
                    goto Unzip_Down;
                }
            }
            else if (pstUnzipState->pstGzipHead != NULL)
            {
                pstUnzipState->pstGzipHead->pucFileName = NULL;
            }
            pstUnzipState->uiCopyLen = 0;
            pstUnzipState->eMode = E_UNZIP_COMMENT;
        case E_UNZIP_COMMENT:
            if (pstUnzipState->iGzipFlg & 0x1000)
			{
                if (uiInHave == 0)
				{
					goto Unzip_Down;
				}
                uiNeedCopy = 0;
                do
				{
                    uiLenCopyMore = (WE_UINT32)(pucNextIn[uiNeedCopy++]);
                    if (pstUnzipState->pstGzipHead != NULL &&
                        pstUnzipState->pstGzipHead->pucComment != NULL &&
                        pstUnzipState->uiCopyLen < pstUnzipState->pstGzipHead->uiMaxComment)
					{
                        pstUnzipState->pstGzipHead->pucComment[pstUnzipState->uiCopyLen++] = (WE_UINT8)uiLenCopyMore;
					}
				} while (uiLenCopyMore && uiNeedCopy < uiInHave);
                if (pstUnzipState->iGzipFlg & 0x0200)
				{
                    pstUnzipState->ulCheckValue = Zip_CrcCheck(pstUnzipState->ulCheckValue, pucNextIn, uiNeedCopy);
                }
				uiInHave -= uiNeedCopy;
                pucNextIn += uiNeedCopy;
                if (uiLenCopyMore)
				{
					goto Unzip_Down;
				}
            }
            else if (pstUnzipState->pstGzipHead != NULL)
			{
				pstUnzipState->pstGzipHead->pucComment = NULL;
            }
			pstUnzipState->eMode = E_UNZIP_HCRC;
        case E_UNZIP_HCRC:
            if (pstUnzipState->iGzipFlg & 0x0200)
			{
                NEEDBITS(16);
                if (ulDataHold != (pstUnzipState->ulCheckValue & 0xffff))
				{
                    pstUnzipState->eMode = E_UNZIP_WRONG;
                    break;
                }
                INITBITS();
            }
            if (pstUnzipState->pstGzipHead != NULL)
			{
                pstUnzipState->pstGzipHead->iCrcHeadCheck = (WE_INT32)(((WE_UINT32)pstUnzipState->iGzipFlg >> 9) & 1);
                pstUnzipState->pstGzipHead->iDownHeadRead = 1;
            }
            pstStream->ulAdlerCheck = pstUnzipState->ulCheckValue = Zip_CrcCheck(0L, NULL, 0);
            pstUnzipState->eMode = E_UNZIP_TYPEINI;
            break;
        case E_UNZIP_DICTCHECK:
            NEEDBITS(32);
            pstStream->ulAdlerCheck = pstUnzipState->ulCheckValue = REVERSE(ulDataHold);
            INITBITS();
            pstUnzipState->eMode = E_UNZIP_SETDICT;
        case E_UNZIP_SETDICT:
            if (pstUnzipState->iHaveDict == 0)
			{
                RESTORE();
                return ZIP_NEED_DICT;
            }
            pstStream->ulAdlerCheck = pstUnzipState->ulCheckValue = Zip_AdlerCheck(0L, NULL, 0);
            pstUnzipState->eMode = E_UNZIP_TYPEINI;
        case E_UNZIP_TYPEINI:
            if (flush == ZIP_BLOCK) 
			{
				goto Unzip_Down;
			}
        case E_UNZIP_TYPESKIP:
            if (pstUnzipState->iIsLastBlock) 
			{
                BYTEBITS();
                pstUnzipState->eMode = E_UNZIP_CHECK;
                break;
            }
            NEEDBITS(3);
            pstUnzipState->iIsLastBlock = BITS(1);
            DROPBITS(1);
            switch (BITS(2)) 
			{
            case 0:                             /* stored block */
                pstUnzipState->eMode = E_UNZIP_RESTORE;
                break;
            case 1:                             /* fixed block */
                Lz77tablesFixed(pstUnzipState);
                pstUnzipState->eMode = E_UNZIP_LEN;              /* decode codes */
                break;
            case 2:                             /* dynamic block */
                pstUnzipState->eMode = E_UNZIP_DYNTABLE;
                break;
            case 3:
                pstUnzipState->eMode = E_UNZIP_WRONG;
            default:
                break;
            }
            DROPBITS(2);
            break;
        case E_UNZIP_RESTORE:
            BYTEBITS();                         /* go to byte boundary */
            NEEDBITS(32);
            if ((ulDataHold & 0xffff) != ((ulDataHold >> 16) ^ 0xffff)) 
			{
                pstUnzipState->eMode = E_UNZIP_WRONG;
                break;
            }
            pstUnzipState->uiCopyLen = (WE_UINT32)ulDataHold & 0xffff;
            INITBITS();
            pstUnzipState->eMode = E_UNZIP_TOCOPY;
        case E_UNZIP_TOCOPY:
            uiNeedCopy = pstUnzipState->uiCopyLen;
            if (uiNeedCopy) 
			{
                if (uiNeedCopy > uiInHave) 
				{
					uiNeedCopy = uiInHave;
				}
                if (uiNeedCopy > uiOutLeft)
				{
					uiNeedCopy = uiOutLeft;
				}
                if (uiNeedCopy == 0) 
				{
					goto Unzip_Down;
				}
                pucNextOutput = (WE_UINT8*)MEMCPY(pucNextOutput, pucNextIn, uiNeedCopy);
                uiInHave -= uiNeedCopy;
                pucNextIn += uiNeedCopy;
                uiOutLeft -= uiNeedCopy;
                pucNextOutput += uiNeedCopy;
                pstUnzipState->uiCopyLen -= uiNeedCopy;
                break;
            }
            pstUnzipState->eMode = E_UNZIP_TYPEINI;
            break;
        case E_UNZIP_DYNTABLE:
            NEEDBITS(14);
            pstUnzipState->uiLenNum = BITS(5) + 257;
            DROPBITS(5);
            pstUnzipState->uiDistNum = BITS(5) + 1;
            DROPBITS(5);
            pstUnzipState->uiCodeNum = BITS(4) + 4;
            DROPBITS(4);

            if (pstUnzipState->uiLenNum > 286 || pstUnzipState->uiDistNum > 30) 
			{
                pstUnzipState->eMode = E_UNZIP_WRONG;
                break;
            }

            pstUnzipState->uiHaveNum = 0;
            pstUnzipState->eMode = E_UNZIP_LENLENS;
        case E_UNZIP_LENLENS:
            while (pstUnzipState->uiHaveNum < pstUnzipState->uiCodeNum) 
			{
                NEEDBITS(3);
                pstUnzipState->auCodeLensTemp[s_uaLz77Order[pstUnzipState->uiHaveNum++]] = (WE_UINT16)BITS(3);
                DROPBITS(3);
            }
            while (pstUnzipState->uiHaveNum < 19)
			{
                pstUnzipState->auCodeLensTemp[s_uaLz77Order[pstUnzipState->uiHaveNum++]] = 0;
            }
			pstUnzipState->pstNextAvail = pstUnzipState->astCodes;
            pstUnzipState->pstLenCode = (St_CodeCell const *)(pstUnzipState->pstNextAvail);
            pstUnzipState->uiLenBitsIndex = 7;
            ret = Unzip_Table(E_UNZIP_CODES, pstUnzipState->auCodeLensTemp, 19, &(pstUnzipState->pstNextAvail),
                                &(pstUnzipState->uiLenBitsIndex), pstUnzipState->auWorkSpace);
            if (ret) 
			{
                pstUnzipState->eMode = E_UNZIP_WRONG;
                break;
            }
            pstUnzipState->uiHaveNum = 0;
            pstUnzipState->eMode = E_UNZIP_TREELENS;
        case E_UNZIP_TREELENS:
            while (pstUnzipState->uiHaveNum < pstUnzipState->uiLenNum + pstUnzipState->uiDistNum) 
			{
                for (;;) 
				{
                    stTableNow = pstUnzipState->pstLenCode[BITS(pstUnzipState->uiLenBitsIndex)];
                    if ((WE_UINT32)(stTableNow.ucCodeBits) <= uiHoldBits) break;
                    PULLBYTE();
                }
                if (stTableNow.uOffset < 16) 
				{
                    NEEDBITS(stTableNow.ucCodeBits);
                    DROPBITS(stTableNow.ucCodeBits);
                    pstUnzipState->auCodeLensTemp[pstUnzipState->uiHaveNum++] = stTableNow.uOffset;
                }
                else 
                {
                    if (stTableNow.uOffset == 16) 
                    {
                        NEEDBITS(stTableNow.ucCodeBits + 2);
                        DROPBITS(stTableNow.ucCodeBits);
                        if (pstUnzipState->uiHaveNum == 0) 
                        {
                            pstUnzipState->eMode = E_UNZIP_WRONG;
                            break;
                        }
                        uiLenCopyMore = pstUnzipState->auCodeLensTemp[pstUnzipState->uiHaveNum - 1];
                        uiNeedCopy = 3 + BITS(2);
                        DROPBITS(2);
                    }
                    else if (stTableNow.uOffset == 17)
                    {
                        NEEDBITS(stTableNow.ucCodeBits + 3);
                        DROPBITS(stTableNow.ucCodeBits);
                        uiLenCopyMore = 0;
                        uiNeedCopy = 3 + BITS(3);
                        DROPBITS(3);
                    }
                    else 
                    {
                        NEEDBITS(stTableNow.ucCodeBits + 7);
                        DROPBITS(stTableNow.ucCodeBits);
                        uiLenCopyMore = 0;
                        uiNeedCopy = 11 + BITS(7);
                        DROPBITS(7);
                    }
                    if (pstUnzipState->uiHaveNum + uiNeedCopy > pstUnzipState->uiLenNum + pstUnzipState->uiDistNum)
					{
                        pstUnzipState->eMode = E_UNZIP_WRONG;
                        break;
                    }
                    while (uiNeedCopy--)
					{
                        pstUnzipState->auCodeLensTemp[pstUnzipState->uiHaveNum++] = (WE_UINT16)uiLenCopyMore;
                    }
                }
            }
            
            /* handle error breaks in while */
            if (pstUnzipState->eMode == E_UNZIP_WRONG) 
            {
                break;
            }

            /* build tree tables */
            pstUnzipState->pstNextAvail = pstUnzipState->astCodes;
            pstUnzipState->pstLenCode = (St_CodeCell const *)(pstUnzipState->pstNextAvail);
            pstUnzipState->uiLenBitsIndex = 9;
            ret = Unzip_Table(E_UNZIP_LENS, pstUnzipState->auCodeLensTemp, pstUnzipState->uiLenNum, &(pstUnzipState->pstNextAvail),
                &(pstUnzipState->uiLenBitsIndex), pstUnzipState->auWorkSpace);
            if (ret) 
            {
                pstUnzipState->eMode = E_UNZIP_WRONG;
                break;
            }
            pstUnzipState->pstDistCode = (St_CodeCell const *)(pstUnzipState->pstNextAvail);
            pstUnzipState->uiDistBitsIndex = 6;
            ret = Unzip_Table(E_UNZIP_DISTS, pstUnzipState->auCodeLensTemp + pstUnzipState->uiLenNum, pstUnzipState->uiDistNum,
                &(pstUnzipState->pstNextAvail), &(pstUnzipState->uiDistBitsIndex), pstUnzipState->auWorkSpace);
            if (ret) 
            {
                pstUnzipState->eMode = E_UNZIP_WRONG;
                break;
            }
            pstUnzipState->eMode = E_UNZIP_LEN;
        case E_UNZIP_LEN:
            for (;;) 
            {
                stTableNow = pstUnzipState->pstLenCode[BITS(pstUnzipState->uiLenBitsIndex)];
                if ((WE_UINT32)(stTableNow.ucCodeBits) <= uiHoldBits) break;
                PULLBYTE();
            }
            if (stTableNow.ucOperationBits && (stTableNow.ucOperationBits & 0xf0) == 0) 
            {
                stTableLast = stTableNow;
                for (;;) 
                {
                    stTableNow = pstUnzipState->pstLenCode[stTableLast.uOffset +
                        (BITS(stTableLast.ucCodeBits + stTableLast.ucOperationBits) >> stTableLast.ucCodeBits)];
                    if ((WE_UINT32)(stTableLast.ucCodeBits + stTableNow.ucCodeBits) <= uiHoldBits) break;
                    PULLBYTE();
                }
                DROPBITS(stTableLast.ucCodeBits);
            }
            DROPBITS(stTableNow.ucCodeBits);
            pstUnzipState->uiCopyLen = (WE_UINT32)stTableNow.uOffset;
            if ((WE_INT32)(stTableNow.ucOperationBits) == 0) 
            {
                pstUnzipState->eMode = E_UNZIP_LIT;
                break;
            }
            if (stTableNow.ucOperationBits & 32) 
            {
                pstUnzipState->eMode = E_UNZIP_TYPEINI;
                break;
            }
            if (stTableNow.ucOperationBits & 64) 
            {
                pstUnzipState->eMode = E_UNZIP_WRONG;
                break;
            }
            pstUnzipState->uiNeedExtra = (WE_UINT32)(stTableNow.ucOperationBits) & 15;
            pstUnzipState->eMode = E_UNZIP_EXTREELEN;
        case E_UNZIP_EXTREELEN:
            if (pstUnzipState->uiNeedExtra) 
            {
                NEEDBITS(pstUnzipState->uiNeedExtra);
                pstUnzipState->uiCopyLen += BITS(pstUnzipState->uiNeedExtra);
                DROPBITS(pstUnzipState->uiNeedExtra);
            }
            pstUnzipState->eMode = E_UNZIP_DIST;
        case E_UNZIP_DIST:
            for (;;) 
            {
                stTableNow = pstUnzipState->pstDistCode[BITS(pstUnzipState->uiDistBitsIndex)];
                if ((WE_UINT32)(stTableNow.ucCodeBits) <= uiHoldBits) break;
                PULLBYTE();
            }
            if ((stTableNow.ucOperationBits & 0xf0) == 0) 
            {
                stTableLast = stTableNow;
                for (;;) 
                {
                    stTableNow = pstUnzipState->pstDistCode[stTableLast.uOffset +
                        (BITS(stTableLast.ucCodeBits + stTableLast.ucOperationBits) >> stTableLast.ucCodeBits)];
                    if ((WE_UINT32)(stTableLast.ucCodeBits + stTableNow.ucCodeBits) <= uiHoldBits) break;
                    PULLBYTE();
                }
                DROPBITS(stTableLast.ucCodeBits);
            }
            DROPBITS(stTableNow.ucCodeBits);
            if (stTableNow.ucOperationBits & 64) 
            {
                pstUnzipState->eMode = E_UNZIP_WRONG;
                break;
            }
            pstUnzipState->uiCopyOffset = (WE_UINT32)stTableNow.uOffset;
            pstUnzipState->uiNeedExtra = (WE_UINT32)(stTableNow.ucOperationBits) & 15;
            pstUnzipState->eMode = E_UNZIP_DISTEXT;
        case E_UNZIP_DISTEXT:
            if (pstUnzipState->uiNeedExtra) 
            {
                NEEDBITS(pstUnzipState->uiNeedExtra);
                pstUnzipState->uiCopyOffset += BITS(pstUnzipState->uiNeedExtra);
                DROPBITS(pstUnzipState->uiNeedExtra);
            }
            
            if (pstUnzipState->uiCopyOffset > pstUnzipState->uiWindowHave + uiOutOffset - uiOutLeft) 
            {
                pstUnzipState->eMode = E_UNZIP_WRONG;
                break;
            }
            pstUnzipState->eMode = E_UNZIP_MATCH;
        case E_UNZIP_MATCH:
            if (uiOutLeft == 0) 
            {
                goto Unzip_Down;
            }
            uiNeedCopy = uiOutOffset - uiOutLeft;
            if (pstUnzipState->uiCopyOffset > uiNeedCopy) 
            {         /* copy from window */
                uiNeedCopy = pstUnzipState->uiCopyOffset - uiNeedCopy;
                if (uiNeedCopy > pstUnzipState->uiWindowWrite) 
                {
                    uiNeedCopy -= pstUnzipState->uiWindowWrite;
                    uiCopyFrom = pstUnzipState->pucSlidWindow + (pstUnzipState->uiWindowSize - uiNeedCopy);
                }
                else
                {
                    uiCopyFrom = pstUnzipState->pucSlidWindow + (pstUnzipState->uiWindowWrite - uiNeedCopy);
                }
                if (uiNeedCopy > pstUnzipState->uiCopyLen)
                {
                    uiNeedCopy = pstUnzipState->uiCopyLen;
                }
            }
            else /* copy from output */
            {                              
                uiCopyFrom = pucNextOutput - pstUnzipState->uiCopyOffset;
                uiNeedCopy = pstUnzipState->uiCopyLen;
            }
            if (uiNeedCopy > uiOutLeft) 
            {
                uiNeedCopy = uiOutLeft;
            }
            uiOutLeft -= uiNeedCopy;
            pstUnzipState->uiCopyLen -= uiNeedCopy;
            do
            {
                *pucNextOutput++ = *uiCopyFrom++;
            } while (--uiNeedCopy);
            
            if (pstUnzipState->uiCopyLen == 0)
            {
                pstUnzipState->eMode = E_UNZIP_LEN;
            }
            break;
        case E_UNZIP_LIT:
            if (uiOutLeft == 0) 
            {
                goto Unzip_Down;
            }
            *pucNextOutput++ = (WE_UINT8)(pstUnzipState->uiCopyLen);
            uiOutLeft--;
            pstUnzipState->eMode = E_UNZIP_LEN;
            break;
        case E_UNZIP_CHECK:
            if (pstUnzipState->iFormat) 
            {
                NEEDBITS(32);
                uiOutOffset -= uiOutLeft;
                pstStream->ulTotalOutput += uiOutOffset;
                pstUnzipState->ulOutputCount += uiOutOffset;
                if (uiOutOffset)
                {
                    pstStream->ulAdlerCheck = pstUnzipState->ulCheckValue =
                        UPDATE(pstUnzipState->ulCheckValue, pucNextOutput - uiOutOffset, uiOutOffset);
                }
                uiOutOffset = uiOutLeft;
                if ((pstUnzipState->iGzipFlg ? ulDataHold :REVERSE(ulDataHold)) != pstUnzipState->ulCheckValue)
                {
                    pstUnzipState->eMode = E_UNZIP_WRONG;
                    break;
                }
                INITBITS();
            }
            
            pstUnzipState->eMode = E_UNZIP_LENGTH;
        case E_UNZIP_LENGTH:
            if (pstUnzipState->iFormat && pstUnzipState->iGzipFlg) 
            {
                NEEDBITS(32);
                if (ulDataHold != (pstUnzipState->ulOutputCount & 0xffffffffUL)) 
                {
                    pstUnzipState->eMode = E_UNZIP_WRONG;
                    break;
                }
                INITBITS();                
            }
            
            pstUnzipState->eMode = E_UNZIP_DONE;
        case E_UNZIP_DONE:
            ret = ZIP_STREAM_END;
            goto Unzip_Down;
        case E_UNZIP_WRONG:
            ret = ZIP_DATA_ERROR;
            goto Unzip_Down;
        case E_UNZIP_MEMERR:
            return ZIP_MEM_ERROR;
        case E_UNZIP_SYNC:
        default:
            return ZIP_STREAM_ERROR;
        }
    } /* end for(;;) */
      /*
      Return from Unzip_Stream(), updating the total counts and the check value.
      If there was no progress during the Unzip_Stream() call, return a buffer
      error.  Call OutStoreForContinue() to create and/or update the window state.
      Note: a memory error from Unzip_Stream() is non-recoverable.
      */
Unzip_Down:
    RESTORE();
    if (pstUnzipState->uiWindowSize || (pstUnzipState->eMode < E_UNZIP_CHECK && uiOutOffset != pstStream->uiAvailOutput))
    {
        if (OutStoreForContinue(pstStream, uiOutOffset))
        {
            pstUnzipState->eMode = E_UNZIP_MEMERR;
            return ZIP_MEM_ERROR;
        }
    }
    uiInOffset -= pstStream->uiAvailInput;
    uiOutOffset -= pstStream->uiAvailOutput;
    pstStream->ulTotalInput += uiInOffset;
    pstStream->ulTotalOutput += uiOutOffset;
    pstUnzipState->ulOutputCount += uiOutOffset;
    if (pstUnzipState->iFormat && uiOutOffset)
    {
        pstStream->ulAdlerCheck = pstUnzipState->ulCheckValue =
            UPDATE(pstUnzipState->ulCheckValue, pstStream->pucNextOutput - uiOutOffset, uiOutOffset);
    }
    pstStream->iDataType = (WE_INT32)pstUnzipState->uiBitsHave + (pstUnzipState->iIsLastBlock ? 64 : 0) +
        (pstUnzipState->eMode == E_UNZIP_TYPEINI ? 128 : 0);
    if (((uiInOffset == 0 && uiOutOffset == 0) || flush == ZIP_FINISH) && ret == ZIP_OK)
    {
        ret = ZIP_BUF_ERROR;
    }
    return ret;
}
/*=================================================================================
===
	FUNCTION: Unzip_Close

	CREATE DATE: 2006-11-3
	AUTHOR: duhuanbiao

	DESCRIPTION:
		All dynamically allocated data structures for this stream are freed.
		
	ARGUMENTS PASSED:

	RETURN VALUE:

	CALL BY:

	IMPORTANT NOTES:
		NULL

===================================================================================
==*/
WE_INT32 Unzip_Close(
							P_St_StreamState pstStream
						)
{
    P_St_UnzipState pstUnzipState = NULL;
    if (NULL == pstStream)
    {
        return ZIP_STREAM_ERROR;
    }

    if (pstStream->pstZipState)
    {
        pstUnzipState = (P_St_UnzipState)pstStream->pstZipState;
        TOFREE(pstUnzipState->pucSlidWindow);
        FREE(pstUnzipState);
        pstStream->pstZipState = NULL;
    }

    return ZIP_OK;
}

#if 0
/*=================================================================================
===
	FUNCTION: Unzip_HeaderGet

	CREATE DATE: 2006-11-3
	AUTHOR: duhuanbiao

	DESCRIPTION:
		get the data header if you want
		
	ARGUMENTS PASSED:

	RETURN VALUE:

	CALL BY:

	IMPORTANT NOTES:
		NULL

===================================================================================
==*/
static WE_INT32 Unzip_HeaderGet(
									P_St_StreamState pstStream,
									P_St_GzipHeaderState pstGzipHead
								)
{
    P_St_UnzipState pstUnzipState;
    
    /* check state */
    if (pstStream == NULL || 
        pstStream->pstZipState == NULL ||
        pstGzipHead == NULL)
    {
        return ZIP_PARA_ERR;
    }
    pstUnzipState = (P_St_UnzipState)pstStream->pstZipState;
    if (!pstUnzipState || (pstUnzipState->iFormat & 2) == 0) 
    {
        return ZIP_STREAM_ERROR;
    }
    /* save header structure */
    pstUnzipState->pstGzipHead = pstGzipHead;
    pstGzipHead->iDownHeadRead = 0;
    return ZIP_OK;
}

/*=================================================================================
===
	FUNCTION: SyncCheck

	CREATE DATE: 2006-11-3
	AUTHOR: duhuanbiao

	DESCRIPTION:
		check if the data is right or not
		
	ARGUMENTS PASSED:

	RETURN VALUE:

	CALL BY:

	IMPORTANT NOTES:
		NULL

===================================================================================
==*/
static WE_INT32 SyncCheck(
								WE_UINT32 *puiNumHave,
								WE_UINT8 *pucCheckBuf,
								WE_UINT32 uiBufferLen
						   )
{
    WE_UINT32 uiNewNumHave = 0U;
    WE_UINT32 uiNext = 0U;
    
    if (puiNumHave == NULL ||
        pucCheckBuf == NULL)
    {
        return ZIP_PARA_ERR;
    }
    
    uiNewNumHave = *puiNumHave;
    uiNext = 0;
    while (uiNext < uiBufferLen && uiNewNumHave < 4) 
    {
        if ((WE_INT32)(pucCheckBuf[uiNext]) == (uiNewNumHave < 2 ? 0 : 0xff))
        {
            uiNewNumHave++;
        }
        else if (pucCheckBuf[uiNext])
        {
            uiNewNumHave = 0;
        }
        else
        {
            uiNewNumHave = 4 - uiNewNumHave;
        }
        uiNext++;
    }
    *puiNumHave = uiNewNumHave;
    return (WE_INT32)uiNext;
}
#endif
/*=================================================================================
===
	FUNCTION: Unzip_FindSyncPoint

	CREATE DATE: 2006-11-3
	AUTHOR: duhuanbiao

	DESCRIPTION:
		if find a wrong data, but you really need data, it can find a right block to 
	decoding
		
	ARGUMENTS PASSED:

	RETURN VALUE:

	CALL BY:

	IMPORTANT NOTES:
		NULL

===================================================================================
==*/
#if 0 
static WE_INT32 Unzip_FindSyncPoint(
										P_St_StreamState pstStream
									)
{
    WE_UINT32 uiSyncLen = 0U;		       /* number of bytes to look at or looked at */
    WE_ULONG ulInTemp = 0UL;               /* temporary to save ulTotalInput */
    WE_ULONG ulOutTemp = 0UL;              /* temporary to save ulTotalOutput */
    WE_UINT8 ucaRestoreBuf[4] = {0};	   /* to restore bit buffer to byte string */
    P_St_UnzipState pstUnzipState = NULL;
    
    /* check parameters */
    if (pstStream == NULL || pstStream->pstZipState == NULL) 
    {
        return ZIP_PARA_ERR;
    }
    pstUnzipState = (P_St_UnzipState)pstStream->pstZipState;
    if (pstStream->uiAvailInput == 0 && pstUnzipState->uiBitsHave < 8)
    {
        return ZIP_BUF_ERROR;
    }
    /* if first time, start search in bit buffer */
    if (pstUnzipState->eMode != E_UNZIP_SYNC) 
    {
        pstUnzipState->eMode = E_UNZIP_SYNC;
        pstUnzipState->ulDataHold <<= (pstUnzipState->uiBitsHave & 7);
        pstUnzipState->uiBitsHave -= (pstUnzipState->uiBitsHave & 7);
        uiSyncLen = 0;
        while (pstUnzipState->uiBitsHave >= 8) 
        {
            ucaRestoreBuf[uiSyncLen++] = (WE_UINT8)(pstUnzipState->ulDataHold);
            pstUnzipState->ulDataHold >>= 8;
            pstUnzipState->uiBitsHave -= 8;
        }
        pstUnzipState->uiHaveNum = 0;
        SyncCheck(&(pstUnzipState->uiHaveNum), ucaRestoreBuf, uiSyncLen);
    }
    
    /* search available input */
    uiSyncLen = (WE_UINT32)SyncCheck(&(pstUnzipState->uiHaveNum), pstStream->pucNextInput, pstStream->uiAvailInput);
    pstStream->uiAvailInput -= uiSyncLen;
    pstStream->pucNextInput += uiSyncLen;
    pstStream->ulTotalInput += uiSyncLen;
    
    /* return no joy or set up to restart WE_UnzipStream() on a new block */
    if (pstUnzipState->uiHaveNum != 4) 
    {
        return ZIP_DATA_ERROR;
    }
    ulInTemp = pstStream->ulTotalInput;  ulOutTemp = pstStream->ulTotalOutput;
    Unzip_Reset(pstStream);
    pstStream->ulTotalInput = ulInTemp;  pstStream->ulTotalOutput = ulOutTemp;
    pstUnzipState->eMode = E_UNZIP_TYPEINI;
    return ZIP_OK;
    
}

#endif
		/*  follow fun used as an interface, see detail in the common.h */

/*=================================================================================
===
	FUNCTION: Unzip_StreamRead

	CREATE DATE: 2006-11-6
	AUTHOR: duhuanbiao

	DESCRIPTION:
		read n bytes data as you want,see in the common.h
		
	ARGUMENTS PASSED:

	RETURN VALUE:

	CALL BY:

	IMPORTANT NOTES:
		NULL

===================================================================================
==*/
WE_INT32 Unzip_StreamRead( 
										WE_HANDLE hStream,
										WE_UINT8 *pucBufDest,  
										WE_UINT32 *puiDestLen
								 )
{
    WE_INT32 iErr = ZIP_STREAM_ERROR;

    /* used when you read buffer more times */
    WE_ULONG ulLastTotalOut = 0UL;

   /* the bytes you want to read */
    WE_UINT32 uiNeed = 0U;

    P_St_StreamState pstStream = NULL;
    
    if (hStream == NULL ||
        pucBufDest == NULL ||
        puiDestLen == NULL)
    {
        return ZIP_PARA_ERR;
    }
    
    pstStream = (P_St_StreamState)hStream;
    uiNeed = *puiDestLen;
    ulLastTotalOut = pstStream->ulTotalOutput;
    
    pstStream->pucNextOutput = pucBufDest;
    pstStream->uiAvailOutput = *puiDestLen;

    /* if have an error, return 0 */
    *puiDestLen = 0; 
    do
    {
        iErr = Unzip_Stream(pstStream, ZIP_NO_FLUSH);
        switch (iErr) 
        {
        case ZIP_NEED_DICT:
            iErr = ZIP_DATA_ERROR;     /* and fall through */
        case ZIP_STREAM_ERROR:
        case ZIP_DATA_ERROR:
        case ZIP_MEM_ERROR:
        case ZIP_PARA_ERR:
            return iErr;
            
        default:
            *puiDestLen = (WE_UINT32)(pstStream->ulTotalOutput - ulLastTotalOut);
        }
        
    } while(iErr != ZIP_STREAM_END && *puiDestLen < uiNeed && pstStream->uiAvailInput > 0);
    
    
    return iErr;
    
}

/*=================================================================================
===
	FUNCTION: Unzip_StreamSet

	CREATE DATE: 2006-11-6
	AUTHOR: duhuanbiao

	DESCRIPTION:
		set the stream,and get the handle
		
	ARGUMENTS PASSED:

	RETURN VALUE:

	CALL BY:

	IMPORTANT NOTES:
		NULL

===================================================================================
==*/
WE_HANDLE Unzip_StreamSet(
                          const WE_UINT8 *pucStream, 
                          const WE_UINT32 uiStreamLen, 
                          const WE_BOOL bHaveHead
                          )
{
	WE_UINT8* pucBuf = NULL;
    P_St_StreamState pstStream = NULL;
    WE_INT32 iErr = ZIP_STREAM_ERROR;
    
    if (pucStream == NULL || uiStreamLen == 0)
    {
        return NULL;
    }
    
    pstStream = (P_St_StreamState)TOALLOC(1, sizeof(St_StreamState));
    pucBuf = (WE_UINT8*)TOALLOC(1, uiStreamLen);
    if (NULL == pstStream || NULL == pucBuf) 
    {
       TOFREE(pstStream);
       TOFREE(pucBuf);
       return NULL;
    }

    MEMCPY(pucBuf, pucStream, uiStreamLen);
    
    pstStream->pucNextInput = (WE_UINT8*)pucBuf;
    pstStream->pucBufPtr = pucBuf;

   /* store for the next resuming data */
    pstStream->uiAvailInput = (WE_UINT32)uiStreamLen;
    
    pstStream->pucNextOutput = NULL;
    pstStream->uiAvailOutput = 0;
    
    if(bHaveHead)
    {
        iErr = Unzip_Init(pstStream, DEF_WINDOWBITS + 16);
    }
    else
    {
        iErr = Unzip_Init(pstStream, -DEF_WINDOWBITS);
    }
    
    if (iErr != ZIP_OK) 
    {
        Unzip_Close(pstStream);
        TOFREE(pstStream);
        TOFREE(pucBuf);
        return NULL;
    }
    return pstStream;
}

/*=================================================================================
===
	FUNCTION: Unzip_StreamUpdate

	CREATE DATE: 2006-11-6
	AUTHOR: duhuanbiao

	DESCRIPTION:
		if you had updated the stream, call this fun
		
	ARGUMENTS PASSED:

	RETURN VALUE:

	CALL BY:

	IMPORTANT NOTES:
		NULL

===================================================================================
==*/
WE_INT32 Unzip_StreamUpdate(
                           WE_HANDLE hStream,
                           const WE_UINT8 *pucStream, 
                           const WE_UINT32 uiStreamLen
                           )
{
    P_St_StreamState pstStream = NULL;
    WE_UINT8* pucBuf = NULL;

    if (NULL == hStream ||
     NULL == pucStream || 
     0 == uiStreamLen)
    {
        return ZIP_PARA_ERR;
    }

    pstStream = (P_St_StreamState)hStream;

    pucBuf = (WE_UINT8*)TOALLOC(1, uiStreamLen);
    if (NULL == pucBuf)
    {
        return ZIP_MEM_ERROR;
    }
    
    TOFREE(pstStream->pucBufPtr);
    MEMCPY(pucBuf, pucStream, uiStreamLen);

    pstStream->pucNextInput = pucBuf;
    pstStream->pucBufPtr = pucBuf;

    pstStream->uiAvailInput = (WE_UINT32)uiStreamLen;

    return ZIP_OK;
}

/*=================================================================================
===
	FUNCTION: Unzip_StreamResume

	CREATE DATE: 2006-11-6
	AUTHOR: duhuanbiao

	DESCRIPTION:
		if a new get a compressed data, call this fun
		
	ARGUMENTS PASSED:

	RETURN VALUE:

	CALL BY:

	IMPORTANT NOTES:
		NULL

===================================================================================
==*/
WE_INT32 Unzip_StreamResume(
                           WE_HANDLE hStream,
                           const WE_UINT8 *pucStream,
                           const WE_UINT32 uiStreamLen
                           )
{
    P_St_StreamState pstStream = NULL;
    WE_UINT8* pucBuf = NULL;
    
    if (NULL == hStream)
    {
        return ZIP_PARA_ERR;
    }

    pstStream = (P_St_StreamState)hStream;

    pucBuf = (WE_UINT8*)TOALLOC(1, uiStreamLen);
    if (NULL == pucBuf)
    {
        return ZIP_MEM_ERROR;
    }

    TOFREE(pstStream->pucBufPtr);
    
    MEMCPY(pucBuf, pucStream, uiStreamLen);

    pstStream->pucNextInput = pucBuf;
    pstStream->pucBufPtr = pucBuf;

    pstStream->uiAvailInput = uiStreamLen;
    
    return Unzip_Reset(pstStream);
}

/*=================================================================================
===
	FUNCTION: Unzip_StreamClose

	CREATE DATE: 2006-11-6
	AUTHOR: duhuanbiao

	DESCRIPTION:
		free the struct being allocated in the stream
		
	ARGUMENTS PASSED:

	RETURN VALUE:

	CALL BY:

	IMPORTANT NOTES:
		NULL

===================================================================================
==*/
WE_INT32 Unzip_StreamClose(
                          WE_HANDLE hStream
                          )
{
    WE_INT32 iErr = ZIP_STREAM_ERROR;
    P_St_StreamState pstStream = (P_St_StreamState)hStream;
    
    if (NULL == hStream)
    {
        return ZIP_PARA_ERR;
    }
    
    iErr = Unzip_Close((P_St_StreamState)hStream);
    
    TOFREE(pstStream->pucBufPtr);
    FREE(pstStream);
    
    return iErr;
}

                     /* gzip file process */
/*=================================================================================
===
	FUNCTION: Gzip_Open

	CREATE DATE: 2007-5-29
	AUTHOR: duhuanbiao

	DESCRIPTION:
		open gzip file
		
	ARGUMENTS PASSED:

	RETURN VALUE:
            the handle

	CALL BY:

	IMPORTANT NOTES:
		NULL

===================================================================================
==*/
static WE_UINT32 Gzip_GetFileSize(IFile* pIFile, IFileMgr* pIFileMgr)
{
    FileInfo stFileInfo = { 0 };
    if (pIFile && pIFileMgr && SUCCESS == IFILE_GetInfo(pIFile, &stFileInfo))
    {
        return stFileInfo.dwSize;
    }
    else
    {
        return 0;
    }
}

/*=================================================================================
===
	FUNCTION: Gzip_Open

	CREATE DATE: 2007-5-29
	AUTHOR: duhuanbiao

	DESCRIPTION:
		open gzip file
		
	ARGUMENTS PASSED:

	RETURN VALUE:
            the handle

	CALL BY:

	IMPORTANT NOTES:
		NULL

===================================================================================
==*/
WE_HANDLE Gzip_Open(IShell* pIShell, const WE_CHAR* pcFileName, WE_INT32 iMode)
{
    P_Gzip_State pGzip = NULL; /* local gzip state */
    P_St_StreamState pStream = NULL; /* local stream state */
    WE_UINT8* pucBuf = NULL; /* local buf temp */
    WE_UINT32 uiLen = 0;/* data length in buf */
    IFileMgr* pIFileMgr = NULL;
    IFile* pIFile = NULL;

    if (NULL == pIShell || 
        NULL == pcFileName ||
        SUCCESS != ISHELL_CreateInstance(pIShell, AEECLSID_FILEMGR, (WE_VOID**)&pIFileMgr))
    {
        return NULL;
    }

    /* open the .gz file */
    pIFile = IFILEMGR_OpenFile(pIFileMgr, pcFileName, _OFM_READ);  
    if (NULL == pIFile)
    {
        IFILEMGR_Release(pIFileMgr);
        return NULL;
    }

    /* new the gzip handle */
    pGzip = (P_Gzip_State)MALLOC(sizeof(Gzip_State));
    pucBuf = (WE_UINT8*)MALLOC(GZIP_BUF_LEN);
    if (NULL == pGzip || NULL == pucBuf)
    {
        TOFREE(pGzip);
        TOFREE(pucBuf);
        IFILEMGR_Release(pIFileMgr);
        IFILE_Release(pIFile);
        return NULL;
    }

    /* preload data */
    uiLen = IFILE_Read(pIFile, (WE_VOID*)pucBuf, GZIP_BUF_LEN);

    /* set the input data */
    pStream = uiLen ? (P_St_StreamState)Unzip_StreamSet(pucBuf, uiLen, TRUE) : NULL;
    if (NULL == pStream)
    {
        TOFREE(pGzip);
        TOFREE(pucBuf);
        IFILEMGR_Release(pIFileMgr);
        IFILE_Release(pIFile);
        return NULL;
    }

    /* initial */
    pGzip->pStream = pStream;
    pGzip->pIFile = pIFile;
    pGzip->pIFileMgr = pIFileMgr;
    pGzip->uiFileRead = uiLen;
    pGzip->uiBufLen = GZIP_BUF_LEN;
    pGzip->uiDataLeft = uiLen;
    pGzip->pucBuf = pucBuf;
    pGzip->iErr = ZIP_OK;
    
    return (WE_HANDLE)pGzip;
}

/*=================================================================================
===
	FUNCTION: Gzip_Read

	CREATE DATE: 2007-5-29
	AUTHOR: duhuanbiao

	DESCRIPTION:
		read as much data as you want
		
	ARGUMENTS PASSED:

	RETURN VALUE:

	CALL BY:

	IMPORTANT NOTES:
		NULL

===================================================================================
==*/
WE_INT32 Gzip_Read(WE_HANDLE hHnd, WE_UINT8* pucBuf, WE_UINT32 uiLen)
{
    P_Gzip_State pGzip = (P_Gzip_State)hHnd;/* get the handle */
    WE_UINT32 uiDataNeed = uiLen;/* data you needed every time */
    WE_UINT32 uiDataRead = 0;/* data you have read, that is the data length in buf */
    WE_UINT32 uiFileSize = 0;/* file size temp */
    WE_INT32 iRet = ZIP_DATA_ERROR;

    if (NULL == hHnd || NULL == pucBuf)
    {
        return 0;
    }
    /* first of all, we must process file block error */
    if (pGzip->iErr == ZIP_FILE_WOULDBLOCK)
    {
        /* pGzip->pucBuf store the data decoded but not output when error happen */
        /* pGzip->uiDataLeft is its data length */
        if (uiDataNeed <= pGzip->uiDataLeft)
        {
            /* have enough data */
            MEMCPY(pucBuf, pGzip->pucBuf, uiDataNeed);
            pGzip->uiDataLeft -= uiDataNeed;
            MEMCPY(pGzip->pucBuf, pGzip->pucBuf + uiDataNeed, pGzip->uiDataLeft);
            return (WE_INT32)uiDataNeed;
        }
        else
        {
            /* data are not enough */
            MEMCPY(pucBuf, pGzip->pucBuf,pGzip->uiDataLeft);
            uiDataRead = pGzip->uiDataLeft;
            uiDataNeed -= pGzip->uiDataLeft;
            /* try to read more zip data */
            if (NULL == pGzip->pIFile)
            {
                pGzip->iErr = ZIP_ERR_FILE;
                return 0;
            }
            pGzip->uiDataLeft = IFILE_Read(pGzip->pIFile, (WE_VOID*)pGzip->pucBuf, pGzip->uiBufLen);
            if (0 == pGzip->uiDataLeft)
            {
                uiFileSize = Gzip_GetFileSize(pGzip->pIFile, pGzip->pIFileMgr);
                /* is no more data? */
                if (pGzip->uiFileRead != uiFileSize)
                {
                    MEMCPY(pGzip->pucBuf, pucBuf, uiDataRead); /* store the data */
                    pGzip->uiDataLeft = uiDataRead;
                    return 0;
                }
                pGzip->iErr = ZIP_ERR_FILE;
                return 0;
            }
            pGzip->uiFileRead += pGzip->uiDataLeft;
        }
    }

    pGzip->iErr = ZIP_OK;
    
    while (1)
    {
        /* get the upzip data */
        iRet = Unzip_StreamRead((WE_HANDLE)pGzip->pStream, (pucBuf + uiDataRead), &uiDataNeed);
        if (iRet < 0 && iRet != ZIP_BUF_ERROR)
        {
            pGzip->iErr = iRet;
            return 0;
        }
        
        /* you have read more uiDataNeed bytes */
        uiDataRead += uiDataNeed;
        /* how many bytes left */
        uiDataNeed = uiLen - uiDataRead;
        /* we get we needed or it is the end of the file */
        if (0 == uiDataNeed || ZIP_STREAM_END == iRet)
        {
            if (ZIP_STREAM_END == iRet)
            {
                pGzip->iErr = ZIP_END;
            }
            return (WE_INT32)uiDataRead;
        }
        else if (pGzip->pIFile)
        {
            /* get the source of the rest */
            pGzip->uiDataLeft = IFILE_Read(pGzip->pIFile, (WE_VOID*)pGzip->pucBuf, pGzip->uiBufLen);
            if (0 == pGzip->uiDataLeft)
            {
                uiFileSize = Gzip_GetFileSize(pGzip->pIFile, pGzip->pIFileMgr);
                if (pGzip->uiFileRead != uiFileSize && uiDataRead <= GZIP_BUF_LEN)
                {
                    MEMCPY(pGzip->pucBuf, pucBuf, uiDataRead); /* store the data */
                    pGzip->uiDataLeft = uiDataRead;
                    pGzip->iErr = ZIP_FILE_WOULDBLOCK;
                    return 0;
                }
                pGzip->iErr = ZIP_ERR_FILE;
                return 0;
            }
            pGzip->uiFileRead += pGzip->uiDataLeft;
            /* update the buffer */
            if (ZIP_OK != Unzip_StreamUpdate((WE_HANDLE)pGzip->pStream, pGzip->pucBuf, 
                                               pGzip->uiDataLeft))
            {
                pGzip->iErr = ZIP_STREAM_ERROR;
                return 0;
            }
        }
        else
        {
            pGzip->iErr = ZIP_ERR_FILE;
            return 0;
        }

    }
}


/*=================================================================================
===
	FUNCTION: Gzip_GetLastErr

	CREATE DATE: 2007-5-29
	AUTHOR: duhuanbiao

	DESCRIPTION:
    This function is used for registering a callback function which tries to read 
    from the gzip file at a later time
		
	ARGUMENTS PASSED:

	RETURN VALUE:

	CALL BY:

	IMPORTANT NOTES:
		NULL

===================================================================================
==*/
WE_VOID Gzip_Readable(WE_HANDLE hHnd, PFNNOTIFY pfn, WE_VOID * pUser)
{
    P_Gzip_State pGzip = (P_Gzip_State)hHnd;
    if (pGzip)
    {
        IFILE_Readable(pGzip->pIFile, pfn, pUser);
    }
}

/*=================================================================================
===
	FUNCTION: Gzip_GetLastErr

	CREATE DATE: 2007-5-29
	AUTHOR: duhuanbiao

	DESCRIPTION:
		return the last error
		
	ARGUMENTS PASSED:

	RETURN VALUE:

	CALL BY:

	IMPORTANT NOTES:
		NULL

===================================================================================
==*/
WE_INT32 Gzip_GetLastErr(WE_HANDLE hHnd)
{
    P_Gzip_State pGzip = (P_Gzip_State)hHnd;
    return pGzip ? pGzip->iErr : ZIP_PARA_ERR;
}


/*=================================================================================
===
	FUNCTION: Gzip_Close

	CREATE DATE: 2007-5-29
	AUTHOR: duhuanbiao

	DESCRIPTION:
		close the handle
		
	ARGUMENTS PASSED:

	RETURN VALUE:

	CALL BY:

	IMPORTANT NOTES:
		NULL

===================================================================================
==*/
WE_INT32 Gzip_Close(WE_HANDLE hHnd)
{
    P_Gzip_State pGzip = (P_Gzip_State)hHnd;
    if (pGzip)
    {
        TOFREE(pGzip->pucBuf);
        if (pGzip->pIFileMgr)
        {
            IFILEMGR_Release(pGzip->pIFileMgr);
			pGzip->pIFileMgr = NULL;
        }
        if (pGzip->pIFile)
        {
            IFILE_Release(pGzip->pIFile);
			pGzip->pIFile = NULL;
        }
        Unzip_StreamClose((WE_HANDLE)pGzip->pStream);
		pGzip->pStream = NULL;
        FREE(pGzip);
		pGzip = NULL;
    }

    return ZIP_OK;
}