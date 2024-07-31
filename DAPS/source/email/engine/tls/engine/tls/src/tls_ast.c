/*=====================================================================================
    FILE NAME   : wap_ast.c
    MODULE NAME : WAP_AST
    
    GENERAL DESCRIPTION
        Ast is assistant
        
    TechFaith Software Confidential Proprietary
    Copyright (c) 2006-2007 by TechFaith Software. All Rights Reserved.
=======================================================================================
    Revision History
    
    Modification Tracking
    Date       Author         Number    Description of changes
    ---------- -------------- --------- --------------------------------------
    2006-05-24 ChenZhifeng    none      Init
    2006-08-02 ChenZhifeng    none      Add MEM_TEST
    
=====================================================================================*/

/**************************************************************************************
*   Include File Section
**************************************************************************************/


#include "tls_ast.h"

#ifdef MEM_TEST
#include "we_scl.h"
#endif


#define WAP_ARRAY_CHRT_TYPE "\x10\x00\x10\x00\x10\x00\x10\x00\x10\x00\x10\x00\x10\x00\x10\x00\
\x10\x00\
\xBA\x00\
\x92\x00\
\x12\x00\
\x12\x00\
\x92\x00\
\x10\x00\x10\x00\
\x10\x00\x10\x00\x10\x00\x10\x00\x10\x00\x10\x00\x10\x00\x10\x00\
\x10\x00\x10\x00\x10\x00\x10\x00\x10\x00\x10\x00\x10\x00\x10\x00\
\xAE\x00\
\x00\x10\
\x20\x80\
\x00\x80\
\x00\x20\
\x40\x80\
\x00\x20\
\x00\x10\
\x20\x10\
\x20\x10\
\x00\x10\
\x00\x20\
\x20\x20\
\x00\x10\
\x00\x10\
\x20\x40\
\x00\x04\
\x00\x04\
\x00\x04\
\x00\x04\
\x00\x04\
\x00\x04\
\x00\x04\
\x00\x04\
\x00\x04\
\x00\x04\
\x20\x20\
\x20\x40\
\x20\x80\
\x20\x20\
\x20\x80\
\x20\x40\
\x20\x20\
\x00\x09\
\x00\x09\
\x00\x09\
\x00\x09\
\x00\x09\
\x00\x09\
\x00\x01\
\x00\x01\
\x00\x01\
\x00\x01\
\x00\x01\
\x00\x01\
\x00\x01\
\x00\x01\
\x00\x01\
\x00\x01\
\x00\x01\
\x00\x01\
\x00\x01\
\x00\x01\
\x00\x01\
\x00\x01\
\x00\x01\
\x00\x01\
\x00\x01\
\x00\x01\
\x21\x00\
\x21\x00\
\x21\x00\
\x01\x00\
\x00\x10\
\x01\x00\
\x00\x0A\
\x00\x0A\
\x00\x0A\
\x00\x0A\
\x00\x0A\
\x00\x0A\
\x00\x02\
\x00\x02\
\x00\x02\
\x00\x02\
\x00\x02\
\x00\x02\
\x00\x02\
\x00\x02\
\x00\x02\
\x00\x02\
\x00\x02\
\x00\x02\
\x00\x02\
\x00\x02\
\x00\x02\
\x00\x02\
\x00\x02\
\x00\x02\
\x00\x02\
\x00\x02\
\x21\x00\
\x01\x00\
\x21\x00\
\x00\x10\
\x10\x00\
\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\
\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\
\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\
\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\
\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\
\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\
\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\
\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"




