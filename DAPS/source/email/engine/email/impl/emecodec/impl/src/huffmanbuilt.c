/*=================================================================================
====
	MODULE NAME : zip-huffmanbuilt.c
	MODULE NAME : zip

    GENERAL DESCRIPTION
        generate Huffman trees for efficient decoding

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

//pclint
/*lint -e428 */
/*lint -e661 */


/*******************************************************************************
* Include File Section
*******************************************************************************/

#include "microdef.h"
#include "unzip.h"


/*******************************************************************************
* Macro Define Section
*******************************************************************************/

#define MAXBITS 15


/************************************************************************/
/* Function prototype */
/************************************************************************/
/*=================================================================================
===
	FUNCTION: Unzip_Table

	CREATE DATE: 2006-11-1
	AUTHOR: duhuanbiao

	DESCRIPTION:
		Build a set of tables to decode the provided canonical Huffman code
		
	ARGUMENTS PASSED:
		eType:    the table type
		puLens:   the work area lens can't less than this value
		uiCodes:  code length
		ppstTable:table start at *ppstTable
		puiBits:   when as a input,it is the requested root table index bits, 
	and on return it is the actual root table index bits.
		puWork:   work area point.
		
	RETURN VALUE:
		0:        success
	   -1:		  an invalid code
	    1:		  ENOUGH isn't enough

	CALL BY:
		unzip fun

	IMPORTANT NOTES:
		It will differ if the request is greater than the
	longest code or if it is less than the shortest code.

===================================================================================
==*/
WE_INT32 Unzip_Table(
                     E_UNZIP_CodeType eType, 
                     WE_UINT16 *puLens,
                     WE_UINT32	uiCodes,
                     St_CodeCell **ppstTable,
                     WE_UINT32 *puiBits,
                     WE_UINT16 *puWork
                     )
{
    WE_UINT32 uiCodeLen = 0U;             /* a code's length in bits */
    WE_UINT32 uiSymbolIndex = 0U;         /* index of code symbols */
    WE_UINT32 uiMinLens  = 0U;            /* minimum code lengths */
    WE_UINT32 uiMaxLens = 0U;             /* maximum code lengths */
    WE_UINT32 uiRootIndexNum = 0U;		  /* number of index bits for root table */
    WE_UINT32 uiCurrIndexNum = 0U;		  /* number of index bits for current table */
    WE_UINT32 uiBitsDrop = 0U;			  /* code bits to drop for sub-table */
    WE_UINT32 uiCodeUsed = 0U;			  /* code entries in table used */
    WE_UINT32 uiHuffmanCode = 0U;		  /* Huffman code */
    WE_UINT32 uiIncrement = 0U;			  /* for incrementing code, index */
    WE_UINT32 uiCopyIndex = 0U;			  /* index for replicating entries */
    WE_UINT32 uiLowBits = 0U;			  /* low bits for current root entry */
    WE_UINT32 uiLowBitsMask = 0U;		  /* mask for low root bits */
    St_CodeCell stTableNow = {0};         /* table entry for duplication */
    St_CodeCell *pstTableNext = NULL;	  /* next available space in table */
    const WE_UINT16 *puTableBase = NULL;  /* base value table to use */
    const WE_UINT16 *puTableExtra = NULL; /* extra bits table to use */
    WE_INT32 iLeftNum = 0;                /* number of prefix codes available */
    WE_INT32 iEnd = 0;                    /* use base and extra for symbol > iEnd */
    WE_UINT16 auCodeLens[MAXBITS+1] = {0};/* number of codes of each length */
    WE_UINT16 auCodeOffset[MAXBITS+1] = {0};  /* offsets in table for each length */
    
    /* Length codes 257..285 base */
    static const WE_UINT16 s_auLensBase[31] = { 3, 4, 5, 6, 7, 8, 9, 10, 11,
        13, 15, 17, 19, 23, 27, 31,35, 43, 51, 59, 67, 83, 99, 115, 131, 163,
        195, 227, 258, 0, 0};
    
    /* Length codes 257..285 extra */
    static const WE_UINT16 s_auLensExtra[31] = { 16, 16, 16, 16, 16, 16, 16,
        16, 17, 17, 17, 17, 18, 18, 18, 18,19, 19, 19, 19, 20, 20, 20, 20, 
        21, 21, 21, 21, 16, 201, 196};
    
    /* Distance codes 0..29 base */
    static const WE_UINT16 s_auDistBase[32] = { 1, 2, 3, 4, 5, 7, 9, 13, 17,
        25, 33, 49, 65, 97, 129, 193,257, 385, 513, 769, 1025, 1537, 2049,
        3073, 4097, 6145, 8193, 12289, 16385, 24577, 0, 0};
    
    /* Distance codes 0..29 extra */
    static const WE_UINT16 s_auDistExtra[32] = { 16, 16, 16, 16, 17, 17, 18, 
        18, 19, 19, 20, 20, 21, 21, 22, 22, 23, 23, 24, 24, 25, 25, 26, 26,
        27, 27, 28, 28, 29, 29, 64, 64};
    
    if (puLens == NULL ||
        puiBits == NULL ||
        ppstTable == NULL ||
        *ppstTable == NULL ||
        puWork == NULL)
    {
        return ZIP_PARA_ERR;
    }
    /* accumulate lengths for codes (assumes puLens[] all in 0..MAXBITS) */
    for (uiCodeLen = 0; uiCodeLen <= MAXBITS; uiCodeLen++)
    {
        auCodeLens[uiCodeLen] = 0;
    }
    for (uiSymbolIndex = 0; uiSymbolIndex < uiCodes; uiSymbolIndex++)
    {
        auCodeLens[puLens[uiSymbolIndex]]++;
    }
    
    /* bound code lengths, force root to be within code lengths */
    uiRootIndexNum = *puiBits;
    for (uiMaxLens = MAXBITS; uiMaxLens >= 1; uiMaxLens--)
    {
        if (auCodeLens[uiMaxLens] != 0) 
        {
            break;
        }
    }
    if (uiRootIndexNum > uiMaxLens) 
    {
        uiRootIndexNum = uiMaxLens;
    }
    if (uiMaxLens == 0) /* no symbols to code at all */
    {                     
        stTableNow.ucOperationBits = (WE_UINT8)64;    /* invalid code marker */
        stTableNow.ucCodeBits = (WE_UINT8)1;
        stTableNow.uOffset = (WE_UINT16)0;
        *(*ppstTable)++ = stTableNow;             /* make a table to force an error */
        *(*ppstTable)++ = stTableNow;
        *puiBits = 1;
        return 0;     /* no symbols, but wait for decoding to report error */
    }
    for (uiMinLens = 1; uiMinLens <= MAXBITS; uiMinLens++)
    {
        if (auCodeLens[uiMinLens] != 0)	
        {
            break;
        }
    }
    if (uiRootIndexNum < uiMinLens) 
    {
        uiRootIndexNum = uiMinLens;
    }
    
    /* check for an over-subscribed or incomplete set of lengths */
    iLeftNum = 1;
    for (uiCodeLen = 1; uiCodeLen <= MAXBITS; uiCodeLen++) 
    {
        iLeftNum = (WE_INT32)((WE_UINT32)iLeftNum << 1);
        iLeftNum -= auCodeLens[uiCodeLen];
        if (iLeftNum < 0) 
        {
            return -1;        /* over-subscribed */
        }    
    }
    if (iLeftNum > 0 && (eType == E_UNZIP_CODES || uiMaxLens != 1))
    {
        return -1;                      /* incomplete set */
    }
    /* generate offsets into symbol table for each length for sorting */
    auCodeOffset[1] = 0;
    for (uiCodeLen = 1; uiCodeLen < MAXBITS; uiCodeLen++)
    {
        auCodeOffset[uiCodeLen + 1] = auCodeOffset[uiCodeLen] + auCodeLens[uiCodeLen];
    }
    /* sort symbols by length, by symbol order within each length */
    for (uiSymbolIndex = 0; uiSymbolIndex < uiCodes; uiSymbolIndex++)
    {
        if (puLens[uiSymbolIndex] != 0) 
        {
            puWork[auCodeOffset[puLens[uiSymbolIndex]]++] = (WE_UINT16)uiSymbolIndex;
        }
    }
    
    /* set up for code type */
    switch (eType) 
    {
    case E_UNZIP_CODES:
        puTableBase = puTableExtra = puWork;    /* dummy value--not used */
        iEnd = 19;
        break;
    case E_UNZIP_LENS:
        puTableBase = s_auLensBase;
        puTableBase -= 257;
        puTableExtra = s_auLensExtra;
        puTableExtra -= 257;
        iEnd = 256;
        break;
    default:            /* DISTS */
        puTableBase = s_auDistBase;
        puTableExtra = s_auDistExtra;
        iEnd = -1;
    }
    
    /* initialize state for loop */
    uiHuffmanCode = 0;                    /* starting code */
    uiSymbolIndex = 0;                    /* starting code symbol */
    uiCodeLen = uiMinLens;                /* starting code length */
    pstTableNext = *ppstTable;            /* current table to fill in */
    uiCurrIndexNum = uiRootIndexNum;      /* current table index bits */
    uiBitsDrop = 0;						  /* current bits to uiBitsDrop from code for index */
    uiLowBits = (WE_UINT32)(-1);          /* trigger new sub-table when len > root */
    uiCodeUsed = 1U << uiRootIndexNum;    /* use root table entries */
    uiLowBitsMask = uiCodeUsed - 1;       /* mask for comparing low */
    
    /* check available table space */
    if (eType == E_UNZIP_LENS && uiCodeUsed >= ENOUGH - MAXD)
    {
        return 1;
    }
    /* process all codes and make table entries */
    for (;;) 
    {
        /* create table entry */
        stTableNow.ucCodeBits = (WE_UINT8)(uiCodeLen - uiBitsDrop);
        if ((WE_INT32)(puWork[uiSymbolIndex]) < iEnd) 
        {
            stTableNow.ucOperationBits = 0U;
            stTableNow.uOffset = puWork[uiSymbolIndex];
        }
        else if ((WE_INT32)(puWork[uiSymbolIndex]) > iEnd) 
        {
            stTableNow.ucOperationBits = (WE_UINT8)(puTableExtra[puWork[uiSymbolIndex]]);
            stTableNow.uOffset = puTableBase[puWork[uiSymbolIndex]];
        }
        else 
        {
            stTableNow.ucOperationBits = (WE_UINT8)(32 + 64);         /* end of block */
            stTableNow.uOffset = 0;
        }
        
        /* replicate for those indices with low len bits equal to huff */
        uiIncrement = 1U << (uiCodeLen - uiBitsDrop);
        uiCopyIndex = 1U << uiCurrIndexNum;
        uiMinLens = uiCopyIndex;                 /* save offset to next table */
        do 
        {
            uiCopyIndex -= uiIncrement;
            pstTableNext[(uiHuffmanCode >> uiBitsDrop) + uiCopyIndex] = stTableNow;
        } while (uiCopyIndex != 0);
        
        /* backwards increment the len-bit code huff */
        uiIncrement = 1U << (uiCodeLen - 1);
        while (uiHuffmanCode & uiIncrement)
        {
            uiIncrement >>= 1;
        }
        if (uiIncrement != 0) 
        {
            uiHuffmanCode &= uiIncrement - 1;
            uiHuffmanCode += uiIncrement;
        }
        else
        {
            uiHuffmanCode = 0;
        }
        /* go to next symbol, update count, len */
        uiSymbolIndex++;
        if (--(auCodeLens[uiCodeLen]) == 0) 
        {
            if (uiCodeLen == uiMaxLens) 
            {
                break;
            }
            uiCodeLen = puLens[puWork[uiSymbolIndex]];
        }
        
        /* create new sub-table if needed */
        if (uiCodeLen > uiRootIndexNum && (uiHuffmanCode & uiLowBitsMask) != uiLowBits) 
        {
            /* if first time, transition to sub-tables */
            if (uiBitsDrop == 0)
            {
                uiBitsDrop = uiRootIndexNum;
            }
            /* increment past last table */
            pstTableNext += uiMinLens;            /* here uiMinLens is 1 << curr */
            
            /* determine length of next table */
            uiCurrIndexNum = uiCodeLen - uiBitsDrop;
            iLeftNum = (WE_INT32)(1 << uiCurrIndexNum);
            while (uiCurrIndexNum + uiBitsDrop < uiMaxLens) 
            {
                iLeftNum -= auCodeLens[uiCurrIndexNum + uiBitsDrop];
                if (iLeftNum <= 0)
                {
                    break;
                }
                uiCurrIndexNum++;
                iLeftNum = (WE_INT32)((WE_UINT32)iLeftNum << 1);
            }
            
            /* check for enough space */
            uiCodeUsed += 1U << uiCurrIndexNum;
            if (eType == E_UNZIP_LENS && uiCodeUsed >= ENOUGH - MAXD)
            {
                return 1;
            }
            /* point entry in root table to sub-table */
            uiLowBits = uiHuffmanCode & uiLowBitsMask;
            (*ppstTable)[uiLowBits].ucOperationBits = (WE_UINT8)uiCurrIndexNum;
            (*ppstTable)[uiLowBits].ucCodeBits = (WE_UINT8)uiRootIndexNum;
            (*ppstTable)[uiLowBits].uOffset = (WE_UINT16)(pstTableNext - *ppstTable);
        }
    }
    
    stTableNow.ucOperationBits = (WE_UINT8)64;                /* invalid code marker */
    stTableNow.ucCodeBits = (WE_UINT8)(uiCodeLen - uiBitsDrop);
    stTableNow.uOffset = (WE_UINT16)0;
    while (uiHuffmanCode != 0) 
    {
        /* when done with sub-table, drop back to root table */
        if (uiBitsDrop != 0 && (uiHuffmanCode & uiLowBitsMask) != uiLowBits) 
        {
            uiBitsDrop = 0;
            uiCodeLen = uiRootIndexNum;
            pstTableNext = *ppstTable;
            stTableNow.ucCodeBits = (WE_UINT8)uiCodeLen;
        }
        
        /* put invalid code marker in table */
        pstTableNext[uiHuffmanCode >> uiBitsDrop] = stTableNow;
        
        /* backwards increment the len-bit code huff */
        uiIncrement = 1U << (uiCodeLen - 1);
        while (uiHuffmanCode & uiIncrement)
        {
            uiIncrement >>= 1;
        }
        if (uiIncrement != 0) 
        {
            uiHuffmanCode &= uiIncrement - 1;
            uiHuffmanCode += uiIncrement;
        }
        else
        {
            uiHuffmanCode = 0;
        }
    }
    
    /* set return parameters */
    *ppstTable += uiCodeUsed;
    *puiBits = uiRootIndexNum;
    return 0;
}
