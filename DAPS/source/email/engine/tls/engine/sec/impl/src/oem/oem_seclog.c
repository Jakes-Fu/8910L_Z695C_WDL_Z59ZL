/*==================================================================================================
    FILE NAME : oem_seclog.c
    MODULE NAME : SEC
    
    GENERAL DESCRIPTION
        In this file, define the log function.
    
    TechFaith Software Confidential Proprietary
    Copyright (c) 2006-2007 by TechFaith Software. All Rights Reserved.
====================================================================================================
    Revision History
       
    Modification                              Tracking
    Date                  Author             Number                    Description of changes
    ----------   ------------    ---------   -------------------------------------------------------
    2006-07-07     Kevin Yang                                         create oem_seclog.c
    
==================================================================================================*/

/*==================================================================================================
*   Include File 
*=================================================================================================*/
    
#include "we_def.h"
    
#include "oem_seclog.h"

#ifdef WE_LOG_MODULE



/***************************************************************************************************
*   Function Define Section
***************************************************************************************************/

/*==================================================================================================
FUNCTION: 
    Sec_LogMsg
CREATE DATE:
    2006-07-07
AUTHOR:
    Kevin Yang
DESCRIPTION:
    print log message.
ARGUMENTS PASSED:
    iType[IN]: no use 
    ucId[IN]: no use
    pcFormat[IN]:the message to be printed.
RETURN VALUE:
    None.
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    Omit
IMPORTANT NOTES:
    None
==================================================================================================*/
void Sec_LogMsg(WE_CHAR* pcFileName, WE_UINT8 iNum, const WE_CHAR * pcFormat,...)
{
    /*modified by Bird Zhang,add filename and number to log information*/
    va_list arglist;
    if(pcFileName)
    {
        WE_CHAR caBuf[64] = {0};
        /* save file name */
        const char  *rChr = strrchr( pcFileName, '\\' );
        if( NULL == rChr )
        {
            rChr = strrchr( pcFileName, '/' );
            if(rChr == NULL)
            {
                rChr = pcFileName - 1;
            }
        }
        
        rChr ++;  
        va_start(arglist,pcFormat);
        vsnprintf(caBuf,sizeof(caBuf),pcFormat,arglist);
        printf("%s-%d:%s",rChr,iNum,caBuf);    
    }
    else
    {
        va_start(arglist,pcFormat);
        vprintf(pcFormat,arglist);
    }
    va_end(arglist);
}
/*==================================================================================================
FUNCTION: 
    Sec_LogData
CREATE DATE:
    2006-07-07
AUTHOR:
    Kevin Yang
DESCRIPTION:
    print  log message
ARGUMENTS PASSED:
    iType[IN]: no use.
    ucId[IN]: no use.
    pucData[IN]: pointer to the data message.
    iDataLen[IN]: data length.
RETURN VALUE:
    Counter number
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    Omit
IMPORTANT NOTES:
    None
==================================================================================================*/
void Sec_LogData (WE_INT32 iType, WE_UINT8 ucId, const WE_UCHAR *pucData, WE_INT32 iDataLen)
{
    WE_INT32 i = 0;
    iType = iType;
    ucId = ucId;
    /* add by sam [070308] */
    if (NULL == pucData)
    {
        printf("POINT TO NULL! Buffer length : %ld\n", iDataLen);
        return;
    }
    for(i=0;i<iDataLen;i++)
    {
        printf("%x", (*pucData));
        pucData++;
    }    
}

#endif