#ifndef MEM_TEST
/*when you invoke this function, you must free the return value */
WE_CHAR * TLS_Strndup   
(
    const WE_CHAR *pcStrSource,
    WE_INT32      iLen
)
{
    WE_CHAR *pcDest = NULL;
    
    if( iLen < 0)
    {
        return NULL;
    }
    if (NULL == pcStrSource)
    {
        pcDest = (WE_CHAR*)WAP_MALLOC((WE_ULONG)1);
        /*
#ifdef WE_CFG_BREW_PLATFORM        
        pcDest = (WE_CHAR*)MALLOC((WE_ULONG)1);
#else  
#ifdef WIN32_DEBUG
        pcDest = (WE_CHAR*)malloc((WE_ULONG)1);
#endif
*/

        if (pcDest != NULL)
        {
            pcDest[0] = '\0';
        }
    }
    else
    {
        pcDest = (WE_CHAR*)WAP_MALLOC((WE_ULONG)(iLen + 1));
        /*
#ifdef WE_CFG_BREW_PLATFORM
        pcDest = (WE_CHAR*)MALLOC((WE_ULONG)(iLen + 1));
#else
        pcDest = (WE_CHAR*)malloc((WE_ULONG)(iLen + 1));
#endif
*/
        if (NULL == pcDest)
        {
            return NULL;
        }
        WAP_STRNCPY(pcDest,pcStrSource,(WE_UINT32)iLen);
        /*
#ifdef WE_CFG_BREW_PLATFORM
        (WE_VOID)STRNCPY(pcDest,pcStrSource,(WE_UINT32)iLen);
#else
        strncpy(pcDest,pcStrSource,(WE_UINT32)iLen);
#endif  
*/
    }
    return pcDest;
}
#endif

WE_UINT32 TlsAst_GetCurrentTime( void )
{
    WE_UINT32 uiTmpTime = 0;
    
#ifdef WE_CFG_BREW_PLATFORM
    uiTmpTime = GETTIMESECONDS();
    uiTmpTime += (WE_UINT32)((10 * 365 + 2 + 5) * 3600 * 24);
#else

#ifdef WIN32_DEBUG   
    time( (time_t *)&uiTmpTime );
#else
    uiTmpTime = WE_SCL_GETSECONDS();
#endif

#endif
    return uiTmpTime;
}

#if 0
WE_UINT32  Wap_GetRand(void)
{
    WE_UINT32 uiTmp = 0;
#ifdef BREW
    GETRAND((WE_UCHAR*)&uiTmp, 4);
    return uiTmp;
#else
    srand((unsigned)time(NULL));
    uiTmp = (WE_UINT32)rand();
    return uiTmp;
#endif 
    
}
#endif
#if 0
WE_INT32 Wap_Is(WE_CHAR cCh)
{
    WE_UINT8 *aAscTable = (WE_UINT8 *)WAP_ARRAY_CHRT_TYPE;
    WE_UINT16 usTempHigh; 
    WE_UINT16 usTempLow; 

    usTempHigh= aAscTable[cCh * 2];
    usTempLow = aAscTable[cCh * 2 + 1];

    return (((usTempHigh << 8) & 0xff00) + (usTempLow & 0x00ff));
}
#endif

#ifdef MEM_TEST
#undef WE_LOGINFO
#define WE_LOGINFO DBGPRINTF
#define MEM_MAX_MALLOC      5120
typedef struct
{
    WE_CHAR     acFileName[16];
    WE_INT32    iLine;
    WE_INT32    iSize;
    void       *pvMemBlock;
} Mem_TestInfo;
Mem_TestInfo    astInfo[MEM_MAX_MALLOC] = {0};
WE_INT32        iMaxCount=0;
WE_INT32        iUsedMemory = 0;
WE_INT32        g_iMaxUsedMemory = 0;
WE_INT32        g_iMemSize = 10000000;

