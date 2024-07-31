/*=================================================================================
====
	MODULE NAME : zip-adler_check.c
	MODULE NAME : zip

    GENERAL DESCRIPTION
         compute the Adler-32 checksum of a data stream
         
    Copyright (C) 1995-2004 Mark Adler
    For conditions of distribution and use, see copyright notice in common.h
===================================================================================
====
Revision History
Modification Tracking
	Date		Author			Number		Description of changes
   ---------- -------------- --------- --------------------------------------
   2006-10-29   duhuanbiao       1               create

===================================================================================
==*/


/*******************************************************************************
* Include File Section
*******************************************************************************/

#include "zip.h"

/*******************************************************************************
* Macro Define Section
*******************************************************************************/
/* ADLER_BASE_LEN is the largest n such that 
   255n(n+1)/2 + (n+1)(BASE-1) <= 2^32-1 */
#define ADLER_BASE_LEN 4096U

/* mod base */
#define ADLER_MOD_BASE 65521UL


/************************************************************************/
/* function prototype */
/************************************************************************/
/*=================================================================================
===
	FUNCTION: Zip_AdlerCheck

	CREATE DATE: 2006-11-1
	AUTHOR: duhuanbiao

	DESCRIPTION:
		this function is a simply buffer data check.
		
	ARGUMENTS PASSED:
		ulAdlerCheck:       the value is the value get by Zip_AdlerCheck,first used is Zip_AdlerCheck(0, NULL, 0).
		pucBufBeChecked:    the buffer you want to check.
		uiBufferLen:        the buffer length.

	RETURN VALUE:
		the buffer checksum

	CALL BY:
		any fun want to get a checksum

	IMPORTANT NOTES:
		NULL

===================================================================================
==*/
WE_ULONG  Zip_AdlerCheck(
                         WE_ULONG ulAdlerCheck,
                         const WE_UINT8 *pucBufBeChecked, 
                         WE_UINT32 uiBufferLen
                         )
{
    /* ulSum_A = 1 + D1 + D2 + ... + Dn (mod 65521) */
    WE_ULONG ulSum_A = 0UL;

    /* ulSum_B = (1 + D1) + (1 + D1 + D2) + ...
                  + (1 + D1 + D2 + ... + Dn) (mod 65521) */
    WE_ULONG ulSum_B = 0UL;

    /* every times we only process ADLER_BASE_LEN bytes */
    WE_UINT32 uiPerLen = 0U;

    /* used to initial */
    if (NULL == pucBufBeChecked)
    {
        return 1UL;
    }

    ulSum_A += ulAdlerCheck & 0xFFFFUL;
    ulSum_B += (ulAdlerCheck >> 16) & 0xFFFFUL;

    while(uiBufferLen)
    {
        uiPerLen = uiBufferLen > ADLER_BASE_LEN ? ADLER_BASE_LEN : uiBufferLen;
        uiBufferLen -= uiPerLen;
        while(uiPerLen--)
        {
            ulSum_A += *(pucBufBeChecked++);
            ulSum_B += ulSum_A;
        }
        /* mod 65521 */
        ulSum_A %= ADLER_MOD_BASE;
        ulSum_B %= ADLER_MOD_BASE;
    }

    return ulSum_A | (ulSum_B << 16);
}