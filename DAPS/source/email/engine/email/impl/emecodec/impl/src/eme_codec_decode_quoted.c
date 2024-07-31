/*=====================================================================================
FILE NAME   : eme_decode_quoted.c
MODULE NAME : email codec

  
    GENERAL DESCRIPTION
      This File will support some function to operate string.
      These functions don't be changed by project changed.
    
      TechFaith Software Confidential Proprietary
      Copyright (c) 2006-2007 by TechFaith Software. All Rights Reserved.
      =======================================================================================
      Revision History
      
        Modification                   Tracking
        Date              Author       Number       Description of changes
        ----------   --------------   ---------   --------------------------------------
         20070306       wxs             01                  create
                 
=====================================================================================*/

/************************************************************************/
/*                     include files                                   */
/************************************************************************/
#include "eme_codec_decode_quoted.h"


/*============function implement==========*/

/*Hex ch -> Dec ch*/
WE_UINT8 EME_HexToDec( WE_UINT8 C )
{
   if(C <= 'z' && C >= 'a')
      C -= 'a' - 'A';

    return C <= '9' ? C - '0' : C - 'A' + 10;
}


/*get decoded string size from original string*/
WE_UINT32 EME_QuotedDecodeSize(WE_UINT32 iSize)
{
    /* return (iSize + 3) / 4 * 3; */
    return (iSize + 1); /* we allocate enough mem, code upwards may be some problem. */ 
}


/*
 *	decode the quoted printable string.
 */
WE_INT32 EME_FromQuoted(WE_CHAR *pDest, WE_CHAR *pSrc, WE_UINT32 uiInSize)
{
    WE_UINT8 nA = 0;
    WE_UINT8 nB = 0;
    WE_CHAR C[2] = {0};
			    
    WE_CHAR *InLimitPtr = NULL; 
    WE_CHAR *pDestOrg = NULL;
    WE_UINT32 nSize = 0;

    nSize = uiInSize;
    if ((pSrc == NULL) || (pDest == NULL) || (nSize == 0))
        return 0;
	    
    InLimitPtr = pSrc + nSize;
    pDestOrg = pDest;

    while (pSrc < InLimitPtr)
    {
        C[0] = *pSrc++; /*get first ch*/
        if (C[0] == '=') /*it is coded*/
        {
            if (*pSrc == '\r' && *(pSrc+1) == '\n' )
            {
                pSrc++;
                pSrc++;
                continue;
            }
            else if (*pSrc == '\n' && *(pSrc+1) == '\r')
            {
                pSrc++;
                pSrc++;
                continue;
            }
            else if (*pSrc == '\n' || *pSrc == '\r')
            {
                pSrc++;
                continue;
            }

            C[0] = *pSrc++; /*get next 2 ch*/
            C[1] = *pSrc++;			

            nA = EME_HexToDec((WE_UINT8)C[0]);
            nB = EME_HexToDec((WE_UINT8)C[1]);
            *pDest++ = (nA << 4) + nB;	
        }
//         else if (C[0] == '\r' || C[0] == '\n')  /*discard '\r' '\n'*/
//         {
//             continue;
//         }
        else /*else get it directly*/
        {
            *pDest++ = C[0];
        }
    }
    return (WE_INT32)(pDest - pDestOrg);
}