void *Wap_Malloc
(
    WE_CHAR *pcFileName,
    WE_INT32 iLine,
    WE_INT32 iSize
)
{
    void        *pvMemBlock=NULL;
    WE_INT32    iCount;
    WE_CHAR     *rChr;
    WE_INT32    iIsFound=0;

    if(iUsedMemory+iSize > g_iMemSize)
    {
        WAP_LOG_PRINT((E_WAP_DBG_MEM_TEST,E_WAP_DBG_LEVEL_LOW)( "WAP_MALLOC: Total Memory is %d, Used MemorySize is %d \n", \
                                            g_iMemSize, iUsedMemory);)
        rChr = STRRCHR( pcFileName, '\\' );

        WAP_LOG_PRINT((E_WAP_DBG_MEM_TEST,E_WAP_DBG_LEVEL_LOW)( "WAP_MALLOC:%s,%d,Size is %d, Malloc failed.\n", \
                                                NULL!=rChr ? rChr:pcFileName, iLine, iSize );)
        return NULL;
    }
    
    if(iSize<=0)
    {
        WAP_LOG_PRINT((E_WAP_DBG_MEM_TEST,E_WAP_DBG_LEVEL_LOW)( "WAP_MALLOC()___ iSize<=0 " );)
        return NULL;
    }
    pvMemBlock = MALLOC( iSize );
    if( NULL == pvMemBlock )
    {
        WAP_LOG_PRINT((E_WAP_DBG_MEM_TEST,E_WAP_DBG_LEVEL_LOW)( "WAP_MALLOC:%s,%d,Size is %d, Malloc failed.\n", \
                                                pcFileName, iLine, iSize );)
    }
    else
    {
        /*
         *Check if the memory has been malloced
         */
        iUsedMemory += iSize;
        g_iMaxUsedMemory = MAX(g_iMaxUsedMemory, iUsedMemory);
        for (iCount=0; iCount<MEM_MAX_MALLOC; iCount++)
        {
            if (pvMemBlock == astInfo[iCount].pvMemBlock)
            {
                WAP_LOG_PRINT((E_WAP_DBG_MEM_TEST,E_WAP_DBG_LEVEL_LOW)("==========================================================\n");)
                WAP_LOG_PRINT((E_WAP_DBG_MEM_TEST,E_WAP_DBG_LEVEL_LOW)("WAP_MALLOC:The memory exists\n");)
                WAP_LOG_PRINT((E_WAP_DBG_MEM_TEST,E_WAP_DBG_LEVEL_LOW)("WAP_MALLOC:-1st-[%d]%s,%d, %d, 0x%x\n",iCount, astInfo[iCount].acFileName, \
                    astInfo[iCount].iLine, astInfo[iCount].iSize, astInfo[iCount].pvMemBlock );)
                WAP_LOG_PRINT((E_WAP_DBG_MEM_TEST,E_WAP_DBG_LEVEL_LOW)("==========================================================\n");)
                break;
            }
        }

        for(iCount = 0; iCount < MEM_MAX_MALLOC; iCount++)
        {
            if(0 == astInfo[iCount].iSize)
            {
                rChr = STRRCHR( pcFileName, '\\' );
                if( NULL != rChr )
                {
                    rChr+=1;
                    MEMSET( astInfo[iCount].acFileName, 0, 16 );
                    MEMCPY( astInfo[iCount].acFileName, rChr,WAP_STRLEN(rChr) );
                }
                else
                {
                    MEMSET( astInfo[iCount].acFileName, 0, 16 );
                }
                iIsFound = 1;
                astInfo[iCount].iLine = iLine;
                astInfo[iCount].iSize = iSize;
                astInfo[iCount].pvMemBlock = pvMemBlock;
                WAP_LOG_PRINT((E_WAP_DBG_MEM_TEST,E_WAP_DBG_LEVEL_LOW)( "WAP_MALLOC:[%d]%s,%d, %d, 0x%x\n",iCount, astInfo[iCount].acFileName, \
                    astInfo[iCount].iLine, astInfo[iCount].iSize, astInfo[iCount].pvMemBlock );)
                iMaxCount = MAX(iMaxCount, iCount);
                break;
            }
        }
    }
    
    if( 0 == iIsFound )
    {
        WAP_LOG_PRINT((E_WAP_DBG_MEM_TEST,E_WAP_DBG_LEVEL_LOW)( "WAP_MALLOC:-------------------------------------------------\n" );)
        WAP_LOG_PRINT((E_WAP_DBG_MEM_TEST,E_WAP_DBG_LEVEL_LOW)( "WAP_MALLOC:Malloc times exceed MEM_MAX_MALLOC.\n" );)
        WAP_LOG_PRINT((E_WAP_DBG_MEM_TEST,E_WAP_DBG_LEVEL_LOW)( "WAP_MALLOC:[%d]%s,%d,Block is 0x%x\n",iCount, (NULL == rChr) ? "NULL":(rChr+1), iLine, pvMemBlock );)
        WAP_LOG_PRINT((E_WAP_DBG_MEM_TEST,E_WAP_DBG_LEVEL_LOW)( "WAP_MALLOC:-------------------------------------------------\n" );)
    }

    WAP_LOG_PRINT((E_WAP_DBG_MEM_TEST,E_WAP_DBG_LEVEL_LOW)( "WAP_MALLOC: Total Memory is %d, Used MemorySize is %d \n", \
                                            g_iMemSize, iUsedMemory);)
    
    return pvMemBlock;
}

