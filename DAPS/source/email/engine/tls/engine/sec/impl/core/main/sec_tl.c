/*=================================================================================================
    MODULE NAME : sec_tl.c
    MODULE NAME : SEC

    GENERAL DESCRIPTION:
        This file offers four function to verify that the platform if big-endian 
        or little-endian, and then convert, store or export between two different 
        types.
        
    TechFaith Software Confidential Proprietary
    Copyright (c) 2006-2007 by TechFaith Software. All Rights Reserved.
====================================================================================================
    Revision History
       
    Modification                                 Tracking
    Date                      Author           Number                 Description of changes
    ----------   --------------   ---------   ----------------------------------------------------
    2006-07-11            Sam                                              create sec_tl.c

==================================================================================================*/

/*==================================================================================================
*   Include File 
*=================================================================================================*/
#include "sec_comm.h"

/***************************************************************************************************
*   Function Define Section
***************************************************************************************************/
/*==================================================================================================
FUNCTION: 
    Sec_StoreStrUint8to16
CREATE DATE:
    2006-07-07
AUTHOR:
    Sam
DESCRIPTION:
    Judge the platform is big endian or little endian, evaluate and get 16 bit value.
ARGUMENTS PASSED: 
    pucStr8[IN]: 8-bit pointer 
    pusAdd16[OUT]: 16-bit pointer 
RETURN VALUE:
    None
USED GLOBAL VARIABLES: 
    None
USED STATIC VARIABLES:
    None
CALL BY: 
    Omit
IMPORTANT NOTES: 
    None
==================================================================================================*/
void Sec_StoreStrUint8to16( const WE_UINT8 *pucStr8, WE_UINT16 *pusAdd16 )
{
    WE_UINT16 usTemp = 0x0102; 

    if ((NULL == pucStr8) || (NULL == pusAdd16))
    {
        return ;
    }
    
    if (0x01 == *((WE_UINT8 *)&(usTemp))) 
    {/* big endian */
        *((WE_UINT8 *)pusAdd16) = pucStr8[0];
        *(((WE_UINT8 *)pusAdd16)+1) = pucStr8[1];
    }
    else                        
    {/* little endian */
        *((WE_UINT8 *)pusAdd16) = pucStr8[1];
        *(((WE_UINT8 *)pusAdd16)+1) = pucStr8[0];
    }
}

/*==================================================================================================
FUNCTION: 
    Sec_StoreStrUint8to32
CREATE DATE:
    2006-07-07
AUTHOR:
    Sam
DESCRIPTION:
    Judge the platform is big endian or little endian, evaluate and get 32 bit value.
ARGUMENTS PASSED: 
    pucStr8[IN]: 8-bit pointer 
    puiAdd32[OUT]: 32-bit pointer 
RETURN VALUE:
    None
USED GLOBAL VARIABLES: 
    None
USED STATIC VARIABLES:
    None
CALL BY: 
    Omit
IMPORTANT NOTES: 
    None
==================================================================================================*/
void Sec_StoreStrUint8to32( const WE_UINT8 *pucStr8, WE_UINT32 *puiAdd32 )
{
    WE_UINT16 usTemp = 0x0102; 

    if ((NULL == pucStr8) || (NULL == puiAdd32))
    {
        return ;
    }
    
    if (0x01 == *((WE_UINT8 *)&(usTemp))) 
    {/* big endian */
        *((WE_UINT8 *)puiAdd32) = pucStr8[0];
        *(((WE_UINT8 *)puiAdd32)+1) = pucStr8[1];
        *(((WE_UINT8 *)puiAdd32)+2) = pucStr8[2];
        *(((WE_UINT8 *)puiAdd32)+3) = pucStr8[3];
    }
    else                        
    {/* little endian */
        *((WE_UINT8 *)puiAdd32) = pucStr8[3];
        *(((WE_UINT8 *)puiAdd32)+1) = pucStr8[2];
        *(((WE_UINT8 *)puiAdd32)+2) = pucStr8[1];
        *(((WE_UINT8 *)puiAdd32)+3) = pucStr8[0];
    }
}

/*==================================================================================================
FUNCTION: 
    Sec_ExportStrUint16to8
CREATE DATE:
    2006-07-07
AUTHOR:
    Sam
DESCRIPTION:
    Judge the platform is big endian or little endian, put the 16 bit value to 8 bit.
ARGUMENTS PASSED: 
    pusAdd16[IN] : 16-bit pointer 
    pucStr8[OUT]: 8-bit pointer 
RETURN VALUE:
    None
USED GLOBAL VARIABLES: 
    None
USED STATIC VARIABLES:
    None
CALL BY: 
    Omit
IMPORTANT NOTES: 
    None
==================================================================================================*/
void Sec_ExportStrUint16to8( WE_UINT16 *pusAdd16, WE_UINT8 *pucStr8 )
{
    WE_UINT16 usTemp = 0x0102; 

    if ((NULL == pucStr8) || (NULL == pusAdd16))
    {
        return ;
    }
    
    if (0x01 == *((WE_UINT8 *)&(usTemp))) 
    {/* big endian */
        pucStr8[0] = *((WE_UINT8 *)pusAdd16);
        pucStr8[1] = *(((WE_UINT8 *)pusAdd16)+1);
    }
    else                        
    {/* little endian */
        pucStr8[1] = *((WE_UINT8 *)pusAdd16);
        pucStr8[0] = *(((WE_UINT8 *)pusAdd16)+1);
    }
}

/*==================================================================================================
FUNCTION: 
    Sec_ExportStrUint32to8
CREATE DATE:
    2006-07-07
AUTHOR:
    Sam
DESCRIPTION:
    Judge the platform is big endian or little endian, put the 32 bit value to 8 bit.
ARGUMENTS PASSED: 
    puiAdd32[IN]: 32-bit pointer 
    pucStr8[OUT]: 8-bit pointer 
RETURN VALUE:
    None
USED GLOBAL VARIABLES: 
    None
USED STATIC VARIABLES:
    None
CALL BY: 
    Omit
IMPORTANT NOTES: 
    None
==================================================================================================*/
void Sec_ExportStrUint32to8( WE_UINT32 *puiAdd32, WE_UINT8 *pucStr8 )
{
    WE_UINT16 usTemp = 0x0102; 

    if ((NULL == pucStr8) || (NULL == puiAdd32))
    {
        return ;
    }
    
    if (0x01 == *((WE_UINT8 *)&(usTemp))) 
    {/* big endian */
        pucStr8[0] = *((WE_UINT8 *)puiAdd32);
        pucStr8[1] = *(((WE_UINT8 *)puiAdd32)+1);
        pucStr8[2] = *(((WE_UINT8 *)puiAdd32)+2);
        pucStr8[3] = *(((WE_UINT8 *)puiAdd32)+3);
    }
    else                        
    {/* little endian */
        pucStr8[3] = *((WE_UINT8 *)puiAdd32);
        pucStr8[2] = *(((WE_UINT8 *)puiAdd32)+1);
        pucStr8[1] = *(((WE_UINT8 *)puiAdd32)+2);
        pucStr8[0] = *(((WE_UINT8 *)puiAdd32)+3);
    }
}