void Wap_Free
(
    WE_CHAR *pcFileName,
    WE_INT32 iLine,
    const void *pstMemBlock
)
{
    WE_INT32    iCount;
    WE_INT32    iIsFound=0;
    WE_CHAR     *rChr;

    rChr = STRRCHR( pcFileName, '\\' );
    
    if( NULL == pstMemBlock )
    {
        WAP_LOG_PRINT((E_WAP_DBG_MEM_TEST,E_WAP_DBG_LEVEL_LOW)( "WAP_FREE:-------------------------------------------------\n" );)
        WAP_LOG_PRINT((E_WAP_DBG_MEM_TEST,E_WAP_DBG_LEVEL_LOW)( "WAP_FREE:You wanted to free a NULL memory\n" );)
        WAP_LOG_PRINT((E_WAP_DBG_MEM_TEST,E_WAP_DBG_LEVEL_LOW)( "WAP_FREE:%s,%d,Block is 0x%x\n", rChr+1, iLine, pstMemBlock );)
        WAP_LOG_PRINT((E_WAP_DBG_MEM_TEST,E_WAP_DBG_LEVEL_LOW)( "WAP_FREE:-------------------------------------------------\n" );)
        return;
    }
    
    for( iCount = 0; iCount < MEM_MAX_MALLOC; iCount++ )
    {
        if( astInfo[iCount].pvMemBlock == pstMemBlock )
        {
            iUsedMemory -= astInfo[iCount].iSize; 
            astInfo[iCount].iSize = 0;
            astInfo[iCount].pvMemBlock = NULL;
            iIsFound = 1;
            break;
        }
    }
    
    WAP_LOG_PRINT((E_WAP_DBG_MEM_TEST,E_WAP_DBG_LEVEL_LOW)( "WAP_FREE:[%d]%s,%d,Block is 0x%x\n", iCount, rChr+1, iLine, pstMemBlock);)
    if( 0 == iIsFound )
    {
        WAP_LOG_PRINT((E_WAP_DBG_MEM_TEST,E_WAP_DBG_LEVEL_LOW)( "WAP_FREE:-------------------------------------------------\n" );)
        WAP_LOG_PRINT((E_WAP_DBG_MEM_TEST,E_WAP_DBG_LEVEL_LOW)( "WAP_FREE:You want to free a unmalloc memory.\n" );)
        WAP_LOG_PRINT((E_WAP_DBG_MEM_TEST,E_WAP_DBG_LEVEL_LOW)( "WAP_FREE:[%d]%s,%d,Block is 0x%x\n", iCount, rChr+1, iLine, pstMemBlock );)
        WAP_LOG_PRINT((E_WAP_DBG_MEM_TEST,E_WAP_DBG_LEVEL_LOW)( "WAP_FREE:-------------------------------------------------\n" );)
        return;
    }
    FREE( (void*)pstMemBlock );

    WAP_LOG_PRINT((E_WAP_DBG_MEM_TEST,E_WAP_DBG_LEVEL_LOW)( "WAP_FREE: Total Memory is %d, Used MemorySize is %d \n", \
                                            g_iMemSize, iUsedMemory);)

}

WE_CHAR *Wap_StrDup
(
    WE_CHAR *pcFileName,
    WE_INT32 iLine,
    const WE_CHAR *pcString
)
{
    WE_CHAR *pcDupString;
    WE_CHAR     *rChr;
    WE_INT32    iCount;

    if( NULL == pcString )
    {
        return NULL;
    }
    rChr = STRRCHR( pcFileName, '\\' );

    if(iUsedMemory+WAP_STRLEN(pcString) + 1 > g_iMemSize)
    {
        WAP_LOG_PRINT((E_WAP_DBG_MEM_TEST,E_WAP_DBG_LEVEL_LOW)( "WAP_STRDUP:%s,%d, strdup failed.\n", \
                                                pcFileName, iLine );)
        WAP_LOG_PRINT((E_WAP_DBG_MEM_TEST,E_WAP_DBG_LEVEL_LOW)( "WAP_STRDUP: Total Memory is %d, Used MemorySize is %d \n", \
                                                g_iMemSize, iUsedMemory);)
        return NULL;
    }
    pcDupString = STRDUP( pcString );
    for( iCount = 0; iCount < MEM_MAX_MALLOC; iCount++ )
    {
        if( 0 == astInfo[iCount].iSize )
        {
            rChr+=1;
            MEMSET( astInfo[iCount].acFileName, 0, 16 );
            MEMCPY( astInfo[iCount].acFileName, rChr,WAP_STRLEN(rChr) );
            astInfo[iCount].iLine = iLine;
            astInfo[iCount].iSize = WAP_STRLEN(pcString) + 1;
            astInfo[iCount].pvMemBlock = pcDupString;
            iUsedMemory += astInfo[iCount].iSize;
            g_iMaxUsedMemory = MAX(g_iMaxUsedMemory, iUsedMemory);
            WAP_LOG_PRINT((E_WAP_DBG_MEM_TEST,E_WAP_DBG_LEVEL_LOW)( "WAP_STRDUP:[%d]%s,%d, %d, 0x%x\n", iCount, astInfo[iCount].acFileName, \
                    astInfo[iCount].iLine, astInfo[iCount].iSize, astInfo[iCount].pvMemBlock );)
            iMaxCount = MAX(iMaxCount,iCount);
            break;
        }
    }

    WAP_LOG_PRINT((E_WAP_DBG_MEM_TEST,E_WAP_DBG_LEVEL_LOW)( "WAP_STRDUP: Total Memory is %d, Used MemorySize is %d \n", \
                                            g_iMemSize, iUsedMemory);)

    return pcDupString;
}

WE_CHAR * TLS_Strndup   
(
    WE_CHAR *pcFileName,
    WE_INT32 iLine,
    const WE_CHAR *pcStrSource,
    WE_INT32      iLen
)
{
    WE_CHAR *pcDest = NULL;
    WE_CHAR     *rChr;
    WE_INT32    iCount;
    
    if( iLen < 0)
    {
        return NULL;
    }

    WAP_LOG_PRINT((E_WAP_DBG_MEM_TEST,E_WAP_DBG_LEVEL_LOW)(
                  "WAP_MALLOC:%s,%d, iLen=%d, TLS_Strndup failed.\n",
                  pcFileName, iLine, iLen );)

    if (NULL == pcStrSource)
    {
        if(iUsedMemory + 1 > g_iMemSize)
        {
            WAP_LOG_PRINT((E_WAP_DBG_MEM_TEST,E_WAP_DBG_LEVEL_LOW)( 
                          "TLS_Strndup: Total Memory is %d, Used MemorySize is %d, malloc len=%d \n",
                          g_iMemSize, iUsedMemory, iLen);)
            return NULL;
        }

        pcDest = (WE_CHAR*)MALLOC((WE_ULONG)1);
        if (pcDest != NULL)
        {
            pcDest[0] = '\0';
        }
    }
    else
    {
        if(iUsedMemory+iLen+1 > g_iMemSize)
        {
            WAP_LOG_PRINT((E_WAP_DBG_MEM_TEST,E_WAP_DBG_LEVEL_LOW)( 
                          "TLS_Strndup: Total Memory is %d, Used MemorySize is %d, malloc len=%d \n",
                          g_iMemSize, iUsedMemory, iLen);)
            return NULL;
        }

        pcDest = (WE_CHAR*)MALLOC((WE_ULONG)(iLen + 1));
        if (NULL == pcDest)
        {
            return NULL;
        }
        STRNCPY(pcDest,pcStrSource,(WE_UINT32)iLen);
    }

    rChr = STRRCHR( pcFileName, '\\' );
    for( iCount = 0; iCount < MEM_MAX_MALLOC; iCount++ )
    {
        if( 0 == astInfo[iCount].iSize )
        {
            rChr+=1;
            MEMSET( astInfo[iCount].acFileName, 0, 16 );
            MEMCPY( astInfo[iCount].acFileName, rChr, WAP_STRLEN(rChr) );
            astInfo[iCount].iLine = iLine;
            astInfo[iCount].iSize = iLen+1;
            iUsedMemory += astInfo[iCount].iSize;
            g_iMaxUsedMemory = MAX(g_iMaxUsedMemory, iUsedMemory);
            astInfo[iCount].pvMemBlock = pcDest;
            WAP_LOG_PRINT((E_WAP_DBG_MEM_TEST,E_WAP_DBG_LEVEL_LOW)( "TLS_Strndup:[%d]%s,%d, %d, 0x%x\n", iCount, astInfo[iCount].acFileName, \
                    astInfo[iCount].iLine, astInfo[iCount].iSize, astInfo[iCount].pvMemBlock );)
            iMaxCount = MAX(iMaxCount,iCount);
            break;
        }
    }

    WAP_LOG_PRINT((E_WAP_DBG_MEM_TEST,E_WAP_DBG_LEVEL_LOW)( "TLS_Strndup: Total Memory is %d, Used MemorySize is %d \n", \
                                            g_iMemSize, iUsedMemory);)
    
    return pcDest;
}


void *Wap_MemCpy
(
    WE_CHAR *pcFileName,
    WE_INT32 iLine,
    void    *pvDest,
    const void    *pvSrc,
    WE_UINT32 iSize
)
{
    WE_CHAR     *rChr;
    WE_INT32    iCount1,iCount2;
    
    rChr = STRRCHR( pcFileName, '\\' );
    for( iCount1 = 0; iCount1 < MEM_MAX_MALLOC; iCount1++ )
    {
        if (pvDest == astInfo[iCount1].pvMemBlock)
        {
            if( iSize > (WE_UINT32)astInfo[iCount1].iSize )
            {
                WAP_LOG_PRINT((E_WAP_DBG_MEM_TEST,E_WAP_DBG_LEVEL_LOW)( "WAP_MEMCPY:-------------------------------------------------\n" );)
                WAP_LOG_PRINT((E_WAP_DBG_MEM_TEST,E_WAP_DBG_LEVEL_LOW)( "WAP_MEMCPY:Operate memory[DEST] out-of-bourn.\n" );)
                WAP_LOG_PRINT((E_WAP_DBG_MEM_TEST,E_WAP_DBG_LEVEL_LOW)( "WAP_MEMCPY:[%d]%s,%d,Block is 0x%x\n",iCount1, rChr+1, iLine, pvDest);)
                WAP_LOG_PRINT((E_WAP_DBG_MEM_TEST,E_WAP_DBG_LEVEL_LOW)( "WAP_MEMCPY:-------------------------------------------------\n" );)
            }
            break;
        }
    }
    for( iCount2 = 0; iCount2 < MEM_MAX_MALLOC; iCount2++ )
    {
        if (pvSrc == astInfo[iCount2].pvMemBlock)
        {
            if( iSize > (WE_UINT32)astInfo[iCount2].iSize )
            {
                WAP_LOG_PRINT((E_WAP_DBG_MEM_TEST,E_WAP_DBG_LEVEL_LOW)( "WAP_MEMCPY:-------------------------------------------------\n" );)
                WAP_LOG_PRINT((E_WAP_DBG_MEM_TEST,E_WAP_DBG_LEVEL_LOW)( "WAP_MEMCPY:Operate memory[SRC] out-of-bourn.\n" );)
                WAP_LOG_PRINT((E_WAP_DBG_MEM_TEST,E_WAP_DBG_LEVEL_LOW)( "WAP_MEMCPY:[%d]%s,%d,Block is 0x%x\n",iCount2, rChr+1, iLine, pvSrc);)
                WAP_LOG_PRINT((E_WAP_DBG_MEM_TEST,E_WAP_DBG_LEVEL_LOW)( "WAP_MEMCPY:-------------------------------------------------\n" );)
            }
            break;
        }
    }

    WAP_LOG_PRINT((E_WAP_DBG_MEM_TEST,E_WAP_DBG_LEVEL_LOW)( "WAP_MEMCPY:size=%d\n",iSize);)
    WAP_LOG_PRINT((E_WAP_DBG_MEM_TEST,E_WAP_DBG_LEVEL_LOW)( "WAP_MEMCPY:[%d,%d]%s,%d,Dest is 0x%x, Src is 0x%x\n",iCount1,iCount2,rChr+1, iLine, pvDest, pvSrc);)
    return MEMCPY(pvDest,pvSrc,iSize);
}

void *Wap_MemSet
(
    WE_CHAR *pcFileName,
    WE_INT32 iLine,
    void       *pvDest,
    WE_INT32    iChar,
    WE_INT32    iSize
)
{
    WE_CHAR     *rChr;
    WE_INT32    iCount;
    
    rChr = STRRCHR( pcFileName, '\\' );
    for( iCount = 0; iCount < MEM_MAX_MALLOC; iCount++ )
    {
        if (pvDest == astInfo[iCount].pvMemBlock)
        {
            if( iSize > astInfo[iCount].iSize )
            {
                WAP_LOG_PRINT((E_WAP_DBG_MEM_TEST,E_WAP_DBG_LEVEL_LOW)( "WAP_MEMSET:-------------------------------------------------\n" );)
                WAP_LOG_PRINT((E_WAP_DBG_MEM_TEST,E_WAP_DBG_LEVEL_LOW)( "WAP_MEMSET:Operate memory[DEST] out-of-bourn.\n" );)
                WAP_LOG_PRINT((E_WAP_DBG_MEM_TEST,E_WAP_DBG_LEVEL_LOW)( "WAP_MEMSET:[%d]%s,%d,Block is 0x%x\n",iCount, rChr+1, iLine, pvDest);)
                WAP_LOG_PRINT((E_WAP_DBG_MEM_TEST,E_WAP_DBG_LEVEL_LOW)( "WAP_MEMSET:-------------------------------------------------\n" );)
            }
            break;
        }
    }

    WAP_LOG_PRINT((E_WAP_DBG_MEM_TEST,E_WAP_DBG_LEVEL_LOW)( "WAP_MEMSET:[%d]%s,%d,Dest is 0x%x\n",iCount, rChr+1, iLine, pvDest);)
    return MEMSET(pvDest,iChar,iSize);
}

void *Wap_MemMove
(
    WE_CHAR *pcFileName,
    WE_INT32 iLine,
    void    *pvDest,
    const void    *pvSrc,
    WE_INT32 iSize
)
{
    WE_CHAR     *rChr;
    WE_INT32    iCount1,iCount2;
    
    rChr = STRRCHR( pcFileName, '\\' );
    for( iCount1 = 0; iCount1 < MEM_MAX_MALLOC; iCount1++ )
    {
        if (pvDest == astInfo[iCount1].pvMemBlock)
        {
            if( iSize > astInfo[iCount1].iSize )
            {
                WAP_LOG_PRINT((E_WAP_DBG_MEM_TEST,E_WAP_DBG_LEVEL_LOW)( "WAP_MEMMOVE:-------------------------------------------------\n" );)
                WAP_LOG_PRINT((E_WAP_DBG_MEM_TEST,E_WAP_DBG_LEVEL_LOW)( "WAP_MEMMOVE:Operate memory[DEST] out-of-bourn.\n" );)
                WAP_LOG_PRINT((E_WAP_DBG_MEM_TEST,E_WAP_DBG_LEVEL_LOW)( "WAP_MEMMOVE:[%d]%s,%d,Block is 0x%x\n",iCount1, rChr+1, iLine, pvDest);)
                WAP_LOG_PRINT((E_WAP_DBG_MEM_TEST,E_WAP_DBG_LEVEL_LOW)( "WAP_MEMMOVE:-------------------------------------------------\n" );)
            }
            break;
        }
    }
    for( iCount2 = 0; iCount2 < MEM_MAX_MALLOC; iCount2++ )
    {
        if (pvSrc == astInfo[iCount2].pvMemBlock)
        {
            if( iSize > astInfo[iCount2].iSize )
            {
                WAP_LOG_PRINT((E_WAP_DBG_MEM_TEST,E_WAP_DBG_LEVEL_LOW)( "WAP_MEMMOVE:-------------------------------------------------\n" );)
                WAP_LOG_PRINT((E_WAP_DBG_MEM_TEST,E_WAP_DBG_LEVEL_LOW)( "WAP_MEMMOVE:Operate memory[SRC] out-of-bourn.\n" );)
                WAP_LOG_PRINT((E_WAP_DBG_MEM_TEST,E_WAP_DBG_LEVEL_LOW)( "WAP_MEMMOVE:[%d]%s,%d,Block is 0x%x\n",iCount2, rChr+1, iLine, pvSrc);)
                WAP_LOG_PRINT((E_WAP_DBG_MEM_TEST,E_WAP_DBG_LEVEL_LOW)( "WAP_MEMMOVE:-------------------------------------------------\n" );)
            }
            break;
        }
    }

    WAP_LOG_PRINT((E_WAP_DBG_MEM_TEST,E_WAP_DBG_LEVEL_LOW)( "WAP_MEMMOVE:[%d,%d]%s,%d,Dest is 0x%x,Src is 0x%x\n", \
            iCount1, iCount2, rChr+1, iLine, pvDest,pvSrc);)

    return MEMMOVE(pvDest,pvSrc,iSize);
}

void Wap_StrnCpyLc
(
    WE_CHAR *pcFileName,
    WE_INT32 iLine,
    WE_CHAR *pcStrDest,
    const WE_CHAR *pcStrSource,
    WE_INT32 iLen
)
{
    WE_CHAR     *rChr;
    WE_INT32    iCount1, iCount2;
    
    rChr = STRRCHR( pcFileName, '\\' );
    for( iCount1 = 0; iCount1 < MEM_MAX_MALLOC; iCount1++ )
    {
        if (pcStrDest== astInfo[iCount1].pvMemBlock)
        {
            if( iLen> astInfo[iCount1].iSize )
            {
                WAP_LOG_PRINT((E_WAP_DBG_MEM_TEST,E_WAP_DBG_LEVEL_LOW)( "WAP_STRNCPYLC:-------------------------------------------------\n" );)
                WAP_LOG_PRINT((E_WAP_DBG_MEM_TEST,E_WAP_DBG_LEVEL_LOW)( "WAP_STRNCPYLC:Operate memory[DEST] out-of-bourn.\n" );)
                WAP_LOG_PRINT((E_WAP_DBG_MEM_TEST,E_WAP_DBG_LEVEL_LOW)( "WAP_STRNCPYLC:[%d]%s,%d,Block is 0x%x\n",iCount1,rChr+1, iLine, pcStrDest);)
                WAP_LOG_PRINT((E_WAP_DBG_MEM_TEST,E_WAP_DBG_LEVEL_LOW)( "WAP_STRNCPYLC:-------------------------------------------------\n" );)
            }
            break;
        }
    }
    for( iCount2 = 0; iCount2 < MEM_MAX_MALLOC; iCount2++ )
    {
        if (pcStrSource== astInfo[iCount2].pvMemBlock)
        {
            if( iLen> astInfo[iCount2].iSize )
            {
                WAP_LOG_PRINT((E_WAP_DBG_MEM_TEST,E_WAP_DBG_LEVEL_LOW)( "WAP_STRNCPYLC:-------------------------------------------------\n" );)
                WAP_LOG_PRINT((E_WAP_DBG_MEM_TEST,E_WAP_DBG_LEVEL_LOW)( "WAP_STRNCPYLC:Operate memory[SRC] out-of-bourn.\n" );)
                WAP_LOG_PRINT((E_WAP_DBG_MEM_TEST,E_WAP_DBG_LEVEL_LOW)( "WAP_STRNCPYLC:[%d]%s,%d,Block is 0x%x\n",iCount2, rChr+1, iLine, pcStrSource);)
                WAP_LOG_PRINT((E_WAP_DBG_MEM_TEST,E_WAP_DBG_LEVEL_LOW)( "WAP_STRNCPYLC:-------------------------------------------------\n" );)
            }
            break;
        }
    }
    
    WAP_LOG_PRINT((E_WAP_DBG_MEM_TEST,E_WAP_DBG_LEVEL_LOW)( "WAP_STRNCPYLC:[%d,%d]%s,%d,Dest is 0x%x,Src is 0x%x\n", \
                    iCount1, iCount2, rChr+1, iLine, pcStrDest, pcStrSource);)
    WE_SCL_STRNCPYLC(pcStrDest,pcStrSource,iLen);
}

WE_INT32 Wap_PrintMemInfo( void )
{
    WE_INT32    iLoop;
    WE_INT32    iCount=0;
    WE_INT32    iWastedSize=0;
    for( iLoop = 0; iLoop < MEM_MAX_MALLOC; iLoop++ )
    {
        if( astInfo[iLoop].iSize != 0 )
        {
            WAP_LOG_PRINT((E_WAP_DBG_MEM_TEST,E_WAP_DBG_LEVEL_LOW)( "MEMINFO:[%d]%s, %d, %d, 0x%x\n", \
                iLoop, astInfo[iLoop].acFileName, astInfo[iLoop].iLine, \
                astInfo[iLoop].iSize, astInfo[iLoop].pvMemBlock );)
            iWastedSize += astInfo[iLoop].iSize;
            iCount++;
        }
    }
    DBGPRINTF("___________________________________________g_iMaxUsedMemory = %d", g_iMaxUsedMemory);
    WAP_LOG_PRINT((E_WAP_DBG_MEM_TEST,E_WAP_DBG_LEVEL_LOW)( "MEMINFO:Wasted size is %d[%d]\n", iWastedSize, iMaxCount );)
    return iCount;
}
#endif
