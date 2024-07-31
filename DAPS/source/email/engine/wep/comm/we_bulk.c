/*=====================================================================================
    FILE   NAME : we_bulk.c
    MODULE NAME : WE
    
    GENERAL DESCRIPTION
        The functions in this file manage the certificate(x509 or wtls).

    TechFaith Software Confidential Proprietary
    Copyright (c) 2006-2007 by TechFaith Software. All Rights Reserved.
=======================================================================================
    Revision History

    Modification                   Tracking
    Date              Author       Number       Description of changes
    ----------   --------------   ---------   --------------------------------------
    2006-11-03         Tang         None          Init
    2007-06-28         bluefire                 after malloc,memset it

=====================================================================================*/
//pclint
/*lint -e662 */


/*=====================================================================================
*   Include File Section
*=====================================================================================*/
#include "we_def.h"
#include "we_libalg.h"
#include "we_mem.h"
#include "we_bulk.h"

/*******************************************************************************
*   Prototype Declare Section
*******************************************************************************/

static WE_UINT8 *We_LibDesSchedule(WE_UINT8 *pucKey, 
                                   E_WeBsapiModeOp eOp,  
                                   WE_UINT8 *pucSched);

/*******************************************************************************
*   Function Define Section
*******************************************************************************/
/*=====================================================================================
FUNCTION: 
    We_LibDesF
CREATE DATE: 
    2006-12-04
AUTHOR: 
    Sam
DESCRIPTION:
    Transform operation and get round key.
ARGUMENTS PASSED:
    uiK[IN]: ;
    pucSubKey[IN]: ;
RETURN VALUE:
    
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    Omit
IMPORTANT NOTES:
=====================================================================================*/
WE_UINT32 We_LibDesF(WE_UINT32 uiK, WE_UINT8 *pucSubKey)
{
  WE_UINT32 uiRval=0;
  static const WE_UINT32 auiSp[8][64] = {
    /*sp1*/
    {0x808200, 0x0, 0x8000, 0x808202, 0x808002,
     0x8202, 0x2, 0x8000, 0x200, 0x808200, 0x808202, 0x200, 0x800202, 0x808002,
     0x800000, 0x2, 0x202, 0x800200, 0x800200, 0x8200, 0x8200, 0x808000,
     0x808000, 0x800202, 0x8002, 0x800002, 0x800002, 0x8002, 0x0, 0x202,
     0x8202, 0x800000, 0x8000, 0x808202, 0x2, 0x808000, 0x808200, 0x800000,
     0x800000, 0x200, 0x808002, 0x8000, 0x8200, 0x800002, 0x200, 0x2, 0x800202,
     0x8202, 0x808202, 0x8002, 0x808000, 0x800202, 0x800002, 0x202, 0x8202,
     0x808200, 0x202, 0x800200, 0x800200, 0x0, 0x8002, 0x8200, 0x0, 0x808002},
   /*sp2*/
    {0x40084010, 0x40004000, 0x4000, 0x84010, 0x80000, 0x10, 0x40080010,
     0x40004010, 0x40000010, 0x40084010, 0x40084000, 0x40000000, 0x40004000,
     0x80000, 0x10, 0x40080010, 0x84000, 0x80010, 0x40004010, 0x0, 0x40000000,
     0x4000, 0x84010, 0x40080000, 0x80010, 0x40000010, 0x0, 0x84000, 0x4010,
     0x40084000, 0x40080000, 0x4010, 0x0, 0x84010, 0x40080010, 0x80000,
     0x40004010, 0x40080000, 0x40084000, 0x4000, 0x40080000, 0x40004000, 0x10,
     0x40084010, 0x84010, 0x10, 0x4000, 0x40000000, 0x4010, 0x40084000,
     0x80000, 0x40000010, 0x80010, 0x40004010, 0x40000010, 0x80010, 0x84000,
     0x0, 0x40004000, 0x4010, 0x40000000, 0x40080010, 0x40084010, 0x84000},
   /*sp3*/
    {0x104, 0x4010100, 0x0, 0x4010004, 0x4000100, 0x0, 0x10104, 0x4000100,
     0x10004, 0x4000004, 0x4000004, 0x10000, 0x4010104, 0x10004, 0x4010000,
     0x104, 0x4000000, 0x4, 0x4010100, 0x100, 0x10100, 0x4010000, 0x4010004,
     0x10104, 0x4000104, 0x10100, 0x10000, 0x4000104, 0x4, 0x4010104, 0x100,
     0x4000000, 0x4010100, 0x4000000, 0x10004, 0x104, 0x10000, 0x4010100,
     0x4000100, 0x0, 0x100, 0x10004, 0x4010104, 0x4000100, 0x4000004, 0x100,
     0x0, 0x4010004, 0x4000104, 0x10000, 0x4000000, 0x4010104, 0x4, 0x10104,
     0x10100, 0x4000004, 0x4010000, 0x4000104, 0x104, 0x4010000, 0x10104, 0x4,
     0x4010004, 0x10100},
    /*sp4*/
    {0x80401000, 0x80001040, 0x80001040, 0x40, 0x401040, 0x80400040,
     0x80400000, 0x80001000, 0x0, 0x401000, 0x401000, 0x80401040, 0x80000040,
     0x0, 0x400040, 0x80400000, 0x80000000, 0x1000, 0x400000, 0x80401000, 0x40,
     0x400000, 0x80001000, 0x1040, 0x80400040, 0x80000000, 0x1040, 0x400040,
     0x1000, 0x401040, 0x80401040, 0x80000040, 0x400040, 0x80400000, 0x401000,
     0x80401040, 0x80000040, 0x0, 0x0, 0x401000, 0x1040, 0x400040, 0x80400040,
     0x80000000, 0x80401000, 0x80001040, 0x80001040, 0x40, 0x80401040,
     0x80000040, 0x80000000, 0x1000, 0x80400000, 0x80001000, 0x401040,
     0x80400040, 0x80001000, 0x1040, 0x400000, 0x80401000, 0x40, 0x400000,
     0x1000, 0x401040},
    /*sp5*/
    {0x80, 0x1040080, 0x1040000, 0x21000080, 0x40000, 0x80, 0x20000000,
     0x1040000, 0x20040080, 0x40000, 0x1000080, 0x20040080, 0x21000080,
     0x21040000, 0x40080, 0x20000000, 0x1000000, 0x20040000, 0x20040000, 0x0,
     0x20000080, 0x21040080, 0x21040080, 0x1000080, 0x21040000, 0x20000080,
     0x0, 0x21000000, 0x1040080, 0x1000000, 0x21000000, 0x40080, 0x40000,
     0x21000080, 0x80, 0x1000000, 0x20000000, 0x1040000, 0x21000080,
     0x20040080, 0x1000080, 0x20000000, 0x21040000, 0x1040080, 0x20040080,
     0x80, 0x1000000, 0x21040000, 0x21040080, 0x40080, 0x21000000, 0x21040080,
     0x1040000, 0x0, 0x20040000, 0x21000000, 0x40080, 0x1000080, 0x20000080,
     0x40000, 0x0, 0x20040000, 0x1040080, 0x20000080},
    /*sp6*/ 
    {0x10000008, 0x10200000, 0x2000, 0x10202008, 0x10200000, 0x8, 0x10202008,
     0x200000, 0x10002000, 0x202008, 0x200000, 0x10000008, 0x200008,
     0x10002000, 0x10000000, 0x2008, 0x0, 0x200008, 0x10002008, 0x2000,
     0x202000, 0x10002008, 0x8, 0x10200008, 0x10200008, 0x0, 0x202008,
     0x10202000, 0x2008, 0x202000, 0x10202000, 0x10000000, 0x10002000, 0x8,
     0x10200008, 0x202000, 0x10202008, 0x200000, 0x2008, 0x10000008, 0x200000,
     0x10002000, 0x10000000, 0x2008, 0x10000008, 0x10202008, 0x202000,
     0x10200000, 0x202008, 0x10202000, 0x0, 0x10200008, 0x8, 0x2000,
     0x10200000, 0x202008, 0x2000, 0x200008, 0x10002008, 0x0, 0x10202000,
     0x10000000, 0x200008, 0x10002008},
    /*sp7*/
    {0x100000, 0x2100001, 0x2000401, 0x0, 0x400, 0x2000401, 0x100401,
     0x2100400, 0x2100401, 0x100000, 0x0, 0x2000001, 0x1, 0x2000000, 0x2100001,
     0x401, 0x2000400, 0x100401, 0x100001, 0x2000400, 0x2000001, 0x2100000,
     0x2100400, 0x100001, 0x2100000, 0x400, 0x401, 0x2100401, 0x100400, 0x1,
     0x2000000, 0x100400, 0x2000000, 0x100400, 0x100000, 0x2000401, 0x2000401,
     0x2100001, 0x2100001, 0x1, 0x100001, 0x2000000, 0x2000400, 0x100000,
     0x2100400, 0x401, 0x100401, 0x2100400, 0x401, 0x2000001, 0x2100401,
     0x2100000, 0x100400, 0x0, 0x1, 0x2100401, 0x0, 0x100401, 0x2100000, 0x400,
     0x2000001, 0x2000400, 0x400, 0x100001},
    /*sp8*/
    {0x8000820, 0x800, 0x20000, 0x8020820, 0x8000000, 0x8000820, 0x20,
     0x8000000, 0x20020, 0x8020000, 0x8020820, 0x20800, 0x8020800, 0x20820,
     0x800, 0x20, 0x8020000, 0x8000020, 0x8000800, 0x820, 0x20800, 0x20020,
     0x8020020, 0x8020800, 0x820, 0x0, 0x0, 0x8020020, 0x8000020, 0x8000800,
     0x20820, 0x20000, 0x20820, 0x20000, 0x8020800, 0x800, 0x20, 0x8020020,
     0x800, 0x20820, 0x8000800, 0x20, 0x8000020, 0x8020000, 0x8020020,
     0x8000000, 0x20000, 0x8000820, 0x0, 0x8020820, 0x20020, 0x8000020,
     0x8020000, 0x8000800, 0x8000820, 0x0, 0x8020820, 0x20800, 0x20800, 0x820,
     0x820, 0x20020, 0x8000000, 0x8020800}
    };
  
  if(!pucSubKey)
  {
      return M_WE_LIB_ERR_INVALID_PARAMETER;
  }
  uiRval = auiSp[0][(((uiK >> 27) | (uiK << 5)) & 63) ^ *pucSubKey++];
  uiRval |= auiSp[1][((uiK >> 23) & 63) ^ *pucSubKey++];
  uiRval |= auiSp[2][((uiK >> 19) & 63) ^ *pucSubKey++];
  uiRval |= auiSp[3][((uiK >> 15) & 63) ^ *pucSubKey++];
  uiRval |= auiSp[4][((uiK >> 11) & 63) ^ *pucSubKey++];
  uiRval |= auiSp[5][((uiK >> 7) & 63) ^ *pucSubKey++];
  uiRval |= auiSp[6][((uiK >> 3) & 63) ^ *pucSubKey++];
  uiRval |= auiSp[7][(((uiK << 1) | (uiK >> 31)) & 63) ^ *pucSubKey++];

  return uiRval;
}

/*=====================================================================================
FUNCTION: 
    We_LibDesFPerm
CREATE DATE: 
    2006-12-04
AUTHOR: 
    Sam
DESCRIPTION:
    
ARGUMENTS PASSED:
    puiData[IN]: ;
RETURN VALUE:
    None
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    Omit
IMPORTANT NOTES:
=====================================================================================*/
WE_VOID We_LibDesFPerm(WE_UINT32 *puiData)
{
    WE_UINT32  uiHi = 0;
    WE_UINT32  uiLo = 0;
    WE_UINT32 uiT=0;
    if(!puiData)
    {        
        return ;        
    }

    uiHi = puiData[0];
    uiLo = puiData[1];
    
    uiT = (uiLo ^ (uiHi >> 4)) & 0xf0f0f0f; uiLo ^= uiT; uiHi ^= uiT << 4;
    uiT = (uiLo ^ (uiLo >> 18)) & 0x3333; uiLo ^= uiT + (uiT << 18);
    uiT = (uiHi ^ (uiHi >> 18)) & 0x3333; uiHi ^= uiT + (uiT << 18);
    uiT = (uiLo ^ (uiLo >> 9)) & 0x550055; uiLo ^= uiT + (uiT << 9);
    uiT = (uiHi ^ (uiHi >> 9)) & 0x550055; uiHi ^= uiT + (uiT << 9);
    uiT = (uiLo ^ (uiLo >> 6)) & 0x3030303; uiLo ^= uiT + (uiT << 6);
    uiT = (uiHi ^ (uiHi >> 6)) & 0x3030303; uiHi ^= uiT + (uiT << 6);
    uiT = (uiLo ^ (uiLo >> 3)) & 0x11111111; uiLo ^= uiT + (uiT << 3);
    uiT = (uiHi ^ (uiHi >> 3)) & 0x11111111; uiHi ^= uiT + (uiT << 3);
    
    puiData[0] = uiHi;
    puiData[1] = uiLo;
}

/*=====================================================================================
FUNCTION: 
    We_LibDesIPerm
CREATE DATE: 
    2006-12-04
AUTHOR: 
    Sam
DESCRIPTION:
    
ARGUMENTS PASSED:
    puiData[IN]: ;
RETURN VALUE:
    None
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    Omit
IMPORTANT NOTES:
=====================================================================================*/
WE_VOID We_LibDesIPerm(WE_UINT32 *puiData)
{
  WE_UINT32  uiHi = 0;
  WE_UINT32  uiLo = 0;
  WE_UINT32 uiT=0;

  if (NULL == puiData)
  {
    return;
  }
  uiHi = puiData[0];
  uiLo = puiData[1];

  uiT = (uiLo ^ (uiHi >> 1)) & 0x55555555; uiLo ^= uiT; uiHi ^= uiT << 1;
  uiT = (uiLo ^ (uiLo >> 12)) & 0xf0f0; uiLo ^= uiT + (uiT << 12);
  uiT = (uiHi ^ (uiHi >> 12)) & 0xf0f0; uiHi ^= uiT + (uiT << 12);
  uiT = (uiLo ^ (uiLo >> 6)) & 0xcc00cc; uiLo ^= uiT + (uiT << 6);
  uiT = (uiHi ^ (uiHi >> 6)) & 0xcc00cc; uiHi ^= uiT + (uiT << 6);
  uiT = (uiLo ^ (uiLo >> 3)) & 0xa0a0a0a; uiLo ^= uiT + (uiT << 3);
  uiT = (uiHi ^ (uiHi >> 3)) & 0xa0a0a0a; uiHi ^= uiT + (uiT << 3);
  uiT = (uiLo ^ (uiLo >> 3)) & 0x11111111; uiLo ^= uiT + (uiT << 3);
  uiT = (uiHi ^ (uiHi >> 3)) & 0x11111111; uiHi ^= uiT + (uiT << 3);

  puiData[0] = uiHi;
  puiData[1] = uiLo;
}

/*=====================================================================================
FUNCTION: 
    We_LibDesSchedule
CREATE DATE: 
    2006-12-04
AUTHOR: 
    Sam
DESCRIPTION:
    
ARGUMENTS PASSED:
    pucKey[IN]: data to be decrypt;
    eOp[IN]: length of data to be decrypt;
    eKeyType[IN]: decrypted data;
    pucSched[OUT]: length of decrypted data.
RETURN VALUE:
    
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    Omit
IMPORTANT NOTES:
=====================================================================================*/
static WE_UINT8 *We_LibDesSchedule
(
    WE_UINT8 *pucKey, 
    E_WeBsapiModeOp eOp, 
    WE_UINT8 *pucSched
)
{
    WE_UINT32 uiKhi=0;
    WE_UINT32 uiKlo=0;
    WE_UINT32 uiC=0;
    WE_UINT32 uiD=0;
    WE_UINT32 iIndex = 16;
    
    static WE_INT32 aiShifts[] = {1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1};
    
    CHECK_FOR_NULL_ARG(pucKey, 0);

    if (!pucSched)
    {
        pucSched = (WE_UINT8 *)WE_MALLOC(128 * sizeof(WE_UINT8));
        if (!pucSched)
        {
            return 0;
        }
    }
    
    uiKlo = (WE_UINT32) pucKey[0] + (((WE_UINT32) pucKey[1]) << 8)\
            + (((WE_UINT32) pucKey[2]) << 16)+ (((WE_UINT32) pucKey[3]) << 24);
    uiKhi = (WE_UINT32) pucKey[4] + (((WE_UINT32) pucKey[5]) << 8) 
           + (((WE_UINT32) pucKey[6]) << 16)+ (((WE_UINT32) pucKey[7]) << 24);
    
    uiD   = (uiKhi & 0xf0f0f0f) + ((uiKlo & 0xf0f0f0f) << 4);
    uiKlo = (uiKlo & 0xf0f0f0f0) + ((uiKhi >> 4) & 0xf0f0f0f);
    uiKhi = (uiD >> 24) + ((uiD >> 8) & 0xff00) + ((uiD & 0xff00) << 8) + (uiD << 24);
    uiC   = (uiKlo ^ (uiKlo >> 9)) & 0x550055; uiKlo ^= uiC + (uiC << 9);
    uiD   = (uiKhi ^ (uiKhi >> 7)) & 0xaa00aa; uiKhi ^= uiD + (uiD << 7);
    uiC   = (uiKlo ^ (uiKlo >> 18)) & 0x3333; uiKlo ^= uiC + (uiC << 18);
    uiD   = (uiKhi ^ (uiKhi >> 14)) & 0xcccc; uiKhi ^= uiD + (uiD << 14);
    uiD   = (uiKhi >> 8) + ((uiKlo >> 4) & 0xf000000);
    uiC   = uiKlo & 0xfffffff;
    
    while(iIndex--)
    {
        WE_INT32  iL = aiShifts[iIndex];
        WE_INT32  iR = 28 - iL ;
        WE_UINT8  *p = pucSched + ((eOp == BSAPI_ENCRYPT ? iIndex : 15 - iIndex) << 3);
        
        *p++ = (WE_UINT8) (((uiC >> 8) & 32) | ((uiC >> 12) & 16) | ((uiC >> 7) & 8)\
               | ((uiC >> 21) & 4) | ((uiC << 1) & 2) | ((uiC >> 4) & 1));
        *p++ = (WE_UINT8) (((uiC << 3) & 32) | ((uiC >> 23) & 16) | ((uiC >> 11) & 8)\
               | ((uiC >> 3) & 4) | ((uiC >> 19) & 2) | ((uiC >> 9) & 1));
        *p++ = (WE_UINT8) (((uiC >> 17) & 32) | ((uiC >> 14) & 16) | ((uiC >> 8) & 8)\
               | ((uiC >> 1) & 4) | ((uiC >> 24) & 2) | ((uiC >> 7) & 1));
        *p++ = (WE_UINT8) (((uiC >> 10) & 32) | ((uiC >> 2) & 16) | ((uiC >> 23) & 8)\
               | ((uiC >> 17) & 4) | ((uiC >> 11) & 2) | ((uiC >> 1) & 1));
        *p++ = (WE_UINT8) (((uiD >> 7) & 32) | ((uiD >> 19) & 16) | ((uiD << 1) & 8)\
               | ((uiD >> 6) & 4) | ((uiD >> 17) & 2) | ((uiD >> 26) & 1));
        *p++ = (WE_UINT8) (((uiD << 4) & 32) | ((uiD >> 7) & 16) | ((uiD >> 19) & 8)\
               | ((uiD >> 14) & 4) | ((uiD >> 3) & 2) | ((uiD >> 19) & 1));
        *p++ = (WE_UINT8) (((uiD >> 10) & 32) | ((uiD >> 16) & 16) | ((uiD >> 7) & 8)\
               | ((uiD >> 25) & 4) | ((uiD >> 4) & 2) | ((uiD >> 24) & 1));
        *p   = (WE_UINT8) (((uiD >> 12) & 32) | ((uiD >> 9) & 16) | ((uiD >> 18) & 8)\
               | ((uiD >> 5) & 4) | ((uiD << 1) & 2) | ((uiD >> 3) & 1));
        uiC = (uiC << iL ) | (uiC >> iR);
        uiD = (uiD << iL ) | (uiD >> iR);
    }
    
    return pucSched;
}

/*=====================================================================================
FUNCTION: 
    We_LibDesDes1
CREATE DATE: 
    2006-12-04
AUTHOR: 
    Sam
DESCRIPTION:
    
ARGUMENTS PASSED:
    puiData[IN]: data to be decrypt;
    pavKs[IN]: length of data to be decrypt;
RETURN VALUE:
    None
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    Omit
IMPORTANT NOTES:
=====================================================================================*/
WE_VOID We_LibDesDes1(WE_UINT32 *puiData, WE_VOID *pavKs[])
{
    WE_UINT8  *pucPtr=NULL;
    WE_UINT8  *pucEnd=NULL;
    WE_UINT32  uiH = 0;
    WE_UINT32  uiL = 0;

    if (NULL == puiData||NULL == pavKs)
    {
        return;
    }
    uiH = puiData[1];
    uiL = puiData[0];
    
    /*round=8*2*/
    for(pucPtr = pavKs[0], pucEnd = pucPtr + 128; pucPtr < pucEnd; pucPtr += 16)
    {
        uiL ^= We_LibDesF(uiH, pucPtr),uiH ^= We_LibDesF(uiL, pucPtr + 8);
    }
    
    puiData[0] = uiH;
    puiData[1] = uiL;
}

/*=====================================================================================
FUNCTION: 
    We_LibDesDes1P
CREATE DATE: 
    2006-12-04
AUTHOR: 
    Sam
DESCRIPTION:
    
ARGUMENTS PASSED:
    puiData[IN]: data to be decrypt;
    pavKs[IN]: length of data to be decrypt;
RETURN VALUE:
    None
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    Omit
IMPORTANT NOTES:
=====================================================================================*/
WE_VOID We_LibDesDes1P(WE_UINT32 *puiData, WE_VOID *pavKs[])
{
    We_LibDesIPerm(puiData);
    We_LibDesDes1(puiData, pavKs);
    We_LibDesFPerm(puiData);
}

/*=====================================================================================
FUNCTION: 
    We_LibDesDes3
CREATE DATE: 
    2006-12-04
AUTHOR: 
    Sam
DESCRIPTION:
    
ARGUMENTS PASSED:
    puiData[IN]: data to be decrypt;
    pavKs[IN]: length of data to be decrypt;
RETURN VALUE:
    None
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    Omit
IMPORTANT NOTES:
=====================================================================================*/
WE_VOID We_LibDesDes3(WE_UINT32 *puiData, WE_VOID *pavKs[])
{
    We_LibBlock3(We_LibDesDes1, puiData, pavKs);
}

/*=====================================================================================
FUNCTION: 
    We_LibDesDes3P
CREATE DATE: 
    2006-12-04
AUTHOR: 
    Sam
DESCRIPTION:
    
ARGUMENTS PASSED:
    puiData[IN]: data to be decrypt;
    pavKs[IN]: length of data to be decrypt;
RETURN VALUE:
    None
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    Omit
IMPORTANT NOTES:
=====================================================================================*/
WE_VOID We_LibDesDes3P(WE_UINT32 *puiData, WE_VOID *pavKs[])
{
    We_LibDesIPerm(puiData);
    We_LibDesDes3(puiData, pavKs);
    We_LibDesFPerm(puiData);
}

/*=====================================================================================
FUNCTION: 
    We_LibStartDesCore
CREATE DATE: 
    2006-12-04
AUTHOR: 
    Sam
DESCRIPTION:
    Start des encrypt.
ARGUMENTS PASSED:
    pstMode[IN]: mode type;
    pstKeys[IN]: key parameter;
    pHandle[OUT]: data handle;
RETURN VALUE:
    M_WE_LIB_ERR_OK if success,others error code.
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    Omit
IMPORTANT NOTES:
=====================================================================================*/
WE_INT32 We_LibStartDesCore
(
    St_WeLibBsapiMode *pstMode, 
    St_WeCipherCrptKey *pstKey, 
    WE_HANDLE *phandle
)
{
    St_WeBlockMAtchS     *pstNew=NULL;
    WE_UINT8 aucKeyTmp[KEYSIZE] = {0};
    
    CHECK_FOR_NULL_ARG(pstMode, 0x08);
    CHECK_FOR_NULL_ARG(pstKey, 0x08);
    CHECK_FOR_NULL_ARG(pstKey->pucKey, 0x08);
    CHECK_FOR_NULL_ARG(pstKey->pucIv, 0x08);    
    CHECK_FOR_NULL_ARG(phandle, 0x08);
    
    pstNew = WE_MALLOC(sizeof(St_WeBlockMAtchS));
    if (!pstNew)
    {
        return M_WE_LIB_ERR_INSUFFICIENT_MEMORY;
    }
    (WE_VOID)WE_MEMSET(pstNew, 0, sizeof(St_WeBlockMAtchS));
    pstNew->eOp = pstMode->eOp;    
    
    /* key1 */
    (WE_VOID)WE_MEMCPY(aucKeyTmp, pstKey->pucKey, KEYSIZE);
    pstNew->pvRoundKey[0] = We_LibDesSchedule(aucKeyTmp, pstNew->eOp,0);
    if(! pstNew->pvRoundKey[0])
    {       
        WE_LIB_FREE(pstNew);
        return M_WE_LIB_ERR_GENERAL_ERROR;
    }
    
    /*3des*/    
    if(pstMode->eTriple == BSAPI_TRIPLE && pstKey->iKeyLen!=KEYSIZE) 
    {
        E_WeBsapiModeOp eOp2;
        eOp2 =  (pstMode->eOp == BSAPI_ENCRYPT ? BSAPI_DECRYPT : BSAPI_ENCRYPT);
        /*key1 */
        (WE_VOID)WE_MEMCPY(aucKeyTmp, pstKey->pucKey + KEYSIZE, KEYSIZE);
        pstNew->pvRoundKey[1] = We_LibDesSchedule(aucKeyTmp, eOp2, 0);
        if(!pstNew->pvRoundKey[1])
        {            
            WE_LIB_FREEIF(pstNew->pvRoundKey[0]);
            WE_LIB_FREE(pstNew);
            return M_WE_LIB_ERR_GENERAL_ERROR;
        }
        
        /*key2 */
        if(pstKey->iKeyLen == 16)/*k1=k3*/
        {
            pstNew->pvRoundKey[2] = pstNew->pvRoundKey[0];
        }
        else if (pstKey->iKeyLen == 24)
        {
            (WE_VOID)WE_MEMCPY(aucKeyTmp, pstKey->pucKey + 2*KEYSIZE, KEYSIZE);
            pstNew->pvRoundKey[2] = We_LibDesSchedule(aucKeyTmp, pstMode->eOp, 0);
            if(!pstNew->pvRoundKey[2])
            {                
                WE_LIB_FREE(pstNew->pvRoundKey[1]);
                WE_LIB_FREE(pstNew->pvRoundKey[0]);
                WE_LIB_FREE(pstNew);
                return M_WE_LIB_ERR_GENERAL_ERROR;
            }            
            if(pstMode->eOp == BSAPI_DECRYPT) 
            {
                WE_UINT8 *tmp = pstNew->pvRoundKey[0];
                pstNew->pvRoundKey[0] = pstNew->pvRoundKey[2];
                pstNew->pvRoundKey[2] = tmp;
            }
        }
        else
        {
            WE_LIB_FREE(pstNew->pvRoundKey[1]);
            WE_LIB_FREE(pstNew->pvRoundKey[0]);
            WE_LIB_FREE(pstNew);
            return M_WE_LIB_ERR_KEY_LENGTH;
        }
        pstNew->uiType= (WE_UINT32)E_WE_ALG_CIPHER_3DES_CBC;
    }
    /*des*/
    else
    {
        pstNew->pvRoundKey[1] = pstNew->pvRoundKey[2] = 0;
        pstNew->uiType = (WE_UINT32)E_WE_ALG_CIPHER_DES_CBC;
    }
    if (pstKey->pucIv)
    {
        WE_UINT8 *iv = pstKey->pucIv;                    
        pstNew->iv[0][0] = (WE_UINT32) iv[3] + (((WE_UINT32) iv[2]) << 8)+ (((WE_UINT32) iv[1]) << 16) + (((WE_UINT32) iv[0]) << 24);
        pstNew->iv[0][1] = (WE_UINT32) iv[7] + (((WE_UINT32) iv[6]) << 8)+ (((WE_UINT32) iv[5]) << 16) + (((WE_UINT32) iv[4]) << 24);
    }    
    
    pstNew->fn = We_LibBlockCBC;   
    
    if(pstMode->eTriple == BSAPI_TRIPLE)
    {
        pstNew->corefn =We_LibDesDes3P;
    }
    else
    {
        pstNew->corefn =We_LibDesDes1P;
    }   
    
    pstNew->iBufLen = 0;
    (WE_VOID)WE_MEMSET(pstNew->aucData8, 0, sizeof(pstNew->aucData8));       
    *phandle=(WE_VOID *)pstNew;
    
    return M_WE_LIB_ERR_OK;    
}


/*=====================================================================================
FUNCTION: 
    We_LibAppendDes
CREATE DATE: 
    2006-12-04
AUTHOR: 
    Sam
DESCRIPTION:
    
ARGUMENTS PASSED:
    pucData[IN]: data to be decrypt;
    uiLength[IN]: length of data to be decrypt;
    pucOut[OUT]: decrypted data;
    pHandle[IN]: length of decrypted data.
RETURN VALUE:
    M_WE_LIB_ERR_OK if success,others error code.
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    Omit
IMPORTANT NOTES:
=====================================================================================*/
WE_INT32 We_LibAppendDes
(
    WE_UINT8 *pucData,
    WE_UINT32 uiLength, 
    WE_UCHAR* pucOut, 
    WE_VOID * pHandle
)
{
    return We_LibAppendBlock(pucData, uiLength,pucOut, pHandle);
}

/*=====================================================================================
FUNCTION: 
    We_LibKillDes
CREATE DATE: 
    2006-12-04
AUTHOR: 
    Sam
DESCRIPTION:
    Free memory.
ARGUMENTS PASSED:
    hDESHandle[IN]: data handle.
RETURN VALUE:
    None
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    Omit
IMPORTANT NOTES:
=====================================================================================*/
WE_VOID We_LibKillDes(WE_HANDLE hDESHandle)
{
    St_WeBlockMAtchS *pstMach = NULL;

    if (!hDESHandle)
    {
        return;
    }
    pstMach = (St_WeBlockMAtchS *)hDESHandle;

    if (pstMach->pvRoundKey[2] && pstMach->pvRoundKey[2] != pstMach->pvRoundKey[0])
    {
        WE_LIB_FREE(pstMach->pvRoundKey[2]);
    }
    if (pstMach->pvRoundKey[1])
    {
        WE_LIB_FREE(pstMach->pvRoundKey[1]);
    }
    WE_LIB_FREEIF(pstMach->pvRoundKey[0]);
    WE_LIB_FREE(pstMach);
}


/*=====================================================================================
FUNCTION: 
    We_LibAppendBlock
CREATE DATE: 
    2006-12-04
AUTHOR: 
    Sam
DESCRIPTION:
    
ARGUMENTS PASSED:
    pucIn[IN]: data to be decrypt;
    uiLength[IN]: length of data to be decrypt;
    pucOut[OUT]: decrypted data;
    handle[IN]: length of decrypted data.
RETURN VALUE:
    M_WE_LIB_ERR_OK if success,others error code.
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    Omit
IMPORTANT NOTES:
=====================================================================================*/
/* from block.c */
WE_INT32 We_LibAppendBlock
(
    WE_UINT8 *pucIn, 
    WE_UINT32 uiLength, 
    WE_UCHAR* pucOut, 
    WE_VOID * handle
)
{
    WE_INT32 iResult=0;
    St_WeBlockMAtchS *pstM =NULL;   
    if(!pucIn||!uiLength||!pucOut||!handle)
    {
        return M_WE_LIB_ERR_INVALID_PARAMETER;
    }

    pstM=(St_WeBlockMAtchS *) handle; 
    We_LibBlockCBC(pstM, uiLength,pucIn,pucOut);
    return iResult;
}

/*=====================================================================================
FUNCTION: 
    We_LibBlockCBC
CREATE DATE: 
    2006-12-04
AUTHOR: 
    Sam
DESCRIPTION:
    
ARGUMENTS PASSED:
    pstM[IN]: data to be decrypt;
    uiLen[IN]: length of data to be decrypt;
    pucIn[OUT]: length of decrypted data.
    pucOut[OUT]: decrypted data;
RETURN VALUE:
    None
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    Omit
IMPORTANT NOTES:
=====================================================================================*/
WE_VOID We_LibBlockCBC
(
    St_WeBlockMAtchS * pstM, 
    WE_UINT32 uiLen, 
    WE_UINT8 *pucIn, 
    WE_UINT8 *pucOut
)
{
    
    WE_INT32      iEncryting =0; 
    WE_UINT32     uiTemLen = 0;

    if(!pstM||!uiLen||!pucIn||!pucOut)
    {
        return ;
    }

    iEncryting = pstM->eOp == BSAPI_ENCRYPT;
    uiTemLen = uiLen;
    
    while(uiTemLen >= 8) 
    {
        WE_UINT32 auiData[2]={0};
        WE_UINT32 auiTmp[2]={0};
        auiData[0] = ((((((WE_UINT32) pucIn[0] << 8) + pucIn[1]) << 8) + pucIn[2]) << 8) + pucIn[3];
        auiData[1] = ((((((WE_UINT32) pucIn[4] << 8) + pucIn[5]) << 8) + pucIn[6]) << 8) + pucIn[7];
        if(iEncryting)
        {
            auiData[0] ^= pstM->iv[0][0], auiData[1] ^= pstM->iv[0][1];
        }
        else
        {
            auiTmp[0] = pstM->iv[0][0], auiTmp[1] = pstM->iv[0][1];
            pstM->iv[0][0] = auiData[0], pstM->iv[0][1] = auiData[1];
        }
        pucIn += 8;
        pstM->corefn(auiData, pstM->pvRoundKey + 0);
        if(iEncryting)
        {
            pstM->iv[0][0] = auiData[0], pstM->iv[0][1] = auiData[1];
        }
        else
        {
            auiData[0] ^= auiTmp[0], auiData[1] ^= auiTmp[1];
        }
        pucOut[0] = (WE_UINT8) (auiData[0] >> 24);
        pucOut[1] = (WE_UINT8) (auiData[0] >> 16);
        pucOut[2] = (WE_UINT8) (auiData[0] >> 8);
        pucOut[3] = (WE_UINT8) auiData[0];
        pucOut[4] = (WE_UINT8) (auiData[1] >> 24);
        pucOut[5] = (WE_UINT8) (auiData[1] >> 16);
        pucOut[6] = (WE_UINT8) (auiData[1] >> 8);
        pucOut[7] = (WE_UINT8) auiData[1];
        pucOut += 8;
        uiTemLen -= 8;
    }
    iEncryting ^= 1;
    pucIn = (pucOut -= uiLen - uiTemLen);
    
}

/*=====================================================================================
FUNCTION: 
    We_LibBlock3
CREATE DATE: 
    2006-12-04
AUTHOR: 
    Sam
DESCRIPTION:
    
ARGUMENTS PASSED:
    core[IN]: function pointer;
    puiData[IN]: length of data to be decrypt;
    pavKs[IN]: decrypted data;
RETURN VALUE:
    None
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    Omit
IMPORTANT NOTES:
=====================================================================================*/
WE_VOID We_LibBlock3(dcfn core, WE_UINT32 *puiData, WE_VOID *pavKs[])
{
    if(!core||!puiData||!pavKs)
    {
        return ;
    }
    core(puiData, pavKs++);
    core(puiData, pavKs++);
    core(puiData, pavKs);
}

/***************************************************************************************************
*   DES Encrypt                                                                    
***************************************************************************************************/
/*=====================================================================================
FUNCTION: 
    We_LibDesEncrypt
CREATE DATE: 
    2006-12-04
AUTHOR: 
    Sam
DESCRIPTION:
    Set sub encrypt key and encrypt data with DES algorithm.
ARGUMENTS PASSED:
    iRounds[IN]: total rounds.
    stKey[IN]: cipher key
    pucIn[IN]: data to be encrypted;
    uiInLen[IN]: length of data to be encrypted;
    pucOut[OUT]: encrypted data;
    puiOutLen[OUT]: length of encrypted data;
RETURN VALUE:
    M_WE_LIB_ERR_OK if success,others error code.
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    Omit
IMPORTANT NOTES:
    no padding: the input data had been padded, so the length of 
                input data is multiples of block size.Padding is 
                stripped in the extern module.
=====================================================================================*/
/* No Padding */
WE_INT32 We_LibDesEncrypt
(
    St_WeCipherCrptKey stKey, 
    const WE_UCHAR* pucIn, 
    WE_UINT32 uiInLen, 
    WE_UCHAR* pucOut, 
    WE_UINT32* puiOutLen
)
{
    WE_HANDLE handle=NULL; 
    WE_INT32 iResult=0;
    WE_UINT32 uiLen=0;   
    St_WeLibBsapiMode stMd = {BSAPI_DECRYPT, BSAPI_CBC, BSAPI_SINGLE, BSAPI_KEY};

    CHECK_FOR_NULL_ARG(pucIn, 0x08);
    CHECK_FOR_NULL_ARG(pucOut, 0x08);
    CHECK_FOR_NULL_ARG(puiOutLen, 0x08);
    if (!stKey.pucIv||!stKey.pucKey||stKey.iIvLen!= BLOCKSIZE
        ||stKey.iKeyLen!= KEYSIZE)
    {
        return M_WE_LIB_ERR_INVALID_KEY;
    }
    stMd.eOp = BSAPI_ENCRYPT;
    
    iResult= (WE_INT32)We_LibStartDesCore(&stMd,&stKey,&handle); 
    if(iResult!=M_WE_LIB_ERR_OK)
    {
        return M_WE_LIB_ERR_INSUFFICIENT_MEMORY;
    }
    
    uiLen = uiInLen % 8;
    if(uiLen && (*puiOutLen >= ((1 + uiInLen / 8) * 8)))
    {
        *puiOutLen = ((1 + uiInLen / 8) * 8);
    }
    else if(!uiLen && (*puiOutLen >= uiInLen))
    {
        *puiOutLen = uiInLen;
    }
    else
    {
        return M_WE_LIB_ERR_BUFFER_TOO_SMALL;
    }
    if(uiInLen-uiLen>0)
    {
        iResult = We_LibAppendDes((WE_UCHAR*)pucIn, uiInLen-uiLen,pucOut,handle);
    }
    if(iResult!=0)
    {
        return iResult;
    }
    if(uiLen)
    {
        WE_UCHAR aucData[8] = {0};
        (WE_VOID)WE_MEMSET(aucData,0,8);
        (WE_VOID)WE_MEMCPY(aucData,(pucIn + uiInLen - uiLen),uiLen);   
        if(uiInLen==uiLen)
        {
            iResult=We_LibAppendDes((WE_UCHAR*)aucData, 8,pucOut,handle);
        }
        else
        {
            iResult=We_LibAppendDes((WE_UCHAR*)aucData, 8,(pucOut+ uiInLen - uiLen),handle);
        }
    }    
    We_LibKillDes(handle);
    return M_WE_LIB_ERR_OK;
}


/*=====================================================================================
FUNCTION: 
    We_LibDesEncryptInit
CREATE DATE: 
    2006-12-04
AUTHOR: 
    Sam
DESCRIPTION:
    Encrypt init with DES algorithm.
ARGUMENTS PASSED:
    stKey[IO]: cipher key
    pHandle[OUT]: data handle;
RETURN VALUE:
    M_WE_LIB_ERR_OK if success,others error code.
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    Omit
IMPORTANT NOTES:
    no padding: the input data had been padded, so the length of 
                input data is multiples of block size.Padding is 
                stripped in the extern module.
=====================================================================================*/
WE_INT32 We_LibDesEncryptInit
(
    St_WeCipherCrptKey stKey,
    WE_HANDLE* pHandle
)
{
    WE_INT32 iResult=0;
    St_WeLibBsapiMode stMd = {BSAPI_DECRYPT, BSAPI_CBC, BSAPI_SINGLE, BSAPI_KEY};
    
    if(NULL==pHandle)
    {
        return M_WE_LIB_ERR_INVALID_PARAMETER;
    }
    stMd.eOp = BSAPI_ENCRYPT;
    iResult= (WE_INT32)We_LibStartDesCore(&stMd,&stKey,pHandle);

    return iResult;
}


/*=====================================================================================
FUNCTION: 
    We_LibDesEnCryptUpdate
CREATE DATE: 
    2006-12-04
AUTHOR: 
    Sam
DESCRIPTION:
    Encrypt data with DES algorithm.
ARGUMENTS PASSED:
    pucIn[IN]: data part to be encrypted;
    uiInLen[IN]: length of data part;
    pucOut[OUT]: encrypted data;
    puiOutLen[OUT]: length of encrypted data;
    handle[IO]: data handle.
RETURN VALUE:
    M_WE_LIB_ERR_OK if success,others error code.
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    Omit
IMPORTANT NOTES:
    no padding: the input data had been padded, so the length of 
                input data is multiples of block size.Padding is 
                stripped in the extern module.
=====================================================================================*/
WE_INT32 We_LibDesEnCryptUpdate
(
    const WE_UCHAR* pucIn, 
    WE_UINT32 uiInLen, 
    WE_UCHAR* pucOut, 
    WE_UINT32* puiOutLen, 
    WE_HANDLE handle
)
{
    WE_INT32 iResult = M_WE_LIB_ERR_OK;
    WE_UINT32 uiLen = 0; 
    if((NULL==handle)||(NULL==pucIn)||(0==uiInLen)||!puiOutLen)
    {
        return M_WE_LIB_ERR_INVALID_PARAMETER;
    }
    
    uiLen = uiInLen % 8;
    if(*puiOutLen >= ((1 + uiInLen / 8) * 8))
    {
        *puiOutLen = ((1 + uiInLen / 8) * 8);
    }
    else
    {
        return M_WE_LIB_ERR_BUFFER_TOO_SMALL;
    }    
    if(0 == uiLen)
    {        
        iResult=We_LibAppendDes((WE_UCHAR *)pucIn, uiInLen,pucOut, handle);  
    }
    else 
    {
        return M_WE_LIB_ERR_GENERAL_ERROR;
    }
    
    return iResult;    
}


/*=====================================================================================
FUNCTION: 
    We_LibDesEnCryptFinal
CREATE DATE: 
    2006-12-04
AUTHOR: 
    Sam
DESCRIPTION:
    Free the data handle.
ARGUMENTS PASSED:
    handle[IN]: data handle.
RETURN VALUE:
    M_WE_LIB_ERR_OK if success,others error code.
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    Omit
IMPORTANT NOTES:
    no padding: the input data had been padded, so the length of 
                input data is multiples of block size.Padding is 
                stripped in the extern module.
=====================================================================================*/
WE_INT32 We_LibDesEnCryptFinal(WE_HANDLE handle) 
{
    WE_INT32 iResult=0;    
    if(NULL==handle)
    {
        return M_WE_LIB_ERR_INVALID_PARAMETER;
    }
    We_LibKillDes(handle);
    
    return iResult;    
}


/*=====================================================================================
FUNCTION: 
    We_LibDesEncrypt1
CREATE DATE: 
    2006-12-04
AUTHOR: 
    Sam
DESCRIPTION:
    Set sub encrypt key and padding data, encrypt data with DES algorithm.
ARGUMENTS PASSED:
    stKey[IN]: cipher key
    pucIn[IN]: data to be encrypted;
    uiInLen[IN]: length of data to be encrypted;
    pucOut[OUT]: encrypted data;
    puiOutLen[OUT]: length of encrypted data;
RETURN VALUE:
    M_WE_LIB_ERR_OK if success,others error code.
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    Omit
IMPORTANT NOTES:
    rfc2630 padding: pad interior, and pad each byte with # bytes 
                     padded(# is equal to the length of padding, 
                     at least 1 byte).Padding is stripped on decrypt.
=====================================================================================*/
/* RFC2630 Padding */
WE_INT32 We_LibDesEncrypt1
(
    St_WeCipherCrptKey stKey, 
    const WE_UCHAR* pucIn, 
    WE_UINT32 uiInLen, 
    WE_UCHAR* pucOut, 
    WE_UINT32* puiOutLen
)
{
    WE_HANDLE handle=NULL;
    WE_INT32 iResult=0;
    WE_UINT32 uiLen=0;
    St_WeLibBsapiMode stMd = {BSAPI_DECRYPT, BSAPI_CBC, BSAPI_SINGLE, BSAPI_KEY};
    
    CHECK_FOR_NULL_ARG(pucIn, 0x08);
    CHECK_FOR_NULL_ARG(pucOut, 0x08);
    CHECK_FOR_NULL_ARG(puiOutLen, 0x08);
    
    stMd.eOp = BSAPI_ENCRYPT;
    iResult= We_LibStartDesCore(&stMd,&stKey,&handle);
    
    if(iResult!=M_WE_LIB_ERR_OK)
    {
        return M_WE_LIB_ERR_INSUFFICIENT_MEMORY;
    }
    
    uiLen = uiInLen % 8;
    if(uiLen && (*puiOutLen >= ((1 + uiInLen / 8) * 8)))
    {
        *puiOutLen = ((1 + uiInLen / 8) * 8);
    }
    else if(!uiLen && (*puiOutLen >= uiInLen))
    {
        *puiOutLen = uiInLen;
    }
    else
    {
        return M_WE_LIB_ERR_BUFFER_TOO_SMALL;
    }
    if(uiInLen-uiLen>0)
    {
        iResult=We_LibAppendDes((WE_UCHAR*)pucIn, uiInLen-uiLen,pucOut,handle);
    }
    if(iResult!=0)
    {
        return iResult;
    }
    if(uiLen)
    {
        WE_UCHAR aucData[8] = {0};
        (WE_VOID)WE_MEMSET(aucData,8-(WE_INT32)uiLen,8);
        (WE_VOID)WE_MEMCPY(aucData,(pucIn + uiInLen - uiLen),uiLen);   
        if(uiInLen==uiLen)
        {
            iResult=We_LibAppendDes((WE_UCHAR*)aucData, 8,pucOut,handle);
        }
        else
        {
            iResult=We_LibAppendDes((WE_UCHAR*)aucData, 8,(pucOut+ uiInLen - uiLen),handle);
        }
    }    
    We_LibKillDes(handle);
    return M_WE_LIB_ERR_OK;
    
}


/*=====================================================================================
FUNCTION: 
    We_LibDesEncryptInit1
CREATE DATE: 
    2006-12-04
AUTHOR: 
    Sam
DESCRIPTION:
    Encrypt init with DES algorithm.
ARGUMENTS PASSED:
    iRounds[IN]: total rounds.
    stKey[IN]: cipher key;
    pHandle[IN]: data handle.
RETURN VALUE:
    M_WE_LIB_ERR_OK if success,others error code.
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    Omit
IMPORTANT NOTES:
    rfc2630 padding: pad interior, and pad each byte with # bytes 
                     padded(# is equal to the length of padding, 
                     at least 1 byte).Padding is stripped on decrypt.
=====================================================================================*/
WE_INT32 We_LibDesEncryptInit1
(
    St_WeCipherCrptKey stKey,
    WE_HANDLE* pHandle
)
{
    WE_INT32 iResult=0;
    St_WeLibBsapiMode stMd = {BSAPI_DECRYPT, BSAPI_CBC, BSAPI_SINGLE, BSAPI_KEY};

    if(NULL==pHandle)
    {
        return M_WE_LIB_ERR_INVALID_PARAMETER;
    }
    stMd.eOp = BSAPI_ENCRYPT;
    iResult= We_LibStartDesCore(&stMd,&stKey,pHandle); 
    
    return iResult;
}


/*=====================================================================================
FUNCTION: 
    We_LibDesEnCryptUpdate1
CREATE DATE: 
    2006-12-04
AUTHOR: 
    Sam
DESCRIPTION:
    Set padding data and encrypt data with DES algorithm.
ARGUMENTS PASSED:
    pucIn[IN]: data part to be encrypted;
    uiInLen[IN]: length of data part;
    pucOut[OUT]: encrypted data;
    puiOutLen[OUT]: length of encrypted data;
    handle[IO]: data handle.
RETURN VALUE:
    M_WE_LIB_ERR_OK if success,others error code.
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    Omit
IMPORTANT NOTES:
    rfc2630 padding: pad interior, and pad each byte with # bytes 
                     padded(# is equal to the length of padding, 
                     at least 1 byte).Padding is stripped on decrypt.
=====================================================================================*/
WE_INT32 We_LibDesEnCryptUpdate1
(
    const WE_UCHAR* pucIn, 
    WE_UINT32 uiInLen, 
    WE_UCHAR* pucOut, 
    WE_UINT32* puiOutLen, 
    WE_HANDLE handle
)
{
    WE_INT32 iResult = M_WE_LIB_ERR_OK; 
    St_WeBlockMAtchS *pstPara=NULL;
    WE_UINT32 iIndex=0;
    WE_INT32 iBufLen=0;
    if((NULL==handle)||(NULL==pucIn)||(0==uiInLen)||!pucOut||!puiOutLen)
    {
        return M_WE_LIB_ERR_INVALID_PARAMETER;
    }  
    
    iBufLen = (uiInLen % 8);
    if(iBufLen && (*puiOutLen >= ((1 + uiInLen / 8) * 8)))
    {
        *puiOutLen = ((1 + uiInLen / 8) * 8);
    }
    else if(!iBufLen && (*puiOutLen >= uiInLen))
    {
        *puiOutLen = uiInLen;
    }
    else
    {
        return M_WE_LIB_ERR_BUFFER_TOO_SMALL;
    }
    
    pstPara=(St_WeBlockMAtchS *)handle;
    
    iIndex = pstPara->iBufLen;
    if((uiInLen + (WE_UINT32)iIndex) == *puiOutLen)
    {
        pstPara->iBufLen = 0;
        (WE_VOID)WE_MEMCPY((pstPara->aucData8 + iIndex), pucIn, (WE_UINT32)(8 - iIndex));
        iResult=We_LibAppendDes((WE_UCHAR *)(pstPara->aucData8), 8,pucOut, handle);
        if(8 != *puiOutLen)
        {
            iResult=We_LibAppendDes((WE_VOID *)(pucIn + (8 - iIndex)), \
                (uiInLen - (WE_UINT32)(8 - iIndex) - (WE_UINT32)(pstPara->iBufLen)),pucOut+8, handle);
        }
        
    }
    else if ((uiInLen + (WE_UINT32)iIndex) < *puiOutLen) 
    {
        *puiOutLen -= 8 ; 
        if((uiInLen + (WE_UINT32)iIndex) > 8)
        {
            pstPara->iBufLen = (iIndex + uiInLen) % 8;
            (WE_VOID)WE_MEMCPY((pstPara->aucData8 + iIndex), pucIn, (WE_UINT32)(8 - iIndex));
            iResult=We_LibAppendDes((WE_UCHAR *)(pstPara->aucData8), 8,pucOut, handle);
            if((uiInLen - (WE_UINT32)(8 - iIndex) - (WE_UINT32)(pstPara->iBufLen)))
            {
                iResult=We_LibAppendDes((WE_VOID *)(pucIn + (8 - iIndex)), \
                    (uiInLen - (WE_UINT32)(8 - iIndex) - (WE_UINT32)(pstPara->iBufLen)),pucOut+8, handle);
            }
            iIndex = uiInLen - pstPara->iBufLen;
            (WE_VOID)WE_MEMCPY(pstPara->aucData8, pucIn + iIndex, (WE_UINT32)(pstPara->iBufLen));
        }
        else
        {
            (WE_VOID)WE_MEMCPY(pstPara->aucData8 + iIndex, pucIn, uiInLen);
            pstPara->iBufLen = uiInLen;
        }
    }
    else
    {
        pstPara->iBufLen = (iIndex + uiInLen) - *puiOutLen;
        (WE_VOID)WE_MEMCPY((pstPara->aucData8 + iIndex), pucIn, (WE_UINT32)(8 - iIndex));
        iResult=We_LibAppendDes( pstPara->aucData8 ,8, pucOut, handle);
        if((uiInLen - (8 - (WE_UINT32)iIndex) - (WE_UINT32)(pstPara->iBufLen)))
        {
            iResult=We_LibAppendDes((WE_VOID*)(pucIn + (8 - iIndex)), \
                (uiInLen - (8 - (WE_UINT32)iIndex) - (WE_UINT32)(pstPara->iBufLen)),pucOut + 8, handle);
        }
        iIndex = uiInLen - pstPara->iBufLen;
        (WE_VOID)WE_MEMCPY(pstPara->aucData8, pucIn + iIndex, (WE_UINT32)(pstPara->iBufLen));
    }
    return iResult;
}   

/*=====================================================================================
FUNCTION: 
    We_LibDesEnCryptFinal1
CREATE DATE: 
    2006-12-04
AUTHOR: 
    Sam
DESCRIPTION:
    Get encrypted data and free memory.
ARGUMENTS PASSED:
    pucOut[OUT]: encrypted data;
    puiOutLen[OUT]: length of encrypted data;
    handle[IN]: data handle.
RETURN VALUE:
    M_WE_LIB_ERR_OK if success,others error code.
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    Omit
IMPORTANT NOTES:
    rfc2630 padding: pad interior, and pad each byte with # bytes 
                     padded(# is equal to the length of padding, 
                     at least 1 byte).Padding is stripped on decrypt.
=====================================================================================*/
WE_INT32 We_LibDesEnCryptFinal1
(
    WE_UCHAR* pucOut,
    WE_UINT32* puiOutLen, 
    WE_HANDLE handle
)
{
    WE_INT32 iResult = M_WE_LIB_ERR_OK;
    St_WeBlockMAtchS *pstPara = NULL;
    WE_UCHAR aucData[8] = {0};
    
    if((NULL==handle)||(NULL==pucOut)||(0==puiOutLen))
    {
        return M_WE_LIB_ERR_INVALID_PARAMETER;
    } 
    pstPara=(St_WeBlockMAtchS *)handle;
    if(*puiOutLen < 8)
    {
        return M_WE_LIB_ERR_BUFFER_TOO_SMALL;
    }
    *puiOutLen = 8;
    (WE_VOID)WE_MEMSET(aucData, (WE_UCHAR)(8-pstPara->iBufLen), (WE_UINT32)8);
    (WE_VOID)WE_MEMCPY(aucData, pstPara->aucData8, (WE_UINT32)pstPara->iBufLen);    
    iResult=We_LibAppendDes( aucData ,8, pucOut, handle);
    
    We_LibKillDes(handle);   
    return iResult;
}



/***************************************************************************************************
*   DES Decrypt                                                                    
***************************************************************************************************/
/*=====================================================================================
FUNCTION: 
    We_LibDesDecrypt
CREATE DATE: 
    2006-12-04
AUTHOR: 
    Sam
DESCRIPTION:
    Set sub decrypt key and decrypt data with DES algorithm.
ARGUMENTS PASSED:
    iRounds[IN]: total rounds.
    stKey[IN]: cipher key
    pucIn[IN]: data to be decrypted;
    uiInLen[IN]: length of data to be decrypted;
    pucOut[OUT]: decrypted data;
    puiOutLen[OUT]: length of decrypted data;
RETURN VALUE:
    M_WE_LIB_ERR_OK if success,others error code.
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    Omit
IMPORTANT NOTES:
    no padding: the input data had been padded, so the length of 
                input data is multiples of block size.Padding is 
                stripped in the extern module.
=====================================================================================*/
/* No Padding */
WE_INT32 We_LibDesDecrypt
(
    St_WeCipherCrptKey stKey, 
    const WE_UCHAR* pucIn, 
    WE_UINT32 uiInLen, 
    WE_UCHAR* pucOut, 
    WE_UINT32* puiOutLen
)
{
    WE_HANDLE handle=NULL;
    WE_INT32 iResult=0;
    St_WeLibBsapiMode stMd = {BSAPI_DECRYPT, BSAPI_CBC, BSAPI_SINGLE, BSAPI_KEY};

    CHECK_FOR_NULL_ARG(pucIn, 0x08);
    CHECK_FOR_NULL_ARG(pucOut, 0x08);
    CHECK_FOR_NULL_ARG(puiOutLen, 0x08);
    
    stMd.eOp = BSAPI_DECRYPT;
    iResult= We_LibStartDesCore(&stMd,&stKey,&handle);    
    
    if(iResult!=M_WE_LIB_ERR_OK)
    {
        return M_WE_LIB_ERR_INSUFFICIENT_MEMORY;
    }
    
    if(!(uiInLen % 8) && (*puiOutLen >= uiInLen))
    {
        *puiOutLen = uiInLen;
    }
    else
    {
        return M_WE_LIB_ERR_BUFFER_TOO_SMALL;
    }
    iResult=We_LibAppendDes((WE_UCHAR*)pucIn, uiInLen,pucOut,handle);   

    We_LibKillDes(handle);
    return M_WE_LIB_ERR_OK;
}


/*=====================================================================================
FUNCTION: 
    We_LibDesDeCryptInit
CREATE DATE: 
    2006-12-04
AUTHOR: 
    Sam
DESCRIPTION:
    Decrypt init with DES algorithm.
ARGUMENTS PASSED:
    stKey[IN]: cipher key
    pHandle[OUT]: data handle;
RETURN VALUE:
    M_WE_LIB_ERR_OK if success,others error code.
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    Omit
IMPORTANT NOTES:
    no padding: the input data had been padded, so the length of 
                input data is multiples of block size.Padding is 
                stripped in the extern module.
=====================================================================================*/
WE_INT32 We_LibDesDeCryptInit
(
    St_WeCipherCrptKey stKey,
    WE_HANDLE* pHandle
)
{
    WE_INT32 iResult=0;
    St_WeLibBsapiMode stMd = {BSAPI_DECRYPT, BSAPI_CBC, BSAPI_SINGLE, BSAPI_KEY};

    if(NULL==pHandle)
    {
        return M_WE_LIB_ERR_INVALID_PARAMETER;
    }
    stMd.eOp = BSAPI_DECRYPT;
    iResult= We_LibStartDesCore(&stMd,&stKey,pHandle); 
    
    return iResult;
}


/*=====================================================================================
FUNCTION: 
    We_LibDesDeCryptUpdate
CREATE DATE: 
    2006-12-04
AUTHOR: 
    Sam
DESCRIPTION:
    Decrypt data with DES algorithm.
ARGUMENTS PASSED:
    pucIn[IN]: data part to be decrypted;
    uiInLen[IN]: length of data part;
    pucOut[OUT]: decrypted data;
    puiOutLen[OUT]: length of decrypted data;
    handle[IO]: data handle.
RETURN VALUE:
    M_WE_LIB_ERR_OK if success,others error code.
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    Omit
IMPORTANT NOTES:
    no padding: the input data had been padded, so the length of 
                input data is multiples of block size.Padding is 
                stripped in the extern module.
=====================================================================================*/
WE_INT32 We_LibDesDeCryptUpdate
(
    const WE_UCHAR* pucIn, 
    WE_UINT32 uiInLen, 
    WE_UCHAR* pucOut, 
    WE_UINT32* puiOutLen, 
    WE_HANDLE handle
)
{
    WE_INT32              iResult       = M_WE_LIB_ERR_OK;
    WE_UINT32             uiLen     = 0; 
    if((NULL==handle)||(NULL==pucIn)||(0==uiInLen)||!pucOut||!puiOutLen)
    {
        return M_WE_LIB_ERR_INVALID_PARAMETER;
    }
    
    uiLen = uiInLen % 8;
    if(!uiLen && (*puiOutLen >= uiInLen))
    {
        *puiOutLen = uiLen;        
    }
    else
    {
        return M_WE_LIB_ERR_BUFFER_TOO_SMALL;
    } 
    iResult=We_LibAppendDes((WE_UCHAR *)pucIn, uiInLen,pucOut, handle);  
    return iResult;    
}


/*=====================================================================================
FUNCTION: 
    We_LibDesDeCryptFinal
CREATE DATE: 
    2006-12-04
AUTHOR: 
    Sam
DESCRIPTION:
    Free the data handle.
ARGUMENTS PASSED:
    handle[IN]: data handle.
RETURN VALUE:
    M_WE_LIB_ERR_OK if success,others error code.
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    Omit
IMPORTANT NOTES:
    no padding: the input data had been padded, so the length of 
                input data is multiples of block size.Padding is 
                stripped in the extern module.
=====================================================================================*/
WE_INT32 We_LibDesDeCryptFinal(WE_HANDLE handle) 
{
    WE_INT32 iResult=0;    
    if(NULL==handle)
    {
        return M_WE_LIB_ERR_INVALID_PARAMETER;
    }
    We_LibKillDes(handle);
    
    return iResult;    
}

/*=====================================================================================
FUNCTION: 
    We_LibDesDecrypt1
CREATE DATE: 
    2006-12-04
AUTHOR: 
    Sam
DESCRIPTION:
    Set sub decrypt key and decrypt data with DES algorithm.
ARGUMENTS PASSED:
    stKey[IN]: cipher key
    pucIn[IN]: data to be decrypted;
    uiInLen[IN]: length of data to be decrypted;
    pucOut[OUT]: decrypted data;
    puiOutLen[OUT]: length of decrypted data;
RETURN VALUE:
    M_WE_LIB_ERR_OK if success,others error code.
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    Omit
IMPORTANT NOTES:
    rfc2630 padding: pad interior, and pad each byte with # bytes 
                     padded(# is equal to the length of padding, 
                     at least 1 byte).Padding is stripped on decrypt.
=====================================================================================*/
/* RFC2630 Padding */
WE_INT32 We_LibDesDecrypt1
(
    St_WeCipherCrptKey stKey, 
    const WE_UCHAR* pucIn, 
    WE_UINT32 uiInLen, 
    WE_UCHAR* pucOut, 
    WE_UINT32* puiOutLen
)
{
    WE_HANDLE handle=NULL; 
    WE_INT32 iResult=0;
    WE_UINT32 uiLen=0;
    St_WeLibBsapiMode stMd = {BSAPI_DECRYPT, BSAPI_CBC, BSAPI_SINGLE, BSAPI_KEY};

    CHECK_FOR_NULL_ARG(pucIn, 0x08);
    CHECK_FOR_NULL_ARG(pucOut, 0x08);
    CHECK_FOR_NULL_ARG(puiOutLen, 0x08);
    
    if(!(uiInLen % 8) && (*puiOutLen >= uiInLen))
    {
        *puiOutLen = uiInLen;
    }
    else
    {
        return M_WE_LIB_ERR_BUFFER_TOO_SMALL;
    }
    
    stMd.eOp = BSAPI_DECRYPT;
    iResult = We_LibStartDesCore(&stMd,&stKey,&handle);   
    
    if(iResult!=M_WE_LIB_ERR_OK)
    {
        return M_WE_LIB_ERR_INSUFFICIENT_MEMORY;
    }
    iResult=We_LibAppendDes((WE_UCHAR *)pucIn, uiInLen,pucOut, handle);
    
    if(iResult!=M_WE_LIB_ERR_OK)         
    {
        return M_WE_LIB_ERR_INSUFFICIENT_MEMORY;
    }
    
    uiLen = *(pucOut + uiInLen - 1);
    if(uiLen&&(uiLen <= 8))
    {
        WE_UINT32   uiLoop = 0;
        for(; uiLoop < uiLen; uiLoop++)
        {
            if(uiLen != *(pucOut + uiInLen - 1 - uiLoop))
            {
                break;
            }
        }
        if(uiLoop == uiLen)
        {
            *puiOutLen -= uiLen;
        }
    }
    We_LibKillDes(handle);
    return M_WE_LIB_ERR_OK;    
}


/*=====================================================================================
FUNCTION: 
    We_LibDesDecryptInit1
CREATE DATE: 
    2006-12-04
AUTHOR: 
    Sam
DESCRIPTION:
    Decrypt init with DES algorithm.
ARGUMENTS PASSED:
    stKey[IN]: cipher key
    handle[OUT]: data handle;
RETURN VALUE:
    M_WE_LIB_ERR_OK if success,others error code.
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    Omit
IMPORTANT NOTES:
    rfc2630 padding: pad interior, and pad each byte with # bytes 
                     padded(# is equal to the length of padding, 
                     at least 1 byte).Padding is stripped on decrypt.
=====================================================================================*/
WE_INT32 We_LibDesDecryptInit1
(
    St_WeCipherCrptKey stKey,
    WE_HANDLE* pHandle
)
{
    WE_INT32 iResult=0;
    St_WeLibBsapiMode stMd = {BSAPI_DECRYPT, BSAPI_CBC, BSAPI_SINGLE, BSAPI_KEY};

    if(NULL==pHandle)
    {
        return M_WE_LIB_ERR_INVALID_PARAMETER;
    }
    stMd.eOp = BSAPI_DECRYPT;
    /* get subkey */
    iResult= We_LibStartDesCore(&stMd,&stKey,pHandle);  
    
    return iResult;
}


/*=====================================================================================
FUNCTION: 
    We_LibDesDeCryptUpdate1
CREATE DATE: 
    2006-12-04
AUTHOR: 
    Sam
DESCRIPTION:
    Decrypt data with DES algorithm.
ARGUMENTS PASSED:
    pucIn[IN]: data part to be decrypted;
    uiInLen[IN]: length of data part;
    pucOut[OUT]: decrypted data;
    puiOutLen[OUT]: length of decrypted data;
    handle[IO]: data handle.
RETURN VALUE:
    M_WE_LIB_ERR_OK if success,others error code.
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    Omit
IMPORTANT NOTES:
    rfc2630 padding: pad interior, and pad each byte with # bytes 
                     padded(# is equal to the length of padding, 
                     at least 1 byte).Padding is stripped on decrypt.
=====================================================================================*/
WE_INT32 We_LibDesDeCryptUpdate1
(
    const WE_UCHAR* pucIn, 
    WE_UINT32 uiInLen, 
    WE_UCHAR* pucOut, 
    WE_UINT32* puiOutLen, 
    WE_HANDLE handle
)
{
    WE_INT32 iResult = M_WE_LIB_ERR_OK; 
    St_WeBlockMAtchS *pstPara=NULL;
    WE_UINT32 iIndex=0;
    WE_INT32 iBufLen=0;
    if((NULL==handle)||(NULL==pucIn)||(0==uiInLen)||!pucOut||!puiOutLen)
    {
        return M_WE_LIB_ERR_INVALID_PARAMETER;
    }  
    
    iBufLen = (uiInLen % 8);
    if(iBufLen && (*puiOutLen >= ((1 + uiInLen / 8) * 8)))
    {
        *puiOutLen = ((1 + uiInLen / 8) * 8);
    }
    else if(!iBufLen && (*puiOutLen >= uiInLen))
    {
        *puiOutLen = uiInLen;
    }
    else
    {
        return M_WE_LIB_ERR_BUFFER_TOO_SMALL;
    }
    
    pstPara=(St_WeBlockMAtchS *)handle;
    
    iIndex = pstPara->iBufLen;
    if((uiInLen + (WE_UINT32)iIndex) == *puiOutLen)
    {
        pstPara->iBufLen = 0;
        (WE_VOID)WE_MEMCPY((pstPara->aucData8 + iIndex), pucIn, (WE_UINT32)(8 - iIndex));
        iResult=We_LibAppendDes((WE_UCHAR *)(pstPara->aucData8), 8,pucOut, handle);
        if(8 != *puiOutLen)
        {
            iResult=We_LibAppendDes((WE_VOID *)(pucIn + (8 - iIndex)), \
                (uiInLen - (WE_UINT32)(8 - iIndex) - (WE_UINT32)(pstPara->iBufLen)),pucOut+8, handle);
        }
        
    }
    else if ((uiInLen + (WE_UINT32)iIndex) < *puiOutLen) 
    {
        *puiOutLen -= 8 ; 
        if((uiInLen + (WE_UINT32)iIndex) > 8)
        {
            pstPara->iBufLen = (iIndex + uiInLen) % 8;
            (WE_VOID)WE_MEMCPY((pstPara->aucData8 + iIndex), pucIn, (WE_UINT32)(8 - iIndex));
            iResult=We_LibAppendDes((WE_UCHAR *)(pstPara->aucData8), 8,pucOut, handle);
            if((uiInLen - (WE_UINT32)(8 - iIndex) - (WE_UINT32)(pstPara->iBufLen)))
            {
                iResult=We_LibAppendDes((WE_VOID *)(pucIn + (8 - iIndex)), \
                    (uiInLen - (WE_UINT32)(8 - iIndex) - (WE_UINT32)(pstPara->iBufLen)),pucOut+8, handle);
            }
            iIndex = uiInLen - pstPara->iBufLen;
            (WE_VOID)WE_MEMCPY(pstPara->aucData8, pucIn + iIndex, (WE_UINT32)(pstPara->iBufLen));
        }
        else
        {
            (WE_VOID)WE_MEMCPY(pstPara->aucData8 + iIndex, pucIn, uiInLen);
            pstPara->iBufLen = uiInLen;
        }
    }
    else
    {
        pstPara->iBufLen = (iIndex + uiInLen) - *puiOutLen;
        (WE_VOID)WE_MEMCPY((pstPara->aucData8 + iIndex), pucIn, (WE_UINT32)(8 - iIndex));
        iResult=We_LibAppendDes( pstPara->aucData8 ,8, pucOut, handle);
        if((uiInLen - (8 - (WE_UINT32)iIndex) - (WE_UINT32)(pstPara->iBufLen)))
        {
            iResult=We_LibAppendDes((WE_VOID*)(pucIn + (8 - iIndex)), \
                (uiInLen - (8 - (WE_UINT32)iIndex) - (WE_UINT32)(pstPara->iBufLen)),pucOut + 8, handle);
        }
        iIndex = uiInLen - pstPara->iBufLen;
        (WE_VOID)WE_MEMCPY(pstPara->aucData8, pucIn + iIndex, (WE_UINT32)(pstPara->iBufLen));
    }
    return iResult;
}   


/*=====================================================================================
FUNCTION: 
    We_LibDesDeCryptFinal1
CREATE DATE: 
    2006-12-04
AUTHOR: 
    Sam
DESCRIPTION:
    Stripped the padding and free memory.
ARGUMENTS PASSED:
    pucOut[IN]: pointer to the end of decrypted data;
    puiOutLen[OUT]: length of padding;
    handle[IN]: data handle.
RETURN VALUE:
    M_WE_LIB_ERR_OK if success,others error code.
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    Omit
IMPORTANT NOTES:
    rfc2630 padding: pad interior, and pad each byte with # bytes 
                     padded(# is equal to the length of padding, 
                     at least 1 byte).Padding is stripped on decrypt.
=====================================================================================*/
WE_INT32 We_LibDesDeCryptFinal1
(
    WE_UCHAR* pucOut,
    WE_UINT32* puiOutLen, 
    WE_HANDLE handle
)
{
    WE_INT32 iResult = M_WE_LIB_ERR_OK;
    WE_UINT32 uiLen = 0; 
    St_WeBlockMAtchS *pstPara=NULL;
    WE_UINT32 uiLoop=0;
    
    if((NULL==handle)||(NULL==pucOut)||(0==puiOutLen))
    {
        return M_WE_LIB_ERR_INVALID_PARAMETER;
    } 
    pstPara=(St_WeBlockMAtchS *)handle;
    if(*puiOutLen < 8)
    {
        return M_WE_LIB_ERR_BUFFER_TOO_SMALL;
    }
    *puiOutLen = 8; 
    iResult=We_LibAppendDes( pstPara->aucData8 ,8, pucOut, handle);
    
    uiLen = *(pucOut + 8 - 1);
    if(uiLen&&(uiLen <= 8))
    {
        for(; uiLoop < uiLen; uiLoop++)
        {
            if(uiLen != *(pucOut + 8 - 1 - uiLoop))
            {
                break;
            }
        }
        if(uiLoop == uiLen)
        {
            *puiOutLen -= uiLen;
        }
    }
    
    We_LibKillDes(handle);   
    return iResult;
}

/*3des*/
/*=====================================================================================
FUNCTION: 
    We_LibStartTripleDes
CREATE DATE: 
    2006-12-04
AUTHOR: 
    Sam
DESCRIPTION:
    Set encrypt key and encrypt data with 3DES algorithm.
ARGUMENTS PASSED:
    pstMode[IN]: mode param;
    pstKeys[IN]: cipher key;
    pHandle[OUT]: data handle.
RETURN VALUE:
    M_WE_LIB_ERR_OK if success,others error code.
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    Omit
IMPORTANT NOTES:
=====================================================================================*/
WE_INT32 We_LibStartTripleDes
(
    St_WeLibBsapiMode *pstMode,
    St_WeCipherCrptKey stKey, 
    WE_HANDLE *pHandle
)
{
    St_WeLibBsapiMode newmode;
    WE_INT32 iResult=0;
    CHECK_FOR_NULL_ARG(pstMode, 0);
    (WE_VOID)WE_MEMCPY(&newmode, pstMode, sizeof(*pstMode)); 
    newmode.eTriple = BSAPI_TRIPLE;
    iResult = We_LibStartDesCore(&newmode,&stKey,pHandle);
    return iResult;
}
/*=====================================================================================
FUNCTION: 
    We_LibAppendTripleDes
CREATE DATE: 
    2006-12-04
AUTHOR: 
    Sam
DESCRIPTION:
    Encrypt data with 3DES algorithm.
ARGUMENTS PASSED:
    pucData[IN]: data to be encrypted;
    uiLength[IN]: length of data to be encrypted;
    pucOut[OUT]: encrypted data;
    pHandle[OUT]: data handle.
RETURN VALUE:
    M_WE_LIB_ERR_OK if success,others error code.
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    Omit
IMPORTANT NOTES:
=====================================================================================*/
WE_INT32 We_LibAppendTripleDes
(
    WE_UCHAR *pucData, 
    WE_UINT32 uiLength,
    WE_UCHAR* pucOut, 
    WE_VOID *pHandle
)
{
    return We_LibAppendBlock(pucData, uiLength, pucOut,pHandle);
}

/***************************************************************************************************
*   Tripartite DES Encrypt                                                                    
***************************************************************************************************/
/*=====================================================================================
FUNCTION: 
    We_Lib3DesEncrypt
CREATE DATE: 
    2006-12-04
AUTHOR: 
    Sam
DESCRIPTION:
    Set encrypt key and encrypt data with 3DES algorithm.
ARGUMENTS PASSED:
    stKey[IN]: cipher key
    pucIn[IN]: data to be encrypted;
    uiInLen[IN]: length of data to be encrypted;
    pucOut[OUT]: encrypted data;
    puiOutLen[OUT]: length of encrypted data;
RETURN VALUE:
    M_WE_LIB_ERR_OK if success,others error code.
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    Omit
IMPORTANT NOTES:
    no padding: the input data had been padded, so the length of 
                input data is multiples of block size.Padding is 
                stripped in the extern module.
=====================================================================================*/
/* No Padding */
WE_INT32 We_Lib3DesEncrypt
(
    St_WeCipherCrptKey stKey, 
    const WE_UCHAR* pucIn, 
    WE_UINT32 uiInLen, 
    WE_UCHAR* pucOut, 
    WE_UINT32* puiOutLen
)
{
    WE_HANDLE handle=NULL;
    WE_INT32 iResult=0;
    WE_UINT32 uiLen=0;
    St_WeLibBsapiMode stMd = {BSAPI_DECRYPT, BSAPI_CBC, BSAPI_SINGLE, BSAPI_KEY};

    CHECK_FOR_NULL_ARG(pucIn, 0x08);
    CHECK_FOR_NULL_ARG(pucOut, 0x08);
    CHECK_FOR_NULL_ARG(puiOutLen, 0x08);
    
    stMd.eOp = BSAPI_ENCRYPT;
    iResult= We_LibStartTripleDes(&stMd,stKey,&handle);
    
    if(iResult!=M_WE_LIB_ERR_OK)
    {
        return M_WE_LIB_ERR_INSUFFICIENT_MEMORY;
    }
    
    uiLen = uiInLen % 8;
    if(uiLen && (*puiOutLen >= ((1 + uiInLen / 8) * 8)))
    {
        *puiOutLen = ((1 + uiInLen / 8) * 8);
    }
    else if(!uiLen && (*puiOutLen >= uiInLen))
    {
        *puiOutLen = uiInLen;
    }
    else
    {
        return M_WE_LIB_ERR_BUFFER_TOO_SMALL;
    }
    if(uiInLen-uiLen>0)
    {
        iResult=We_LibAppendTripleDes((WE_UCHAR*)pucIn, uiInLen-uiLen,pucOut,handle);
    }
    if(iResult!=0)
    {
        return iResult;
    }
    if(uiLen)
    {
        WE_UCHAR aucData[8] = {0};
        (WE_VOID)WE_MEMSET(aucData,0,8);
        (WE_VOID)WE_MEMCPY(aucData,(pucIn + uiInLen - uiLen),uiLen);   
        if(uiInLen==uiLen)
        {
            iResult=We_LibAppendTripleDes((WE_UCHAR*)aucData, 8,pucOut,handle);
        }
        else
        {
            iResult=We_LibAppendTripleDes((WE_UCHAR*)aucData, 8,(pucOut+ uiInLen - uiLen),handle);
        }
    } 
    We_LibKillDes(handle);
    return M_WE_LIB_ERR_OK;
} 
/*=====================================================================================
FUNCTION: 
    We_Lib3DesEncryptInit
CREATE DATE: 
    2006-12-04
AUTHOR: 
    Sam
DESCRIPTION:
    Encrypt init with 3DES algorithm.
ARGUMENTS PASSED:
    stKey[IO]: cipher key
    pHandle[OUT]: data handle;
RETURN VALUE:
    M_WE_LIB_ERR_OK if success,others error code.
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    Omit
IMPORTANT NOTES:
    no padding: the input data had been padded, so the length of 
                input data is multiples of block size.Padding is 
                stripped in the extern module.
=====================================================================================*/
WE_INT32 We_Lib3DesEncryptInit
(
    St_WeCipherCrptKey stKey,
    WE_HANDLE* pHandle
)
{
    WE_INT32 iResult=0;
    St_WeLibBsapiMode stMd = {BSAPI_DECRYPT, BSAPI_CBC, BSAPI_SINGLE, BSAPI_KEY};
    
    if (NULL==pHandle)
    {
        return M_WE_LIB_ERR_INVALID_PARAMETER;
    }
    stMd.eOp = BSAPI_ENCRYPT;
    iResult= We_LibStartTripleDes(&stMd,stKey,pHandle);  

    return iResult;
}
/*=====================================================================================
FUNCTION: 
    We_Lib3DesEnCryptUpdate
CREATE DATE: 
    2006-12-04
AUTHOR: 
    Sam
DESCRIPTION:
    Encrypt data with 3DES algorithm.
ARGUMENTS PASSED:
    pucIn[IN]: data part to be encrypted;
    uiInLen[IN]: length of data part;
    pucOut[OUT]: encrypted data;
    puiOutLen[OUT]: length of encrypted data;
    handle[IO]: data handle.
RETURN VALUE:
    M_WE_LIB_ERR_OK if success,others error code.
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    Omit
IMPORTANT NOTES:
    no padding: the input data had been padded, so the length of 
                input data is multiples of block size.Padding is 
                stripped in the extern module.
=====================================================================================*/
WE_INT32 We_Lib3DesEnCryptUpdate
(
    const WE_UCHAR* pucIn, 
    WE_UINT32 uiInLen,
    WE_UCHAR* pucOut, 
    WE_UINT32* puiOutLen, 
    WE_HANDLE handle
)
{
    WE_UINT32 uiLen=0;
    WE_INT32 iResult=0;
    if(!pucIn||!uiInLen||!handle||!puiOutLen)
    {
        return M_WE_LIB_ERR_INVALID_PARAMETER;
    }
    
    uiLen = uiInLen % 8;
    if(*puiOutLen >= ((1 + uiInLen / 8) * 8))
    {
        *puiOutLen = ((1 + uiInLen / 8) * 8);
    }
    else
    {
        return M_WE_LIB_ERR_BUFFER_TOO_SMALL;
    }    
    if(0 == uiLen)
    {        
        iResult=We_LibAppendTripleDes((WE_UCHAR *)pucIn, uiInLen,pucOut, handle);  
    }
    else 
    {
        return M_WE_LIB_ERR_GENERAL_ERROR;
    }
    return iResult;    
    
}
/*=====================================================================================
FUNCTION: 
    We_Lib3DesEnCryptFinal
CREATE DATE: 
    2006-12-04
AUTHOR: 
    Sam
DESCRIPTION:
    Free the data handle.
ARGUMENTS PASSED:
    handle[IN]: data handle.
RETURN VALUE:
    M_WE_LIB_ERR_OK if success,others error code.
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    Omit
IMPORTANT NOTES:
    no padding: the input data had been padded, so the length of 
                input data is multiples of block size.Padding is 
                stripped in the extern module.
=====================================================================================*/
WE_INT32 We_Lib3DesEnCryptFinal(WE_HANDLE handle) 
{
    
    if(NULL==handle)
    {
        return M_WE_LIB_ERR_INVALID_PARAMETER;
    }
    We_LibKillDes(handle);
    return 0;    
}
/*=====================================================================================
FUNCTION: 
    We_Lib3DesEncrypt1
CREATE DATE: 
    2006-12-04
AUTHOR: 
    Sam
DESCRIPTION:
    Set sub encrypt key and padding data, encrypt data with 3DES algorithm.
ARGUMENTS PASSED:
    stKey[IN]: cipher key
    pucIn[IN]: data to be encrypted;
    uiInLen[IN]: length of data to be encrypted;
    pucOut[OUT]: encrypted data;
    puiOutLen[OUT]: length of encrypted data;
RETURN VALUE:
    M_WE_LIB_ERR_OK if success,others error code.
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    Omit
IMPORTANT NOTES:
    rfc2630 padding: pad interior, and pad each byte with # bytes 
                     padded(# is equal to the length of padding, 
                     at least 1 byte).Padding is stripped on decrypt.
=====================================================================================*/
/* RFC2630 Padding */
WE_INT32 We_Lib3DesEncrypt1
(
    St_WeCipherCrptKey stKey, 
    const WE_UCHAR* pucIn, 
    WE_UINT32 uiInLen, 
    WE_UCHAR* pucOut, 
    WE_UINT32* puiOutLen
)
{
    WE_HANDLE handle=NULL;
    WE_INT32 iResult=0;
    WE_UINT32 uiLen=0;
    St_WeLibBsapiMode stMd = {BSAPI_DECRYPT, BSAPI_CBC, BSAPI_SINGLE, BSAPI_KEY};

    CHECK_FOR_NULL_ARG(pucIn, 0x08);
    CHECK_FOR_NULL_ARG(pucOut, 0x08);
    CHECK_FOR_NULL_ARG(puiOutLen, 0x08);

    stMd.eOp = BSAPI_ENCRYPT;
    iResult= We_LibStartTripleDes(&stMd,stKey,&handle); 

    if(iResult!=M_WE_LIB_ERR_OK)
    {
        return M_WE_LIB_ERR_INSUFFICIENT_MEMORY;
    }
    
    uiLen = uiInLen % 8;
    if(uiLen && (*puiOutLen >= ((1 + uiInLen / 8) * 8)))
    {
        *puiOutLen = ((1 + uiInLen / 8) * 8);
    }
    else if(!uiLen && (*puiOutLen >= uiInLen))
    {
        *puiOutLen = uiInLen;
    }
    else
    {
        return M_WE_LIB_ERR_BUFFER_TOO_SMALL;
    }
    
    if(uiInLen-uiLen>0)
    {
        iResult=We_LibAppendTripleDes((WE_UCHAR*)pucIn, uiInLen-uiLen,pucOut,handle);
    }
    if(iResult!=0)
    {
        return iResult;
    }
    if(uiLen)
    {
        WE_UCHAR aucData[8] = {0};
        (WE_VOID)WE_MEMSET(aucData,8-(WE_INT32)uiLen,8);
        (WE_VOID)WE_MEMCPY(aucData,(pucIn + uiInLen - uiLen),uiLen);   
        if(uiInLen==uiLen)
        {
            iResult=We_LibAppendTripleDes((WE_UCHAR*)aucData, 8,pucOut,handle);
        }
        else
        {
            iResult=We_LibAppendTripleDes((WE_UCHAR*)aucData, 8,(pucOut+ uiInLen - uiLen),handle);
        }
    }
    
    We_LibKillDes(handle);
    return M_WE_LIB_ERR_OK;
}
/*=====================================================================================
FUNCTION: 
    We_Lib3DesEncryptInit1
CREATE DATE: 
    2006-12-04
AUTHOR: 
    Sam
DESCRIPTION:
    Encrypt init with 3DES algorithm.
ARGUMENTS PASSED:
    stKey[IN]: cipher key;
    pHandle[IN]: data handle.
RETURN VALUE:
    M_WE_LIB_ERR_OK if success,others error code.
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    Omit
IMPORTANT NOTES:
    rfc2630 padding: pad interior, and pad each byte with # bytes 
                     padded(# is equal to the length of padding, 
                     at least 1 byte).Padding is stripped on decrypt.
=====================================================================================*/
WE_INT32 We_Lib3DesEncryptInit1
(
    St_WeCipherCrptKey stKey,
    WE_HANDLE* pHandle
)
{
    WE_INT32 iResult=0;
    St_WeLibBsapiMode stMd = {BSAPI_DECRYPT, BSAPI_CBC, BSAPI_SINGLE, BSAPI_KEY};

    if(NULL==pHandle)
    {
        return M_WE_LIB_ERR_INVALID_PARAMETER;
    }
    stMd.eOp = BSAPI_ENCRYPT;
    iResult= We_LibStartTripleDes(&stMd,stKey,pHandle);  

    return iResult;
}
/*=====================================================================================
FUNCTION: 
    We_Lib3DesEnCryptUpdate1
CREATE DATE: 
    2006-12-04
AUTHOR: 
    Sam
DESCRIPTION:
    Set padding data and encrypt data with 3DES algorithm.
ARGUMENTS PASSED:
    pucIn[IN]: data part to be encrypted;
    uiInLen[IN]: length of data part;
    pucOut[OUT]: encrypted data;
    puiOutLen[OUT]: length of encrypted data;
    handle[IO]: data handle.
RETURN VALUE:
    M_WE_LIB_ERR_OK if success,others error code.
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    Omit
IMPORTANT NOTES:
    rfc2630 padding: pad interior, and pad each byte with # bytes 
                     padded(# is equal to the length of padding, 
                     at least 1 byte).Padding is stripped on decrypt.
=====================================================================================*/
WE_INT32 We_Lib3DesEnCryptUpdate1
(
    const WE_UCHAR* pucIn, 
    WE_UINT32 uiInLen,  
    WE_UCHAR* pucOut, 
    WE_UINT32* puiOutLen, 
    WE_HANDLE handle
)
{
    WE_INT32 iResult = M_WE_LIB_ERR_OK;
    St_WeBlockMAtchS *pstPara=NULL;
    WE_UINT32 iIndex=0;
    WE_INT32 iBufLen=0;
    if((NULL==handle)||(NULL==pucIn)||(0==uiInLen)||!puiOutLen||!pucOut)
    {
        return M_WE_LIB_ERR_INVALID_PARAMETER;
    }  
    
    iBufLen = (uiInLen % 8);
    if(iBufLen && (*puiOutLen >= ((1 + uiInLen / 8) * 8)))
    {
        *puiOutLen = ((1 + uiInLen / 8) * 8);
    }
    else if(!iBufLen && (*puiOutLen >= uiInLen))
    {
        *puiOutLen = uiInLen;
    }
    else
    {
        return M_WE_LIB_ERR_BUFFER_TOO_SMALL;
    }
    
    pstPara=(St_WeBlockMAtchS *)handle;
    
    iIndex = pstPara->iBufLen;
    if((uiInLen + (WE_UINT32)iIndex) == *puiOutLen)
    {
        pstPara->iBufLen = 0;
        (WE_VOID)WE_MEMCPY((pstPara->aucData8 + iIndex), pucIn, (WE_UINT32)(8 - iIndex));
        iResult=We_LibAppendTripleDes((WE_UCHAR *)(pstPara->aucData8), 8,pucOut, handle);
        if(8 != *puiOutLen)
        {
            iResult=We_LibAppendTripleDes((WE_VOID *)(pucIn + (8 - iIndex)), \
                (uiInLen - (WE_UINT32)(8 - iIndex) - (WE_UINT32)(pstPara->iBufLen)),pucOut+8, handle);
        }
        
    }
    else if ((uiInLen + (WE_UINT32)iIndex) < *puiOutLen) 
    {
        *puiOutLen -= 8 ; 
        if((uiInLen + (WE_UINT32)iIndex) > 8)
        {
            pstPara->iBufLen = (iIndex + uiInLen) % 8;
            (WE_VOID)WE_MEMCPY((pstPara->aucData8 + iIndex), pucIn, (WE_UINT32)(8 - iIndex));
            iResult=We_LibAppendTripleDes((WE_UCHAR *)(pstPara->aucData8), 8,pucOut, handle);
            if((uiInLen - (WE_UINT32)(8 - iIndex) - (WE_UINT32)(pstPara->iBufLen)))
            {
                iResult=We_LibAppendTripleDes((WE_VOID *)(pucIn + (8 - iIndex)), \
                    (uiInLen - (WE_UINT32)(8 - iIndex) - (WE_UINT32)(pstPara->iBufLen)),pucOut+8, handle);
            }
            iIndex = uiInLen - pstPara->iBufLen;
            (WE_VOID)WE_MEMCPY(pstPara->aucData8, pucIn + iIndex, (WE_UINT32)(pstPara->iBufLen));
        }
        else
        {
            (WE_VOID)WE_MEMCPY(pstPara->aucData8 + iIndex, pucIn, uiInLen);
            pstPara->iBufLen = uiInLen;
        }
    }
    else
    {
        pstPara->iBufLen = (iIndex + uiInLen) - *puiOutLen;
        (WE_VOID)WE_MEMCPY((pstPara->aucData8 + iIndex), pucIn, (WE_UINT32)(8 - iIndex));
        iResult=We_LibAppendTripleDes( pstPara->aucData8 ,8, pucOut, handle);
        if((uiInLen - (8 - (WE_UINT32)iIndex) - (WE_UINT32)(pstPara->iBufLen)))
        {
            iResult=We_LibAppendTripleDes((WE_VOID*)(pucIn + (8 - iIndex)), \
                (uiInLen - (8 - (WE_UINT32)iIndex) - (WE_UINT32)(pstPara->iBufLen)),pucOut + 8, handle);
        }
        iIndex = uiInLen - pstPara->iBufLen;
        (WE_VOID)WE_MEMCPY(pstPara->aucData8, pucIn + iIndex, (WE_UINT32)(pstPara->iBufLen));
    }
    return iResult;
}
/*=====================================================================================
FUNCTION: 
    We_Lib3DesEnCryptFinal1
CREATE DATE: 
    2006-12-04
AUTHOR: 
    Sam
DESCRIPTION:
    Get encrypted data and free memory.
ARGUMENTS PASSED:
    pucOut[OUT]: encrypted data;
    puiOutLen[OUT]: length of encrypted data;
    handle[IN]: data handle.
RETURN VALUE:
    M_WE_LIB_ERR_OK if success,others error code.
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    Omit
IMPORTANT NOTES:
    rfc2630 padding: pad interior, and pad each byte with # bytes 
                     padded(# is equal to the length of padding, 
                     at least 1 byte).Padding is stripped on decrypt.
=====================================================================================*/
WE_INT32 We_Lib3DesEnCryptFinal1
(
    WE_UCHAR* pucOut,
    WE_UINT32* puiOutLen, 
    WE_HANDLE handle
)
{
    WE_INT32 iResult = M_WE_LIB_ERR_OK;
    St_WeBlockMAtchS *pstPara=NULL;
    WE_UCHAR aucData[8] = {0};
    
    if((NULL==handle)||(NULL==pucOut)||(0==puiOutLen))
    {
        return M_WE_LIB_ERR_INVALID_PARAMETER;
    } 
    pstPara=(St_WeBlockMAtchS *)handle;
    if(*puiOutLen < 8)
    {
        return M_WE_LIB_ERR_BUFFER_TOO_SMALL;
    }
    *puiOutLen = 8;
    (WE_VOID)WE_MEMSET(aucData, (WE_UCHAR)(8-pstPara->iBufLen), (WE_UINT32)8);
    (WE_VOID)WE_MEMCPY(aucData, pstPara->aucData8, (WE_UINT32)pstPara->iBufLen);    
    iResult=We_LibAppendTripleDes( aucData ,8, pucOut, handle);
    
    We_LibKillDes(handle);   
    return iResult;
}
/***************************************************************************************************
*   Tripartite DES Decrypt                                                                    
***************************************************************************************************/
/*=====================================================================================
FUNCTION: 
    We_Lib3DesDecrypt
CREATE DATE: 
    2006-12-04
AUTHOR: 
    Sam
DESCRIPTION:
    Set sub decrypt key and decrypt data with 3DES algorithm.
ARGUMENTS PASSED:
    stKey[IN]: cipher key
    pucIn[IN]: data to be decrypted;
    uiInLen[IN]: length of data to be decrypted;
    pucOut[OUT]: decrypted data;
    puiOutLen[OUT]: length of decrypted data;
RETURN VALUE:
    M_WE_LIB_ERR_OK if success,others error code.
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    Omit
IMPORTANT NOTES:
    no padding: the input data had been padded, so the length of 
                input data is multiples of block size.Padding is 
                stripped in the extern module.
=====================================================================================*/
/* No Padding */
WE_INT32 We_Lib3DesDecrypt
(
    St_WeCipherCrptKey stKey, 
    const WE_UCHAR* pucIn, 
    WE_UINT32 uiInLen, 
    WE_UCHAR* pucOut, 
    WE_UINT32* puiOutLen
)
{
    WE_HANDLE handle=NULL;  
    WE_INT32 iResult=0;
    St_WeLibBsapiMode stMd = {BSAPI_DECRYPT, BSAPI_CBC, BSAPI_SINGLE, BSAPI_KEY};

    CHECK_FOR_NULL_ARG(pucIn, 0x08);
    CHECK_FOR_NULL_ARG(pucOut, 0x08);
    CHECK_FOR_NULL_ARG(puiOutLen, 0x08);

    stMd.eOp = BSAPI_DECRYPT;
    iResult= We_LibStartTripleDes(&stMd,stKey,&handle);  

    if(iResult!=M_WE_LIB_ERR_OK)
    {
        return M_WE_LIB_ERR_INSUFFICIENT_MEMORY;
    }
    
    if(!(uiInLen % 8) && (*puiOutLen >= uiInLen))
    {
        *puiOutLen = uiInLen;
    }
    else
    {
        return M_WE_LIB_ERR_BUFFER_TOO_SMALL;
    }
    iResult=We_LibAppendTripleDes((WE_UCHAR*)pucIn, uiInLen,pucOut,handle); 

    We_LibKillDes(handle);
    return iResult;
    
}
/*=====================================================================================
FUNCTION: 
    We_Lib3DesDecryptInit
CREATE DATE: 
    2006-12-04
AUTHOR: 
    Sam
DESCRIPTION:
    Decrypt init with 3DES algorithm.
ARGUMENTS PASSED:
    stKey[IN]: cipher key
    pHandle[OUT]: data handle;
RETURN VALUE:
    M_WE_LIB_ERR_OK if success,others error code.
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    Omit
IMPORTANT NOTES:
    no padding: the input data had been padded, so the length of 
                input data is multiples of block size.Padding is 
                stripped in the extern module.
=====================================================================================*/
WE_INT32 We_Lib3DesDecryptInit
(
    St_WeCipherCrptKey stKey,
    WE_HANDLE* pHandle
)
{
    WE_INT32 iResult=0;
    St_WeLibBsapiMode stMd = {BSAPI_DECRYPT, BSAPI_CBC, BSAPI_SINGLE, BSAPI_KEY};

    if(NULL==pHandle)
    {
        return M_WE_LIB_ERR_INVALID_PARAMETER;
    }
    stMd.eOp = BSAPI_DECRYPT;
    iResult= We_LibStartTripleDes(&stMd,stKey,pHandle);

    return iResult;
}


/*=====================================================================================
FUNCTION: 
    We_Lib3DesDeCryptUpdate
CREATE DATE: 
    2006-12-04
AUTHOR: 
    Sam
DESCRIPTION:
    Decrypt data with 3DES algorithm.
ARGUMENTS PASSED:
    pucIn[IN]: data part to be decrypted;
    uiInLen[IN]: length of data part;
    pucOut[OUT]: decrypted data;
    puiOutLen[OUT]: length of decrypted data;
    handle[IO]: data handle.
RETURN VALUE:
    M_WE_LIB_ERR_OK if success,others error code.
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    Omit
IMPORTANT NOTES:
    no padding: the input data had been padded, so the length of 
                input data is multiples of block size.Padding is 
                stripped in the extern module.
=====================================================================================*/
WE_INT32 We_Lib3DesDeCryptUpdate
(
    const WE_UCHAR* pucIn, 
    WE_UINT32 uiInLen,
    WE_UCHAR* pucOut, 
    WE_UINT32* puiOutLen, 
    WE_HANDLE handle
)
{ 
    WE_INT32 iResult = M_WE_LIB_ERR_OK;
    WE_UINT32 uiLen = 0; 
    if((NULL==handle)||(NULL==pucIn)||(0==uiInLen)||!pucOut||!puiOutLen)
    {
        return M_WE_LIB_ERR_INVALID_PARAMETER;
    }
    
    uiLen = uiInLen % 8;
    if(!uiLen && (*puiOutLen >= uiInLen))
    {
        *puiOutLen = uiInLen;        
    }
    else
    {
        return M_WE_LIB_ERR_BUFFER_TOO_SMALL;
    }    
    iResult=We_LibAppendTripleDes((WE_UCHAR *)pucIn, uiInLen,pucOut, handle);  
    
    return iResult; 
    
}


/*=====================================================================================
FUNCTION: 
    We_Lib3DesDeCryptFinal
CREATE DATE: 
    2006-12-04
AUTHOR: 
    Sam
DESCRIPTION:
    Free the data handle.
ARGUMENTS PASSED:
    handle[IN]: data handle.
RETURN VALUE:
    M_WE_LIB_ERR_OK if success,others error code.
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    Omit
IMPORTANT NOTES:
    no padding: the input data had been padded, so the length of 
                input data is multiples of block size.Padding is 
                stripped in the extern module.
=====================================================================================*/
WE_INT32 We_Lib3DesDeCryptFinal(WE_HANDLE handle)
{
    WE_INT32 iResult=0;    
    if(NULL==handle)
    {
        return M_WE_LIB_ERR_INVALID_PARAMETER;
    }
    We_LibKillDes(handle);
    
    return iResult;
}

/*=====================================================================================
FUNCTION: 
    We_Lib3DesDecrypt1
CREATE DATE: 
    2006-12-04
AUTHOR: 
    Sam
DESCRIPTION:
    Set sub decrypt key and decrypt data with 3DES algorithm.
ARGUMENTS PASSED:
    stKey[IN]: cipher key
    pucIn[IN]: data to be decrypted;
    uiInLen[IN]: length of data to be decrypted;
    pucOut[OUT]: decrypted data;
    puiOutLen[OUT]: length of decrypted data;
RETURN VALUE:
    M_WE_LIB_ERR_OK if success,others error code.
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    Omit
IMPORTANT NOTES:
    rfc2630 padding: pad interior, and pad each byte with # bytes 
                     padded(# is equal to the length of padding, 
                     at least 1 byte).Padding is stripped on decrypt.
=====================================================================================*/
/* RFC2630 Padding */
WE_INT32 We_Lib3DesDecrypt1
(
    St_WeCipherCrptKey stKey, 
    const WE_UCHAR* pucIn, 
    WE_UINT32 uiInLen, 
    WE_UCHAR* pucOut, 
    WE_UINT32* puiOutLen
)
{
    WE_HANDLE handle=NULL; 
    WE_INT32 iResult=0;
    WE_UINT32 uiLen=0;
    St_WeLibBsapiMode stMd = {BSAPI_DECRYPT, BSAPI_CBC, BSAPI_SINGLE, BSAPI_KEY};

    CHECK_FOR_NULL_ARG(pucIn, 0x08);
    CHECK_FOR_NULL_ARG(pucOut, 0x08);
    CHECK_FOR_NULL_ARG(puiOutLen, 0x08);

    if(!(uiInLen % 8) && (*puiOutLen >= uiInLen))
    {
        *puiOutLen = uiInLen;
    }
    else
    {
        return M_WE_LIB_ERR_BUFFER_TOO_SMALL;
    }    
    stMd.eOp = BSAPI_DECRYPT;
    iResult = We_LibStartTripleDes(&stMd,stKey,&handle);  

    if(iResult!=M_WE_LIB_ERR_OK)
    {
        return M_WE_LIB_ERR_INSUFFICIENT_MEMORY;
    }
    iResult=We_LibAppendTripleDes((WE_UCHAR *)pucIn, uiInLen,pucOut, handle);    
    
    uiLen = *(pucOut + uiInLen - 1);
    if(uiLen&&(uiLen <= 8))
    {
        WE_UINT32   uiLoop = 0;
        for(; uiLoop < uiLen; uiLoop++)
        {
            if(uiLen != *(pucOut + uiInLen - 1 - uiLoop))
            {
                break;
            }
        }
        if(uiLoop == uiLen)
        {
            *puiOutLen -= uiLen;
        }
    }  
    We_LibKillDes(handle);
    return M_WE_LIB_ERR_OK;
    
}


/*=====================================================================================
FUNCTION: 
    We_Lib3DesDeCryptInit1
CREATE DATE: 
    2006-12-04
AUTHOR: 
    Sam
DESCRIPTION:
    Decrypt init with 3DES algorithm.
ARGUMENTS PASSED:
    stKey[IN]: cipher key
    handle[OUT]: data handle;
RETURN VALUE:
    M_WE_LIB_ERR_OK if success,others error code.
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    Omit
IMPORTANT NOTES:
    rfc2630 padding: pad interior, and pad each byte with # bytes 
                     padded(# is equal to the length of padding, 
                     at least 1 byte).Padding is stripped on decrypt.
=====================================================================================*/
WE_INT32 We_Lib3DesDeCryptInit1
( 
    St_WeCipherCrptKey stKey,
    WE_HANDLE* pHandle
)
{
    WE_INT32 iResult=0;
    St_WeLibBsapiMode stMd = {BSAPI_DECRYPT, BSAPI_CBC, BSAPI_SINGLE, BSAPI_KEY};

    if(NULL==pHandle)
    {
        return M_WE_LIB_ERR_INVALID_PARAMETER;
    }
    stMd.eOp = BSAPI_DECRYPT;
    /* get subkey */
    iResult= We_LibStartTripleDes(&stMd,stKey,pHandle);  

    return iResult;
}


/*=====================================================================================
FUNCTION: 
    We_Lib3DesDeCryptUpdate1
CREATE DATE: 
    2006-12-04
AUTHOR: 
    Sam
DESCRIPTION:
    Decrypt data with 3DES algorithm.
ARGUMENTS PASSED:
    pucIn[IN]: data part to be decrypted;
    uiInLen[IN]: length of data part;
    pucOut[OUT]: decrypted data;
    puiOutLen[OUT]: length of decrypted data;
    handle[IO]: data handle.
RETURN VALUE:
    M_WE_LIB_ERR_OK if success,others error code.
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    Omit
IMPORTANT NOTES:
    rfc2630 padding: pad interior, and pad each byte with # bytes 
                     padded(# is equal to the length of padding, 
                     at least 1 byte).Padding is stripped on decrypt.
=====================================================================================*/
WE_INT32 We_Lib3DesDeCryptUpdate1
(
    const WE_UCHAR* pucIn, 
    WE_UINT32 uiInLen,  
    WE_UCHAR* pucOut, 
    WE_UINT32* puiOutLen, 
    WE_HANDLE handle
)
{
    WE_INT32 iResult = M_WE_LIB_ERR_OK;
    St_WeBlockMAtchS *pstPara=NULL;
    WE_UINT32 iIndex=0;
    WE_INT32 iBufLen=0;
    
    if((NULL==handle)||(NULL==pucIn)||(0==uiInLen)||!pucOut||!puiOutLen)
    {
        return M_WE_LIB_ERR_INVALID_PARAMETER;
    }  
    
    iBufLen = (uiInLen % 8);
    if(iBufLen && (*puiOutLen >= ((1 + uiInLen / 8) * 8)))
    {
        *puiOutLen = ((1 + uiInLen / 8) * 8);
    }
    else if(!iBufLen && (*puiOutLen >= uiInLen))
    {
        *puiOutLen = uiInLen;
    }
    else
    {
        return M_WE_LIB_ERR_BUFFER_TOO_SMALL;
    }
    
    pstPara=(St_WeBlockMAtchS *)handle;
    
    iIndex = pstPara->iBufLen;
    if((uiInLen + (WE_UINT32)iIndex) == *puiOutLen)
    {
        pstPara->iBufLen = 0;
        (WE_VOID)WE_MEMCPY((pstPara->aucData8 + iIndex), pucIn, (WE_UINT32)(8 - iIndex));
        iResult=We_LibAppendTripleDes((WE_UCHAR *)(pstPara->aucData8), 8,pucOut, handle);
        if(8 != *puiOutLen)
        {
            iResult=We_LibAppendTripleDes((WE_VOID *)(pucIn + (8 - iIndex)), \
                (uiInLen - (WE_UINT32)(8 - iIndex) - (WE_UINT32)(pstPara->iBufLen)),pucOut+8, handle);
        }
        
    }
    else if ((uiInLen + (WE_UINT32)iIndex) < *puiOutLen) 
    {
        *puiOutLen -= 8 ; 
        if((uiInLen + (WE_UINT32)iIndex) > 8)
        {
            pstPara->iBufLen = (iIndex + uiInLen) % 8;
            (WE_VOID)WE_MEMCPY((pstPara->aucData8 + iIndex), pucIn, (WE_UINT32)(8 - iIndex));
            iResult=We_LibAppendTripleDes((WE_UCHAR *)(pstPara->aucData8), 8,pucOut, handle);
            if((uiInLen - (WE_UINT32)(8 - iIndex) - (WE_UINT32)(pstPara->iBufLen)))
            {
                iResult=We_LibAppendTripleDes((WE_VOID *)(pucIn + (8 - iIndex)), \
                    (uiInLen - (WE_UINT32)(8 - iIndex) - (WE_UINT32)(pstPara->iBufLen)),pucOut+8, handle);
            }
            iIndex = uiInLen - pstPara->iBufLen;
            (WE_VOID)WE_MEMCPY(pstPara->aucData8, pucIn + iIndex, (WE_UINT32)(pstPara->iBufLen));
        }
        else
        {
            (WE_VOID)WE_MEMCPY(pstPara->aucData8 + iIndex, pucIn, uiInLen);
            pstPara->iBufLen = uiInLen;
        }
    }
    else
    {
        pstPara->iBufLen = (iIndex + uiInLen) - *puiOutLen;
        (WE_VOID)WE_MEMCPY((pstPara->aucData8 + iIndex), pucIn, (WE_UINT32)(8 - iIndex));
        iResult=We_LibAppendTripleDes( pstPara->aucData8 ,8, pucOut, handle);
        if((uiInLen - (8 - (WE_UINT32)iIndex) - (WE_UINT32)(pstPara->iBufLen)))
        {
            iResult=We_LibAppendTripleDes((WE_VOID*)(pucIn + (8 - iIndex)), \
                (uiInLen - (8 - (WE_UINT32)iIndex) - (WE_UINT32)(pstPara->iBufLen)),pucOut + 8, handle);
        }
        iIndex = uiInLen - pstPara->iBufLen;
        (WE_VOID)WE_MEMCPY(pstPara->aucData8, pucIn + iIndex, (WE_UINT32)(pstPara->iBufLen));
    }
    return iResult;
    
}


/*=====================================================================================
FUNCTION: 
    We_Lib3DesDeCryptFinal1
CREATE DATE: 
    2006-12-04
AUTHOR: 
    Sam
DESCRIPTION:
    Stripped the padding and free memory.
ARGUMENTS PASSED:
    pucOut[IN]: pointer to the end of decrypted data;
    puiOutLen[OUT]: length of padding;
    handle[IN]: data handle.
RETURN VALUE:
    M_WE_LIB_ERR_OK if success,others error code.
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    Omit
IMPORTANT NOTES:
    rfc2630 padding: pad interior, and pad each byte with # bytes 
                     padded(# is equal to the length of padding, 
                     at least 1 byte).Padding is stripped on decrypt.
=====================================================================================*/
WE_INT32 We_Lib3DesDeCryptFinal1
(
    WE_UCHAR* pucOut,
    WE_UINT32* puiOutLen, 
    WE_HANDLE handle
) 
{
    WE_INT32 iResult = M_WE_LIB_ERR_OK;
    WE_UINT32  uiLen = 0; 
    St_WeBlockMAtchS *pstPara=NULL;
    WE_UINT32 uiLoop=0;
    
    if((NULL==handle)||(NULL==pucOut)||(0==puiOutLen))
    {
        return M_WE_LIB_ERR_INVALID_PARAMETER;
    } 
    pstPara=(St_WeBlockMAtchS *)handle;
    if(*puiOutLen < 8)
    {
        return M_WE_LIB_ERR_BUFFER_TOO_SMALL;
    }
    *puiOutLen = 8;  
    iResult=We_LibAppendTripleDes( pstPara->aucData8 ,8, pucOut, handle);
    
    uiLen = *(pucOut + 8 - 1);
    if(uiLen&&(uiLen <= 8))
    {
        for(; uiLoop < uiLen; uiLoop++)
        {
            if(uiLen != *(pucOut + 8 - 1 - uiLoop))
            {
                break;
            }
        }
        if(uiLoop == uiLen)
        {
            *puiOutLen -= uiLen;
        }
    }
    
    We_LibKillDes(handle);   
    return iResult;  
}

/*AES*/
/***************************************************************************************************
* Type Define Section
***************************************************************************************************/
static const WE_UINT32 Te0[256] = 
{
    0xc66363a5U, 0xf87c7c84U, 0xee777799U, 0xf67b7b8dU,
    0xfff2f20dU, 0xd66b6bbdU, 0xde6f6fb1U, 0x91c5c554U,
    0x60303050U, 0x02010103U, 0xce6767a9U, 0x562b2b7dU,
    0xe7fefe19U, 0xb5d7d762U, 0x4dababe6U, 0xec76769aU,
    0x8fcaca45U, 0x1f82829dU, 0x89c9c940U, 0xfa7d7d87U,
    0xeffafa15U, 0xb25959ebU, 0x8e4747c9U, 0xfbf0f00bU,
    0x41adadecU, 0xb3d4d467U, 0x5fa2a2fdU, 0x45afafeaU,
    0x239c9cbfU, 0x53a4a4f7U, 0xe4727296U, 0x9bc0c05bU,
    0x75b7b7c2U, 0xe1fdfd1cU, 0x3d9393aeU, 0x4c26266aU,
    0x6c36365aU, 0x7e3f3f41U, 0xf5f7f702U, 0x83cccc4fU,
    0x6834345cU, 0x51a5a5f4U, 0xd1e5e534U, 0xf9f1f108U,
    0xe2717193U, 0xabd8d873U, 0x62313153U, 0x2a15153fU,
    0x0804040cU, 0x95c7c752U, 0x46232365U, 0x9dc3c35eU,
    0x30181828U, 0x379696a1U, 0x0a05050fU, 0x2f9a9ab5U,
    0x0e070709U, 0x24121236U, 0x1b80809bU, 0xdfe2e23dU,
    0xcdebeb26U, 0x4e272769U, 0x7fb2b2cdU, 0xea75759fU,
    0x1209091bU, 0x1d83839eU, 0x582c2c74U, 0x341a1a2eU,
    0x361b1b2dU, 0xdc6e6eb2U, 0xb45a5aeeU, 0x5ba0a0fbU,
    0xa45252f6U, 0x763b3b4dU, 0xb7d6d661U, 0x7db3b3ceU,
    0x5229297bU, 0xdde3e33eU, 0x5e2f2f71U, 0x13848497U,
    0xa65353f5U, 0xb9d1d168U, 0x00000000U, 0xc1eded2cU,
    0x40202060U, 0xe3fcfc1fU, 0x79b1b1c8U, 0xb65b5bedU,
    0xd46a6abeU, 0x8dcbcb46U, 0x67bebed9U, 0x7239394bU,
    0x944a4adeU, 0x984c4cd4U, 0xb05858e8U, 0x85cfcf4aU,
    0xbbd0d06bU, 0xc5efef2aU, 0x4faaaae5U, 0xedfbfb16U,
    0x864343c5U, 0x9a4d4dd7U, 0x66333355U, 0x11858594U,
    0x8a4545cfU, 0xe9f9f910U, 0x04020206U, 0xfe7f7f81U,
    0xa05050f0U, 0x783c3c44U, 0x259f9fbaU, 0x4ba8a8e3U,
    0xa25151f3U, 0x5da3a3feU, 0x804040c0U, 0x058f8f8aU,
    0x3f9292adU, 0x219d9dbcU, 0x70383848U, 0xf1f5f504U,
    0x63bcbcdfU, 0x77b6b6c1U, 0xafdada75U, 0x42212163U,
    0x20101030U, 0xe5ffff1aU, 0xfdf3f30eU, 0xbfd2d26dU,
    0x81cdcd4cU, 0x180c0c14U, 0x26131335U, 0xc3ecec2fU,
    0xbe5f5fe1U, 0x359797a2U, 0x884444ccU, 0x2e171739U,
    0x93c4c457U, 0x55a7a7f2U, 0xfc7e7e82U, 0x7a3d3d47U,
    0xc86464acU, 0xba5d5de7U, 0x3219192bU, 0xe6737395U,
    0xc06060a0U, 0x19818198U, 0x9e4f4fd1U, 0xa3dcdc7fU,
    0x44222266U, 0x542a2a7eU, 0x3b9090abU, 0x0b888883U,
    0x8c4646caU, 0xc7eeee29U, 0x6bb8b8d3U, 0x2814143cU,
    0xa7dede79U, 0xbc5e5ee2U, 0x160b0b1dU, 0xaddbdb76U,
    0xdbe0e03bU, 0x64323256U, 0x743a3a4eU, 0x140a0a1eU,
    0x924949dbU, 0x0c06060aU, 0x4824246cU, 0xb85c5ce4U,
    0x9fc2c25dU, 0xbdd3d36eU, 0x43acacefU, 0xc46262a6U,
    0x399191a8U, 0x319595a4U, 0xd3e4e437U, 0xf279798bU,
    0xd5e7e732U, 0x8bc8c843U, 0x6e373759U, 0xda6d6db7U,
    0x018d8d8cU, 0xb1d5d564U, 0x9c4e4ed2U, 0x49a9a9e0U,
    0xd86c6cb4U, 0xac5656faU, 0xf3f4f407U, 0xcfeaea25U,
    0xca6565afU, 0xf47a7a8eU, 0x47aeaee9U, 0x10080818U,
    0x6fbabad5U, 0xf0787888U, 0x4a25256fU, 0x5c2e2e72U,
    0x381c1c24U, 0x57a6a6f1U, 0x73b4b4c7U, 0x97c6c651U,
    0xcbe8e823U, 0xa1dddd7cU, 0xe874749cU, 0x3e1f1f21U,
    0x964b4bddU, 0x61bdbddcU, 0x0d8b8b86U, 0x0f8a8a85U,
    0xe0707090U, 0x7c3e3e42U, 0x71b5b5c4U, 0xcc6666aaU,
    0x904848d8U, 0x06030305U, 0xf7f6f601U, 0x1c0e0e12U,
    0xc26161a3U, 0x6a35355fU, 0xae5757f9U, 0x69b9b9d0U,
    0x17868691U, 0x99c1c158U, 0x3a1d1d27U, 0x279e9eb9U,
    0xd9e1e138U, 0xebf8f813U, 0x2b9898b3U, 0x22111133U,
    0xd26969bbU, 0xa9d9d970U, 0x078e8e89U, 0x339494a7U,
    0x2d9b9bb6U, 0x3c1e1e22U, 0x15878792U, 0xc9e9e920U,
    0x87cece49U, 0xaa5555ffU, 0x50282878U, 0xa5dfdf7aU,
    0x038c8c8fU, 0x59a1a1f8U, 0x09898980U, 0x1a0d0d17U,
    0x65bfbfdaU, 0xd7e6e631U, 0x844242c6U, 0xd06868b8U,
    0x824141c3U, 0x299999b0U, 0x5a2d2d77U, 0x1e0f0f11U,
    0x7bb0b0cbU, 0xa85454fcU, 0x6dbbbbd6U, 0x2c16163aU,
};
static const WE_UINT32 Te1[256] = 
{
    0xa5c66363U, 0x84f87c7cU, 0x99ee7777U, 0x8df67b7bU,
    0x0dfff2f2U, 0xbdd66b6bU, 0xb1de6f6fU, 0x5491c5c5U,
    0x50603030U, 0x03020101U, 0xa9ce6767U, 0x7d562b2bU,
    0x19e7fefeU, 0x62b5d7d7U, 0xe64dababU, 0x9aec7676U,
    0x458fcacaU, 0x9d1f8282U, 0x4089c9c9U, 0x87fa7d7dU,
    0x15effafaU, 0xebb25959U, 0xc98e4747U, 0x0bfbf0f0U,
    0xec41adadU, 0x67b3d4d4U, 0xfd5fa2a2U, 0xea45afafU,
    0xbf239c9cU, 0xf753a4a4U, 0x96e47272U, 0x5b9bc0c0U,
    0xc275b7b7U, 0x1ce1fdfdU, 0xae3d9393U, 0x6a4c2626U,
    0x5a6c3636U, 0x417e3f3fU, 0x02f5f7f7U, 0x4f83ccccU,
    0x5c683434U, 0xf451a5a5U, 0x34d1e5e5U, 0x08f9f1f1U,
    0x93e27171U, 0x73abd8d8U, 0x53623131U, 0x3f2a1515U,
    0x0c080404U, 0x5295c7c7U, 0x65462323U, 0x5e9dc3c3U,
    0x28301818U, 0xa1379696U, 0x0f0a0505U, 0xb52f9a9aU,
    0x090e0707U, 0x36241212U, 0x9b1b8080U, 0x3ddfe2e2U,
    0x26cdebebU, 0x694e2727U, 0xcd7fb2b2U, 0x9fea7575U,
    0x1b120909U, 0x9e1d8383U, 0x74582c2cU, 0x2e341a1aU,
    0x2d361b1bU, 0xb2dc6e6eU, 0xeeb45a5aU, 0xfb5ba0a0U,
    0xf6a45252U, 0x4d763b3bU, 0x61b7d6d6U, 0xce7db3b3U,
    0x7b522929U, 0x3edde3e3U, 0x715e2f2fU, 0x97138484U,
    0xf5a65353U, 0x68b9d1d1U, 0x00000000U, 0x2cc1ededU,
    0x60402020U, 0x1fe3fcfcU, 0xc879b1b1U, 0xedb65b5bU,
    0xbed46a6aU, 0x468dcbcbU, 0xd967bebeU, 0x4b723939U,
    0xde944a4aU, 0xd4984c4cU, 0xe8b05858U, 0x4a85cfcfU,
    0x6bbbd0d0U, 0x2ac5efefU, 0xe54faaaaU, 0x16edfbfbU,
    0xc5864343U, 0xd79a4d4dU, 0x55663333U, 0x94118585U,
    0xcf8a4545U, 0x10e9f9f9U, 0x06040202U, 0x81fe7f7fU,
    0xf0a05050U, 0x44783c3cU, 0xba259f9fU, 0xe34ba8a8U,
    0xf3a25151U, 0xfe5da3a3U, 0xc0804040U, 0x8a058f8fU,
    0xad3f9292U, 0xbc219d9dU, 0x48703838U, 0x04f1f5f5U,
    0xdf63bcbcU, 0xc177b6b6U, 0x75afdadaU, 0x63422121U,
    0x30201010U, 0x1ae5ffffU, 0x0efdf3f3U, 0x6dbfd2d2U,
    0x4c81cdcdU, 0x14180c0cU, 0x35261313U, 0x2fc3ececU,
    0xe1be5f5fU, 0xa2359797U, 0xcc884444U, 0x392e1717U,
    0x5793c4c4U, 0xf255a7a7U, 0x82fc7e7eU, 0x477a3d3dU,
    0xacc86464U, 0xe7ba5d5dU, 0x2b321919U, 0x95e67373U,
    0xa0c06060U, 0x98198181U, 0xd19e4f4fU, 0x7fa3dcdcU,
    0x66442222U, 0x7e542a2aU, 0xab3b9090U, 0x830b8888U,
    0xca8c4646U, 0x29c7eeeeU, 0xd36bb8b8U, 0x3c281414U,
    0x79a7dedeU, 0xe2bc5e5eU, 0x1d160b0bU, 0x76addbdbU,
    0x3bdbe0e0U, 0x56643232U, 0x4e743a3aU, 0x1e140a0aU,
    0xdb924949U, 0x0a0c0606U, 0x6c482424U, 0xe4b85c5cU,
    0x5d9fc2c2U, 0x6ebdd3d3U, 0xef43acacU, 0xa6c46262U,
    0xa8399191U, 0xa4319595U, 0x37d3e4e4U, 0x8bf27979U,
    0x32d5e7e7U, 0x438bc8c8U, 0x596e3737U, 0xb7da6d6dU,
    0x8c018d8dU, 0x64b1d5d5U, 0xd29c4e4eU, 0xe049a9a9U,
    0xb4d86c6cU, 0xfaac5656U, 0x07f3f4f4U, 0x25cfeaeaU,
    0xafca6565U, 0x8ef47a7aU, 0xe947aeaeU, 0x18100808U,
    0xd56fbabaU, 0x88f07878U, 0x6f4a2525U, 0x725c2e2eU,
    0x24381c1cU, 0xf157a6a6U, 0xc773b4b4U, 0x5197c6c6U,
    0x23cbe8e8U, 0x7ca1ddddU, 0x9ce87474U, 0x213e1f1fU,
    0xdd964b4bU, 0xdc61bdbdU, 0x860d8b8bU, 0x850f8a8aU,
    0x90e07070U, 0x427c3e3eU, 0xc471b5b5U, 0xaacc6666U,
    0xd8904848U, 0x05060303U, 0x01f7f6f6U, 0x121c0e0eU,
    0xa3c26161U, 0x5f6a3535U, 0xf9ae5757U, 0xd069b9b9U,
    0x91178686U, 0x5899c1c1U, 0x273a1d1dU, 0xb9279e9eU,
    0x38d9e1e1U, 0x13ebf8f8U, 0xb32b9898U, 0x33221111U,
    0xbbd26969U, 0x70a9d9d9U, 0x89078e8eU, 0xa7339494U,
    0xb62d9b9bU, 0x223c1e1eU, 0x92158787U, 0x20c9e9e9U,
    0x4987ceceU, 0xffaa5555U, 0x78502828U, 0x7aa5dfdfU,
    0x8f038c8cU, 0xf859a1a1U, 0x80098989U, 0x171a0d0dU,
    0xda65bfbfU, 0x31d7e6e6U, 0xc6844242U, 0xb8d06868U,
    0xc3824141U, 0xb0299999U, 0x775a2d2dU, 0x111e0f0fU,
    0xcb7bb0b0U, 0xfca85454U, 0xd66dbbbbU, 0x3a2c1616U,
};
static const WE_UINT32 Te2[256] = 
{
    0x63a5c663U, 0x7c84f87cU, 0x7799ee77U, 0x7b8df67bU,
    0xf20dfff2U, 0x6bbdd66bU, 0x6fb1de6fU, 0xc55491c5U,
    0x30506030U, 0x01030201U, 0x67a9ce67U, 0x2b7d562bU,
    0xfe19e7feU, 0xd762b5d7U, 0xabe64dabU, 0x769aec76U,
    0xca458fcaU, 0x829d1f82U, 0xc94089c9U, 0x7d87fa7dU,
    0xfa15effaU, 0x59ebb259U, 0x47c98e47U, 0xf00bfbf0U,
    0xadec41adU, 0xd467b3d4U, 0xa2fd5fa2U, 0xafea45afU,
    0x9cbf239cU, 0xa4f753a4U, 0x7296e472U, 0xc05b9bc0U,
    0xb7c275b7U, 0xfd1ce1fdU, 0x93ae3d93U, 0x266a4c26U,
    0x365a6c36U, 0x3f417e3fU, 0xf702f5f7U, 0xcc4f83ccU,
    0x345c6834U, 0xa5f451a5U, 0xe534d1e5U, 0xf108f9f1U,
    0x7193e271U, 0xd873abd8U, 0x31536231U, 0x153f2a15U,
    0x040c0804U, 0xc75295c7U, 0x23654623U, 0xc35e9dc3U,
    0x18283018U, 0x96a13796U, 0x050f0a05U, 0x9ab52f9aU,
    0x07090e07U, 0x12362412U, 0x809b1b80U, 0xe23ddfe2U,
    0xeb26cdebU, 0x27694e27U, 0xb2cd7fb2U, 0x759fea75U,
    0x091b1209U, 0x839e1d83U, 0x2c74582cU, 0x1a2e341aU,
    0x1b2d361bU, 0x6eb2dc6eU, 0x5aeeb45aU, 0xa0fb5ba0U,
    0x52f6a452U, 0x3b4d763bU, 0xd661b7d6U, 0xb3ce7db3U,
    0x297b5229U, 0xe33edde3U, 0x2f715e2fU, 0x84971384U,
    0x53f5a653U, 0xd168b9d1U, 0x00000000U, 0xed2cc1edU,
    0x20604020U, 0xfc1fe3fcU, 0xb1c879b1U, 0x5bedb65bU,
    0x6abed46aU, 0xcb468dcbU, 0xbed967beU, 0x394b7239U,
    0x4ade944aU, 0x4cd4984cU, 0x58e8b058U, 0xcf4a85cfU,
    0xd06bbbd0U, 0xef2ac5efU, 0xaae54faaU, 0xfb16edfbU,
    0x43c58643U, 0x4dd79a4dU, 0x33556633U, 0x85941185U,
    0x45cf8a45U, 0xf910e9f9U, 0x02060402U, 0x7f81fe7fU,
    0x50f0a050U, 0x3c44783cU, 0x9fba259fU, 0xa8e34ba8U,
    0x51f3a251U, 0xa3fe5da3U, 0x40c08040U, 0x8f8a058fU,
    0x92ad3f92U, 0x9dbc219dU, 0x38487038U, 0xf504f1f5U,
    0xbcdf63bcU, 0xb6c177b6U, 0xda75afdaU, 0x21634221U,
    0x10302010U, 0xff1ae5ffU, 0xf30efdf3U, 0xd26dbfd2U,
    0xcd4c81cdU, 0x0c14180cU, 0x13352613U, 0xec2fc3ecU,
    0x5fe1be5fU, 0x97a23597U, 0x44cc8844U, 0x17392e17U,
    0xc45793c4U, 0xa7f255a7U, 0x7e82fc7eU, 0x3d477a3dU,
    0x64acc864U, 0x5de7ba5dU, 0x192b3219U, 0x7395e673U,
    0x60a0c060U, 0x81981981U, 0x4fd19e4fU, 0xdc7fa3dcU,
    0x22664422U, 0x2a7e542aU, 0x90ab3b90U, 0x88830b88U,
    0x46ca8c46U, 0xee29c7eeU, 0xb8d36bb8U, 0x143c2814U,
    0xde79a7deU, 0x5ee2bc5eU, 0x0b1d160bU, 0xdb76addbU,
    0xe03bdbe0U, 0x32566432U, 0x3a4e743aU, 0x0a1e140aU,
    0x49db9249U, 0x060a0c06U, 0x246c4824U, 0x5ce4b85cU,
    0xc25d9fc2U, 0xd36ebdd3U, 0xacef43acU, 0x62a6c462U,
    0x91a83991U, 0x95a43195U, 0xe437d3e4U, 0x798bf279U,
    0xe732d5e7U, 0xc8438bc8U, 0x37596e37U, 0x6db7da6dU,
    0x8d8c018dU, 0xd564b1d5U, 0x4ed29c4eU, 0xa9e049a9U,
    0x6cb4d86cU, 0x56faac56U, 0xf407f3f4U, 0xea25cfeaU,
    0x65afca65U, 0x7a8ef47aU, 0xaee947aeU, 0x08181008U,
    0xbad56fbaU, 0x7888f078U, 0x256f4a25U, 0x2e725c2eU,
    0x1c24381cU, 0xa6f157a6U, 0xb4c773b4U, 0xc65197c6U,
    0xe823cbe8U, 0xdd7ca1ddU, 0x749ce874U, 0x1f213e1fU,
    0x4bdd964bU, 0xbddc61bdU, 0x8b860d8bU, 0x8a850f8aU,
    0x7090e070U, 0x3e427c3eU, 0xb5c471b5U, 0x66aacc66U,
    0x48d89048U, 0x03050603U, 0xf601f7f6U, 0x0e121c0eU,
    0x61a3c261U, 0x355f6a35U, 0x57f9ae57U, 0xb9d069b9U,
    0x86911786U, 0xc15899c1U, 0x1d273a1dU, 0x9eb9279eU,
    0xe138d9e1U, 0xf813ebf8U, 0x98b32b98U, 0x11332211U,
    0x69bbd269U, 0xd970a9d9U, 0x8e89078eU, 0x94a73394U,
    0x9bb62d9bU, 0x1e223c1eU, 0x87921587U, 0xe920c9e9U,
    0xce4987ceU, 0x55ffaa55U, 0x28785028U, 0xdf7aa5dfU,
    0x8c8f038cU, 0xa1f859a1U, 0x89800989U, 0x0d171a0dU,
    0xbfda65bfU, 0xe631d7e6U, 0x42c68442U, 0x68b8d068U,
    0x41c38241U, 0x99b02999U, 0x2d775a2dU, 0x0f111e0fU,
    0xb0cb7bb0U, 0x54fca854U, 0xbbd66dbbU, 0x163a2c16U,
};
static const WE_UINT32 Te3[256] = 
{
    0x6363a5c6U, 0x7c7c84f8U, 0x777799eeU, 0x7b7b8df6U,
    0xf2f20dffU, 0x6b6bbdd6U, 0x6f6fb1deU, 0xc5c55491U,
    0x30305060U, 0x01010302U, 0x6767a9ceU, 0x2b2b7d56U,
    0xfefe19e7U, 0xd7d762b5U, 0xababe64dU, 0x76769aecU,
    0xcaca458fU, 0x82829d1fU, 0xc9c94089U, 0x7d7d87faU,
    0xfafa15efU, 0x5959ebb2U, 0x4747c98eU, 0xf0f00bfbU,
    0xadadec41U, 0xd4d467b3U, 0xa2a2fd5fU, 0xafafea45U,
    0x9c9cbf23U, 0xa4a4f753U, 0x727296e4U, 0xc0c05b9bU,
    0xb7b7c275U, 0xfdfd1ce1U, 0x9393ae3dU, 0x26266a4cU,
    0x36365a6cU, 0x3f3f417eU, 0xf7f702f5U, 0xcccc4f83U,
    0x34345c68U, 0xa5a5f451U, 0xe5e534d1U, 0xf1f108f9U,
    0x717193e2U, 0xd8d873abU, 0x31315362U, 0x15153f2aU,
    0x04040c08U, 0xc7c75295U, 0x23236546U, 0xc3c35e9dU,
    0x18182830U, 0x9696a137U, 0x05050f0aU, 0x9a9ab52fU,
    0x0707090eU, 0x12123624U, 0x80809b1bU, 0xe2e23ddfU,
    0xebeb26cdU, 0x2727694eU, 0xb2b2cd7fU, 0x75759feaU,
    0x09091b12U, 0x83839e1dU, 0x2c2c7458U, 0x1a1a2e34U,
    0x1b1b2d36U, 0x6e6eb2dcU, 0x5a5aeeb4U, 0xa0a0fb5bU,
    0x5252f6a4U, 0x3b3b4d76U, 0xd6d661b7U, 0xb3b3ce7dU,
    0x29297b52U, 0xe3e33eddU, 0x2f2f715eU, 0x84849713U,
    0x5353f5a6U, 0xd1d168b9U, 0x00000000U, 0xeded2cc1U,
    0x20206040U, 0xfcfc1fe3U, 0xb1b1c879U, 0x5b5bedb6U,
    0x6a6abed4U, 0xcbcb468dU, 0xbebed967U, 0x39394b72U,
    0x4a4ade94U, 0x4c4cd498U, 0x5858e8b0U, 0xcfcf4a85U,
    0xd0d06bbbU, 0xefef2ac5U, 0xaaaae54fU, 0xfbfb16edU,
    0x4343c586U, 0x4d4dd79aU, 0x33335566U, 0x85859411U,
    0x4545cf8aU, 0xf9f910e9U, 0x02020604U, 0x7f7f81feU,
    0x5050f0a0U, 0x3c3c4478U, 0x9f9fba25U, 0xa8a8e34bU,
    0x5151f3a2U, 0xa3a3fe5dU, 0x4040c080U, 0x8f8f8a05U,
    0x9292ad3fU, 0x9d9dbc21U, 0x38384870U, 0xf5f504f1U,
    0xbcbcdf63U, 0xb6b6c177U, 0xdada75afU, 0x21216342U,
    0x10103020U, 0xffff1ae5U, 0xf3f30efdU, 0xd2d26dbfU,
    0xcdcd4c81U, 0x0c0c1418U, 0x13133526U, 0xecec2fc3U,
    0x5f5fe1beU, 0x9797a235U, 0x4444cc88U, 0x1717392eU,
    0xc4c45793U, 0xa7a7f255U, 0x7e7e82fcU, 0x3d3d477aU,
    0x6464acc8U, 0x5d5de7baU, 0x19192b32U, 0x737395e6U,
    0x6060a0c0U, 0x81819819U, 0x4f4fd19eU, 0xdcdc7fa3U,
    0x22226644U, 0x2a2a7e54U, 0x9090ab3bU, 0x8888830bU,
    0x4646ca8cU, 0xeeee29c7U, 0xb8b8d36bU, 0x14143c28U,
    0xdede79a7U, 0x5e5ee2bcU, 0x0b0b1d16U, 0xdbdb76adU,
    0xe0e03bdbU, 0x32325664U, 0x3a3a4e74U, 0x0a0a1e14U,
    0x4949db92U, 0x06060a0cU, 0x24246c48U, 0x5c5ce4b8U,
    0xc2c25d9fU, 0xd3d36ebdU, 0xacacef43U, 0x6262a6c4U,
    0x9191a839U, 0x9595a431U, 0xe4e437d3U, 0x79798bf2U,
    0xe7e732d5U, 0xc8c8438bU, 0x3737596eU, 0x6d6db7daU,
    0x8d8d8c01U, 0xd5d564b1U, 0x4e4ed29cU, 0xa9a9e049U,
    0x6c6cb4d8U, 0x5656faacU, 0xf4f407f3U, 0xeaea25cfU,
    0x6565afcaU, 0x7a7a8ef4U, 0xaeaee947U, 0x08081810U,
    0xbabad56fU, 0x787888f0U, 0x25256f4aU, 0x2e2e725cU,
    0x1c1c2438U, 0xa6a6f157U, 0xb4b4c773U, 0xc6c65197U,
    0xe8e823cbU, 0xdddd7ca1U, 0x74749ce8U, 0x1f1f213eU,
    0x4b4bdd96U, 0xbdbddc61U, 0x8b8b860dU, 0x8a8a850fU,
    0x707090e0U, 0x3e3e427cU, 0xb5b5c471U, 0x6666aaccU,
    0x4848d890U, 0x03030506U, 0xf6f601f7U, 0x0e0e121cU,
    0x6161a3c2U, 0x35355f6aU, 0x5757f9aeU, 0xb9b9d069U,
    0x86869117U, 0xc1c15899U, 0x1d1d273aU, 0x9e9eb927U,
    0xe1e138d9U, 0xf8f813ebU, 0x9898b32bU, 0x11113322U,
    0x6969bbd2U, 0xd9d970a9U, 0x8e8e8907U, 0x9494a733U,
    0x9b9bb62dU, 0x1e1e223cU, 0x87879215U, 0xe9e920c9U,
    0xcece4987U, 0x5555ffaaU, 0x28287850U, 0xdfdf7aa5U,
    0x8c8c8f03U, 0xa1a1f859U, 0x89898009U, 0x0d0d171aU,
    0xbfbfda65U, 0xe6e631d7U, 0x4242c684U, 0x6868b8d0U,
    0x4141c382U, 0x9999b029U, 0x2d2d775aU, 0x0f0f111eU,
    0xb0b0cb7bU, 0x5454fca8U, 0xbbbbd66dU, 0x16163a2cU,
};
static const WE_UINT32 Te4[256] = 
{
    0x63636363U, 0x7c7c7c7cU, 0x77777777U, 0x7b7b7b7bU,
    0xf2f2f2f2U, 0x6b6b6b6bU, 0x6f6f6f6fU, 0xc5c5c5c5U,
    0x30303030U, 0x01010101U, 0x67676767U, 0x2b2b2b2bU,
    0xfefefefeU, 0xd7d7d7d7U, 0xababababU, 0x76767676U,
    0xcacacacaU, 0x82828282U, 0xc9c9c9c9U, 0x7d7d7d7dU,
    0xfafafafaU, 0x59595959U, 0x47474747U, 0xf0f0f0f0U,
    0xadadadadU, 0xd4d4d4d4U, 0xa2a2a2a2U, 0xafafafafU,
    0x9c9c9c9cU, 0xa4a4a4a4U, 0x72727272U, 0xc0c0c0c0U,
    0xb7b7b7b7U, 0xfdfdfdfdU, 0x93939393U, 0x26262626U,
    0x36363636U, 0x3f3f3f3fU, 0xf7f7f7f7U, 0xccccccccU,
    0x34343434U, 0xa5a5a5a5U, 0xe5e5e5e5U, 0xf1f1f1f1U,
    0x71717171U, 0xd8d8d8d8U, 0x31313131U, 0x15151515U,
    0x04040404U, 0xc7c7c7c7U, 0x23232323U, 0xc3c3c3c3U,
    0x18181818U, 0x96969696U, 0x05050505U, 0x9a9a9a9aU,
    0x07070707U, 0x12121212U, 0x80808080U, 0xe2e2e2e2U,
    0xebebebebU, 0x27272727U, 0xb2b2b2b2U, 0x75757575U,
    0x09090909U, 0x83838383U, 0x2c2c2c2cU, 0x1a1a1a1aU,
    0x1b1b1b1bU, 0x6e6e6e6eU, 0x5a5a5a5aU, 0xa0a0a0a0U,
    0x52525252U, 0x3b3b3b3bU, 0xd6d6d6d6U, 0xb3b3b3b3U,
    0x29292929U, 0xe3e3e3e3U, 0x2f2f2f2fU, 0x84848484U,
    0x53535353U, 0xd1d1d1d1U, 0x00000000U, 0xededededU,
    0x20202020U, 0xfcfcfcfcU, 0xb1b1b1b1U, 0x5b5b5b5bU,
    0x6a6a6a6aU, 0xcbcbcbcbU, 0xbebebebeU, 0x39393939U,
    0x4a4a4a4aU, 0x4c4c4c4cU, 0x58585858U, 0xcfcfcfcfU,
    0xd0d0d0d0U, 0xefefefefU, 0xaaaaaaaaU, 0xfbfbfbfbU,
    0x43434343U, 0x4d4d4d4dU, 0x33333333U, 0x85858585U,
    0x45454545U, 0xf9f9f9f9U, 0x02020202U, 0x7f7f7f7fU,
    0x50505050U, 0x3c3c3c3cU, 0x9f9f9f9fU, 0xa8a8a8a8U,
    0x51515151U, 0xa3a3a3a3U, 0x40404040U, 0x8f8f8f8fU,
    0x92929292U, 0x9d9d9d9dU, 0x38383838U, 0xf5f5f5f5U,
    0xbcbcbcbcU, 0xb6b6b6b6U, 0xdadadadaU, 0x21212121U,
    0x10101010U, 0xffffffffU, 0xf3f3f3f3U, 0xd2d2d2d2U,
    0xcdcdcdcdU, 0x0c0c0c0cU, 0x13131313U, 0xececececU,
    0x5f5f5f5fU, 0x97979797U, 0x44444444U, 0x17171717U,
    0xc4c4c4c4U, 0xa7a7a7a7U, 0x7e7e7e7eU, 0x3d3d3d3dU,
    0x64646464U, 0x5d5d5d5dU, 0x19191919U, 0x73737373U,
    0x60606060U, 0x81818181U, 0x4f4f4f4fU, 0xdcdcdcdcU,
    0x22222222U, 0x2a2a2a2aU, 0x90909090U, 0x88888888U,
    0x46464646U, 0xeeeeeeeeU, 0xb8b8b8b8U, 0x14141414U,
    0xdedededeU, 0x5e5e5e5eU, 0x0b0b0b0bU, 0xdbdbdbdbU,
    0xe0e0e0e0U, 0x32323232U, 0x3a3a3a3aU, 0x0a0a0a0aU,
    0x49494949U, 0x06060606U, 0x24242424U, 0x5c5c5c5cU,
    0xc2c2c2c2U, 0xd3d3d3d3U, 0xacacacacU, 0x62626262U,
    0x91919191U, 0x95959595U, 0xe4e4e4e4U, 0x79797979U,
    0xe7e7e7e7U, 0xc8c8c8c8U, 0x37373737U, 0x6d6d6d6dU,
    0x8d8d8d8dU, 0xd5d5d5d5U, 0x4e4e4e4eU, 0xa9a9a9a9U,
    0x6c6c6c6cU, 0x56565656U, 0xf4f4f4f4U, 0xeaeaeaeaU,
    0x65656565U, 0x7a7a7a7aU, 0xaeaeaeaeU, 0x08080808U,
    0xbabababaU, 0x78787878U, 0x25252525U, 0x2e2e2e2eU,
    0x1c1c1c1cU, 0xa6a6a6a6U, 0xb4b4b4b4U, 0xc6c6c6c6U,
    0xe8e8e8e8U, 0xddddddddU, 0x74747474U, 0x1f1f1f1fU,
    0x4b4b4b4bU, 0xbdbdbdbdU, 0x8b8b8b8bU, 0x8a8a8a8aU,
    0x70707070U, 0x3e3e3e3eU, 0xb5b5b5b5U, 0x66666666U,
    0x48484848U, 0x03030303U, 0xf6f6f6f6U, 0x0e0e0e0eU,
    0x61616161U, 0x35353535U, 0x57575757U, 0xb9b9b9b9U,
    0x86868686U, 0xc1c1c1c1U, 0x1d1d1d1dU, 0x9e9e9e9eU,
    0xe1e1e1e1U, 0xf8f8f8f8U, 0x98989898U, 0x11111111U,
    0x69696969U, 0xd9d9d9d9U, 0x8e8e8e8eU, 0x94949494U,
    0x9b9b9b9bU, 0x1e1e1e1eU, 0x87878787U, 0xe9e9e9e9U,
    0xcecececeU, 0x55555555U, 0x28282828U, 0xdfdfdfdfU,
    0x8c8c8c8cU, 0xa1a1a1a1U, 0x89898989U, 0x0d0d0d0dU,
    0xbfbfbfbfU, 0xe6e6e6e6U, 0x42424242U, 0x68686868U,
    0x41414141U, 0x99999999U, 0x2d2d2d2dU, 0x0f0f0f0fU,
    0xb0b0b0b0U, 0x54545454U, 0xbbbbbbbbU, 0x16161616U,
};
static const WE_UINT32 Td0[256] = 
{
    0x51f4a750U, 0x7e416553U, 0x1a17a4c3U, 0x3a275e96U,
    0x3bab6bcbU, 0x1f9d45f1U, 0xacfa58abU, 0x4be30393U,
    0x2030fa55U, 0xad766df6U, 0x88cc7691U, 0xf5024c25U,
    0x4fe5d7fcU, 0xc52acbd7U, 0x26354480U, 0xb562a38fU,
    0xdeb15a49U, 0x25ba1b67U, 0x45ea0e98U, 0x5dfec0e1U,
    0xc32f7502U, 0x814cf012U, 0x8d4697a3U, 0x6bd3f9c6U,
    0x038f5fe7U, 0x15929c95U, 0xbf6d7aebU, 0x955259daU,
    0xd4be832dU, 0x587421d3U, 0x49e06929U, 0x8ec9c844U,
    0x75c2896aU, 0xf48e7978U, 0x99583e6bU, 0x27b971ddU,
    0xbee14fb6U, 0xf088ad17U, 0xc920ac66U, 0x7dce3ab4U,
    0x63df4a18U, 0xe51a3182U, 0x97513360U, 0x62537f45U,
    0xb16477e0U, 0xbb6bae84U, 0xfe81a01cU, 0xf9082b94U,
    0x70486858U, 0x8f45fd19U, 0x94de6c87U, 0x527bf8b7U,
    0xab73d323U, 0x724b02e2U, 0xe31f8f57U, 0x6655ab2aU,
    0xb2eb2807U, 0x2fb5c203U, 0x86c57b9aU, 0xd33708a5U,
    0x302887f2U, 0x23bfa5b2U, 0x02036abaU, 0xed16825cU,
    0x8acf1c2bU, 0xa779b492U, 0xf307f2f0U, 0x4e69e2a1U,
    0x65daf4cdU, 0x0605bed5U, 0xd134621fU, 0xc4a6fe8aU,
    0x342e539dU, 0xa2f355a0U, 0x058ae132U, 0xa4f6eb75U,
    0x0b83ec39U, 0x4060efaaU, 0x5e719f06U, 0xbd6e1051U,
    0x3e218af9U, 0x96dd063dU, 0xdd3e05aeU, 0x4de6bd46U,
    0x91548db5U, 0x71c45d05U, 0x0406d46fU, 0x605015ffU,
    0x1998fb24U, 0xd6bde997U, 0x894043ccU, 0x67d99e77U,
    0xb0e842bdU, 0x07898b88U, 0xe7195b38U, 0x79c8eedbU,
    0xa17c0a47U, 0x7c420fe9U, 0xf8841ec9U, 0x00000000U,
    0x09808683U, 0x322bed48U, 0x1e1170acU, 0x6c5a724eU,
    0xfd0efffbU, 0x0f853856U, 0x3daed51eU, 0x362d3927U,
    0x0a0fd964U, 0x685ca621U, 0x9b5b54d1U, 0x24362e3aU,
    0x0c0a67b1U, 0x9357e70fU, 0xb4ee96d2U, 0x1b9b919eU,
    0x80c0c54fU, 0x61dc20a2U, 0x5a774b69U, 0x1c121a16U,
    0xe293ba0aU, 0xc0a02ae5U, 0x3c22e043U, 0x121b171dU,
    0x0e090d0bU, 0xf28bc7adU, 0x2db6a8b9U, 0x141ea9c8U,
    0x57f11985U, 0xaf75074cU, 0xee99ddbbU, 0xa37f60fdU,
    0xf701269fU, 0x5c72f5bcU, 0x44663bc5U, 0x5bfb7e34U,
    0x8b432976U, 0xcb23c6dcU, 0xb6edfc68U, 0xb8e4f163U,
    0xd731dccaU, 0x42638510U, 0x13972240U, 0x84c61120U,
    0x854a247dU, 0xd2bb3df8U, 0xaef93211U, 0xc729a16dU,
    0x1d9e2f4bU, 0xdcb230f3U, 0x0d8652ecU, 0x77c1e3d0U,
    0x2bb3166cU, 0xa970b999U, 0x119448faU, 0x47e96422U,
    0xa8fc8cc4U, 0xa0f03f1aU, 0x567d2cd8U, 0x223390efU,
    0x87494ec7U, 0xd938d1c1U, 0x8ccaa2feU, 0x98d40b36U,
    0xa6f581cfU, 0xa57ade28U, 0xdab78e26U, 0x3fadbfa4U,
    0x2c3a9de4U, 0x5078920dU, 0x6a5fcc9bU, 0x547e4662U,
    0xf68d13c2U, 0x90d8b8e8U, 0x2e39f75eU, 0x82c3aff5U,
    0x9f5d80beU, 0x69d0937cU, 0x6fd52da9U, 0xcf2512b3U,
    0xc8ac993bU, 0x10187da7U, 0xe89c636eU, 0xdb3bbb7bU,
    0xcd267809U, 0x6e5918f4U, 0xec9ab701U, 0x834f9aa8U,
    0xe6956e65U, 0xaaffe67eU, 0x21bccf08U, 0xef15e8e6U,
    0xbae79bd9U, 0x4a6f36ceU, 0xea9f09d4U, 0x29b07cd6U,
    0x31a4b2afU, 0x2a3f2331U, 0xc6a59430U, 0x35a266c0U,
    0x744ebc37U, 0xfc82caa6U, 0xe090d0b0U, 0x33a7d815U,
    0xf104984aU, 0x41ecdaf7U, 0x7fcd500eU, 0x1791f62fU,
    0x764dd68dU, 0x43efb04dU, 0xccaa4d54U, 0xe49604dfU,
    0x9ed1b5e3U, 0x4c6a881bU, 0xc12c1fb8U, 0x4665517fU,
    0x9d5eea04U, 0x018c355dU, 0xfa877473U, 0xfb0b412eU,
    0xb3671d5aU, 0x92dbd252U, 0xe9105633U, 0x6dd64713U,
    0x9ad7618cU, 0x37a10c7aU, 0x59f8148eU, 0xeb133c89U,
    0xcea927eeU, 0xb761c935U, 0xe11ce5edU, 0x7a47b13cU,
    0x9cd2df59U, 0x55f2733fU, 0x1814ce79U, 0x73c737bfU,
    0x53f7cdeaU, 0x5ffdaa5bU, 0xdf3d6f14U, 0x7844db86U,
    0xcaaff381U, 0xb968c43eU, 0x3824342cU, 0xc2a3405fU,
    0x161dc372U, 0xbce2250cU, 0x283c498bU, 0xff0d9541U,
    0x39a80171U, 0x080cb3deU, 0xd8b4e49cU, 0x6456c190U,
    0x7bcb8461U, 0xd532b670U, 0x486c5c74U, 0xd0b85742U,
};
static const WE_UINT32 Td1[256] = 
{
    0x5051f4a7U, 0x537e4165U, 0xc31a17a4U, 0x963a275eU,
    0xcb3bab6bU, 0xf11f9d45U, 0xabacfa58U, 0x934be303U,
    0x552030faU, 0xf6ad766dU, 0x9188cc76U, 0x25f5024cU,
    0xfc4fe5d7U, 0xd7c52acbU, 0x80263544U, 0x8fb562a3U,
    0x49deb15aU, 0x6725ba1bU, 0x9845ea0eU, 0xe15dfec0U,
    0x02c32f75U, 0x12814cf0U, 0xa38d4697U, 0xc66bd3f9U,
    0xe7038f5fU, 0x9515929cU, 0xebbf6d7aU, 0xda955259U,
    0x2dd4be83U, 0xd3587421U, 0x2949e069U, 0x448ec9c8U,
    0x6a75c289U, 0x78f48e79U, 0x6b99583eU, 0xdd27b971U,
    0xb6bee14fU, 0x17f088adU, 0x66c920acU, 0xb47dce3aU,
    0x1863df4aU, 0x82e51a31U, 0x60975133U, 0x4562537fU,
    0xe0b16477U, 0x84bb6baeU, 0x1cfe81a0U, 0x94f9082bU,
    0x58704868U, 0x198f45fdU, 0x8794de6cU, 0xb7527bf8U,
    0x23ab73d3U, 0xe2724b02U, 0x57e31f8fU, 0x2a6655abU,
    0x07b2eb28U, 0x032fb5c2U, 0x9a86c57bU, 0xa5d33708U,
    0xf2302887U, 0xb223bfa5U, 0xba02036aU, 0x5ced1682U,
    0x2b8acf1cU, 0x92a779b4U, 0xf0f307f2U, 0xa14e69e2U,
    0xcd65daf4U, 0xd50605beU, 0x1fd13462U, 0x8ac4a6feU,
    0x9d342e53U, 0xa0a2f355U, 0x32058ae1U, 0x75a4f6ebU,
    0x390b83ecU, 0xaa4060efU, 0x065e719fU, 0x51bd6e10U,
    0xf93e218aU, 0x3d96dd06U, 0xaedd3e05U, 0x464de6bdU,
    0xb591548dU, 0x0571c45dU, 0x6f0406d4U, 0xff605015U,
    0x241998fbU, 0x97d6bde9U, 0xcc894043U, 0x7767d99eU,
    0xbdb0e842U, 0x8807898bU, 0x38e7195bU, 0xdb79c8eeU,
    0x47a17c0aU, 0xe97c420fU, 0xc9f8841eU, 0x00000000U,
    0x83098086U, 0x48322bedU, 0xac1e1170U, 0x4e6c5a72U,
    0xfbfd0effU, 0x560f8538U, 0x1e3daed5U, 0x27362d39U,
    0x640a0fd9U, 0x21685ca6U, 0xd19b5b54U, 0x3a24362eU,
    0xb10c0a67U, 0x0f9357e7U, 0xd2b4ee96U, 0x9e1b9b91U,
    0x4f80c0c5U, 0xa261dc20U, 0x695a774bU, 0x161c121aU,
    0x0ae293baU, 0xe5c0a02aU, 0x433c22e0U, 0x1d121b17U,
    0x0b0e090dU, 0xadf28bc7U, 0xb92db6a8U, 0xc8141ea9U,
    0x8557f119U, 0x4caf7507U, 0xbbee99ddU, 0xfda37f60U,
    0x9ff70126U, 0xbc5c72f5U, 0xc544663bU, 0x345bfb7eU,
    0x768b4329U, 0xdccb23c6U, 0x68b6edfcU, 0x63b8e4f1U,
    0xcad731dcU, 0x10426385U, 0x40139722U, 0x2084c611U,
    0x7d854a24U, 0xf8d2bb3dU, 0x11aef932U, 0x6dc729a1U,
    0x4b1d9e2fU, 0xf3dcb230U, 0xec0d8652U, 0xd077c1e3U,
    0x6c2bb316U, 0x99a970b9U, 0xfa119448U, 0x2247e964U,
    0xc4a8fc8cU, 0x1aa0f03fU, 0xd8567d2cU, 0xef223390U,
    0xc787494eU, 0xc1d938d1U, 0xfe8ccaa2U, 0x3698d40bU,
    0xcfa6f581U, 0x28a57adeU, 0x26dab78eU, 0xa43fadbfU,
    0xe42c3a9dU, 0x0d507892U, 0x9b6a5fccU, 0x62547e46U,
    0xc2f68d13U, 0xe890d8b8U, 0x5e2e39f7U, 0xf582c3afU,
    0xbe9f5d80U, 0x7c69d093U, 0xa96fd52dU, 0xb3cf2512U,
    0x3bc8ac99U, 0xa710187dU, 0x6ee89c63U, 0x7bdb3bbbU,
    0x09cd2678U, 0xf46e5918U, 0x01ec9ab7U, 0xa8834f9aU,
    0x65e6956eU, 0x7eaaffe6U, 0x0821bccfU, 0xe6ef15e8U,
    0xd9bae79bU, 0xce4a6f36U, 0xd4ea9f09U, 0xd629b07cU,
    0xaf31a4b2U, 0x312a3f23U, 0x30c6a594U, 0xc035a266U,
    0x37744ebcU, 0xa6fc82caU, 0xb0e090d0U, 0x1533a7d8U,
    0x4af10498U, 0xf741ecdaU, 0x0e7fcd50U, 0x2f1791f6U,
    0x8d764dd6U, 0x4d43efb0U, 0x54ccaa4dU, 0xdfe49604U,
    0xe39ed1b5U, 0x1b4c6a88U, 0xb8c12c1fU, 0x7f466551U,
    0x049d5eeaU, 0x5d018c35U, 0x73fa8774U, 0x2efb0b41U,
    0x5ab3671dU, 0x5292dbd2U, 0x33e91056U, 0x136dd647U,
    0x8c9ad761U, 0x7a37a10cU, 0x8e59f814U, 0x89eb133cU,
    0xeecea927U, 0x35b761c9U, 0xede11ce5U, 0x3c7a47b1U,
    0x599cd2dfU, 0x3f55f273U, 0x791814ceU, 0xbf73c737U,
    0xea53f7cdU, 0x5b5ffdaaU, 0x14df3d6fU, 0x867844dbU,
    0x81caaff3U, 0x3eb968c4U, 0x2c382434U, 0x5fc2a340U,
    0x72161dc3U, 0x0cbce225U, 0x8b283c49U, 0x41ff0d95U,
    0x7139a801U, 0xde080cb3U, 0x9cd8b4e4U, 0x906456c1U,
    0x617bcb84U, 0x70d532b6U, 0x74486c5cU, 0x42d0b857U,
};
static const WE_UINT32 Td2[256] = 
{
    0xa75051f4U, 0x65537e41U, 0xa4c31a17U, 0x5e963a27U,
    0x6bcb3babU, 0x45f11f9dU, 0x58abacfaU, 0x03934be3U,
    0xfa552030U, 0x6df6ad76U, 0x769188ccU, 0x4c25f502U,
    0xd7fc4fe5U, 0xcbd7c52aU, 0x44802635U, 0xa38fb562U,
    0x5a49deb1U, 0x1b6725baU, 0x0e9845eaU, 0xc0e15dfeU,
    0x7502c32fU, 0xf012814cU, 0x97a38d46U, 0xf9c66bd3U,
    0x5fe7038fU, 0x9c951592U, 0x7aebbf6dU, 0x59da9552U,
    0x832dd4beU, 0x21d35874U, 0x692949e0U, 0xc8448ec9U,
    0x896a75c2U, 0x7978f48eU, 0x3e6b9958U, 0x71dd27b9U,
    0x4fb6bee1U, 0xad17f088U, 0xac66c920U, 0x3ab47dceU,
    0x4a1863dfU, 0x3182e51aU, 0x33609751U, 0x7f456253U,
    0x77e0b164U, 0xae84bb6bU, 0xa01cfe81U, 0x2b94f908U,
    0x68587048U, 0xfd198f45U, 0x6c8794deU, 0xf8b7527bU,
    0xd323ab73U, 0x02e2724bU, 0x8f57e31fU, 0xab2a6655U,
    0x2807b2ebU, 0xc2032fb5U, 0x7b9a86c5U, 0x08a5d337U,
    0x87f23028U, 0xa5b223bfU, 0x6aba0203U, 0x825ced16U,
    0x1c2b8acfU, 0xb492a779U, 0xf2f0f307U, 0xe2a14e69U,
    0xf4cd65daU, 0xbed50605U, 0x621fd134U, 0xfe8ac4a6U,
    0x539d342eU, 0x55a0a2f3U, 0xe132058aU, 0xeb75a4f6U,
    0xec390b83U, 0xefaa4060U, 0x9f065e71U, 0x1051bd6eU,

    0x8af93e21U, 0x063d96ddU, 0x05aedd3eU, 0xbd464de6U,
    0x8db59154U, 0x5d0571c4U, 0xd46f0406U, 0x15ff6050U,
    0xfb241998U, 0xe997d6bdU, 0x43cc8940U, 0x9e7767d9U,
    0x42bdb0e8U, 0x8b880789U, 0x5b38e719U, 0xeedb79c8U,
    0x0a47a17cU, 0x0fe97c42U, 0x1ec9f884U, 0x00000000U,
    0x86830980U, 0xed48322bU, 0x70ac1e11U, 0x724e6c5aU,
    0xfffbfd0eU, 0x38560f85U, 0xd51e3daeU, 0x3927362dU,
    0xd9640a0fU, 0xa621685cU, 0x54d19b5bU, 0x2e3a2436U,
    0x67b10c0aU, 0xe70f9357U, 0x96d2b4eeU, 0x919e1b9bU,
    0xc54f80c0U, 0x20a261dcU, 0x4b695a77U, 0x1a161c12U,
    0xba0ae293U, 0x2ae5c0a0U, 0xe0433c22U, 0x171d121bU,
    0x0d0b0e09U, 0xc7adf28bU, 0xa8b92db6U, 0xa9c8141eU,
    0x198557f1U, 0x074caf75U, 0xddbbee99U, 0x60fda37fU,
    0x269ff701U, 0xf5bc5c72U, 0x3bc54466U, 0x7e345bfbU,
    0x29768b43U, 0xc6dccb23U, 0xfc68b6edU, 0xf163b8e4U,
    0xdccad731U, 0x85104263U, 0x22401397U, 0x112084c6U,
    0x247d854aU, 0x3df8d2bbU, 0x3211aef9U, 0xa16dc729U,
    0x2f4b1d9eU, 0x30f3dcb2U, 0x52ec0d86U, 0xe3d077c1U,
    0x166c2bb3U, 0xb999a970U, 0x48fa1194U, 0x642247e9U,
    0x8cc4a8fcU, 0x3f1aa0f0U, 0x2cd8567dU, 0x90ef2233U,
    0x4ec78749U, 0xd1c1d938U, 0xa2fe8ccaU, 0x0b3698d4U,
    0x81cfa6f5U, 0xde28a57aU, 0x8e26dab7U, 0xbfa43fadU,
    0x9de42c3aU, 0x920d5078U, 0xcc9b6a5fU, 0x4662547eU,
    0x13c2f68dU, 0xb8e890d8U, 0xf75e2e39U, 0xaff582c3U,
    0x80be9f5dU, 0x937c69d0U, 0x2da96fd5U, 0x12b3cf25U,
    0x993bc8acU, 0x7da71018U, 0x636ee89cU, 0xbb7bdb3bU,
    0x7809cd26U, 0x18f46e59U, 0xb701ec9aU, 0x9aa8834fU,
    0x6e65e695U, 0xe67eaaffU, 0xcf0821bcU, 0xe8e6ef15U,
    0x9bd9bae7U, 0x36ce4a6fU, 0x09d4ea9fU, 0x7cd629b0U,
    0xb2af31a4U, 0x23312a3fU, 0x9430c6a5U, 0x66c035a2U,
    0xbc37744eU, 0xcaa6fc82U, 0xd0b0e090U, 0xd81533a7U,
    0x984af104U, 0xdaf741ecU, 0x500e7fcdU, 0xf62f1791U,
    0xd68d764dU, 0xb04d43efU, 0x4d54ccaaU, 0x04dfe496U,
    0xb5e39ed1U, 0x881b4c6aU, 0x1fb8c12cU, 0x517f4665U,
    0xea049d5eU, 0x355d018cU, 0x7473fa87U, 0x412efb0bU,
    0x1d5ab367U, 0xd25292dbU, 0x5633e910U, 0x47136dd6U,
    0x618c9ad7U, 0x0c7a37a1U, 0x148e59f8U, 0x3c89eb13U,
    0x27eecea9U, 0xc935b761U, 0xe5ede11cU, 0xb13c7a47U,
    0xdf599cd2U, 0x733f55f2U, 0xce791814U, 0x37bf73c7U,
    0xcdea53f7U, 0xaa5b5ffdU, 0x6f14df3dU, 0xdb867844U,
    0xf381caafU, 0xc43eb968U, 0x342c3824U, 0x405fc2a3U,
    0xc372161dU, 0x250cbce2U, 0x498b283cU, 0x9541ff0dU,
    0x017139a8U, 0xb3de080cU, 0xe49cd8b4U, 0xc1906456U,
    0x84617bcbU, 0xb670d532U, 0x5c74486cU, 0x5742d0b8U,
};
static const WE_UINT32 Td3[256] = 
{
    0xf4a75051U, 0x4165537eU, 0x17a4c31aU, 0x275e963aU,
    0xab6bcb3bU, 0x9d45f11fU, 0xfa58abacU, 0xe303934bU,
    0x30fa5520U, 0x766df6adU, 0xcc769188U, 0x024c25f5U,
    0xe5d7fc4fU, 0x2acbd7c5U, 0x35448026U, 0x62a38fb5U,
    0xb15a49deU, 0xba1b6725U, 0xea0e9845U, 0xfec0e15dU,
    0x2f7502c3U, 0x4cf01281U, 0x4697a38dU, 0xd3f9c66bU,
    0x8f5fe703U, 0x929c9515U, 0x6d7aebbfU, 0x5259da95U,
    0xbe832dd4U, 0x7421d358U, 0xe0692949U, 0xc9c8448eU,
    0xc2896a75U, 0x8e7978f4U, 0x583e6b99U, 0xb971dd27U,
    0xe14fb6beU, 0x88ad17f0U, 0x20ac66c9U, 0xce3ab47dU,
    0xdf4a1863U, 0x1a3182e5U, 0x51336097U, 0x537f4562U,
    0x6477e0b1U, 0x6bae84bbU, 0x81a01cfeU, 0x082b94f9U,
    0x48685870U, 0x45fd198fU, 0xde6c8794U, 0x7bf8b752U,
    0x73d323abU, 0x4b02e272U, 0x1f8f57e3U, 0x55ab2a66U,
    0xeb2807b2U, 0xb5c2032fU, 0xc57b9a86U, 0x3708a5d3U,
    0x2887f230U, 0xbfa5b223U, 0x036aba02U, 0x16825cedU,
    0xcf1c2b8aU, 0x79b492a7U, 0x07f2f0f3U, 0x69e2a14eU,
    0xdaf4cd65U, 0x05bed506U, 0x34621fd1U, 0xa6fe8ac4U,
    0x2e539d34U, 0xf355a0a2U, 0x8ae13205U, 0xf6eb75a4U,
    0x83ec390bU, 0x60efaa40U, 0x719f065eU, 0x6e1051bdU,
    0x218af93eU, 0xdd063d96U, 0x3e05aeddU, 0xe6bd464dU,
    0x548db591U, 0xc45d0571U, 0x06d46f04U, 0x5015ff60U,
    0x98fb2419U, 0xbde997d6U, 0x4043cc89U, 0xd99e7767U,
    0xe842bdb0U, 0x898b8807U, 0x195b38e7U, 0xc8eedb79U,
    0x7c0a47a1U, 0x420fe97cU, 0x841ec9f8U, 0x00000000U,
    0x80868309U, 0x2bed4832U, 0x1170ac1eU, 0x5a724e6cU,
    0x0efffbfdU, 0x8538560fU, 0xaed51e3dU, 0x2d392736U,
    0x0fd9640aU, 0x5ca62168U, 0x5b54d19bU, 0x362e3a24U,
    0x0a67b10cU, 0x57e70f93U, 0xee96d2b4U, 0x9b919e1bU,
    0xc0c54f80U, 0xdc20a261U, 0x774b695aU, 0x121a161cU,
    0x93ba0ae2U, 0xa02ae5c0U, 0x22e0433cU, 0x1b171d12U,
    0x090d0b0eU, 0x8bc7adf2U, 0xb6a8b92dU, 0x1ea9c814U,
    0xf1198557U, 0x75074cafU, 0x99ddbbeeU, 0x7f60fda3U,
    0x01269ff7U, 0x72f5bc5cU, 0x663bc544U, 0xfb7e345bU,
    0x4329768bU, 0x23c6dccbU, 0xedfc68b6U, 0xe4f163b8U,
    0x31dccad7U, 0x63851042U, 0x97224013U, 0xc6112084U,
    0x4a247d85U, 0xbb3df8d2U, 0xf93211aeU, 0x29a16dc7U,
    0x9e2f4b1dU, 0xb230f3dcU, 0x8652ec0dU, 0xc1e3d077U,
    0xb3166c2bU, 0x70b999a9U, 0x9448fa11U, 0xe9642247U,
    0xfc8cc4a8U, 0xf03f1aa0U, 0x7d2cd856U, 0x3390ef22U,
    0x494ec787U, 0x38d1c1d9U, 0xcaa2fe8cU, 0xd40b3698U,
    0xf581cfa6U, 0x7ade28a5U, 0xb78e26daU, 0xadbfa43fU,
    0x3a9de42cU, 0x78920d50U, 0x5fcc9b6aU, 0x7e466254U,
    0x8d13c2f6U, 0xd8b8e890U, 0x39f75e2eU, 0xc3aff582U,
    0x5d80be9fU, 0xd0937c69U, 0xd52da96fU, 0x2512b3cfU,
    0xac993bc8U, 0x187da710U, 0x9c636ee8U, 0x3bbb7bdbU,
    0x267809cdU, 0x5918f46eU, 0x9ab701ecU, 0x4f9aa883U,
    0x956e65e6U, 0xffe67eaaU, 0xbccf0821U, 0x15e8e6efU,
    0xe79bd9baU, 0x6f36ce4aU, 0x9f09d4eaU, 0xb07cd629U,
    0xa4b2af31U, 0x3f23312aU, 0xa59430c6U, 0xa266c035U,
    0x4ebc3774U, 0x82caa6fcU, 0x90d0b0e0U, 0xa7d81533U,
    0x04984af1U, 0xecdaf741U, 0xcd500e7fU, 0x91f62f17U,
    0x4dd68d76U, 0xefb04d43U, 0xaa4d54ccU, 0x9604dfe4U,
    0xd1b5e39eU, 0x6a881b4cU, 0x2c1fb8c1U, 0x65517f46U,
    0x5eea049dU, 0x8c355d01U, 0x877473faU, 0x0b412efbU,
    0x671d5ab3U, 0xdbd25292U, 0x105633e9U, 0xd647136dU,
    0xd7618c9aU, 0xa10c7a37U, 0xf8148e59U, 0x133c89ebU,
    0xa927eeceU, 0x61c935b7U, 0x1ce5ede1U, 0x47b13c7aU,
    0xd2df599cU, 0xf2733f55U, 0x14ce7918U, 0xc737bf73U,
    0xf7cdea53U, 0xfdaa5b5fU, 0x3d6f14dfU, 0x44db8678U,
    0xaff381caU, 0x68c43eb9U, 0x24342c38U, 0xa3405fc2U,
    0x1dc37216U, 0xe2250cbcU, 0x3c498b28U, 0x0d9541ffU,
    0xa8017139U, 0x0cb3de08U, 0xb4e49cd8U, 0x56c19064U,
    0xcb84617bU, 0x32b670d5U, 0x6c5c7448U, 0xb85742d0U,
};
static const WE_UINT32 Td4[256] = 
{
    0x52525252U, 0x09090909U, 0x6a6a6a6aU, 0xd5d5d5d5U,
    0x30303030U, 0x36363636U, 0xa5a5a5a5U, 0x38383838U,
    0xbfbfbfbfU, 0x40404040U, 0xa3a3a3a3U, 0x9e9e9e9eU,
    0x81818181U, 0xf3f3f3f3U, 0xd7d7d7d7U, 0xfbfbfbfbU,
    0x7c7c7c7cU, 0xe3e3e3e3U, 0x39393939U, 0x82828282U,
    0x9b9b9b9bU, 0x2f2f2f2fU, 0xffffffffU, 0x87878787U,
    0x34343434U, 0x8e8e8e8eU, 0x43434343U, 0x44444444U,
    0xc4c4c4c4U, 0xdedededeU, 0xe9e9e9e9U, 0xcbcbcbcbU,
    0x54545454U, 0x7b7b7b7bU, 0x94949494U, 0x32323232U,
    0xa6a6a6a6U, 0xc2c2c2c2U, 0x23232323U, 0x3d3d3d3dU,
    0xeeeeeeeeU, 0x4c4c4c4cU, 0x95959595U, 0x0b0b0b0bU,
    0x42424242U, 0xfafafafaU, 0xc3c3c3c3U, 0x4e4e4e4eU,
    0x08080808U, 0x2e2e2e2eU, 0xa1a1a1a1U, 0x66666666U,
    0x28282828U, 0xd9d9d9d9U, 0x24242424U, 0xb2b2b2b2U,
    0x76767676U, 0x5b5b5b5bU, 0xa2a2a2a2U, 0x49494949U,
    0x6d6d6d6dU, 0x8b8b8b8bU, 0xd1d1d1d1U, 0x25252525U,
    0x72727272U, 0xf8f8f8f8U, 0xf6f6f6f6U, 0x64646464U,
    0x86868686U, 0x68686868U, 0x98989898U, 0x16161616U,
    0xd4d4d4d4U, 0xa4a4a4a4U, 0x5c5c5c5cU, 0xccccccccU,
    0x5d5d5d5dU, 0x65656565U, 0xb6b6b6b6U, 0x92929292U,
    0x6c6c6c6cU, 0x70707070U, 0x48484848U, 0x50505050U,
    0xfdfdfdfdU, 0xededededU, 0xb9b9b9b9U, 0xdadadadaU,
    0x5e5e5e5eU, 0x15151515U, 0x46464646U, 0x57575757U,
    0xa7a7a7a7U, 0x8d8d8d8dU, 0x9d9d9d9dU, 0x84848484U,
    0x90909090U, 0xd8d8d8d8U, 0xababababU, 0x00000000U,
    0x8c8c8c8cU, 0xbcbcbcbcU, 0xd3d3d3d3U, 0x0a0a0a0aU,
    0xf7f7f7f7U, 0xe4e4e4e4U, 0x58585858U, 0x05050505U,
    0xb8b8b8b8U, 0xb3b3b3b3U, 0x45454545U, 0x06060606U,
    0xd0d0d0d0U, 0x2c2c2c2cU, 0x1e1e1e1eU, 0x8f8f8f8fU,
    0xcacacacaU, 0x3f3f3f3fU, 0x0f0f0f0fU, 0x02020202U,
    0xc1c1c1c1U, 0xafafafafU, 0xbdbdbdbdU, 0x03030303U,
    0x01010101U, 0x13131313U, 0x8a8a8a8aU, 0x6b6b6b6bU,
    0x3a3a3a3aU, 0x91919191U, 0x11111111U, 0x41414141U,
    0x4f4f4f4fU, 0x67676767U, 0xdcdcdcdcU, 0xeaeaeaeaU,
    0x97979797U, 0xf2f2f2f2U, 0xcfcfcfcfU, 0xcecececeU,
    0xf0f0f0f0U, 0xb4b4b4b4U, 0xe6e6e6e6U, 0x73737373U,
    0x96969696U, 0xacacacacU, 0x74747474U, 0x22222222U,
    0xe7e7e7e7U, 0xadadadadU, 0x35353535U, 0x85858585U,
    0xe2e2e2e2U, 0xf9f9f9f9U, 0x37373737U, 0xe8e8e8e8U,
    0x1c1c1c1cU, 0x75757575U, 0xdfdfdfdfU, 0x6e6e6e6eU,
    0x47474747U, 0xf1f1f1f1U, 0x1a1a1a1aU, 0x71717171U,
    0x1d1d1d1dU, 0x29292929U, 0xc5c5c5c5U, 0x89898989U,
    0x6f6f6f6fU, 0xb7b7b7b7U, 0x62626262U, 0x0e0e0e0eU,
    0xaaaaaaaaU, 0x18181818U, 0xbebebebeU, 0x1b1b1b1bU,
    0xfcfcfcfcU, 0x56565656U, 0x3e3e3e3eU, 0x4b4b4b4bU,
    0xc6c6c6c6U, 0xd2d2d2d2U, 0x79797979U, 0x20202020U,
    0x9a9a9a9aU, 0xdbdbdbdbU, 0xc0c0c0c0U, 0xfefefefeU,
    0x78787878U, 0xcdcdcdcdU, 0x5a5a5a5aU, 0xf4f4f4f4U,
    0x1f1f1f1fU, 0xddddddddU, 0xa8a8a8a8U, 0x33333333U,
    0x88888888U, 0x07070707U, 0xc7c7c7c7U, 0x31313131U,
    0xb1b1b1b1U, 0x12121212U, 0x10101010U, 0x59595959U,
    0x27272727U, 0x80808080U, 0xececececU, 0x5f5f5f5fU,
    0x60606060U, 0x51515151U, 0x7f7f7f7fU, 0xa9a9a9a9U,
    0x19191919U, 0xb5b5b5b5U, 0x4a4a4a4aU, 0x0d0d0d0dU,
    0x2d2d2d2dU, 0xe5e5e5e5U, 0x7a7a7a7aU, 0x9f9f9f9fU,
    0x93939393U, 0xc9c9c9c9U, 0x9c9c9c9cU, 0xefefefefU,
    0xa0a0a0a0U, 0xe0e0e0e0U, 0x3b3b3b3bU, 0x4d4d4d4dU,
    0xaeaeaeaeU, 0x2a2a2a2aU, 0xf5f5f5f5U, 0xb0b0b0b0U,
    0xc8c8c8c8U, 0xebebebebU, 0xbbbbbbbbU, 0x3c3c3c3cU,
    0x83838383U, 0x53535353U, 0x99999999U, 0x61616161U,
    0x17171717U, 0x2b2b2b2bU, 0x04040404U, 0x7e7e7e7eU,
    0xbabababaU, 0x77777777U, 0xd6d6d6d6U, 0x26262626U,
    0xe1e1e1e1U, 0x69696969U, 0x14141414U, 0x63636363U,
    0x55555555U, 0x21212121U, 0x0c0c0c0cU, 0x7d7d7d7dU,
};
static const WE_UINT32 rcon[] = 
{
    0x01000000, 0x02000000, 0x04000000, 0x08000000,
    0x10000000, 0x20000000, 0x40000000, 0x80000000,
    0x1B000000, 0x36000000, 
};





/***************************************************************************************************
*   Function Define Section
***************************************************************************************************/
/*=====================================================================================
FUNCTION: 
    We_AESSetEncryptKey
CREATE DATE: 
    2006-12-04
AUTHOR: 
    Sam
DESCRIPTION:
    Set AES encrypt key.
ARGUMENTS PASSED:
    pucUserKey[IN]: original key
    iNBits[IN]: length in bit;
    pstAESKey[OUT]: AES key.
RETURN VALUE:
    M_WE_LIB_ERR_OK if success,others error code.
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    Omit
IMPORTANT NOTES:
=====================================================================================*/
WE_INT32 We_AESSetEncryptKey
(
    const WE_UINT8  *pucUserKey, 
    const WE_INT32  iNBits,
    St_WeAesKey     *pstAESKey
) 
{
    WE_UINT32 *puiRoundKey = NULL;
    WE_INT32  i = 0;
    WE_UINT32 uiTemp = 0;

    if (!pucUserKey || !pstAESKey)
    {
        return 0x08;
    }
    if (iNBits != 128 && iNBits != 192 && iNBits != 256)
    {
        return 0x09;
    }

    puiRoundKey = pstAESKey->auiRoundKey;

    if (iNBits==128)
    {
        pstAESKey->iNRounds = 10;
    }
    else if (iNBits==192)
    {
        pstAESKey->iNRounds = 12;
    }
    else
    {
        pstAESKey->iNRounds = 14;
    }

    puiRoundKey[0] = GETU32(pucUserKey     );
    puiRoundKey[1] = GETU32(pucUserKey +  4);
    puiRoundKey[2] = GETU32(pucUserKey +  8);
    puiRoundKey[3] = GETU32(pucUserKey + 12);
    if (iNBits == 128) 
    {
        while (1) 
        {
            uiTemp  = puiRoundKey[3];
            puiRoundKey[4] = puiRoundKey[0] ^
                             (Te4[(uiTemp >> 16) & 0xff] & 0xff000000) ^
                             (Te4[(uiTemp >>  8) & 0xff] & 0x00ff0000) ^
                             (Te4[(uiTemp      ) & 0xff] & 0x0000ff00) ^
                             (Te4[(uiTemp >> 24)       ] & 0x000000ff) ^
                             rcon[i];
            puiRoundKey[5] = puiRoundKey[1] ^ puiRoundKey[4];
            puiRoundKey[6] = puiRoundKey[2] ^ puiRoundKey[5];
            puiRoundKey[7] = puiRoundKey[3] ^ puiRoundKey[6];
            if (++i == 10) 
            {
                return 0;
            }
            puiRoundKey += 4;
        }
    }
    
    puiRoundKey[4] = GETU32(pucUserKey + 16);
    puiRoundKey[5] = GETU32(pucUserKey + 20);
    if (iNBits == 192) 
    {
        while (1) 
        {
            uiTemp = puiRoundKey[ 5];
            puiRoundKey[ 6] = puiRoundKey[ 0] ^
                              (Te4[(uiTemp >> 16) & 0xff] & 0xff000000) ^
                              (Te4[(uiTemp >>  8) & 0xff] & 0x00ff0000) ^
                              (Te4[(uiTemp      ) & 0xff] & 0x0000ff00) ^
                              (Te4[(uiTemp >> 24)       ] & 0x000000ff) ^
                              rcon[i];
            puiRoundKey[ 7] = puiRoundKey[ 1] ^ puiRoundKey[ 6];
            puiRoundKey[ 8] = puiRoundKey[ 2] ^ puiRoundKey[ 7];
            puiRoundKey[ 9] = puiRoundKey[ 3] ^ puiRoundKey[ 8];
            if (++i == 8) 
            {
                return 0;
            }
            puiRoundKey[10] = puiRoundKey[ 4] ^ puiRoundKey[ 9];
            puiRoundKey[11] = puiRoundKey[ 5] ^ puiRoundKey[10];
            puiRoundKey += 6;
        }
    }
    
    puiRoundKey[6] = GETU32(pucUserKey + 24);
    puiRoundKey[7] = GETU32(pucUserKey + 28);
    if (iNBits == 256) 
    {
        while (1) 
        {
            uiTemp = puiRoundKey[ 7];
            puiRoundKey[ 8] = puiRoundKey[ 0] ^
                              (Te4[(uiTemp >> 16) & 0xff] & 0xff000000) ^
                              (Te4[(uiTemp >>  8) & 0xff] & 0x00ff0000) ^
                              (Te4[(uiTemp      ) & 0xff] & 0x0000ff00) ^
                              (Te4[(uiTemp >> 24)       ] & 0x000000ff) ^
                              rcon[i];
            puiRoundKey[ 9] = puiRoundKey[ 1] ^ puiRoundKey[ 8];
            puiRoundKey[10] = puiRoundKey[ 2] ^ puiRoundKey[ 9];
            puiRoundKey[11] = puiRoundKey[ 3] ^ puiRoundKey[10];
            if (++i == 7) 
            {
                return 0;
            }
            uiTemp = puiRoundKey[11];
            puiRoundKey[12] = puiRoundKey[ 4] ^
                              (Te4[(uiTemp >> 24)       ] & 0xff000000) ^
                              (Te4[(uiTemp >> 16) & 0xff] & 0x00ff0000) ^
                              (Te4[(uiTemp >>  8) & 0xff] & 0x0000ff00) ^
                              (Te4[(uiTemp      ) & 0xff] & 0x000000ff);
            puiRoundKey[13] = puiRoundKey[ 5] ^ puiRoundKey[12];
            puiRoundKey[14] = puiRoundKey[ 6] ^ puiRoundKey[13];
            puiRoundKey[15] = puiRoundKey[ 7] ^ puiRoundKey[14];

            puiRoundKey += 8;
        }
    }
    return 0;
}



/*=====================================================================================
FUNCTION: 
    We_AESSetDecryptKey
CREATE DATE: 
    2006-12-04
AUTHOR: 
    Sam
DESCRIPTION:
    Set AES decrypt key.
ARGUMENTS PASSED:
    pucUserKey[IN]: original key
    iNBits[IN]: length in bit;
    pstAESKey[OUT]: AES key.
RETURN VALUE:
    M_WE_LIB_ERR_OK if success,others error code.
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    Omit
IMPORTANT NOTES:
=====================================================================================*/
WE_INT32 We_AESSetDecryptKey
(
    const WE_UINT8  *pucUserKey, 
    const WE_INT32  iNBits,
    St_WeAesKey     *pstAESKey
) 
{

    WE_UINT32 *puiRoundKey;
    WE_INT32 i, j, iStatus;
    WE_UINT32 uiTemp;

    
    iStatus = We_AESSetEncryptKey(pucUserKey, iNBits, pstAESKey);
    if (iStatus != 0)
    {
        return iStatus;
    }

    puiRoundKey = pstAESKey->auiRoundKey;

    
    for (i = 0, j = 4*(pstAESKey->iNRounds); i < j; i += 4, j -= 4) 
    {
        uiTemp = puiRoundKey[i    ]; puiRoundKey[i    ] = puiRoundKey[j    ]; puiRoundKey[j    ] = uiTemp;
        uiTemp = puiRoundKey[i + 1]; puiRoundKey[i + 1] = puiRoundKey[j + 1]; puiRoundKey[j + 1] = uiTemp;
        uiTemp = puiRoundKey[i + 2]; puiRoundKey[i + 2] = puiRoundKey[j + 2]; puiRoundKey[j + 2] = uiTemp;
        uiTemp = puiRoundKey[i + 3]; puiRoundKey[i + 3] = puiRoundKey[j + 3]; puiRoundKey[j + 3] = uiTemp;
    }
    
    for (i = 1; i < (pstAESKey->iNRounds); i++) 
    {
        puiRoundKey += 4;
        puiRoundKey[0] =
            Td0[Te4[(puiRoundKey[0] >> 24)       ] & 0xff] ^
            Td1[Te4[(puiRoundKey[0] >> 16) & 0xff] & 0xff] ^
            Td2[Te4[(puiRoundKey[0] >>  8) & 0xff] & 0xff] ^
            Td3[Te4[(puiRoundKey[0]      ) & 0xff] & 0xff];
        puiRoundKey[1] =
            Td0[Te4[(puiRoundKey[1] >> 24)       ] & 0xff] ^
            Td1[Te4[(puiRoundKey[1] >> 16) & 0xff] & 0xff] ^
            Td2[Te4[(puiRoundKey[1] >>  8) & 0xff] & 0xff] ^
            Td3[Te4[(puiRoundKey[1]      ) & 0xff] & 0xff];
        puiRoundKey[2] =
            Td0[Te4[(puiRoundKey[2] >> 24)       ] & 0xff] ^
            Td1[Te4[(puiRoundKey[2] >> 16) & 0xff] & 0xff] ^
            Td2[Te4[(puiRoundKey[2] >>  8) & 0xff] & 0xff] ^
            Td3[Te4[(puiRoundKey[2]      ) & 0xff] & 0xff];
        puiRoundKey[3] =
            Td0[Te4[(puiRoundKey[3] >> 24)       ] & 0xff] ^
            Td1[Te4[(puiRoundKey[3] >> 16) & 0xff] & 0xff] ^
            Td2[Te4[(puiRoundKey[3] >>  8) & 0xff] & 0xff] ^
            Td3[Te4[(puiRoundKey[3]      ) & 0xff] & 0xff];
    }
    return 0;
}





/*=====================================================================================
FUNCTION: 
    We_AESSetDecryptKey
CREATE DATE: 
    2006-12-04
AUTHOR: 
    Sam
DESCRIPTION:
    Encrypt block data with AES encrypt key.
ARGUMENTS PASSED:
    pucInData[IN]: block data to be encrypted;
    pucOutData[OUT]: encrypted data;
    pstAESKey[IN]: AES encrypt key.
RETURN VALUE:
    M_WE_LIB_ERR_OK if success,others error code.
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    Omit
IMPORTANT NOTES:
=====================================================================================*/
WE_VOID We_AESEncrypt
(
    const WE_UINT8  *pucInData, 
    WE_UINT8        *pucOutData,
    const St_WeAesKey *pstAESKey
) 
{

    const WE_UINT32 *puiRoundKey;
    WE_UINT32 s0, s1, s2, s3, t0, t1, t2, t3;
    WE_INT32 r;

    puiRoundKey = pstAESKey->auiRoundKey;
    s0 = GETU32(pucInData     ) ^ puiRoundKey[0];
    s1 = GETU32(pucInData +  4) ^ puiRoundKey[1];
    s2 = GETU32(pucInData +  8) ^ puiRoundKey[2];
    s3 = GETU32(pucInData + 12) ^ puiRoundKey[3];

    r = (WE_UINT32)pstAESKey->iNRounds >> 1;
    for (;;) {
        t0 =
            Te0[(s0 >> 24)       ] ^
            Te1[(s1 >> 16) & 0xff] ^
            Te2[(s2 >>  8) & 0xff] ^
            Te3[(s3      ) & 0xff] ^
            puiRoundKey[4];
        t1 =
            Te0[(s1 >> 24)       ] ^
            Te1[(s2 >> 16) & 0xff] ^
            Te2[(s3 >>  8) & 0xff] ^
            Te3[(s0      ) & 0xff] ^
            puiRoundKey[5];
        t2 =
            Te0[(s2 >> 24)       ] ^
            Te1[(s3 >> 16) & 0xff] ^
            Te2[(s0 >>  8) & 0xff] ^
            Te3[(s1      ) & 0xff] ^
            puiRoundKey[6];
        t3 =
            Te0[(s3 >> 24)       ] ^
            Te1[(s0 >> 16) & 0xff] ^
            Te2[(s1 >>  8) & 0xff] ^
            Te3[(s2      ) & 0xff] ^
            puiRoundKey[7];

        puiRoundKey += 8;
        if (--r == 0) {
            break;
        }

        s0 =
            Te0[(t0 >> 24)       ] ^
            Te1[(t1 >> 16) & 0xff] ^
            Te2[(t2 >>  8) & 0xff] ^
            Te3[(t3      ) & 0xff] ^
            puiRoundKey[0];
        s1 =
            Te0[(t1 >> 24)       ] ^
            Te1[(t2 >> 16) & 0xff] ^
            Te2[(t3 >>  8) & 0xff] ^
            Te3[(t0      ) & 0xff] ^
            puiRoundKey[1];
        s2 =
            Te0[(t2 >> 24)       ] ^
            Te1[(t3 >> 16) & 0xff] ^
            Te2[(t0 >>  8) & 0xff] ^
            Te3[(t1      ) & 0xff] ^
            puiRoundKey[2];
        s3 =
            Te0[(t3 >> 24)       ] ^
            Te1[(t0 >> 16) & 0xff] ^
            Te2[(t1 >>  8) & 0xff] ^
            Te3[(t2      ) & 0xff] ^
            puiRoundKey[3];
    }   

    s0 =
        (Te4[(t0 >> 24)       ] & 0xff000000) ^
        (Te4[(t1 >> 16) & 0xff] & 0x00ff0000) ^
        (Te4[(t2 >>  8) & 0xff] & 0x0000ff00) ^
        (Te4[(t3      ) & 0xff] & 0x000000ff) ^
        puiRoundKey[0];
    PUTU32(pucOutData     , s0);
    s1 =
        (Te4[(t1 >> 24)       ] & 0xff000000) ^
        (Te4[(t2 >> 16) & 0xff] & 0x00ff0000) ^
        (Te4[(t3 >>  8) & 0xff] & 0x0000ff00) ^
        (Te4[(t0      ) & 0xff] & 0x000000ff) ^
        puiRoundKey[1];
    PUTU32(pucOutData +  4, s1);
    s2 =
        (Te4[(t2 >> 24)       ] & 0xff000000) ^
        (Te4[(t3 >> 16) & 0xff] & 0x00ff0000) ^
        (Te4[(t0 >>  8) & 0xff] & 0x0000ff00) ^
        (Te4[(t1      ) & 0xff] & 0x000000ff) ^
        puiRoundKey[2];
    PUTU32(pucOutData +  8, s2);
    s3 =
        (Te4[(t3 >> 24)       ] & 0xff000000) ^
        (Te4[(t0 >> 16) & 0xff] & 0x00ff0000) ^
        (Te4[(t1 >>  8) & 0xff] & 0x0000ff00) ^
        (Te4[(t2      ) & 0xff] & 0x000000ff) ^
        puiRoundKey[3];
    PUTU32(pucOutData + 12, s3);
}

/*=====================================================================================
FUNCTION: 
    We_AESDecrypt
CREATE DATE: 
    2006-12-04
AUTHOR: 
    Sam
DESCRIPTION:
    Decrypt block data by AES decrypt key.
ARGUMENTS PASSED:
    pucInData[IN]: data to be decrypted;
    pucOutData[OUT]: decrypted data;
    pstAESKey[IN]: AES decrypt key.
RETURN VALUE:
    M_WE_LIB_ERR_OK if success,others error code.
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    Omit
IMPORTANT NOTES:
=====================================================================================*/

WE_VOID We_AESDecrypt
(
    const WE_UINT8  *pucInData, 
    WE_UINT8        *pucOutData,
    const St_WeAesKey *pstAESKey
) 
{
    const WE_UINT32 *puiRoundKey;
    WE_UINT32 s0, s1, s2, s3, t0, t1, t2, t3;
    WE_INT32 r;

    puiRoundKey = pstAESKey->auiRoundKey;
    s0 = GETU32(pucInData     ) ^ puiRoundKey[0];
    s1 = GETU32(pucInData +  4) ^ puiRoundKey[1];
    s2 = GETU32(pucInData +  8) ^ puiRoundKey[2];
    s3 = GETU32(pucInData + 12) ^ puiRoundKey[3];

    r = (WE_UINT32)pstAESKey->iNRounds >> 1;
    for (;;) {
        t0 =
            Td0[(s0 >> 24)       ] ^
            Td1[(s3 >> 16) & 0xff] ^
            Td2[(s2 >>  8) & 0xff] ^
            Td3[(s1      ) & 0xff] ^
            puiRoundKey[4];
        t1 =
            Td0[(s1 >> 24)       ] ^
            Td1[(s0 >> 16) & 0xff] ^
            Td2[(s3 >>  8) & 0xff] ^
            Td3[(s2      ) & 0xff] ^
            puiRoundKey[5];
        t2 =
            Td0[(s2 >> 24)       ] ^
            Td1[(s1 >> 16) & 0xff] ^
            Td2[(s0 >>  8) & 0xff] ^
            Td3[(s3      ) & 0xff] ^
            puiRoundKey[6];
        t3 =
            Td0[(s3 >> 24)       ] ^
            Td1[(s2 >> 16) & 0xff] ^
            Td2[(s1 >>  8) & 0xff] ^
            Td3[(s0      ) & 0xff] ^
            puiRoundKey[7];

        puiRoundKey += 8;
        if (--r == 0) {
            break;
        }

        s0 =
            Td0[(t0 >> 24)       ] ^
            Td1[(t3 >> 16) & 0xff] ^
            Td2[(t2 >>  8) & 0xff] ^
            Td3[(t1      ) & 0xff] ^
            puiRoundKey[0];
        s1 =
            Td0[(t1 >> 24)       ] ^
            Td1[(t0 >> 16) & 0xff] ^
            Td2[(t3 >>  8) & 0xff] ^
            Td3[(t2      ) & 0xff] ^
            puiRoundKey[1];
        s2 =
            Td0[(t2 >> 24)       ] ^
            Td1[(t1 >> 16) & 0xff] ^
            Td2[(t0 >>  8) & 0xff] ^
            Td3[(t3      ) & 0xff] ^
            puiRoundKey[2];
        s3 =
            Td0[(t3 >> 24)       ] ^
            Td1[(t2 >> 16) & 0xff] ^
            Td2[(t1 >>  8) & 0xff] ^
            Td3[(t0      ) & 0xff] ^
            puiRoundKey[3];
    }

    s0 =
           (Td4[(t0 >> 24)       ] & 0xff000000) ^
           (Td4[(t3 >> 16) & 0xff] & 0x00ff0000) ^
           (Td4[(t2 >>  8) & 0xff] & 0x0000ff00) ^
           (Td4[(t1      ) & 0xff] & 0x000000ff) ^
           puiRoundKey[0];
    PUTU32(pucOutData     , s0);
    s1 =
           (Td4[(t1 >> 24)       ] & 0xff000000) ^
           (Td4[(t0 >> 16) & 0xff] & 0x00ff0000) ^
           (Td4[(t3 >>  8) & 0xff] & 0x0000ff00) ^
           (Td4[(t2      ) & 0xff] & 0x000000ff) ^
           puiRoundKey[1];
    PUTU32(pucOutData +  4, s1);
    s2 =
           (Td4[(t2 >> 24)       ] & 0xff000000) ^
           (Td4[(t1 >> 16) & 0xff] & 0x00ff0000) ^
           (Td4[(t0 >>  8) & 0xff] & 0x0000ff00) ^
           (Td4[(t3      ) & 0xff] & 0x000000ff) ^
           puiRoundKey[2];
    PUTU32(pucOutData +  8, s2);
    s3 =
           (Td4[(t3 >> 24)       ] & 0xff000000) ^
           (Td4[(t2 >> 16) & 0xff] & 0x00ff0000) ^
           (Td4[(t1 >>  8) & 0xff] & 0x0000ff00) ^
           (Td4[(t0      ) & 0xff] & 0x000000ff) ^
           puiRoundKey[3];
    PUTU32(pucOutData + 12, s3);
}


/*=====================================================================================
FUNCTION: 
    We_LibAESEncrypt
CREATE DATE: 
    2006-12-04
AUTHOR: 
    Sam
DESCRIPTION:
    Set encrypt key and encrypt data with AES algorithm.
ARGUMENTS PASSED:
    stKey[IN]: cipher key
    pucIn[IN]: data to be encrypted;
    uiInLen[IN]: length of data to be encrypted;
    pucOut[OUT]: encrypted data;
    puiOutLen[OUT]: length of encrypted data;
RETURN VALUE:
    M_WE_LIB_ERR_OK if success,others error code.
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    Omit
IMPORTANT NOTES:
    no padding: the input data had been padded, so the length of 
                input data is multiples of block size.Padding is 
                stripped in the extern module.
=====================================================================================*/
WE_INT32 We_LibAESEncrypt
(
    St_WeCipherCrptKey  stKey, 
    const WE_UCHAR* pucIn, 
    WE_UINT32 uiInLen, 
    WE_UCHAR* pucOut, 
    WE_UINT32* puiOutLen
)
{
    WE_INT32 iRes = 0;
    WE_UINT32 uiLen = 0;
    St_WeAesKey stAESEncryptKeyEx = {{0},0};

    if (!pucIn||!pucOut||!puiOutLen||!stKey.pucKey)
    {
        return M_WE_LIB_ERR_INVALID_PARAMETER;
    }
    if ((stKey.iKeyLen != 16)||(stKey.iIvLen != 16))
    {
        return M_WE_LIB_ERR_KEY_LENGTH;
    }
    
    uiLen = uiInLen % 16;
    if ((uiLen == 0) && (*puiOutLen >= uiInLen))
    {
        *puiOutLen = uiInLen;
    }
    else
    {
        return M_WE_LIB_ERR_DATA_LENGTH;
    }
    
    iRes = We_AESSetEncryptKey(stKey.pucKey, 128, &stAESEncryptKeyEx);
    if (iRes != M_WE_LIB_ERR_OK)
    {
        return M_WE_LIB_ERR_INVALID_KEY;
    }

    We_AESCBCEncrypt(&stAESEncryptKeyEx, stKey.pucIv, AES_ENCRYPT, 
                     pucIn, uiInLen, pucOut, puiOutLen);
    
    return iRes;
}

/*=====================================================================================
FUNCTION: 
    We_LibAESEncryptInit
CREATE DATE: 
    2006-12-04
AUTHOR: 
    Sam
DESCRIPTION:
    Encrypt init with AES algorithm.
ARGUMENTS PASSED:
    stKey[IO]: cipher key
    hAESHandle[OUT]: data handle;
RETURN VALUE:
    M_WE_LIB_ERR_OK if success,others error code.
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    Omit
IMPORTANT NOTES:
    no padding: the input data had been padded, so the length of 
                input data is multiples of block size.Padding is 
                stripped in the extern module.
=====================================================================================*/
WE_INT32 We_LibAESEncryptInit
( 
    St_WeCipherCrptKey  stKey, 
    WE_HANDLE *hAESHandle
)
{
    St_WeAESParameter *pstAESParam = NULL;
    WE_INT32 iRes = 0;

    if(!stKey.pucKey || !stKey.pucIv || !hAESHandle)
    {
        return M_WE_LIB_ERR_INVALID_PARAMETER;
    }
    if((16 != stKey.iKeyLen)||(16 != stKey.iIvLen))
    {
        return M_WE_LIB_ERR_KEY_LENGTH;
    }

    pstAESParam = WE_MALLOC(sizeof(St_WeAESParameter));
    if (!pstAESParam)
    {
        return M_WE_LIB_ERR_INSUFFICIENT_MEMORY;
    }
    (void)WE_MEMSET( pstAESParam,0,sizeof(St_WeAESParameter) );
    iRes = We_AESSetEncryptKey(stKey.pucKey, 128, &(pstAESParam->stAESCipherKeyEx));
    if (iRes != M_WE_LIB_ERR_OK)
    {
        WE_FREE(pstAESParam);
        return M_WE_LIB_ERR_INVALID_KEY;
    }
    pstAESParam->iType = (WE_INT32)E_WE_ALG_CIPHER_AES_CBC_128;
    (WE_VOID)WE_MEMCPY(pstAESParam->aucIv, stKey.pucIv, 16);
    (WE_VOID)WE_MEMSET(pstAESParam->aucBuf, 0, 16);
    pstAESParam->uiBufLen = 0;

    *hAESHandle = (WE_HANDLE)pstAESParam;
    return M_WE_LIB_ERR_OK;
}


/*=====================================================================================
FUNCTION: 
    We_LibAESEncryptUpdate
CREATE DATE: 
    2006-12-04
AUTHOR: 
    Sam
DESCRIPTION:
    Encrypt data with AES algorithm.
ARGUMENTS PASSED:
    pucIn[IN]: data part to be encrypted;
    uiInLen[IN]: length of data part;
    pucOut[OUT]: encrypted data;
    puiOutLen[OUT]: length of encrypted data;
    hAESHandle[IO]: data handle.
RETURN VALUE:
    M_WE_LIB_ERR_OK if success,others error code.
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    Omit
IMPORTANT NOTES:
    no padding: the input data had been padded, so the length of 
                input data is multiples of block size.Padding is 
                stripped in the extern module.
=====================================================================================*/
WE_INT32 We_LibAESEncryptUpdate
(
    const WE_UCHAR* pucIn, 
    WE_UINT32 uiInLen, 
    WE_UCHAR* pucOut, 
    WE_UINT32* puiOutLen, 
    WE_HANDLE hAESHandle
)
{
    St_WeAESParameter *pstAESParam = NULL;
    WE_INT32 iRes = M_WE_LIB_ERR_OK;
    WE_UINT32 uiLen = 0;

    if( (0 == uiInLen) || (!pucIn) || (!pucOut) || (!puiOutLen) || !hAESHandle)
    {
        return M_WE_LIB_ERR_INVALID_PARAMETER;
    }

    pstAESParam = (St_WeAESParameter *)hAESHandle;
    uiLen = uiInLen % 16;
    if ((0 == uiLen) && (*puiOutLen >= uiInLen))
    {
        *puiOutLen = uiInLen;
    }
    else
    {
        return M_WE_LIB_ERR_DATA_LENGTH;
    }
    
    We_AESCBCEncrypt(&(pstAESParam->stAESCipherKeyEx), pstAESParam->aucIv, 
                            (pstAESParam->iType), pucIn, uiInLen, pucOut, puiOutLen);

    return iRes;
}

/*=====================================================================================
FUNCTION: 
    We_LibAESEncryptFinal
CREATE DATE: 
    2006-12-04
AUTHOR: 
    Sam
DESCRIPTION:
    Free the data handle.
ARGUMENTS PASSED:
    hAESHandle[IN]: data handle.
RETURN VALUE:
    M_WE_LIB_ERR_OK if success,others error code.
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    Omit
IMPORTANT NOTES:
    no padding: the input data had been padded, so the length of 
                input data is multiples of block size.Padding is 
                stripped in the extern module.
=====================================================================================*/
WE_INT32 We_LibAESEncryptFinal
(
    WE_HANDLE hAESHandle
)
{
    St_WeAESParameter *pstAESParam = NULL;
    WE_INT32 iRes = 0;

    if((NULL == hAESHandle))
    {
        return M_WE_LIB_ERR_INVALID_PARAMETER;
    }
    pstAESParam = (St_WeAESParameter *)hAESHandle;
    WE_LIB_FREE(pstAESParam);

    return iRes;
}


/*=====================================================================================
FUNCTION: 
    We_LibAESEncrypt1
CREATE DATE: 
    2006-12-04
AUTHOR: 
    Sam
DESCRIPTION:
    Set encrypt key and padding data, encrypt data with AES algorithm.
ARGUMENTS PASSED:
    stKey[IN]: cipher key
    pucIn[IN]: data to be encrypted;
    uiInLen[IN]: length of data to be encrypted;
    pucOut[OUT]: encrypted data;
    puiOutLen[OUT]: length of encrypted data;
RETURN VALUE:
    M_WE_LIB_ERR_OK if success,others error code.
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    Omit
IMPORTANT NOTES:
    rfc2630 padding: pad interior, and pad each byte with # bytes 
                     padded(# is equal to the length of padding, 
                     at least 1 byte).Padding is stripped on decrypt.
=====================================================================================*/
WE_INT32 We_LibAESEncrypt1
(
    St_WeCipherCrptKey  stKey, 
    const WE_UCHAR* pucIn, 
    WE_UINT32 uiInLen, 
    WE_UCHAR* pucOut, 
    WE_UINT32* puiOutLen
)
{
    WE_INT32 iRes = 0;
    WE_UINT32 uiLen = 0;
    St_WeAesKey stAESEncryptKeyEx = {{0},0};
    
    if (!pucIn||!pucOut||!puiOutLen||!stKey.pucKey||!stKey.pucIv)
    {
        return M_WE_LIB_ERR_INVALID_PARAMETER;
    }
    if ((stKey.iKeyLen != 16)||(stKey.iIvLen != 16))
    {
        return M_WE_LIB_ERR_KEY_LENGTH;
    }

    iRes = We_AESSetEncryptKey(stKey.pucKey, 128, &stAESEncryptKeyEx);
    if (iRes != M_WE_LIB_ERR_OK)
    {
        return M_WE_LIB_ERR_INVALID_KEY;
    }
    
    uiLen = uiInLen % 16;
    if (uiLen && *puiOutLen >= (1 + uiInLen/16)*16)
    {
        *puiOutLen = (1 + uiInLen/16)*16;
    }
    else if (uiLen == 0 && *puiOutLen >= uiInLen)
    {
        *puiOutLen = uiInLen;
    }
    else
    {
        return M_WE_LIB_ERR_DATA_LENGTH;
    }
    
    We_AESCBCEncrypt(&stAESEncryptKeyEx, stKey.pucIv, AES_ENCRYPT, 
                            pucIn, (uiInLen - uiLen), pucOut, puiOutLen);
    
    if (uiLen != 0)
    {
        WE_UINT8 aucDataTmp[16] = {0};

        (WE_VOID)WE_MEMSET(aucDataTmp, (16 - (WE_INT32)uiLen), 16);
        (WE_VOID)WE_MEMCPY(aucDataTmp, pucIn + uiInLen - uiLen, uiLen);

        We_AESCBCEncrypt(&stAESEncryptKeyEx, stKey.pucIv, AES_ENCRYPT, 
                                aucDataTmp, 16, pucOut + uiInLen - uiLen, puiOutLen);
    }
    return M_WE_LIB_ERR_OK;
}

/*=====================================================================================
FUNCTION: 
    We_LibAESEncryptInit1
CREATE DATE: 
    2006-12-04
AUTHOR: 
    Sam
DESCRIPTION:
    Encrypt init with AES algorithm.
ARGUMENTS PASSED:
    stKey[IN]: cipher key;
    hAESHandle[IN]: data handle.
RETURN VALUE:
    M_WE_LIB_ERR_OK if success,others error code.
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    Omit
IMPORTANT NOTES:
    rfc2630 padding: pad interior, and pad each byte with # bytes 
                     padded(# is equal to the length of padding, 
                     at least 1 byte).Padding is stripped on decrypt.
=====================================================================================*/
WE_INT32 We_LibAESEncryptInit1
( 
    St_WeCipherCrptKey  stKey, 
    WE_HANDLE *hAESHandle
)
{
    return We_LibAESEncryptInit( stKey, hAESHandle);
}

/*=====================================================================================
FUNCTION: 
    We_LibAESEncryptUpdate1
CREATE DATE: 
    2006-12-04
AUTHOR: 
    Sam
DESCRIPTION:
    Set padding data and encrypt data with AES algorithm.
ARGUMENTS PASSED:
    pucIn[IN]: data part to be encrypted;
    uiInLen[IN]: length of data part;
    pucOut[OUT]: encrypted data;
    puiOutLen[OUT]: length of encrypted data;
    hAESHandle[IO]: data handle.
RETURN VALUE:
    M_WE_LIB_ERR_OK if success,others error code.
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    Omit
IMPORTANT NOTES:
    rfc2630 padding: pad interior, and pad each byte with # bytes 
                     padded(# is equal to the length of padding, 
                     at least 1 byte).Padding is stripped on decrypt.
=====================================================================================*/
WE_INT32 We_LibAESEncryptUpdate1
(
    const WE_UCHAR* pucIn, 
    WE_UINT32 uiInLen, 
    WE_UCHAR* pucOut, 
    WE_UINT32* puiOutLen, 
    WE_HANDLE hAESHandle
)
{
    St_WeAESParameter *pstAESParam = NULL;
    WE_INT32 iRes = 0;
    WE_UINT32 uiLen = 0;
    WE_UINT32 uiBufLen = 0;

    if( (0 == uiInLen) || (!pucIn) || (!pucOut) || (!puiOutLen) || !hAESHandle)
    {
        return M_WE_LIB_ERR_INVALID_PARAMETER;
    }
    
    pstAESParam = (St_WeAESParameter *)hAESHandle;

    uiLen = uiInLen % 16;
    if (uiLen != 0 && *puiOutLen >= (1 + uiInLen/16)*16)
    {
        *puiOutLen = (1 + uiInLen/16)*16;
    }
    else if (uiLen == 0 && *puiOutLen >= uiInLen)
    {
        *puiOutLen = uiInLen;
    }
    else
    {
        return M_WE_LIB_ERR_DATA_LENGTH;
    }

    uiBufLen = pstAESParam->uiBufLen;
    if (uiInLen + uiBufLen < *puiOutLen)
    {
        pstAESParam->uiBufLen = (uiInLen + uiBufLen) % 16;
        *puiOutLen -= 16;
        if (uiInLen + uiBufLen > 16)
        {
            (WE_VOID)WE_MEMCPY((pstAESParam->aucBuf + uiBufLen), pucIn, (16 - uiBufLen));
            We_AESCBCEncrypt(&(pstAESParam->stAESCipherKeyEx), pstAESParam->aucIv, 
                         AES_ENCRYPT, pstAESParam->aucBuf, 16, pucOut, puiOutLen);
            if (uiInLen + uiBufLen - pstAESParam->uiBufLen > 16)
            {
                We_AESCBCEncrypt(&(pstAESParam->stAESCipherKeyEx), pstAESParam->aucIv, 
                                 AES_ENCRYPT, pucIn + (16 - uiBufLen), 
                                 uiInLen - (16 - uiBufLen) - pstAESParam->uiBufLen, 
                                 pucOut + 16, puiOutLen);
            }
            uiBufLen = uiInLen - pstAESParam->uiBufLen;
            (WE_VOID)WE_MEMCPY(pstAESParam->aucBuf, pucIn + uiBufLen, pstAESParam->uiBufLen);
        }
        else
        {
            (WE_VOID)WE_MEMCPY((pstAESParam->aucBuf + uiBufLen), pucIn, uiInLen);
        }
    }
    else if (uiInLen + uiBufLen == *puiOutLen)
    {
        pstAESParam->uiBufLen = 0;
        (WE_VOID)WE_MEMCPY((pstAESParam->aucBuf + uiBufLen), pucIn, (16 - uiBufLen));
        We_AESCBCEncrypt(&(pstAESParam->stAESCipherKeyEx), pstAESParam->aucIv, 
                         AES_ENCRYPT, pstAESParam->aucBuf, 16, pucOut, puiOutLen);
        if (*puiOutLen > 16)
        {
            We_AESCBCEncrypt(&(pstAESParam->stAESCipherKeyEx), pstAESParam->aucIv, 
                             AES_ENCRYPT, pucIn + 16 - uiBufLen, uiInLen - (16 - uiBufLen), 
                             pucOut + 16, puiOutLen);
        }
    }
    else
    {
        pstAESParam->uiBufLen = (uiInLen + uiBufLen) % 16;
        (WE_VOID)WE_MEMCPY(pstAESParam->aucBuf + uiBufLen, pucIn, (16 - uiBufLen));
        We_AESCBCEncrypt(&(pstAESParam->stAESCipherKeyEx), pstAESParam->aucIv, 
                         AES_ENCRYPT, pstAESParam->aucBuf, 16, pucOut, puiOutLen);
        if (uiInLen + uiBufLen - pstAESParam->uiBufLen > 16)
        {
            We_AESCBCEncrypt(&(pstAESParam->stAESCipherKeyEx), pstAESParam->aucIv, 
                         AES_ENCRYPT, pucIn + (16 - uiBufLen), 
                         uiInLen - (16 - uiBufLen) - pstAESParam->uiBufLen, 
                         pucOut + 16, puiOutLen);
        }
    }

    return iRes;
}

/*=====================================================================================
FUNCTION: 
    We_LibAESEncryptFinal1
CREATE DATE: 
    2006-12-04
AUTHOR: 
    Sam
DESCRIPTION:
    Get encrypted data and free memory.
ARGUMENTS PASSED:
    pucOut[OUT]: encrypted data;
    puiOutLen[OUT]: length of encrypted data;
    hAESHandle[IN]: data handle.
RETURN VALUE:
    M_WE_LIB_ERR_OK if success,others error code.
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    Omit
IMPORTANT NOTES:
    rfc2630 padding: pad interior, and pad each byte with # bytes 
                     padded(# is equal to the length of padding, 
                     at least 1 byte).Padding is stripped on decrypt.
=====================================================================================*/
WE_INT32 We_LibAESEncryptFinal1
(
    WE_UCHAR    *pucOut, 
    WE_UINT32   *puiOutLen,
    WE_HANDLE   hAESHandle
)
{
    St_WeAESParameter *pstAESParam = NULL;
    WE_INT32 iRes = 0;
    WE_UINT8 aucDataTmp[16] = {0};

    if ( !pucOut ||!puiOutLen || !hAESHandle)
    {
        return M_WE_LIB_ERR_INVALID_PARAMETER;
    }
    
    if (*puiOutLen < 16)
    {
        return M_WE_LIB_ERR_INSUFFICIENT_MEMORY;
    }
    *puiOutLen = 16;
    
    pstAESParam = (St_WeAESParameter *)hAESHandle;
    if (pstAESParam->uiBufLen != 0)
    {
        (WE_VOID)WE_MEMSET(aucDataTmp, (16 - (WE_INT32)pstAESParam->uiBufLen), 16);
        (WE_VOID)WE_MEMCPY(aucDataTmp, pstAESParam->aucBuf, pstAESParam->uiBufLen);

        We_AESCBCEncrypt(&(pstAESParam->stAESCipherKeyEx), pstAESParam->aucIv, 
                         AES_ENCRYPT, aucDataTmp, 16, pucOut, puiOutLen);
    }
    WE_LIB_FREE(pstAESParam);
    
    return iRes;
}

/*=====================================================================================
FUNCTION: 
    We_LibAESDecrypt
CREATE DATE: 
    2006-12-04
AUTHOR: 
    Sam
DESCRIPTION:
    Set decrypt key and decrypt data with AES algorithm.
ARGUMENTS PASSED:
    stKey[IN]: cipher key
    pucIn[IN]: data to be decrypted;
    uiInLen[IN]: length of data to be decrypted;
    pucOut[OUT]: decrypted data;
    puiOutLen[OUT]: length of decrypted data;
RETURN VALUE:
    M_WE_LIB_ERR_OK if success,others error code.
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    Omit
IMPORTANT NOTES:
    no padding: the input data had been padded, so the length of 
                input data is multiples of block size.Padding is 
                stripped in the extern module.
=====================================================================================*/
WE_INT32 We_LibAESDecrypt
(
    St_WeCipherCrptKey  stKey, 
    const WE_UCHAR* pucIn, 
    WE_UINT32 uiInLen, 
    WE_UCHAR* pucOut, 
    WE_UINT32* puiOutLen
)
{
    WE_INT32 iRes = 0;
    St_WeAesKey stAESDecryptKey = {{0},0};

    
    iRes = We_AESSetDecryptKey(stKey.pucKey, 128, &stAESDecryptKey);
    if (iRes != 0)
    {
        return M_WE_LIB_ERR_INVALID_KEY;
    }
    We_AESCBCDecrypt(&stAESDecryptKey, stKey.pucIv, AES_DECRYPT, 
                     pucIn, uiInLen, pucOut, puiOutLen);
    return iRes;
}

/*=====================================================================================
FUNCTION: 
    We_LibAESDecryptInit
CREATE DATE: 
    2006-12-04
AUTHOR: 
    Sam
DESCRIPTION:
    Decrypt init with AES algorithm.
ARGUMENTS PASSED:
    stKey[IN]: cipher key
    hAESHandle[OUT]: data handle;
RETURN VALUE:
    M_WE_LIB_ERR_OK if success,others error code.
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    Omit
IMPORTANT NOTES:
    no padding: the input data had been padded, so the length of 
                input data is multiples of block size.Padding is 
                stripped in the extern module.
=====================================================================================*/
WE_INT32 We_LibAESDecryptInit
(
    St_WeCipherCrptKey  stKey, 
    WE_HANDLE *hAESHandle
)
{
    St_WeAESParameter *pstAESParam = NULL;
    WE_INT32 iRes = 0;

    if( !stKey.pucKey || !stKey.pucIv || (0 >= stKey.iKeyLen) || \
        (stKey.iIvLen < 16) || !hAESHandle)
    {
        return M_WE_LIB_ERR_INVALID_PARAMETER;
    }

    if((16 != stKey.iKeyLen)||(16 != stKey.iIvLen))
    {
        return M_WE_LIB_ERR_KEY_LENGTH;
    }

    pstAESParam = WE_MALLOC(sizeof(St_WeAESParameter));
    if (!pstAESParam)
    {
        return M_WE_LIB_ERR_INSUFFICIENT_MEMORY;
    }
    (void)WE_MEMSET( pstAESParam,0,sizeof(St_WeAESParameter) );
    iRes = We_AESSetDecryptKey(stKey.pucKey, 128, &(pstAESParam->stAESCipherKeyEx));
    if (iRes != 0)
    {
        WE_LIB_FREE(pstAESParam);
        return M_WE_LIB_ERR_INVALID_KEY;
    }
    pstAESParam->iType = (WE_INT32)E_WE_ALG_CIPHER_AES_CBC_128;
    (WE_VOID)WE_MEMCPY(pstAESParam->aucIv, stKey.pucIv, 16);
    (WE_VOID)WE_MEMSET(pstAESParam->aucBuf, 0, 16);
    pstAESParam->uiBufLen = 0;

    *hAESHandle = (WE_HANDLE)pstAESParam;
    return M_WE_LIB_ERR_OK;
}

/*=====================================================================================
FUNCTION: 
    We_LibAESDecryptUpdate
CREATE DATE: 
    2006-12-04
AUTHOR: 
    Sam
DESCRIPTION:
    Decrypt data with AES algorithm.
ARGUMENTS PASSED:
    pucIn[IN]: data part to be decrypted;
    uiInLen[IN]: length of data part;
    pucOut[OUT]: decrypted data;
    puiOutLen[OUT]: length of decrypted data;
    hAESHandle[IO]: data handle.
RETURN VALUE:
    M_WE_LIB_ERR_OK if success,others error code.
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    Omit
IMPORTANT NOTES:
    no padding: the input data had been padded, so the length of 
                input data is multiples of block size.Padding is 
                stripped in the extern module.
=====================================================================================*/
WE_INT32 We_LibAESDecryptUpdate
( 
    const WE_UCHAR* pucIn, 
    WE_UINT32 uiInLen, 
    WE_UCHAR* pucOut, 
    WE_UINT32* puiOutLen, 
    WE_HANDLE hAESHandle
)
{
    St_WeAESParameter *pstAESParam = NULL;
    WE_INT32 iRes = 0;
    WE_UINT32 uiLen = 0;

    if( (0 == uiInLen) || (!pucIn) || (!pucOut) || (!puiOutLen) || !hAESHandle)
    {
        return M_WE_LIB_ERR_INVALID_PARAMETER;
    }

    pstAESParam = (St_WeAESParameter *)hAESHandle;
    uiLen = uiInLen % 16;
    if ((0 == uiLen) && (*puiOutLen >= uiInLen))
    {
        *puiOutLen = uiInLen;
    }
    else
    {
        return M_WE_LIB_ERR_DATA_LENGTH;
    }
    
    We_AESCBCDecrypt(&(pstAESParam->stAESCipherKeyEx), pstAESParam->aucIv, 
                            pstAESParam->iType, pucIn, uiInLen, pucOut, puiOutLen);
    
    return iRes;
}

/*=====================================================================================
FUNCTION: 
    We_LibAESDecryptFinal
CREATE DATE: 
    2006-12-04
AUTHOR: 
    Sam
DESCRIPTION:
    Free the data handle.
ARGUMENTS PASSED:
    hAESHandle[IN]: data handle.
RETURN VALUE:
    M_WE_LIB_ERR_OK if success,others error code.
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    Omit
IMPORTANT NOTES:
    no padding: the input data had been padded, so the length of 
                input data is multiples of block size.Padding is 
                stripped in the extern module.
=====================================================================================*/
WE_INT32 We_LibAESDecryptFinal
(
    WE_HANDLE hAESHandle
)
{
    St_WeAESParameter *pstAESParam = NULL;
    WE_INT32 iRes = 0;

    if((NULL == hAESHandle))
    {
        return M_WE_LIB_ERR_INVALID_PARAMETER;
    }

    pstAESParam = (St_WeAESParameter *)hAESHandle;
    WE_LIB_FREE(pstAESParam);

    return iRes;
}

/*=====================================================================================
FUNCTION: 
    We_LibAESDecrypt1
CREATE DATE: 
    2006-12-04
AUTHOR: 
    Sam
DESCRIPTION:
    Set decrypt key and decrypt data with AES algorithm.
ARGUMENTS PASSED:
    stKey[IN]: cipher key
    pucIn[IN]: data to be decrypted;
    uiInLen[IN]: length of data to be decrypted;
    pucOut[OUT]: decrypted data;
    puiOutLen[OUT]: length of decrypted data;
RETURN VALUE:
    M_WE_LIB_ERR_OK if success,others error code.
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    Omit
IMPORTANT NOTES:
    rfc2630 padding: pad interior, and pad each byte with # bytes 
                     padded(# is equal to the length of padding, 
                     at least 1 byte).Padding is stripped on decrypt.
=====================================================================================*/
WE_INT32 We_LibAESDecrypt1
(
    St_WeCipherCrptKey  stKey, 
    const WE_UCHAR* pucIn, 
    WE_UINT32 uiInLen, 
    WE_UCHAR* pucOut, 
    WE_UINT32* puiOutLen
)
{
    WE_INT32 iRes = 0;
    WE_UINT32 uiLen = 0;
    St_WeAesKey stAESDecryptKeyEx = {{0},0};
    
    if (!pucIn||!pucOut||!puiOutLen||!stKey.pucKey)
    {
        return M_WE_LIB_ERR_INVALID_PARAMETER;
    }
    if ((stKey.iKeyLen != 16)||(stKey.iIvLen != 16))
    {
        return M_WE_LIB_ERR_KEY_LENGTH;
    }

    iRes = We_AESSetDecryptKey(stKey.pucKey, 128, &stAESDecryptKeyEx);
    if (iRes != M_WE_LIB_ERR_OK)
    {
        return M_WE_LIB_ERR_INVALID_KEY;
    }
    
    uiLen = uiInLen % 16;
    if ((uiLen == 0) && *puiOutLen >= uiInLen)
    {
        *puiOutLen = uiInLen;
    }
    else
    {
        return M_WE_LIB_ERR_DATA_LENGTH;
    }
    
    We_AESCBCDecrypt(&stAESDecryptKeyEx, stKey.pucIv, AES_DECRYPT, 
                            pucIn, uiInLen, pucOut, puiOutLen);

    uiLen = *(pucOut + uiInLen -1);
    if ((uiLen > 0) && (uiLen <= 16))
    {
        WE_UINT32   uiLoop = 0;
        for(; uiLoop < uiLen; uiLoop++)
        {
            if(uiLen != *(pucOut + uiInLen - 1 - uiLoop))
            {
                break;
            }
        }
        if(uiLoop == uiLen)
        {
            *puiOutLen -= uiLen;
        }    
    }
    return M_WE_LIB_ERR_OK;
}

/*=====================================================================================
FUNCTION: 
    We_LibAESDecryptInit1
CREATE DATE: 
    2006-12-04
AUTHOR: 
    Sam
DESCRIPTION:
    Decrypt init with AES algorithm.
ARGUMENTS PASSED:
    stKey[IN]: cipher key
    hAESHandle[OUT]: data handle;
RETURN VALUE:
    M_WE_LIB_ERR_OK if success,others error code.
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    Omit
IMPORTANT NOTES:
    rfc2630 padding: pad interior, and pad each byte with # bytes 
                     padded(# is equal to the length of padding, 
                     at least 1 byte).Padding is stripped on decrypt.
=====================================================================================*/
WE_INT32 We_LibAESDecryptInit1
(
    St_WeCipherCrptKey  stKey, 
    WE_HANDLE *hAESHandle
)
{
    return We_LibAESDecryptInit( stKey, hAESHandle);
}

/*=====================================================================================
FUNCTION: 
    We_LibAESDecryptUpdate1
CREATE DATE: 
    2006-12-04
AUTHOR: 
    Sam
DESCRIPTION:
    Decrypt data with AES algorithm.
ARGUMENTS PASSED:
    pucIn[IN]: data part to be decrypted;
    uiInLen[IN]: length of data part;
    pucOut[OUT]: decrypted data;
    puiOutLen[OUT]: length of decrypted data;
    hAESHandle[IO]: data handle.
RETURN VALUE:
    M_WE_LIB_ERR_OK if success,others error code.
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    Omit
IMPORTANT NOTES:
    rfc2630 padding: pad interior, and pad each byte with # bytes 
                     padded(# is equal to the length of padding, 
                     at least 1 byte).Padding is stripped on decrypt.
=====================================================================================*/
WE_INT32 We_LibAESDecryptUpdate1
(
    const WE_UCHAR* pucIn, 
    WE_UINT32 uiInLen, 
    WE_UCHAR* pucOut, 
    WE_UINT32* puiOutLen, 
    WE_HANDLE hAESHandle
)
{
    St_WeAESParameter *pstAESParam = NULL;
    WE_INT32 iRes = 0;
    WE_UINT32 uiLen = 0;
    WE_UINT32 uiBufLen = 0;

    if( (0 == uiInLen) || (!pucIn) || (!pucOut) || (!puiOutLen) || !hAESHandle)
    {
        return M_WE_LIB_ERR_INVALID_PARAMETER;
    }
    pstAESParam = (St_WeAESParameter *)hAESHandle;

    uiLen = uiInLen % 16;
    if (uiLen != 0 && *puiOutLen >= (1 + uiInLen/16)*16)
    {
        *puiOutLen = (1 + uiInLen/16)*16;
    }
    else if (uiLen == 0 && *puiOutLen >= uiInLen)
    {
        *puiOutLen = uiInLen;
    }
    else
    {
        return M_WE_LIB_ERR_DATA_LENGTH;
    }

    uiBufLen = pstAESParam->uiBufLen;
    if (uiInLen + uiBufLen < *puiOutLen)
    {
        pstAESParam->uiBufLen = (uiInLen + uiBufLen) % 16;
        *puiOutLen -= 16;
        if (uiInLen + uiBufLen > 16)
        {
            (WE_VOID)WE_MEMCPY((pstAESParam->aucBuf + uiBufLen), pucIn, (16 - uiBufLen));
            We_AESCBCDecrypt(&(pstAESParam->stAESCipherKeyEx), pstAESParam->aucIv, 
                         AES_DECRYPT, pstAESParam->aucBuf, 16, pucOut, puiOutLen);
            if (uiInLen + uiBufLen - pstAESParam->uiBufLen > 16)
            {
                We_AESCBCDecrypt(&(pstAESParam->stAESCipherKeyEx), pstAESParam->aucIv, 
                                 AES_DECRYPT, pucIn + (16 - uiBufLen), 
                                 uiInLen - (16 - uiBufLen) - pstAESParam->uiBufLen, 
                                 pucOut + 16, puiOutLen);
            }
            uiBufLen = uiInLen - pstAESParam->uiBufLen;
            (WE_VOID)WE_MEMCPY(pstAESParam->aucBuf, pucIn + uiBufLen, pstAESParam->uiBufLen);
        }
        else
        {
            (WE_VOID)WE_MEMCPY((pstAESParam->aucBuf + uiBufLen), pucIn, uiInLen);
        }
    }
    else if (uiInLen + uiBufLen == *puiOutLen)
    {
        pstAESParam->uiBufLen = 0;
        (WE_VOID)WE_MEMCPY((pstAESParam->aucBuf + uiBufLen), pucIn, (16 - uiBufLen));
        We_AESCBCDecrypt(&(pstAESParam->stAESCipherKeyEx), pstAESParam->aucIv, 
                         AES_DECRYPT, pstAESParam->aucBuf, 16, pucOut, puiOutLen);
        if (*puiOutLen > 16)
        {
            We_AESCBCDecrypt(&(pstAESParam->stAESCipherKeyEx), pstAESParam->aucIv, 
                             AES_DECRYPT, pucIn + 16 - uiBufLen, uiInLen - (16 - uiBufLen), 
                             pucOut + 16, puiOutLen);
        }
        /* to decrypt the last 16 bytes, which contain padding data. */
        (WE_VOID)WE_MEMCPY(pstAESParam->aucBuf, pucOut + *puiOutLen - 16, 16);
    }
    else
    {
        pstAESParam->uiBufLen = (uiInLen + uiBufLen) % 16;
        (WE_VOID)WE_MEMCPY(pstAESParam->aucBuf + uiBufLen, pucIn, (16 - uiBufLen));
        We_AESCBCDecrypt(&(pstAESParam->stAESCipherKeyEx), pstAESParam->aucIv, 
                         AES_DECRYPT, pstAESParam->aucBuf, 16, pucOut, puiOutLen);
        if (uiInLen + uiBufLen - pstAESParam->uiBufLen > 16)
        {
            We_AESCBCDecrypt(&(pstAESParam->stAESCipherKeyEx), pstAESParam->aucIv, 
                         AES_DECRYPT, pucIn + (16 - uiBufLen), 
                         uiInLen - (16 - uiBufLen) - pstAESParam->uiBufLen, 
                         pucOut + 16, puiOutLen);
        }
    }

    return iRes;
}

/*=====================================================================================
FUNCTION: 
    We_LibAESDecryptFinal1
CREATE DATE: 
    2006-12-04
AUTHOR: 
    Sam
DESCRIPTION:
    Stripped the padding and free memory.
ARGUMENTS PASSED:
    pucOutEndp[IN]: pointer to the end of decrypted data;
    puiPadLen[OUT]: length of padding;
    hAESHandle[IN]: data handle.
RETURN VALUE:
    M_WE_LIB_ERR_OK if success,others error code.
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    Omit
IMPORTANT NOTES:
    rfc2630 padding: pad interior, and pad each byte with # bytes 
                     padded(# is equal to the length of padding, 
                     at least 1 byte).Padding is stripped on decrypt.
=====================================================================================*/
WE_INT32 We_LibAESDecryptFinal1
(
    WE_UCHAR* pucOutEndp, 
    WE_UINT32* puiPadLen, 
    WE_HANDLE hAESHandle
)
{
    St_WeAESParameter *pstAESParam = NULL;
    WE_INT32 iRes = 0;
    WE_UINT32 uiLen = 0;

    if (!pucOutEndp||!puiPadLen||!hAESHandle)
    {
        return M_WE_LIB_ERR_INVALID_PARAMETER;
    }
    *puiPadLen = 0;
    pstAESParam = (St_WeAESParameter *)hAESHandle;
    uiLen = *(pucOutEndp - 1);
    if (uiLen != 0 && uiLen <= 16)
    {
        WE_UINT32   uiLoop = 0;
        
        for(; uiLoop < uiLen; uiLoop++)
        {
            if(uiLen != *(pucOutEndp  - 1 - uiLoop))
            {
                break;
            }
        }
        if(uiLoop == uiLen)
        {
            *puiPadLen = uiLen;
        }
    }
    WE_LIB_FREE(pstAESParam);
    
    return iRes;
}

/*=====================================================================================
FUNCTION: 
    We_AESCBCEncrypt
CREATE DATE: 
    2006-12-04
AUTHOR: 
    Sam
DESCRIPTION:
    Set the input data to block size and encrypt with AES algorithm.
ARGUMENTS PASSED:
    pstAESKey[IN]: cipher key
    pucIVec[IN]: IV value;
    enc[IN]: direction type(ecnryption/decryption);
    pucInData[IN]: data to be ecnrypted;
    uiInDataLen[IN]: length of data to be ecnrypted;
    pucOutData[OUT]: encrypted data;
    puiOutDataLen[OUT]: length of encrypted data.
RETURN VALUE:
    M_WE_LIB_ERR_OK if success,others error code.
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    Omit
IMPORTANT NOTES:
    
=====================================================================================*/
WE_VOID We_AESCBCEncrypt
(
    const St_WeAesKey *pstAESKey,
    WE_UINT8 *pucIVec, 
    const WE_INT32 enc, 
    const WE_UINT8 *pucInData, 
    const WE_UINT32 uiInDataLen, 
    WE_UINT8 *pucOutData,
    WE_UINT32 *puiOutDataLen
)
{
    WE_UINT32 n = 0;
    WE_UINT32 uiLength = uiInDataLen;
    WE_UINT8  aucTmp[AES_BLOCK_SIZE] = {0};

    if (!pucInData||!pucIVec||!pucOutData||!puiOutDataLen)
    {
        return;
    }

    if (AES_ENCRYPT != enc)
    {
        return;
    }
    while (uiLength >= AES_BLOCK_SIZE) 
    {
        for(n=0; n < sizeof aucTmp; ++n)
        {
            aucTmp[n] = pucInData[n] ^ pucIVec[n];
        }
        We_AESEncrypt(aucTmp, pucOutData, pstAESKey);            
        (WE_VOID)WE_MEMCPY(pucIVec, pucOutData, AES_BLOCK_SIZE);
        
        uiLength -= AES_BLOCK_SIZE;
        pucInData += AES_BLOCK_SIZE;
        pucOutData += AES_BLOCK_SIZE;
    }
    if (uiLength > 0 && uiLength < 16) 
    {
        for(n=0; n < uiLength; ++n)
        {
            aucTmp[n] = pucInData[n] ^ pucIVec[n];
        }
        for(n=uiLength; n < AES_BLOCK_SIZE; ++n)
        {
            aucTmp[n] = pucIVec[n];
        }
        We_AESEncrypt(aucTmp, aucTmp, pstAESKey);
        (WE_VOID)WE_MEMCPY(pucOutData, aucTmp, uiLength);
        (WE_VOID)WE_MEMCPY(pucIVec, aucTmp, sizeof aucTmp);
    }
    puiOutDataLen = puiOutDataLen;
}


/*=====================================================================================
FUNCTION: 
    We_AESCBCDecrypt
CREATE DATE: 
    2006-12-04
AUTHOR: 
    Sam
DESCRIPTION:
    Set the input data to block size and decrypt with AES algorithm.
ARGUMENTS PASSED:
    pstAESKey[IN]: cipher key
    pucIVec[IN]: IV value;
    enc[IN]: direction type(ecnryption/decryption);
    pucInData[IN]: data to be denrypted;
    uiInDataLen[IN]: length of data to be denrypted;
    pucOutData[OUT]: decrypted data;
    puiOutDataLen[OUT]: length of decrypted data.
RETURN VALUE:
    M_WE_LIB_ERR_OK if success,others error code.
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    Omit
IMPORTANT NOTES:
    
=====================================================================================*/
WE_VOID We_AESCBCDecrypt
(
    const St_WeAesKey *pstAESKey,
    WE_UINT8 *pucIVec, 
    const WE_INT32 enc, 
    const WE_UINT8 *pucInData, 
    const WE_UINT32 uiInDataLen, 
    WE_UINT8 *pucOutData,
    WE_UINT32 *puiOutDataLen
)
{
    WE_UINT32 n = 0;
    WE_UINT32 uiLength = uiInDataLen;
    WE_UINT8  aucTmp[AES_BLOCK_SIZE] = {0};

    if (!pucIVec||!pucInData||!pucOutData||!puiOutDataLen)
    {
        return;
    }

    if (AES_DECRYPT != enc)
    {
        return;
    }
    while (uiLength >= AES_BLOCK_SIZE) 
    {
        (WE_VOID)WE_MEMCPY(aucTmp, pucInData, sizeof aucTmp);
        We_AESDecrypt(pucInData, pucOutData, pstAESKey);
        for(n=0; n < AES_BLOCK_SIZE; ++n)
        {
            pucOutData[n] ^= pucIVec[n];
        }
        (WE_VOID)WE_MEMCPY(pucIVec, aucTmp, AES_BLOCK_SIZE);

        uiLength -= AES_BLOCK_SIZE;
        pucInData += AES_BLOCK_SIZE;
        pucOutData += AES_BLOCK_SIZE;
    }
    if (uiLength) 
    {
        (WE_VOID)WE_MEMCPY(aucTmp, pucInData, sizeof aucTmp);
        We_AESDecrypt(aucTmp, aucTmp, pstAESKey);
        for(n=0; n < uiLength; ++n)
        {
            aucTmp[n] ^= pucIVec[n];
        }
        (WE_VOID)WE_MEMCPY(pucIVec, aucTmp, sizeof aucTmp);
    }
    puiOutDataLen = puiOutDataLen;
}

/*rc2*/

/*******************************************************************************
*   Type Define Section
*******************************************************************************/
typedef struct tagSt_WeRC2Parameter
{
    WE_UINT32       uiType;
    WE_UINT16       ausXKey[64];
    WE_UCHAR        aucIv[WE_RC2_BLOCK_SIZE];
    WE_UCHAR        aucBuf[WE_RC2_BLOCK_SIZE];
    WE_INT32        iBufLen;
}St_WeRC2Parameter;

/*******************************************************************************
*   Prototype Declare Section
*******************************************************************************/

static WE_INT32 We_LibRC2EncryptCBC
(
    const WE_UCHAR* pucIn, 
    WE_UINT32 uiInLen, 
    WE_UCHAR* pucOut, 
    WE_HANDLE handle
);

static WE_INT32 We_LibRC2Encrypt
(
    const WE_UCHAR* pucIn, 
    WE_UINT32 uiInLen, 
    WE_UCHAR* pucOut, 
    WE_HANDLE handle
);

static WE_INT32 We_LibRC2DecryptCBC
(
    const WE_UCHAR* pucIn, 
    WE_UINT32 uiInLen, 
    WE_UCHAR* pucOut, 
    WE_HANDLE handle
);

static WE_INT32 We_LibRC2Decrypt
(
    const WE_UCHAR* pucIn, 
    WE_UINT32 uiInLen, 
    WE_UCHAR* pucOut, 
    WE_HANDLE handle
);

/*******************************************************************************
*   Function Define Section
*******************************************************************************/
/*==================================================================================================
FUNCTION: 
    We_LibRC2EncryptNoPadding
CREATE DATE:
    2006-11-23
AUTHOR:
    stone An
DESCRIPTION:
    RC2 encrypt with no padding. The length of input is divided exactly by WE_RC2_BLOCK_SIZE.
ARGUMENTS PASSED:
    St_WeCipherKey  stKey[IN]: secret key.
    const WE_UCHAR* pucIn[IN]: the data need to be encrypted.
    WE_UINT32       uiInLen[IN]: length of input data.
    WE_UCHAR*       pucOut[OUT]: the encrypted data.
    WE_UINT32*      puiOutLen[IN/OUT]: the length of output buffer when IN, 
                                       the length of encrypted data when OUT.
RETURN VALUE:
    M_WE_LIB_ERR_OK: success.
    other : fail.
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    omit.
IMPORTANT NOTES:
    None
==================================================================================================*/
WE_INT32 We_LibRC2EncryptNoPadding
(
    St_WeCipherCrptKey  stKey, 
    const WE_UCHAR* pucIn, 
    WE_UINT32 uiInLen, 
    WE_UCHAR* pucOut, 
    WE_UINT32* puiOutLen
)
{
    WE_INT32     iRv      = M_WE_LIB_ERR_OK;
    WE_INT32     iRes     = M_WE_LIB_ERR_OK;
    WE_HANDLE    hHandle  = NULL;
    
    if( !stKey.pucKey || (0 >= stKey.iKeyLen) || 
        (0 == uiInLen) || (!pucIn) || (!pucOut) || (!puiOutLen))
    {
        return M_WE_LIB_ERR_INVALID_PARAMETER;
    }
    
    iRes = We_LibRC2EncryptInit( stKey, 128, &hHandle);    
    if (iRes == M_WE_LIB_ERR_OK)
    {
        iRv = We_LibRC2EncryptUpdate( pucIn, uiInLen, pucOut, puiOutLen, hHandle);
        if (iRv == M_WE_LIB_ERR_OK)
        {
            iRes = We_LibRC2EncryptFinal( hHandle);
        }
        else
        {
            iRes = We_LibRC2EncryptFinal( hHandle);
            return iRv;
        }
    }
    
    return iRes;
}

/*==================================================================================================
FUNCTION: 
    We_LibRC2EncryptPadding
CREATE DATE:
    2006-11-23
AUTHOR:
    stone An
DESCRIPTION:
    RC2 encrypt with padding. The length of input is not divided exactly by WE_RC2_BLOCK_SIZE.
ARGUMENTS PASSED:
    St_WeCipherKey  stKey[IN]: secret key.
    const WE_UCHAR* pucIn[IN]: the data need to be encrypted.
    WE_UINT32       uiInLen[IN]: length of input data.
    WE_UCHAR*       pucOut[OUT]: the encrypted data.
    WE_UINT32*      puiOutLen[IN/OUT]: the length of output buffer when IN, 
                                       the length of encrypted data when OUT.
RETURN VALUE:
    M_WE_LIB_ERR_OK: success.
    other : fail.
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    omit.
IMPORTANT NOTES:
    None
==================================================================================================*/
WE_INT32 We_LibRC2EncryptPadding
(
    St_WeCipherCrptKey  stKey, 
    const WE_UCHAR* pucIn, 
    WE_UINT32 uiInLen, 
    WE_UCHAR* pucOut, 
    WE_UINT32* puiOutLen
)
{
    WE_INT32     iRv      = M_WE_LIB_ERR_OK;
    WE_INT32     iRes     = M_WE_LIB_ERR_OK;
    WE_HANDLE    hHandle  = NULL;
    WE_UINT32    uiTmpLen = 0;
    
    if( !stKey.pucKey || (0 >= stKey.iKeyLen) || 
        (0 == uiInLen) || (!pucIn) || (!pucOut) || (!puiOutLen))
    {
        return M_WE_LIB_ERR_INVALID_PARAMETER;
    }
    uiTmpLen = *puiOutLen;
    
    iRes = We_LibRC2EncryptInitPad( stKey, 128, &hHandle);    
    if (iRes == M_WE_LIB_ERR_OK)
    {
        iRv = We_LibRC2EncryptUpdatePad( pucIn, uiInLen, pucOut, puiOutLen, hHandle);
        uiTmpLen -= (*puiOutLen);
        if (iRv == M_WE_LIB_ERR_OK)
        {
            iRes = We_LibRC2EncryptFinalPad( pucOut+(*puiOutLen), &uiTmpLen, hHandle);
        }
        else
        {
            iRes = We_LibRC2EncryptFinalPad( pucOut+(*puiOutLen), &uiTmpLen, hHandle);
            return iRv;
        }
    }

    (*puiOutLen) += uiTmpLen;
    
    return iRes;
}

/*==================================================================================================
FUNCTION: 
    We_LibRC2EncryptInit
CREATE DATE:
    2006-11-23
AUTHOR:
    stone An
DESCRIPTION:
    initialize RC2 algorithm without padding.
ARGUMENTS PASSED:
    St_WeCipherKey  stKey[IN]: secret key.
    WE_UINT32       uiBits[IN]: the available bits of secret key.
    WE_HANDLE*      pHandle[OUT]: the handle of RC2.
RETURN VALUE:
    M_WE_LIB_ERR_OK: success.
    other : fail.
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    omit.
IMPORTANT NOTES:
    None
==================================================================================================*/
WE_INT32 We_LibRC2EncryptInit
(
    St_WeCipherCrptKey stKey,
    WE_UINT32 uiBits,
    WE_HANDLE* pHandle
)
{
    St_WeRC2Parameter*   pstPara   = NULL;
    WE_INT32             iIndexI = 0; 
    WE_UCHAR             ucTmp = 0;  
    WE_UINT32            uiT8 = 0;
    WE_UINT8             ucTM = 0;
    WE_UINT8             *pucXKey = NULL;

    /* 256-entry permutation table, probably derived somehow from pi */    
    const WE_UCHAR pucPermute[256] = {
            217,120,249,196, 25,221,181,237, 40,233,253,121, 74,160,216,157,
            198,126, 55,131, 43,118, 83,142, 98, 76,100,136, 68,139,251,162,
            23,154, 89,245,135,179, 79, 19, 97, 69,109,141, 9,129,125, 50,
            189,143, 64,235,134,183,123, 11,240,149, 33, 34, 92,107, 78,130,
            84,214,101,147,206, 96,178, 28,115, 86,192, 20,167,140,241,220,
            18,117,202, 31, 59,190,228,209, 66, 61,212, 48,163, 60,182, 38,
            111,191, 14,218, 70,105, 7, 87, 39,242, 29,155,188,148, 67, 3,
            248, 17,199,246,144,239, 62,231, 6,195,213, 47,200,102, 30,215, 
            8,232,234,222,128, 82,238,247,132,170,114,172, 53, 77,106, 42,
            150, 26,210,113, 90, 21, 73,116, 75,159,208, 94, 4, 24,164,236,
            194,224, 65,110, 15, 81,203,204, 36,145,175, 80,161,244,112, 57,
            153,124, 58,133, 35,184,180,122,252, 2, 54, 91, 37, 85,151, 49,
            45, 93,250,152,227,138,146,174, 5,223, 41, 16,103,108,186,201,
            211, 0,230,207,225,158,168, 44, 99, 22, 1, 63, 88,226,137,169,
            13, 56, 52, 27,171, 51,255,176,187, 72, 12, 95,185,177,205, 46, 
            197,243,219, 71,229,165,156,119, 10,166, 32,104,254,127,193,173
    };
    
    if ((uiBits>1024) || (NULL==stKey.pucKey) || 
        (0>=stKey.iKeyLen) || (stKey.iKeyLen>128)||!pHandle)
    {
        return M_WE_LIB_ERR_INVALID_PARAMETER;
    }
    

    if(WE_RC2_BLOCK_SIZE != stKey.iIvLen)
    {
        return M_WE_LIB_ERR_KEY_LENGTH;
    }    
 
    if (0 == uiBits)
    {
        uiBits = 1024;
    }
    
    pstPara = (St_WeRC2Parameter *)WE_MALLOC(sizeof(St_WeRC2Parameter));
    pucXKey = (WE_UINT8 *)WE_MALLOC(128 * sizeof(WE_UINT8));
    if ((NULL == pucXKey)  || (NULL == pstPara))
    {
        if (NULL != pucXKey) 
        {
            WE_FREE(pucXKey);
        }
        if (NULL != pstPara)
        {
            WE_FREE(pstPara);
        }
        return M_WE_LIB_ERR_INSUFFICIENT_MEMORY;
    }

    (WE_VOID)WE_MEMSET(pstPara, 0, sizeof(St_WeRC2Parameter));
    
    for (iIndexI=0; iIndexI<stKey.iKeyLen; iIndexI++)
    {
        pucXKey[iIndexI] = stKey.pucKey[iIndexI];
    }
    /* Phase 1: Expand input key to 128 bytes */
    for (iIndexI=stKey.iKeyLen; iIndexI<128; iIndexI++)
    {
        ucTmp = (pucXKey[iIndexI-1] + pucXKey[iIndexI-stKey.iKeyLen]) % 256;
        pucXKey[iIndexI] = pucPermute[ucTmp]; 
    }
    /* Phase 2 - reduce effective key size to "uiBits" */ 
    uiT8 = (uiBits + 7) >> 3;
    ucTM = 255 % (1 << (8 + uiBits - 8*uiT8));
    
    ucTmp = (pucXKey)[128 - uiT8] & ucTM;   
    pucXKey[128 - uiT8] = pucPermute[ucTmp];
    
    if (127 >= uiT8)
    {
        for (iIndexI=127-(WE_INT32)uiT8; iIndexI>=0; iIndexI--)
        {
            ucTmp = pucXKey[iIndexI+1] ^ pucXKey[iIndexI+(WE_INT32)uiT8];
            pucXKey[iIndexI] = pucPermute[ucTmp];
        }
    }
    /* Phase 3 - copy to (pucXKey) in little-endian order */
    for (iIndexI = 0; iIndexI<64; iIndexI++)
    {
        (pstPara->ausXKey)[iIndexI] = pucXKey[2*iIndexI]
            + (pucXKey[2*iIndexI+1] << 8);
    }

    pstPara->uiType = M_WE_RC2_TYPE;
    (WE_VOID)WE_MEMCPY(pstPara->aucIv, stKey.pucIv, WE_RC2_BLOCK_SIZE);

    *pHandle = pstPara;
    WE_FREE(pucXKey);
    return M_WE_LIB_ERR_OK;
}

/*==================================================================================================
FUNCTION: 
    We_LibRC2EncryptInitPad
CREATE DATE:
    2006-11-23
AUTHOR:
    stone An
DESCRIPTION:
    initialize RC2 algorithm using with padding.
ARGUMENTS PASSED:
    St_WeCipherKey  stKey[IN]: secret key.
    WE_UINT32       uiBits[IN]: the available bits of secret key.
    WE_HANDLE*      pHandle[OUT]: the handle of RC2.
RETURN VALUE:
    M_WE_LIB_ERR_OK: success.
    other : fail.
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    omit.
IMPORTANT NOTES:
    None
==================================================================================================*/
WE_INT32 We_LibRC2EncryptInitPad
(
    St_WeCipherCrptKey stKey,
    WE_UINT32 uiBits,
    WE_HANDLE* pHandle
)
{
    return We_LibRC2EncryptInit( stKey, uiBits, pHandle);
}

/*==================================================================================================
FUNCTION: 
    We_LibRC2EncryptUpdate
CREATE DATE:
    2006-11-23
AUTHOR:
    stone An
DESCRIPTION:
    encrypt data with RC2 algorithm.
ARGUMENTS PASSED:
    const WE_UCHAR* pucIn[IN]: the data need to be encrypted.
    WE_UINT32       uiInLen[IN]: length of input data.
    WE_UCHAR*       pucOut[OUT]: the encrypted data.
    WE_UINT32*      puiOutLen[IN/OUT]: the length of output buffer when IN, 
                                       the length of encrypted data when OUT.
    WE_HANDLE       Handle[IN]: the handle of RC2.
RETURN VALUE:
    M_WE_LIB_ERR_OK: success.
    other : fail.
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    omit.
IMPORTANT NOTES:
    None
==================================================================================================*/
WE_INT32 We_LibRC2EncryptUpdate
(
    const WE_UCHAR* pucIn, 
    WE_UINT32 uiInLen, 
    WE_UCHAR* pucOut, 
    WE_UINT32* puiOutLen,
    WE_HANDLE handle
)
{
    WE_UINT32             uiLen     = 0; 

    if( (0 == uiInLen) || (!pucIn) || (!pucOut) || (!puiOutLen) || !handle)
    {
        return M_WE_LIB_ERR_INVALID_PARAMETER;
    }
    
    if(*puiOutLen >= uiInLen )
    {
        *puiOutLen = uiInLen;
    }
    else
    {
        return M_WE_LIB_ERR_BUFFER_TOO_SMALL;
    }

    uiLen = uiInLen % WE_RC2_BLOCK_SIZE;
    if(0 == uiLen)
    {        
        (WE_VOID)We_LibRC2EncryptCBC( pucIn, uiInLen, pucOut, handle);
    }
    else
    {
        return M_WE_LIB_ERR_DATA_LENGTH;
    }

    return M_WE_LIB_ERR_OK;    
}

/*==================================================================================================
FUNCTION: 
    We_LibRC2EncryptUpdatePad
CREATE DATE:
    2006-11-23
AUTHOR:
    stone An
DESCRIPTION:
    encrypt data with RC2 algorithm using with padding.
ARGUMENTS PASSED:
    const WE_UCHAR* pucIn[IN]: the data need to be encrypted.
    WE_UINT32       uiInLen[IN]: length of input data.
    WE_UCHAR*       pucOut[OUT]: the encrypted data.
    WE_UINT32*      puiOutLen[IN/OUT]: the length of output buffer when IN, 
                                       the length of encrypted data when OUT.
    WE_HANDLE       Handle[IN]: the handle of RC2.
RETURN VALUE:
    M_WE_LIB_ERR_OK: success.
    other : fail.
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    omit.
IMPORTANT NOTES:
    None
==================================================================================================*/
WE_INT32 We_LibRC2EncryptUpdatePad
(
    const WE_UCHAR* pucIn, 
    WE_UINT32 uiInLen, 
    WE_UCHAR* pucOut, 
    WE_UINT32* puiOutLen,
    WE_HANDLE handle
)
{
    WE_UINT32             uiRealInLen = 0;
    WE_UINT32             uiIndex    = 0;
    St_WeRC2Parameter*    pstPara   = NULL;

    if( (0 == uiInLen) || (!pucIn) || (!pucOut) || (!puiOutLen) || !handle)
    {
        return M_WE_LIB_ERR_INVALID_PARAMETER;
    }
    
    pstPara = (St_WeRC2Parameter*)handle;
    uiIndex = (WE_UINT32)(pstPara->iBufLen);

    uiRealInLen = ((uiInLen + uiIndex) / WE_RC2_BLOCK_SIZE) * WE_RC2_BLOCK_SIZE;
    if (*puiOutLen >= uiRealInLen)
    {
        *puiOutLen = uiRealInLen;
    }
    else
    {
        return M_WE_LIB_ERR_BUFFER_TOO_SMALL;
    }

    pstPara->iBufLen = (WE_INT32)(uiInLen + uiIndex - uiRealInLen);
    (WE_VOID)WE_MEMCPY((pstPara->aucBuf + uiIndex), pucIn, (WE_UINT32)(WE_RC2_BLOCK_SIZE - uiIndex));
    (WE_VOID)We_LibRC2EncryptCBC( pstPara->aucBuf, WE_RC2_BLOCK_SIZE, pucOut, handle);
    if (WE_RC2_BLOCK_SIZE != *puiOutLen)
    {
        (WE_VOID)We_LibRC2EncryptCBC( (pucIn + (WE_RC2_BLOCK_SIZE - uiIndex)),            
                uiRealInLen-WE_RC2_BLOCK_SIZE, pucOut+WE_RC2_BLOCK_SIZE, handle);
    }
    uiIndex = uiInLen - (WE_UINT32)(pstPara->iBufLen);
    (WE_VOID)WE_MEMCPY(pstPara->aucBuf, pucIn + uiIndex, (WE_UINT32)(pstPara->iBufLen));
    
    return M_WE_LIB_ERR_OK;    
}

/*==================================================================================================
FUNCTION: 
    We_LibRC2EncryptFinal
CREATE DATE:
    2006-11-23
AUTHOR:
    stone An
DESCRIPTION:
    the final handle of RC2 algorithm, release the handle of RC2 algorithm.
ARGUMENTS PASSED:
    WE_HANDLE       Handle[IN]: the handle of RC2 algorithm.
RETURN VALUE:
    M_WE_LIB_ERR_OK: success.
    other : fail.
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    omit.
IMPORTANT NOTES:
    None
==================================================================================================*/
WE_INT32 We_LibRC2EncryptFinal(WE_HANDLE handle)
{
    St_WeRC2Parameter*   pstPara   = NULL;

    if (!handle)
    {
        return 0x08;
    }
    
    pstPara = (St_WeRC2Parameter *)handle;
    WE_FREE(pstPara);    

    return M_WE_LIB_ERR_OK;
}

/*==================================================================================================
FUNCTION: 
    We_LibRC2EncryptFinalPad
CREATE DATE:
    2006-11-23
AUTHOR:
    stone An
DESCRIPTION:
    the final handle with pad. Encryt the left plain data and then release the handle of RC2 algorthm.
ARGUMENTS PASSED:
    WE_UCHAR*       pucOut[OUT]: the encrypted data.
    WE_UINT32*      puiOutLen[IN/OUT]: the length of output buffer when IN, 
                                       the length of encrypted data when OUT.
    WE_HANDLE       Handle[IN]: the handle of RC2 algorithm.
RETURN VALUE:
    M_WE_LIB_ERR_OK: success.
    other : fail.
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    omit.
IMPORTANT NOTES:
    None
==================================================================================================*/
WE_INT32 We_LibRC2EncryptFinalPad
(
    WE_UCHAR* pucOut, 
    WE_UINT32* puiOutLen,  
    WE_HANDLE handle
)
{
    St_WeRC2Parameter*   pstPara   = NULL;
    WE_UCHAR             aucData[WE_RC2_BLOCK_SIZE]    = {0};
    
    if((NULL == handle) || (NULL == pucOut) || (NULL == puiOutLen))
    {
        return M_WE_LIB_ERR_INVALID_PARAMETER;
    }
    

    pstPara = (St_WeRC2Parameter*)handle;
    
    if (pstPara->iBufLen != 0)
    {  
        if(*puiOutLen < WE_RC2_BLOCK_SIZE)
        {
            return M_WE_LIB_ERR_BUFFER_TOO_SMALL;
        }
        *puiOutLen = WE_RC2_BLOCK_SIZE;
        (WE_VOID)WE_MEMSET(aucData, (WE_UCHAR)(WE_RC2_BLOCK_SIZE-pstPara->iBufLen), (WE_UINT32)WE_RC2_BLOCK_SIZE);
        (WE_VOID)WE_MEMCPY(aucData, pstPara->aucBuf, (WE_UINT32)pstPara->iBufLen);
        (WE_VOID)We_LibRC2EncryptCBC( aucData, WE_RC2_BLOCK_SIZE, pucOut, handle);
    }
    else
    {
        *puiOutLen = 0;
    }
    
    WE_FREE(pstPara); 
    return M_WE_LIB_ERR_OK;
}

/*==================================================================================================
FUNCTION: 
    We_LibRC2EncryptCBC
CREATE DATE:
    2006-11-23
AUTHOR:
    stone An
DESCRIPTION:
    Encryt the input data with RC2 algorthm.
ARGUMENTS PASSED:
    const WE_UCHAR* pucIn[IN]: the data need to be encrypted.
    WE_UINT32       uiInLen[IN]: length of input data.
    WE_UCHAR*       pucOut[OUT]: the encrypted data.
    WE_HANDLE       Handle[IN]: the handle of RC2 algorithm.
RETURN VALUE:
    M_WE_LIB_ERR_OK: success.
    other : fail.
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    omit.
IMPORTANT NOTES:
    None
==================================================================================================*/
static WE_INT32 We_LibRC2EncryptCBC
(
    const WE_UCHAR* pucIn, 
    WE_UINT32 uiInLen, 
    WE_UCHAR* pucOut, 
    WE_HANDLE handle
)
{  
    
    WE_INT32  iLen = (WE_INT32)uiInLen;
    St_WeRC2Parameter*    pstPara = NULL;
    WE_INT32              iRes    = 0;
    WE_INT32              iIndex  = 0;
    WE_INT32              iIndexJ = 0; 
    WE_UCHAR              aucInTmp[WE_RC2_BLOCK_SIZE] = {0}; 
    
    if( (0 == uiInLen) || (!pucIn) || (!pucOut) || !handle)
    {
        return M_WE_LIB_ERR_INVALID_PARAMETER;
    }

    pstPara = (St_WeRC2Parameter*)handle;

    
    for (iIndexJ=0; iIndexJ<iLen; iIndexJ+=WE_RC2_BLOCK_SIZE)
    {
        for (iIndex=0; iIndex<WE_RC2_BLOCK_SIZE; iIndex++)
        {
            aucInTmp[iIndex] = pucIn[iIndexJ+iIndex];
            aucInTmp[iIndex] ^= ((pstPara->aucIv)[iIndex]);
        }

        iRes = We_LibRC2Encrypt( aucInTmp, WE_RC2_BLOCK_SIZE, &pucOut[iIndexJ], handle);
        if (iRes != M_WE_LIB_ERR_OK)
        {
            return iRes;
        }

        (WE_VOID)WE_MEMCPY(pstPara->aucIv, &pucOut[iIndexJ], WE_RC2_BLOCK_SIZE);
    }

    return M_WE_LIB_ERR_OK;        
}

/*==================================================================================================
FUNCTION: 
    We_LibRC2Encrypt
CREATE DATE:
    2006-11-23
AUTHOR:
    stone An
DESCRIPTION:
    Encryt the input data which is one WE_RC2_BLOCK_SIZE with RC2 algorthm.
ARGUMENTS PASSED:
    const WE_UCHAR* pucIn[IN]: the data need to be encrypted.
    WE_UINT32       uiInLen[IN]: length of input data, which is must be WE_RC2_BLOCK_SIZE.
    WE_UCHAR*       pucOut[OUT]: the encrypted data.
    WE_HANDLE       Handle[IN]: the handle of RC2 algorithm.
RETURN VALUE:
    M_WE_LIB_ERR_OK: success.
    other : fail.
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    omit.
IMPORTANT NOTES:
    None
==================================================================================================*/
static WE_INT32 We_LibRC2Encrypt
(
    const WE_UCHAR* pucIn, 
    WE_UINT32 uiInLen, 
    WE_UCHAR* pucOut, 
    WE_HANDLE handle
)
{
    St_WeRC2Parameter*   pstPara   = NULL;
    WE_INT32     iIndexI = 0;
    WE_UINT16    usIn76 = 0;
    WE_UINT16    usIn54 = 0;
    WE_UINT16    usIn32 = 0;
    WE_UINT16    usIn10 = 0;
    
    if ( (WE_RC2_BLOCK_SIZE != uiInLen) || (!pucIn) || (!pucOut) || !handle)
    {
        return M_WE_LIB_ERR_INVALID_PARAMETER;
    }  
   
    pstPara = (St_WeRC2Parameter *)handle;
    usIn76 = (pucIn[7] << 8) + pucIn[6];
    usIn54 = (pucIn[5] << 8) + pucIn[4];  
    usIn32 = (pucIn[3] << 8) + pucIn[2];
    usIn10 = (pucIn[1] << 8) + pucIn[0];
    
    for (iIndexI=0; iIndexI<16; iIndexI++)
    {/* turn the N bits of head to the end. */
        usIn10 += (usIn32 & ~usIn76) + (usIn54 & usIn76) + (pstPara->ausXKey)[4*iIndexI+0];  
        usIn10 = (WE_UINT16)((usIn10 << 1) + (usIn10 >> 15 & 0x01)); 

        usIn32 += (usIn54 & ~usIn10) + (usIn76 & usIn10) + (pstPara->ausXKey)[4*iIndexI+1];
        usIn32 = (WE_UINT16)((usIn32 << 2) + (usIn32 >> 14 & 0x03));

        usIn54 += (usIn76 & ~usIn32) + (usIn10 & usIn32) + (pstPara->ausXKey)[4*iIndexI+2];
        usIn54 = (WE_UINT16)((usIn54 << 3) + (usIn54 >> 13 & 0x07));

        usIn76 += (usIn10 & ~usIn54) + (usIn32 & usIn54) + (pstPara->ausXKey)[4*iIndexI+3];
        usIn76 = (WE_UINT16)((usIn76 << 5) + (usIn76 >> 11 & 0x1f));
        
        if (iIndexI == 4 || iIndexI == 10)
        {
            usIn10 += (pstPara->ausXKey)[usIn76 & 0x3f];
            usIn32 += (pstPara->ausXKey)[usIn10 & 0x3f];
            usIn54 += (pstPara->ausXKey)[usIn32 & 0x3f];
            usIn76 += (pstPara->ausXKey)[usIn54 & 0x3f];
        }
    }
    
    pucOut[0] = (WE_UCHAR)usIn10;
    pucOut[1] = (WE_UCHAR)(usIn10 >> 8);
    pucOut[2] = (WE_UCHAR)usIn32;
    pucOut[3] = (WE_UCHAR)(usIn32 >> 8);
    pucOut[4] = (WE_UCHAR)usIn54;
    pucOut[5] = (WE_UCHAR)(usIn54 >> 8);
    pucOut[6] = (WE_UCHAR)usIn76;
    pucOut[7] = (WE_UCHAR)(usIn76 >> 8);

    return M_WE_LIB_ERR_OK;
}

/*==================================================================================================
FUNCTION: 
    We_LibRC2DecryptNoPadding
CREATE DATE:
    2006-11-23
AUTHOR:
    stone An
DESCRIPTION:
    RC2 decrypt with no padding. The length of input is divided exactly by WE_RC2_BLOCK_SIZE.
ARGUMENTS PASSED:
    St_WeCipherKey  stKey[IN]: secret key.
    const WE_UCHAR* pucIn[IN]: the data need to be decrypted.
    WE_UINT32       uiInLen[IN]: length of input data.
    WE_UCHAR*       pucOut[OUT]: the decrypted data.
    WE_UINT32*      puiOutLen[IN/OUT]: the length of output buffer when IN, 
                                       the length of decrypted data when OUT.
RETURN VALUE:
    M_WE_LIB_ERR_OK: success.
    other : fail.
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    omit.
IMPORTANT NOTES:
    None
==================================================================================================*/
WE_INT32 We_LibRC2DecryptNoPadding
(
    St_WeCipherCrptKey  stKey, 
    const WE_UCHAR* pucIn, 
    WE_UINT32 uiInLen, 
    WE_UCHAR* pucOut, 
    WE_UINT32* puiOutLen
)
{
    WE_INT32     iRv      = M_WE_LIB_ERR_OK;
    WE_INT32     iRes     = M_WE_LIB_ERR_OK;
    WE_HANDLE    hHandle  = NULL;
    
    if ( !stKey.pucKey || (0 >= stKey.iKeyLen) || 
        (0 == uiInLen) || (!pucIn) || (!pucOut) || (!puiOutLen))
    {
        return M_WE_LIB_ERR_INVALID_PARAMETER;
    }

    if (0 != uiInLen % WE_RC2_BLOCK_SIZE)
    {
        return M_WE_LIB_ERR_DATA_LENGTH;
    }
    
    iRes = We_LibRC2DecryptInit( stKey, 128, &hHandle);
    if (iRes == M_WE_LIB_ERR_OK)
    {
        iRv = We_LibRC2DecryptUpdate( pucIn, uiInLen, pucOut, puiOutLen, hHandle);
        if (iRv == M_WE_LIB_ERR_OK)
        {
            iRes = We_LibRC2DecryptFinal( hHandle);
        }
        else
        {
            iRes = We_LibRC2DecryptFinal( hHandle);
            return iRv;
        }
    }
    
    return iRes;
}

/*==================================================================================================
FUNCTION: 
    We_LibRC2DecryptPadding
CREATE DATE:
    2006-11-23
AUTHOR:
    stone An
DESCRIPTION:
    RC2 decrypt with padding. The length of input is not divided exactly by WE_RC2_BLOCK_SIZE.
ARGUMENTS PASSED:
    St_WeCipherKey  stKey[IN]: secret key.
    const WE_UCHAR* pucIn[IN]: the data need to be decrypted.
    WE_UINT32       uiInLen[IN]: length of input data.
    WE_UCHAR*       pucOut[OUT]: the decrypted data.
    WE_UINT32*      puiOutLen[IN/OUT]: the length of output buffer when IN, 
                                       the length of decrypted data when OUT.
RETURN VALUE:
    M_WE_LIB_ERR_OK: success.
    other : fail.
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    omit.
IMPORTANT NOTES:
    None
==================================================================================================*/
WE_INT32 We_LibRC2DecryptPadding
(
    St_WeCipherCrptKey  stKey, 
    const WE_UCHAR* pucIn, 
    WE_UINT32 uiInLen, 
    WE_UCHAR* pucOut, 
    WE_UINT32* puiOutLen
)
{
    WE_INT32     iRv      = M_WE_LIB_ERR_OK;
    WE_INT32     iRes     = M_WE_LIB_ERR_OK;
    WE_HANDLE    hHandle  = NULL;
    WE_UINT32    uiTmpLen = 0;
    
    if( !stKey.pucKey || (0 >= stKey.iKeyLen) || 
        (0 == uiInLen) || (!pucIn) || (!pucOut) || (!puiOutLen))
    {
        return M_WE_LIB_ERR_INVALID_PARAMETER;
    }
    
    iRes = We_LibRC2DecryptInitPad( stKey, 128, &hHandle);    
    if (iRes == M_WE_LIB_ERR_OK)
    {
        iRv = We_LibRC2DecryptUpdatePad( pucIn, uiInLen, pucOut, puiOutLen, hHandle);
        if (iRv == M_WE_LIB_ERR_OK)
        {
            iRes = We_LibRC2DecryptFinalPad( pucOut+(*puiOutLen), &uiTmpLen, hHandle);
        }
        else
        {
            iRes = We_LibRC2DecryptFinalPad( pucOut+(*puiOutLen), &uiTmpLen, hHandle);
            return iRv;
        }
    }

    (*puiOutLen) -= uiTmpLen;
    
    return iRes;
}

/*==================================================================================================
FUNCTION: 
    We_LibRC2DecryptInit
CREATE DATE:
    2006-11-23
AUTHOR:
    stone An
DESCRIPTION:
    initialize RC2 algorithm without padding.
ARGUMENTS PASSED:
    St_WeCipherKey  stKey[IN]: secret key.
    WE_UINT32       uiBits[IN]: the available bits of secret key.
    WE_HANDLE*      pHandle[OUT]: the handle of RC2.
RETURN VALUE:
    M_WE_LIB_ERR_OK: success.
    other : fail.
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    omit.
IMPORTANT NOTES:
    None
==================================================================================================*/
WE_INT32 We_LibRC2DecryptInit
(
    St_WeCipherCrptKey stKey,
    WE_UINT32 uiBits,
    WE_HANDLE* pHandle
)
{
    return We_LibRC2EncryptInit( stKey, uiBits, pHandle);
}

/*==================================================================================================
FUNCTION: 
    We_LibRC2DecryptInitPad
CREATE DATE:
    2006-11-23
AUTHOR:
    stone An
DESCRIPTION:
    initialize RC2 algorithm using with padding.
ARGUMENTS PASSED:
    St_WeCipherKey  stKey[IN]: secret key.
    WE_UINT32       uiBits[IN]: the available bits of secret key.
    WE_HANDLE*      pHandle[OUT]: the handle of RC2.
RETURN VALUE:
    M_WE_LIB_ERR_OK: success.
    other : fail.
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    omit.
IMPORTANT NOTES:
    None
==================================================================================================*/
WE_INT32 We_LibRC2DecryptInitPad
(
    St_WeCipherCrptKey stKey,
    WE_UINT32 uiBits,
    WE_HANDLE* pHandle
)
{
    return We_LibRC2EncryptInit( stKey, uiBits, pHandle);
}

/*==================================================================================================
FUNCTION: 
    We_LibRC2DecryptUpdate
CREATE DATE:
    2006-11-23
AUTHOR:
    stone An
DESCRIPTION:
    decrypt data with RC2 algorithm.
ARGUMENTS PASSED:
    const WE_UCHAR* pucIn[IN]: the data need to be decrypted.
    WE_UINT32       uiInLen[IN]: length of input data.
    WE_UCHAR*       pucOut[OUT]: the decrypted data.
    WE_UINT32*      puiOutLen[IN/OUT]: the length of output buffer when IN, 
                                       the length of encrypted data when OUT.
    WE_HANDLE       Handle[IN]: the handle of RC2.
RETURN VALUE:
    M_WE_LIB_ERR_OK: success.
    other : fail.
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    omit.
IMPORTANT NOTES:
    None
==================================================================================================*/
WE_INT32 We_LibRC2DecryptUpdate
(
    const WE_UCHAR* pucIn, 
    WE_UINT32 uiInLen, 
    WE_UCHAR* pucOut, 
    WE_UINT32* puiOutLen,
    WE_HANDLE handle
)
{
    WE_UINT32             uiLen     = 0; 

    if( (0 == uiInLen) || (!pucIn) || (!pucOut) || (!puiOutLen) || !handle)
    {
        return M_WE_LIB_ERR_INVALID_PARAMETER;
    }
    
    if(*puiOutLen >= uiInLen )
    {
        *puiOutLen = uiInLen;
    }
    else
    {
        return M_WE_LIB_ERR_BUFFER_TOO_SMALL;
    }

    uiLen = uiInLen % WE_RC2_BLOCK_SIZE;
    if(0 == uiLen)
    {        
        (WE_VOID)We_LibRC2DecryptCBC( pucIn, uiInLen, pucOut, handle);
    }
    else
    {
        return M_WE_LIB_ERR_DATA_LENGTH;
    }

    return M_WE_LIB_ERR_OK;    
}

/*==================================================================================================
FUNCTION: 
    We_LibRC2DecryptUpdatePad
CREATE DATE:
    2006-11-23
AUTHOR:
    stone An
DESCRIPTION:
    decrypt data with RC2 algorithm using with padding.
ARGUMENTS PASSED:
    const WE_UCHAR* pucIn[IN]: the data need to be decrypted.
    WE_UINT32       uiInLen[IN]: length of input data.
    WE_UCHAR*       pucOut[OUT]: the decrypted data.
    WE_UINT32*      puiOutLen[IN/OUT]: the length of output buffer when IN, 
                                       the length of encrypted data when OUT.
    WE_HANDLE       Handle[IN]: the handle of RC2.
RETURN VALUE:
    M_WE_LIB_ERR_OK: success.
    other : fail.
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    omit.
IMPORTANT NOTES:
    None
==================================================================================================*/
WE_INT32 We_LibRC2DecryptUpdatePad
(
    const WE_UCHAR* pucIn, 
    WE_UINT32 uiInLen, 
    WE_UCHAR* pucOut, 
    WE_UINT32* puiOutLen,
    WE_HANDLE handle
)
{
    WE_UINT32             uiRealInLen = 0;
    WE_UINT32             uiIndex    = 0;
    St_WeRC2Parameter*    pstPara   = NULL;

    if( (0 == uiInLen) || (!pucIn) || (!pucOut) || (!puiOutLen) || !handle)
    {
        return M_WE_LIB_ERR_INVALID_PARAMETER;
    }
    
    pstPara = (St_WeRC2Parameter*)handle;
    uiIndex = (WE_UINT32)(pstPara->iBufLen);

    uiRealInLen = ((uiInLen + uiIndex) / WE_RC2_BLOCK_SIZE) * WE_RC2_BLOCK_SIZE;
    if (*puiOutLen >= uiRealInLen)
    {
        *puiOutLen = uiRealInLen;
    }
    else
    {
        return M_WE_LIB_ERR_BUFFER_TOO_SMALL;
    }
    
    pstPara->iBufLen = (WE_INT32)(uiInLen + uiIndex - uiRealInLen);
    (WE_VOID)WE_MEMCPY((pstPara->aucBuf + uiIndex), pucIn, (WE_UINT32)(WE_RC2_BLOCK_SIZE - uiIndex));
    (WE_VOID)We_LibRC2DecryptCBC( pstPara->aucBuf, WE_RC2_BLOCK_SIZE, pucOut, handle);
    if (WE_RC2_BLOCK_SIZE != *puiOutLen)
    {
        (WE_VOID)We_LibRC2DecryptCBC( (pucIn + (WE_RC2_BLOCK_SIZE - uiIndex)),            
                uiRealInLen-WE_RC2_BLOCK_SIZE, pucOut+WE_RC2_BLOCK_SIZE, handle);
    }
    uiIndex = uiInLen - (WE_UINT32)(pstPara->iBufLen);
    (WE_VOID)WE_MEMCPY(pstPara->aucBuf, pucIn + uiIndex, (WE_UINT32)(pstPara->iBufLen));
    
    return M_WE_LIB_ERR_OK;
}

/*==================================================================================================
FUNCTION: 
    We_LibRC2DecryptFinal
CREATE DATE:
    2006-11-23
AUTHOR:
    stone An
DESCRIPTION:
    the final handle of RC2 algorithm, release the handle of RC2 algorithm.
ARGUMENTS PASSED:
    WE_HANDLE       Handle[IN]: the handle of RC2 algorithm.
RETURN VALUE:
    M_WE_LIB_ERR_OK: success.
    other : fail.
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    omit.
IMPORTANT NOTES:
    None
==================================================================================================*/
WE_INT32 We_LibRC2DecryptFinal(WE_HANDLE handle)
{
    return We_LibRC2EncryptFinal( handle);
}

/*==================================================================================================
FUNCTION: 
    We_LibRC2DecryptFinalPad
CREATE DATE:
    2006-11-23
AUTHOR:
    stone An
DESCRIPTION:
    the final handle with pad. compute the length of padding data.
ARGUMENTS PASSED:
    WE_UCHAR*       pucOut[OUT]: the end of decrypted data.
    WE_UINT32*      puiOutLen[IN/OUT]: the length of padding data.
    WE_HANDLE       Handle[IN]: the handle of RC2 algorithm.
RETURN VALUE:
    M_WE_LIB_ERR_OK: success.
    other : fail.
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    omit.
IMPORTANT NOTES:
    None
==================================================================================================*/
WE_INT32 We_LibRC2DecryptFinalPad
(
    WE_UCHAR* pucOut, 
    WE_UINT32* puiOutLen, 
    WE_HANDLE handle
)
{
    St_WeRC2Parameter*   pstPara   = NULL;
    WE_UINT32            uiLen     = 0;
    
    if((NULL == handle) || (NULL == pucOut) || (NULL == puiOutLen))
    {
        return M_WE_LIB_ERR_INVALID_PARAMETER;
    }
    pstPara = (St_WeRC2Parameter*)handle;
    
    *puiOutLen = 0;
    uiLen = *(pucOut - 1);
    if ((uiLen > 0) && (uiLen < WE_RC2_BLOCK_SIZE))
    {
        WE_UINT32   uiLoop = 0;
        for (; uiLoop < uiLen; uiLoop++)
        {
            if(uiLen != *(pucOut - 1 - uiLoop))
            {
                break;
            }
        }
        if (uiLoop == uiLen)
        {
            *puiOutLen = uiLen;
        }
    }
    
    WE_FREE(pstPara);    
    return M_WE_LIB_ERR_OK;
}

/*==================================================================================================
FUNCTION: 
    We_LibRC2DecryptCBC
CREATE DATE:
    2006-11-23
AUTHOR:
    stone An
DESCRIPTION:
    Decryt the input data with RC2 algorthm.
ARGUMENTS PASSED:
    const WE_UCHAR* pucIn[IN]: the data need to be decrypted.
    WE_UINT32       uiInLen[IN]: length of input data.
    WE_UCHAR*       pucOut[OUT]: the decrypted data.
    WE_HANDLE       Handle[IN]: the handle of RC2 algorithm.
RETURN VALUE:
    M_WE_LIB_ERR_OK: success.
    other : fail.
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    omit.
IMPORTANT NOTES:
    None
==================================================================================================*/
static WE_INT32 We_LibRC2DecryptCBC
(
    const WE_UCHAR* pucIn, 
    WE_UINT32 uiInLen, 
    WE_UCHAR* pucOut, 
    WE_HANDLE handle
)
{  
    
    WE_INT32  iLen = (WE_INT32)uiInLen;
    St_WeRC2Parameter*    pstPara = NULL;
    WE_INT32              iRes    = 0;
    WE_INT32              iIndex  = 0;
    WE_INT32              iIndexJ = 0; 
    WE_UCHAR              aucInTmp[WE_RC2_BLOCK_SIZE] = {0};  
    
    if( (0 == uiInLen) || (!pucIn) || (!pucOut) || !handle)
    {
        return M_WE_LIB_ERR_INVALID_PARAMETER;
    }
    
    pstPara = (St_WeRC2Parameter*)handle;
    
    
    for (iIndexJ=0; iIndexJ<iLen; iIndexJ+=WE_RC2_BLOCK_SIZE)
    {
        for (iIndex=iIndexJ; iIndex<iIndexJ+WE_RC2_BLOCK_SIZE; iIndex++)
        {
            aucInTmp[iIndex-iIndexJ] = pucIn[iIndex];
        }
        iRes = We_LibRC2Decrypt( aucInTmp, WE_RC2_BLOCK_SIZE, &pucOut[iIndexJ], handle);
        if (iRes != M_WE_LIB_ERR_OK)
        {
            return iRes;
        }
        for (iIndex=0; iIndex<WE_RC2_BLOCK_SIZE; iIndex++)
        {
            pucOut[iIndexJ+iIndex] ^= ((pstPara->aucIv)[iIndex]);
        }
        
        (WE_VOID)WE_MEMCPY(pstPara->aucIv, aucInTmp, WE_RC2_BLOCK_SIZE);
    }
    
    return M_WE_LIB_ERR_OK;        
}

/*==================================================================================================
FUNCTION: 
    We_LibRC2Decrypt
CREATE DATE:
    2006-11-23
AUTHOR:
    stone An
DESCRIPTION:
    Decryt the input data which is one WE_RC2_BLOCK_SIZE with RC2 algorthm.
ARGUMENTS PASSED:
    const WE_UCHAR* pucIn[IN]: the data need to be decrypted.
    WE_UINT32       uiInLen[IN]: length of input data.
    WE_UCHAR*       pucOut[OUT]: the decrypted data.
    WE_HANDLE       Handle[IN]: the handle of RC2 algorithm.
RETURN VALUE:
    M_WE_LIB_ERR_OK: success.
    other : fail.
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    omit.
IMPORTANT NOTES:
    None
==================================================================================================*/
static WE_INT32 We_LibRC2Decrypt
(
    const WE_UCHAR* pucIn, 
    WE_UINT32 uiInLen, 
    WE_UCHAR* pucOut, 
    WE_HANDLE handle
)
{
    St_WeRC2Parameter*   pstPara   = NULL;
    WE_INT32     iIndexI = 0;
    WE_UINT16    usIn76 = 0;
    WE_UINT16    usIn54 = 0;
    WE_UINT16    usIn32 = 0;
    WE_UINT16    usIn10 = 0;
    
    if ( (WE_RC2_BLOCK_SIZE != uiInLen) || (!pucIn) || (!pucOut) || !handle)
    {
        return M_WE_LIB_ERR_INVALID_PARAMETER;
    }
    
    pstPara = (St_WeRC2Parameter *)handle;

    usIn76 = (pucIn[7] << 8) + pucIn[6];
    usIn54 = (pucIn[5] << 8) + pucIn[4];  
    usIn32 = (pucIn[3] << 8) + pucIn[2];
    usIn10 = (pucIn[1] << 8) + pucIn[0];
    
    for (iIndexI=15; iIndexI>=0; iIndexI--)
    {
        usIn76 = (WE_UINT16)((usIn76 << 11) + (usIn76 >> 5));
        usIn76 -= (usIn10 & ~usIn54) + (usIn32 & usIn54) + (pstPara->ausXKey)[4*iIndexI+3];

        usIn54 = (WE_UINT16)((usIn54 << 13) + (usIn54 >> 3));        
        usIn54 -= (usIn76 & ~usIn32) + (usIn10 & usIn32) + (pstPara->ausXKey)[4*iIndexI+2];

        usIn32 = (WE_UINT16)((usIn32 << 14) + (usIn32 >> 2));
        usIn32 -= (usIn54 & ~usIn10) + (usIn76 & usIn10) + (pstPara->ausXKey)[4*iIndexI+1];

        usIn10 = (WE_UINT16)((usIn10 << 15) + (usIn10 >> 1));         
        usIn10 -= (usIn32 & ~usIn76) + (usIn54 & usIn76) + (pstPara->ausXKey)[4*iIndexI+0];   
        
        if (iIndexI == 5 || iIndexI == 11)
        {
            usIn76 -= (pstPara->ausXKey)[usIn54 & 0x3f];
            usIn54 -= (pstPara->ausXKey)[usIn32 & 0x3f];
            usIn32 -= (pstPara->ausXKey)[usIn10 & 0x3f];
            usIn10 -= (pstPara->ausXKey)[usIn76 & 0x3f];
        }
    }
    
    pucOut[0] = (WE_UCHAR)usIn10;
    pucOut[1] = (WE_UCHAR)(usIn10 >> 8);
    pucOut[2] = (WE_UCHAR)usIn32;
    pucOut[3] = (WE_UCHAR)(usIn32 >> 8);
    pucOut[4] = (WE_UCHAR)usIn54;
    pucOut[5] = (WE_UCHAR)(usIn54 >> 8);
    pucOut[6] = (WE_UCHAR)usIn76;
    pucOut[7] = (WE_UCHAR)(usIn76 >> 8);
    
    return M_WE_LIB_ERR_OK;
}

/*rc5*/

/*************************************************************************************************
*   Macro define
*************************************************************************************************/
#define M_WE_RC5_ENC               1
#define M_WE_RC5_DEC               0
#define M_WE_RC5_TYPE              0xf123f456
#define M_WE_RC5_ROUNDS            16


/*************************************************************************************************
   RC5 crypt
*************************************************************************************************/

#define M_WE_RC5_C2L(c,l)    {\
    l =  ((WE_UINT32)(*((c)++)))    ; \
    l |= ((WE_UINT32)(*((c)++))) << 8L; \
    l |= ((WE_UINT32)(*((c)++))) << 16L; \
l |= ((WE_UINT32)(*((c)++))) << 24L;}

#define M_WE_RC5_L2C(l,c)    {\
    *((c)++) = (WE_UCHAR)(((l)) & 0xff); \
    *((c)++) = (WE_UCHAR)(((l) >> 8L) & 0xff); \
    *((c)++) = (WE_UCHAR)(((l) >> 16L) & 0xff); \
    *((c)++) = (WE_UCHAR)(((l) >> 24L) & 0xff);}
             
#define M_WE_RC5_C2LN(c,l1,l2,n)    { \
            c += n; \
            l1 = 0;\
            l2 = 0; \
            switch (n) { \
                        case 8: l2 =  ((WE_UINT32)(*(--(c)))) << 24L;/*fall through*/\
                        case 7: l2 |= ((WE_UINT32)(*(--(c)))) << 16L;/*fall through*/\
                        case 6: l2 |= ((WE_UINT32)(*(--(c)))) << 8L;/*fall through*/\
                        case 5: l2 |= ((WE_UINT32)(*(--(c))));/*fall through*/     \
                        case 4: l1 =  ((WE_UINT32)(*(--(c)))) << 24L;/*fall through*/\
                        case 3: l1 |= ((WE_UINT32)(*(--(c)))) << 16L;/*fall through*/ \
                        case 2: l1 |= ((WE_UINT32)(*(--(c)))) << 8L; /*fall through*/\
                        case 1: l1 |= ((WE_UINT32)(*(--(c)))); /*fall through*/    \
                        default :/*fall through*/\
                        break;\
                        } \
            }

#define M_WE_RC5_L2CN(l1,l2,c,n)    { \
            c += n; \
            switch (n) { \
                            case 8: *(--(c)) = (WE_UCHAR)(((l2) >> 24L) & 0xff);/*fall through*/ \
                            case 7: *(--(c)) = (WE_UCHAR)(((l2) >> 16L) & 0xff);/*fall through*/ \
                            case 6: *(--(c)) = (WE_UCHAR)(((l2) >> 8L) & 0xff); /*fall through*/\
                            case 5: *(--(c)) = (WE_UCHAR)(((l2)) & 0xff); /*fall through*/\
                            case 4: *(--(c)) = (WE_UCHAR)(((l1) >> 24L) & 0xff); /*fall through*/\
                            case 3: *(--(c)) = (WE_UCHAR)(((l1) >>16L) & 0xff); /*fall through*/\
                            case 2: *(--(c)) = (WE_UCHAR)(((l1) >> 8L) & 0xff); /*fall through*/\
                            case 1: *(--(c)) = (WE_UCHAR)(((l1)) & 0xff); /*fall through*/\
                            default :/*fall through*/\
                            break;\
                        } \
            }

#define M_WE_RC5_N2LN(c,l1,l2,n)    { \
            c += n; \
            l1 = l2 = 0; \
            switch (n) { \
                            case 8: l2 =  ((WE_UINT32)(*(--(c))));/*fall through*/ \
                            case 7: l2 |= ((WE_UINT32)(*(--(c)))) << 8;/*fall through*/ \
                            case 6: l2 |= ((WE_UINT32)(*(--(c)))) << 16; /*fall through*/\
                            case 5: l2 |= ((WE_UINT32)(*(--(c)))) <<24; /*fall through*/\
                            case 4: l1 =  ((WE_UINT32)(*(--(c)))) ; /*fall through*/\
                            case 3: l1 |= ((WE_UINT32)(*(--(c)))) << 8;/*fall through*/ \
                            case 2: l1 |= ((WE_UINT32)(*(--(c)))) << 16;/*fall through*/ \
                            case 1: l1 |= ((WE_UINT32)(*(--(c)))) << 24;/*fall through*/ \
                            default :/*fall through*/\
                            break;\
                } \
            }

#define M_WE_RC5_L2NN(l1,l2,c,n)    { \
            c += n; \
            switch (n) { \
                            case 8: *(--(c))=(WE_UCHAR)(((l2) ) &0xff); /*fall through*/\
                            case 7: *(--(c))=(WE_UCHAR)(((l2) >> 8) & 0xff); /*fall through*/\
                            case 6: *(--(c))=(WE_UCHAR)(((l2) >> 16) & 0xff); /*fall through*/\
                            case 5: *(--(c))=(WE_UCHAR)(((l2) >> 24) & 0xff);/*fall through*/\
                            case 4: *(--(c))=(WE_UCHAR)(((l1)) & 0xff); /*fall through*/\
                            case 3: *(--(c))=(WE_UCHAR)(((l1) >> 8) & 0xff);/*fall through*/ \
                            case 2: *(--(c))=(WE_UCHAR)(((l1) >> 16) & 0xff);/*fall through*/ \
                            case 1: *(--(c))=(WE_UCHAR)(((l1) >> 24) & 0xff); /*fall through*/\
                            default :/*fall through*/\
                            break;\
                } \
            }

#define M_WE_ROTATE_L32(a,n)     \
(((a) << (n & 0x1f))|(((a) & 0xffffffff) >> (32 - (n & 0x1f))))

#define M_WE_ROTATE_R32(a,n)     \
(((a) << (32 - (n & 0x1f)))|(((a) & 0xffffffff) >> (n & 0x1f)))

#define E_RC5_32(a,b,s,n) {\
    a ^= b; \
    a = M_WE_ROTATE_L32(a,b); \
    a += s[n]; \
    a &= 0xffffffffL; \
    b ^= a; \
    b = M_WE_ROTATE_L32(b,a); \
    b += s[n+1]; \
b &= 0xffffffffL;}

#define D_RC5_32(a,b,s,n) {\
    b -= s[n+1]; \
    b &= 0xffffffffL; \
    b = M_WE_ROTATE_R32(b,a); \
    b ^= a; \
    a -= s[n]; \
    a &= 0xffffffffL; \
    a = M_WE_ROTATE_R32(a,b); \
a ^= b;}     


/*******************************************************************************
*   Type Define Section
*******************************************************************************/
typedef struct tagSt_WeRC5Key
{
    WE_INT32    iRounds;
    WE_UINT32   auiData[3*(M_WE_RC5_ROUNDS + 1)];
}St_WeRC5Key;

typedef struct tagSt_WeRC5Parameter
{
    WE_UINT32       uiType;
    St_WeRC5Key     stKey;
    WE_UCHAR        aucIv[8];
    WE_UCHAR        aucBuf[8];
    WE_INT32        iBufLen;
}St_WeRC5Parameter;


/*******************************************************************************
*   Prototype Declare Section
*******************************************************************************/
WE_VOID We_LibSetRC5Key(St_WeRC5Key* pstKey, St_WeCipherCrptKey stKey);

WE_VOID We_RC5Encrypt(WE_UINT32* puiData, St_WeRC5Key* pstKey);

WE_VOID We_RC5Decrypt(WE_UINT32* puiData, St_WeRC5Key* pstKey);

WE_VOID We_RC5CryptCBC(const WE_UCHAR* pucIn, WE_UCHAR* pucOut,
                       WE_UINT32 uiInLen, St_WeRC5Key* pstKey, WE_UCHAR* pucIv, WE_INT32 iEnc);



/*******************************************************************************
*   Function Define Section
*******************************************************************************/
/*==================================================================================================
FUNCTION: 
    We_LibSetRC5Key
CREATE DATE:
    2006-07-07
AUTHOR:
    steven ding
DESCRIPTION:
    convert cipher key to RC5 key
ARGUMENTS PASSED:
    St_WeRC5Key*               pstKey[OUT]:
    St_WeCipherCrptKey         stKey[IN]:
RETURN VALUE:
    ERROR CODE
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    self
IMPORTANT NOTES:
    None
==================================================================================================*/

WE_VOID We_LibSetRC5Key(St_WeRC5Key* pstKey, St_WeCipherCrptKey stKey)
{
    WE_UINT32   auiL[64] = {0}, uiL = 0, uiLl = 0, uiA = 0, uiB = 0, 
        *puiS = NULL, uiK = 0;
    WE_INT32    iIndexI = 0, iIndexJ = 0, iIndexM= 0, iIndexC = 0, iIndexT = 0,
        iIndexII = 0, iIndexJJ = 0;
    WE_INT32    iLen = stKey.iKeyLen;
    WE_UCHAR*   pucData = stKey.pucKey;
    
    if (!pstKey)
    {
        return;
    }  
    puiS = &(pstKey->auiData[0]);
    iIndexJ = 0;
    for (iIndexI = 0; iIndexI <= (iLen-8); iIndexI += 8)
    {
        M_WE_RC5_C2L(pucData,uiL);
        auiL[iIndexJ++] = uiL;
        M_WE_RC5_C2L(pucData,uiL);
        auiL[iIndexJ++] = uiL;
    }
    iIndexII = iLen-iIndexI;
    if (iIndexII)
    {
        uiK = iLen & 0x07;
        M_WE_RC5_C2LN(pucData,uiL,uiLl,uiK);
        auiL[iIndexJ+0] = uiL;
        auiL[iIndexJ+1] = uiLl;
    }
    
    iIndexC = (iLen + 3) / 4;
    iIndexT = (pstKey->iRounds + 1) * 2;
    puiS[0] = 0xB7E15163L;
    for (iIndexI = 1; iIndexI < iIndexT; iIndexI ++)
    {
        puiS[iIndexI] = (puiS[iIndexI-1] + 0x9E3779B9L) & 0xffffffffL;
    }    
    
    iIndexJ = (iIndexT > iIndexC) ? iIndexT : iIndexC;
    iIndexJ *= 3;
    iIndexII = iIndexJJ = 0;
    uiA = uiB = 0;
    for (iIndexI = 0; iIndexI < iIndexJ; iIndexI ++)
    {
        uiK = (puiS[iIndexII] + uiA + uiB)&0xffffffffL;
        uiA = puiS[iIndexII] = M_WE_ROTATE_L32(uiK, 3);
        iIndexM = (WE_INT32)(uiA + uiB);
        uiK = (auiL[iIndexJJ] + uiA + uiB)&0xffffffffL;
        uiB = auiL[iIndexJJ] = M_WE_ROTATE_L32(uiK, iIndexM);
        if (++iIndexII >= iIndexT) 
        {
            iIndexII = 0;
        }    
        if (++iIndexJJ >= iIndexC) 
        {
            iIndexJJ = 0;
        }    
    }
    
    return ;
}

/*==================================================================================================
FUNCTION: 
    We_RC5Encrypt
CREATE DATE:
    2006-07-07
AUTHOR:
    steven ding
DESCRIPTION:
    RC5 encrypt function
ARGUMENTS PASSED:
    WE_UINT32*          puiData[IN/OUT]
    St_WeRC5Key*       pstKey[IN]
RETURN VALUE:
    Counter number
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    self
IMPORTANT NOTES:
    None
==================================================================================================*/

WE_VOID We_RC5Encrypt(WE_UINT32* puiData, St_WeRC5Key* pstKey)
{
    WE_UINT32 uiA = 0, uiB = 0,  *puiS = NULL;
    WE_INT32  iIndex = 0,iRounds = 0;
    
    if (!puiData || !pstKey)
    {
        return;
    }
    
    puiS = pstKey->auiData;
    iRounds = pstKey->iRounds;
    uiA = puiData[0] + puiS[0];
    uiB = puiData[1] + puiS[1];
    for(iIndex = 2; iIndex < (2 * (iRounds + 1)); iIndex += 2)
    {
        E_RC5_32(uiA, uiB, puiS, iIndex);
    } 
    
    puiData[0] = uiA;
    puiData[1] = uiB;
    
    return ;
}

/*==================================================================================================
FUNCTION: 
    We_RC5Decrypt
CREATE DATE:
    2006-07-07
AUTHOR:
    steven ding
DESCRIPTION:
    RC5 decrypt function
ARGUMENTS PASSED:
    WE_UINT32*          puiData[IN/OUT]:
    St_WeRC5Key*       pstKey[IN]:
RETURN VALUE:
    ERROR CODE
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    self
IMPORTANT NOTES:
    None
==================================================================================================*/

WE_VOID We_RC5Decrypt(WE_UINT32* puiData, St_WeRC5Key* pstKey)
{
    WE_UINT32 uiA = 0, uiB = 0,  *puiS = NULL;
    WE_INT32  iIndex = 0,iRounds = 0;
    
    if (!puiData || !pstKey)
    {
        return;
    }
    
    puiS = pstKey->auiData;
    iRounds = pstKey->iRounds;
    uiA = puiData[0];
    uiB = puiData[1];
    for(iIndex = (2 * (iRounds )); iIndex > 0; iIndex -= 2)
    {
        D_RC5_32(uiA, uiB, puiS, iIndex);
    }
    puiData[0] = uiA - puiS[0];
    puiData[1] = uiB - puiS[1];
    
    return;
}

/*==================================================================================================
FUNCTION: 
    We_RC5CryptCBC
CREATE DATE:
    2006-07-07
AUTHOR:
    steven ding
DESCRIPTION:
    RC5 crypt function
ARGUMENTS PASSED:
    const WE_UCHAR*     pucIn[IN]:
    WE_UCHAR*           pucOut[OUT]:
    WE_INT32            iInLen[IN]:
    St_WeRC5Key*       pstKey[IN]:
    WE_UCHAR*           pucIv[IN]:
    WE_INT32            iEnc[IN]:
RETURN VALUE:
    NONE
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    self
IMPORTANT NOTES:
    None
==================================================================================================*/

WE_VOID We_RC5CryptCBC (const WE_UCHAR* pucIn, 
                        WE_UCHAR* pucOut, WE_UINT32 uiInLen, 
                        St_WeRC5Key* pstKey, WE_UCHAR* pucIv, WE_INT32 iEnc)
{
    WE_UINT32 uiTin0 = 0, uiTin1 = 0;
    WE_UINT32 uiTout0 = 0, uiTout1 = 0, uiXor0 = 0, uiXor1 = 0;
    WE_INT32  iLen = (WE_INT32)uiInLen;
    WE_UINT32 auiTin[2] = {0};
    
    if ((!pstKey) || (!pucIv) || (!pucIn) || (!pucOut))
    {
        return;
    }
    
    if (iEnc)
    {
        M_WE_RC5_C2L(pucIv,uiTout0);
        M_WE_RC5_C2L(pucIv,uiTout1);
        pucIv -= 8;
        for (iLen -= 8; iLen >= 0; iLen -= 8)
        {
            M_WE_RC5_C2L(pucIn,uiTin0);
            M_WE_RC5_C2L(pucIn,uiTin1);
            uiTin0^=uiTout0;
            uiTin1^=uiTout1;
            auiTin[0]=uiTin0;
            auiTin[1]=uiTin1;
            We_RC5Encrypt(auiTin,pstKey);
            uiTout0=auiTin[0]; M_WE_RC5_L2C(uiTout0,pucOut);
            uiTout1=auiTin[1]; M_WE_RC5_L2C(uiTout1,pucOut);
        }
        if (iLen != -8)
        {
            M_WE_RC5_C2LN(pucIn,uiTin0,uiTin1,(iLen+8));
            uiTin0^=uiTout0;
            uiTin1^=uiTout1;
            auiTin[0]=uiTin0;
            auiTin[1]=uiTin1;
            We_RC5Encrypt(auiTin,pstKey);
            uiTout0=auiTin[0]; M_WE_RC5_L2C(uiTout0,pucOut);
            uiTout1=auiTin[1]; M_WE_RC5_L2C(uiTout1,pucOut);
        }
        M_WE_RC5_L2C(uiTout0,pucIv);
        M_WE_RC5_L2C(uiTout1,pucIv);
    }
    else
    {
        M_WE_RC5_C2L(pucIv,uiXor0);
        M_WE_RC5_C2L(pucIv,uiXor1);
        pucIv-=8;
        for (iLen -= 8; iLen >= 0; iLen -= 8)
        {
            M_WE_RC5_C2L(pucIn,uiTin0); auiTin[0]=uiTin0;
            M_WE_RC5_C2L(pucIn,uiTin1); auiTin[1]=uiTin1;
            We_RC5Decrypt(auiTin,pstKey);
            uiTout0=auiTin[0]^uiXor0;
            uiTout1=auiTin[1]^uiXor1;
            M_WE_RC5_L2C(uiTout0,pucOut);
            M_WE_RC5_L2C(uiTout1,pucOut);
            uiXor0=uiTin0;
            uiXor1=uiTin1;
        }
        if (iLen != -8)
        {
            M_WE_RC5_C2L(pucIn,uiTin0); auiTin[0]=uiTin0;
            M_WE_RC5_C2L(pucIn,uiTin1); auiTin[1]=uiTin1;
            We_RC5Decrypt(auiTin,pstKey);
            uiTout0=auiTin[0]^uiXor0;
            uiTout1=auiTin[1]^uiXor1;
            M_WE_RC5_L2CN(uiTout0,uiTout1,pucOut,(iLen+8));
            uiXor0=uiTin0;
            uiXor1=uiTin1;
        }
        M_WE_RC5_L2C(uiXor0,pucIv);
        M_WE_RC5_L2C(uiXor1,pucIv);
    }
    
    return ;
}

/*==================================================================================================
FUNCTION: 
    We_LibRC5CryptInit
CREATE DATE:
    2006-07-07
AUTHOR:
    steven ding
DESCRIPTION:

ARGUMENTS PASSED:
    St_WeCipherCrptKey     stKey[IN]:
    WE_HANDLE*              pHandle[OUT]:
    WE_INT32                iPadding[IN]:
RETURN VALUE:
    ERROR CODE
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    self
IMPORTANT NOTES:
    None
==================================================================================================*/
  
WE_INT32 We_LibRC5CryptInit(WE_INT32 iRounds,
                              St_WeCipherCrptKey stKey,
                              WE_HANDLE* pHandle)
{
    St_WeRC5Parameter*   pstPara   = NULL;
    WE_INT32              iRv       = M_WE_LIB_ERR_OK;
    if(!stKey.pucKey || !stKey.pucIv || (0 >= stKey.iKeyLen) || \
      (stKey.iIvLen < 8) || (!pHandle))
    {
      return M_WE_LIB_ERR_INVALID_PARAMETER;
    }

    if((16 != stKey.iKeyLen)||(8 != stKey.iIvLen))
    {
      return M_WE_LIB_ERR_KEY_LENGTH;
    }

    pstPara = (St_WeRC5Parameter*)WE_MALLOC(sizeof(St_WeRC5Parameter));
    if (!pstPara)
    {
      return M_WE_LIB_ERR_INSUFFICIENT_MEMORY;
    }
    (void)WE_MEMSET( pstPara,0,sizeof(St_WeRC5Parameter) );
    pstPara->uiType = M_WE_RC5_TYPE;
    //pstPara->iPadding = iPadding;
    pstPara->stKey.iRounds = iRounds;
    We_LibSetRC5Key(&(pstPara->stKey), stKey);
    (void)WE_MEMCPY(pstPara->aucIv, stKey.pucIv, 8);
    pstPara->iBufLen = 0;

    *pHandle = (WE_HANDLE)pstPara;

    return iRv;
}
/*==================================================================================================
FUNCTION: 
  We_libRC5EncryptInit
CREATE DATE:
  2006-07-07
AUTHOR:
    steven ding
DESCRIPTION:
    
ARGUMENTS PASSED:
    St_WeCipherCrptKey     stKey[IN]:
    WE_HANDLE*              pHandle[OUT]:
RETURN VALUE:
    ERROR CODE
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    self
IMPORTANT NOTES:
    None
==================================================================================================*/

 WE_INT32 We_LibRC5EncryptInit (WE_INT32 iRounds,
                                       St_WeCipherCrptKey stKey,
                                       WE_HANDLE* pHandle)
{
    if(!stKey.pucKey || !stKey.pucIv || (0 >= stKey.iKeyLen) || \
        (stKey.iIvLen < 8) || (!pHandle))
    {
        return M_WE_LIB_ERR_INVALID_PARAMETER;
    }

    if((16 != stKey.iKeyLen)||(8 != stKey.iIvLen))
    {
        return M_WE_LIB_ERR_KEY_LENGTH;
    }

    return We_LibRC5CryptInit(iRounds, stKey, pHandle);
}

/*==================================================================================================
FUNCTION: 
    We_libRC5EncryptUpdate
CREATE DATE:
    2006-07-07
AUTHOR:
    steven ding
DESCRIPTION:
    
ARGUMENTS PASSED:
    const WE_UCHAR*     pucIn[IN]:
    WE_INT32            iInLen[IN]:
    WE_UCHAR*           pucOut[OUT]:
    WE_INT32*           piOutLen[IN/OUT]:
    WE_HANDLE           handle[IN]:
RETURN VALUE:
    Counter number
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    self
IMPORTANT NOTES:
    None
==================================================================================================*/

 WE_INT32 We_LibRC5EncryptUpdate (const WE_UCHAR* pucIn, 
                                         WE_UINT32 uiInLen, 
                                         WE_UCHAR* pucOut, 
                                         WE_UINT32* puiOutLen,
                                         WE_HANDLE handle)
{
    St_WeRC5Parameter*   pstPara   = NULL;
    WE_INT32              iRv       = M_WE_LIB_ERR_OK;
    WE_UINT32             uiLen     = 0; 

    if((0 == uiInLen) || (!pucIn) || (!pucOut) || (!puiOutLen) || !handle)
    {
        return M_WE_LIB_ERR_INVALID_PARAMETER;
    }
    uiLen = uiInLen % 8;
    if(*puiOutLen >= ((1 + uiInLen / 8) * 8))
    {
        *puiOutLen = ((1 + uiInLen / 8) * 8);
    }
    else
    {
        return M_WE_LIB_ERR_BUFFER_TOO_SMALL;
    }

    pstPara = (St_WeRC5Parameter*)handle;

    /*if(CIPHER_PADDING_RFC2630 == pstPara->iPadding)
    {
        WE_UCHAR aucData[8] = {0};
        We_RC5CryptCBC(pucIn, pucOut, uiInLen - uiLen, &(pstPara->stKey),\
            pstPara->aucIv, M_WE_RC5_ENC);
        (WE_VOID)WE_MEMSET(aucData,(WE_UCHAR)(8-uiLen),8);
        (WE_VOID)WE_MEMCPY(aucData,(pucIn + uiInLen - uiLen),uiLen);
        We_RC5CryptCBC(aucData, (pucOut + uiInLen - uiLen), 8, \
            &(pstPara->stKey), pstPara->aucIv, M_WE_RC5_ENC);
    }
    else */if(0 == uiLen)
    {        
        We_RC5CryptCBC(pucIn, pucOut, uiInLen, &(pstPara->stKey),\
            pstPara->aucIv, M_WE_RC5_ENC);
    }
    else
    {
        iRv = M_WE_LIB_ERR_DATA_LENGTH;
    }

    return iRv;
}

/*==================================================================================================
FUNCTION: 
    We_libRC5EncryptFinal
CREATE DATE:
    2006-07-07
AUTHOR:
    steven ding
DESCRIPTION:
    
ARGUMENTS PASSED:
    WE_HANDLE       handle[IN]:
RETURN VALUE:
    ERROR CODE
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    self
IMPORTANT NOTES:
    None
==================================================================================================*/

 WE_INT32 We_LibRC5EncryptFinal(WE_HANDLE handle)
{
    St_WeRC5Parameter*   pstPara   = NULL;
    WE_INT32              iRv       = M_WE_LIB_ERR_OK;

    if((NULL == handle))
    {
        return M_WE_LIB_ERR_INVALID_PARAMETER;
    }

    pstPara = (St_WeRC5Parameter*)handle;
    WE_LIB_FREE(pstPara);
    
    return iRv;
}

/*==================================================================================================
FUNCTION: 
    We_libRC5DecryptInit
CREATE DATE:
    2006-07-07
AUTHOR:
    steven ding
DESCRIPTION:
    
ARGUMENTS PASSED:
    St_WeCipherCrptKey     stKey[IN]:
    WE_HANDLE*              pHandle[OUT]:
RETURN VALUE:
    ERROR CODE
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    self
IMPORTANT NOTES:
    None
==================================================================================================*/
 WE_INT32 We_LibRC5DecryptInit (WE_INT32 iRounds, St_WeCipherCrptKey stKey, WE_HANDLE* pHandle)
 {
     return We_LibRC5CryptInit(iRounds, stKey, pHandle);
 }

/*==================================================================================================
FUNCTION: 
    We_libRC5DecryptUpdate
CREATE DATE:
    2006-07-07
AUTHOR:
    steven ding
DESCRIPTION:
    
ARGUMENTS PASSED:
    const WE_UCHAR*     pucIn[IN]:
    WE_INT32            iInLen[IN]:
    WE_UCHAR*           pucOut[OUT]:
    WE_INT32*           piOutLen[IN/OUT]:
    WE_HANDLE           handle[IN]:
RETURN VALUE:
    ERROR CODE
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    self
IMPORTANT NOTES:
    None
==================================================================================================*/
WE_INT32 We_LibRC5DecryptUpdate (const WE_UCHAR* pucIn, WE_UINT32 uiInLen, WE_UCHAR* pucOut, 
                                 WE_UINT32* puiOutLen, WE_HANDLE handle)
{
    St_WeRC5Parameter*   pstPara   = NULL;
    WE_INT32              iRv       = M_WE_LIB_ERR_OK;
    WE_UINT32             uiLen     = 0; 

    if((0 == uiInLen) || !pucIn || \
        !pucOut || !puiOutLen || !handle)
    {
        return M_WE_LIB_ERR_INVALID_PARAMETER;
    }
    
    uiLen = uiInLen % 8;
    if(!uiLen && (*puiOutLen >= uiInLen))
    {
        *puiOutLen = uiLen;        
    }
    else
    {
        return M_WE_LIB_ERR_BUFFER_TOO_SMALL;
    }

    pstPara = (St_WeRC5Parameter*)handle;

    We_RC5CryptCBC(pucIn, pucOut, uiInLen, &(pstPara->stKey),\
                          pstPara->aucIv, M_WE_RC5_DEC);    
    return iRv;
}

/*==================================================================================================
FUNCTION: 
    We_libRC5DecryptFinal
CREATE DATE:
    2006-07-07
AUTHOR:
    steven ding
DESCRIPTION:
    
ARGUMENTS PASSED:
    WE_HANDLE       handle[IN]:
RETURN VALUE:
    ERROR CODE
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    self
IMPORTANT NOTES:
    None
==================================================================================================*/
WE_INT32 We_LibRC5DecryptFinal(WE_HANDLE handle)
{
    return We_LibRC5EncryptFinal(handle);
}

/*==================================================================================================
FUNCTION: 
    We_libRC5EncryptInit1
CREATE DATE:
    2006-07-07
AUTHOR:
    steven ding
DESCRIPTION:
    
ARGUMENTS PASSED:
    St_WeCipherCrptKey     stKey[IN]:
    WE_HANDLE*              pHandle[OUT]:
RETURN VALUE:
    ERROR CODE
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    self
IMPORTANT NOTES:
    None
==================================================================================================*/
WE_INT32 We_LibRC5EncryptInit1 (WE_INT32 iRounds, St_WeCipherCrptKey stKey, WE_HANDLE* pHandle)
{
    return We_LibRC5CryptInit(iRounds, stKey, pHandle);
}

/*==================================================================================================
FUNCTION: 
    We_libRC5EncryptUpdate1
CREATE DATE:
    2006-07-07
AUTHOR:
    steven ding
DESCRIPTION:
    
ARGUMENTS PASSED:
    const WE_UCHAR*         pucIn[IN]:
    WE_INT32                iInLen[IN]:
    WE_UCHAR*               pucOut[OUT]:
    WE_INT32*               piOutLen[IN/OUT]:
    WE_HANDLE               handle[IN]:
RETURN VALUE:
    ERROR CODE
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    self
IMPORTANT NOTES:
    None
==================================================================================================*/
WE_INT32 We_LibRC5EncryptUpdate1 (const WE_UCHAR* pucIn, WE_UINT32 uiInLen, WE_UCHAR* pucOut, 
                                  WE_UINT32* puiOutLen, WE_HANDLE handle)
{
    St_WeRC5Parameter*   pstPara   = NULL;
    WE_INT32              iRv       = M_WE_LIB_ERR_OK;
    WE_INT32              iBufLen   = 0;
    WE_INT32              iIndex    = 0;

    if((0 == uiInLen) || (!pucIn) || (!pucOut) || (!puiOutLen) || !handle)
    {
        return M_WE_LIB_ERR_INVALID_PARAMETER;
    }

    iBufLen = (uiInLen % 8);
    if(iBufLen && (*puiOutLen >= ((1 + uiInLen / 8) * 8)))
    {
        *puiOutLen = ((1 + uiInLen / 8) * 8);
    }
    else if(!iBufLen && (*puiOutLen >= uiInLen))
    {
        *puiOutLen = uiInLen;
    }
    else
    {
        return M_WE_LIB_ERR_BUFFER_TOO_SMALL;
    }

    pstPara = (St_WeRC5Parameter*)handle;
    iIndex = pstPara->iBufLen;
    
    if((uiInLen + (WE_UINT32)iIndex) == *puiOutLen)
    {
        pstPara->iBufLen = 0;
        (void)WE_MEMCPY((pstPara->aucBuf + iIndex), pucIn, (WE_UINT32)(8 - iIndex));
        We_RC5CryptCBC(pstPara->aucBuf , pucOut, 8, &(pstPara->stKey),\
                          pstPara->aucIv, M_WE_RC5_ENC);
        if(8 != *puiOutLen)
        {
            We_RC5CryptCBC((pucIn + (8 - iIndex)), pucOut + 8, \
                (uiInLen - (WE_UINT32)(8 - iIndex) - (WE_UINT32)(pstPara->iBufLen)), &(pstPara->stKey), \
                pstPara->aucIv, M_WE_RC5_ENC);
        }            
    }
    else if((uiInLen + (WE_UINT32)iIndex) < *puiOutLen)
    {
        pstPara->iBufLen = (iIndex + (WE_INT32)uiInLen) % 8;
        *puiOutLen -= 8 ; 
        if((uiInLen + (WE_UINT32)iIndex) > 8)
        {    
            (void)WE_MEMCPY((pstPara->aucBuf + iIndex), pucIn, (WE_UINT32)(8 - iIndex));
            We_RC5CryptCBC(pstPara->aucBuf , pucOut, 8, \
                &(pstPara->stKey), pstPara->aucIv, M_WE_RC5_ENC);
            We_RC5CryptCBC((pucIn + (8 - iIndex)), pucOut + 8, \
                (uiInLen - (WE_UINT32)(8 - iIndex) - (WE_UINT32)(pstPara->iBufLen)), &(pstPara->stKey), \
                pstPara->aucIv, M_WE_RC5_ENC);
            iIndex = (WE_INT32)uiInLen - pstPara->iBufLen;
            (void)WE_MEMCPY(pstPara->aucBuf, pucIn + iIndex, (WE_UINT32)(pstPara->iBufLen));
        }    
        else
        {
            (void)WE_MEMCPY(pstPara->aucBuf + iIndex, pucIn, uiInLen);
        }
    }
    else
    {
        pstPara->iBufLen = (iIndex + (WE_INT32)uiInLen) - (WE_INT32)*puiOutLen;
        (void)WE_MEMCPY((pstPara->aucBuf + iIndex), pucIn, (WE_UINT32)(8 - iIndex));
        We_RC5CryptCBC(pstPara->aucBuf , pucOut, 8, \
                &(pstPara->stKey), pstPara->aucIv, M_WE_RC5_ENC);
        We_RC5CryptCBC((pucIn + (8 - iIndex)), pucOut + 8, \
            (uiInLen - (8 - (WE_UINT32)iIndex) - (WE_UINT32)(pstPara->iBufLen)), &(pstPara->stKey), \
            pstPara->aucIv, M_WE_RC5_ENC);
        iIndex = (WE_INT32)uiInLen - pstPara->iBufLen;
        (void)WE_MEMCPY(pstPara->aucBuf, pucIn + iIndex, (WE_UINT32)(pstPara->iBufLen));
    }

    return iRv;
}

/*==================================================================================================
FUNCTION: 
    We_libRC5EncryptFinal1
CREATE DATE:
    2006-07-07
AUTHOR:
    steven ding
DESCRIPTION:
    
ARGUMENTS PASSED:
    WE_UCHAR*       pucOut[OUT]:
    WE_INT32*       piOutLen[IN/OUT]:
    WE_HANDLE       handle[IN]:
RETURN VALUE:
    ERROR CODE
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    self
IMPORTANT NOTES:
    None
==================================================================================================*/

 WE_INT32 We_LibRC5EncryptFinal1(WE_UCHAR* pucOut, 
                                 WE_UINT32* puiOutLen, WE_HANDLE handle)
{
    St_WeRC5Parameter*   pstPara       = NULL;
    WE_UCHAR              aucData[8]    = {0};
    //WE_INT32             iRv = M_WE_LIB_ERR_OK;

    if(!pucOut || !puiOutLen || !handle)
    {
        return M_WE_LIB_ERR_INVALID_PARAMETER;
    }
    if(*puiOutLen < 8)
    {
        return M_WE_LIB_ERR_BUFFER_TOO_SMALL;
    }
    *puiOutLen = 8;
    pstPara = (St_WeRC5Parameter*)handle;
    {
        (WE_VOID)WE_MEMSET(aucData, (WE_UCHAR)(8-pstPara->iBufLen), (WE_UINT32)8);
        (WE_VOID)WE_MEMCPY(aucData, pstPara->aucBuf, (WE_UINT32)pstPara->iBufLen);
    }
    We_RC5CryptCBC(aucData , pucOut, (WE_UINT32)(8), \
            &(pstPara->stKey), pstPara->aucIv, M_WE_RC5_ENC);

    return We_LibRC5EncryptFinal(handle);
}

/*==================================================================================================
FUNCTION: 
    We_libRC5DecryptInit1
CREATE DATE:
    2006-07-07
AUTHOR:
    steven ding
DESCRIPTION:
    
ARGUMENTS PASSED:
    St_WeCipherCrptKey     stKey[IN]:
    WE_HANDLE*              pHandle[OUT]:
RETURN VALUE:
    ERROR CODE
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    self
IMPORTANT NOTES:
    None
==================================================================================================*/
WE_INT32 We_LibRC5DecryptInit1 (WE_INT32 iRounds,
                                        St_WeCipherCrptKey stKey,
                                        WE_HANDLE* pHandle)
{
    return We_LibRC5CryptInit(iRounds, stKey, pHandle);
}
                                  
/*==================================================================================================
FUNCTION: 
    We_libRC5DecryptUpdate1
CREATE DATE:
    2006-07-07
AUTHOR:
    steven ding
DESCRIPTION:
    
ARGUMENTS PASSED:
    const WE_UCHAR*     pucIn[IN]:
    WE_INT32            iInLen[IN]:
    WE_UCHAR*           pucOut[OUT]:
    WE_INT32*           piOutLen[IN/OUT]:
    WE_HANDLE           handle[IN]:
RETURN VALUE:
    ERROR CODE
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    self
IMPORTANT NOTES:
    None
==================================================================================================*/

 WE_INT32 We_LibRC5DecryptUpdate1 (const WE_UCHAR* pucIn, 
                                          WE_UINT32 uiInLen, 
                                          WE_UCHAR* pucOut, 
                                          WE_UINT32* puiOutLen,
                                          WE_HANDLE handle)
{
    St_WeRC5Parameter*   pstPara   = NULL;
    WE_INT32              iRv       = M_WE_LIB_ERR_OK;
    WE_INT32              iBufLen   = 0;
    WE_INT32              iIndex    = 0;

    if((0 == uiInLen) || (!pucIn) || (!pucOut) || (!puiOutLen) || !handle)
    {
        return M_WE_LIB_ERR_INVALID_PARAMETER;
    }

    iBufLen = (uiInLen % 8);
    if(iBufLen && (*puiOutLen >= ((1 + uiInLen / 8) * 8)))
    {
        *puiOutLen = ((1 + uiInLen / 8) * 8);
    }
    else if(!iBufLen && (*puiOutLen >= uiInLen))
    {
        *puiOutLen = uiInLen;
    }
    else
    {
        return M_WE_LIB_ERR_BUFFER_TOO_SMALL;
    }

    pstPara = (St_WeRC5Parameter*)handle;
    iIndex = pstPara->iBufLen;
    
    if((uiInLen + (WE_UINT32)iIndex) == *puiOutLen)
    {
        pstPara->iBufLen = 0;
        (void)WE_MEMCPY((pstPara->aucBuf + iIndex), pucIn, 8 - (WE_UINT32)iIndex);
        We_RC5CryptCBC(pstPara->aucBuf , pucOut, 8, &(pstPara->stKey),\
                          pstPara->aucIv, M_WE_RC5_DEC);
        if(8 != *puiOutLen)
        {
            We_RC5CryptCBC((pucIn + (8 - iIndex)), pucOut + 8, \
                (uiInLen - (8 - (WE_UINT32)iIndex) - (WE_UINT32)(pstPara->iBufLen)), &(pstPara->stKey), \
                pstPara->aucIv, M_WE_RC5_DEC);
        }            
    }
    else if((uiInLen + (WE_UINT32)iIndex) < *puiOutLen)
    {
        pstPara->iBufLen = (iIndex + (WE_INT32)uiInLen) % 8;
        *puiOutLen -= 8 ; 
        if((uiInLen + (WE_UINT32)iIndex) > 8)
        {    
            (void)WE_MEMCPY((pstPara->aucBuf + iIndex), pucIn, 8 - (WE_UINT32)iIndex);
            We_RC5CryptCBC(pstPara->aucBuf , pucOut, 8, \
                &(pstPara->stKey), pstPara->aucIv, M_WE_RC5_DEC);
            We_RC5CryptCBC((pucIn + (8 - iIndex)), pucOut + 8, \
                (uiInLen - (8 - (WE_UINT32)iIndex) - (WE_UINT32)(pstPara->iBufLen)), &(pstPara->stKey), \
                pstPara->aucIv, M_WE_RC5_DEC);
            iIndex = (WE_INT32)uiInLen - pstPara->iBufLen;
            (void)WE_MEMCPY(pstPara->aucBuf, pucIn + iIndex, (WE_UINT32)(pstPara->iBufLen));
        }    
        else
        {
            (void)WE_MEMCPY(pstPara->aucBuf + iIndex, pucIn, uiInLen);
        }
    }
    else
    {
        pstPara->iBufLen = (iIndex + (WE_INT32)uiInLen) - (WE_INT32)*puiOutLen;
        (void)WE_MEMCPY((pstPara->aucBuf + iIndex), pucIn, 8 - (WE_UINT32)iIndex);
        We_RC5CryptCBC(pstPara->aucBuf , pucOut, 8, \
                &(pstPara->stKey), pstPara->aucIv, M_WE_RC5_DEC);
        We_RC5CryptCBC((pucIn + (8 - iIndex)), pucOut + 8, \
            (uiInLen - (8 - (WE_UINT32)iIndex) - (WE_UINT32)(pstPara->iBufLen)), &(pstPara->stKey), \
            pstPara->aucIv, M_WE_RC5_DEC);
        iIndex = (WE_INT32)uiInLen - pstPara->iBufLen;
        (void)WE_MEMCPY(pstPara->aucBuf, pucIn + iIndex, (WE_UINT32)pstPara->iBufLen);
    }

    return iRv;
}

/*==================================================================================================
FUNCTION: 
    We_libRC5DecryptFinal1
CREATE DATE:
    2006-07-07
AUTHOR:
    steven ding
DESCRIPTION:
    
ARGUMENTS PASSED:
    WE_UCHAR*           pucOut[OUT]:
    WE_INT32*           piOutLen[IN/OUT]:
    WE_HANDLE           handle[IN]:
RETURN VALUE:
    ERROR CODE
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    self
IMPORTANT NOTES:
    None
==================================================================================================*/

 WE_INT32 We_LibRC5DecryptFinal1(WE_UCHAR* pucOut, 
                                        WE_UINT32* puiOutLen, WE_HANDLE handle)
{
    St_WeRC5Parameter*   pstPara = NULL;
    WE_UINT32             uiLen     = 0;

    if(!pucOut || !puiOutLen || !handle)
    {
        return M_WE_LIB_ERR_INVALID_PARAMETER;
    }
    if(*puiOutLen < 8)
    {
        return M_WE_LIB_ERR_BUFFER_TOO_SMALL;
    }
    *puiOutLen = 8;
    pstPara = (St_WeRC5Parameter*)handle;

    We_RC5CryptCBC(pstPara->aucBuf , pucOut, (WE_UINT32)8, \
                &(pstPara->stKey), pstPara->aucIv, M_WE_RC5_DEC);
    uiLen = *(pucOut + 8 - 1);
    if(uiLen&&(uiLen <= 8))
    {
        WE_UINT32   uiLoop = 0;
        for(; uiLoop < uiLen; uiLoop++)
        {
            if(uiLen != *(pucOut + 8 - 1 - uiLoop))
            {
                break;
            }
        }
        if(uiLoop == uiLen)
        {
            *puiOutLen -= uiLen;
        }
    }

    return We_LibRC5EncryptFinal(handle);
}


/*==================================================================================================
FUNCTION: 
    We_libRC5Encrypt
CREATE DATE:
    2006-07-07
AUTHOR:
    steven ding
DESCRIPTION:
    
ARGUMENTS PASSED:
    St_WeCipherCrptKey     stKey[IN]:
    const WE_UCHAR*         pucIn[IN]:
    WE_INT32                iInLen[IN]:
    WE_UCHAR*               pucOut[OUT]:
    WE_INT32*               piOutLen[IN/OUT]:
RETURN VALUE:
    ERROR CODE
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    self
IMPORTANT NOTES:
    None
==================================================================================================*/

WE_INT32 We_LibRC5Encrypt (WE_INT32 iRounds,
                                   St_WeCipherCrptKey stKey, 
                                   const WE_UCHAR* pucIn, 
                                   WE_UINT32 uiInLen, 
                                   WE_UCHAR* pucOut, 
                                   WE_UINT32* puiOutLen)
{
    St_WeRC5Key    stRc5Key    = {0};
    WE_INT32        iRv         = M_WE_LIB_ERR_OK;
    WE_UINT32       uiLen       = 0; 
    WE_UCHAR        aucIv[8]    = {0};

    if(!stKey.pucKey || !stKey.pucIv || (0 >= stKey.iKeyLen) || \
        (stKey.iIvLen < 8) || (0 == uiInLen) || (!pucIn) || (!pucOut) || (!puiOutLen))
    {
        return M_WE_LIB_ERR_INVALID_PARAMETER;
    }

    if((16 != stKey.iKeyLen)||(8 != stKey.iIvLen))
    {
        return M_WE_LIB_ERR_KEY_LENGTH;
    }

    uiLen = uiInLen % 8;
    if(uiLen && (*puiOutLen >= ((1 + uiInLen / 8) * 8)))
    {
        *puiOutLen = ((1 + uiInLen / 8) * 8);
    }
    else if(!uiLen && (*puiOutLen >= uiInLen))
    {
        *puiOutLen = uiInLen;
    }
    else
    {
        return M_WE_LIB_ERR_BUFFER_TOO_SMALL;
    }
    stRc5Key.iRounds = iRounds;
    We_LibSetRC5Key(&stRc5Key, stKey);

    (WE_VOID)WE_MEMCPY((WE_UCHAR*)aucIv, stKey.pucIv, 8);
    We_RC5CryptCBC(pucIn, pucOut, uiInLen, &(stRc5Key),\
        (WE_UCHAR*)aucIv, M_WE_RC5_ENC);
    
    return iRv;
}

/*==================================================================================================
FUNCTION: 
    We_libRC5Decrypt
CREATE DATE:
    2006-07-07
AUTHOR:
    steven ding
DESCRIPTION:
    
ARGUMENTS PASSED:
    St_WeCipherCrptKey     stKey[IN]:
    const WE_UCHAR*         pucIn[IN]:
    WE_INT32                iInLen[IN]:
    WE_UCHAR*               pucOut[OUT]:
    WE_INT32*               piOutLen[IN/OUT]:
RETURN VALUE:
    ERROR CODE
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    self
IMPORTANT NOTES:
    None
==================================================================================================*/

 WE_INT32 We_LibRC5Decrypt (WE_INT32 iRounds,
                                   St_WeCipherCrptKey stKey, 
                                   const WE_UCHAR* pucIn, 
                                   WE_UINT32 uiInLen, 
                                   WE_UCHAR* pucOut, 
                                   WE_UINT32* puiOutLen)
{
    St_WeRC5Key    stRc5Key    = {0};
    WE_INT32        iRv         = M_WE_LIB_ERR_OK;
    WE_UCHAR        aucIv[8]    = {0};

    if(!stKey.pucKey || !stKey.pucIv || (0 >= stKey.iKeyLen) || \
        (stKey.iIvLen < 8) || (0 == uiInLen) || (!pucIn) || (!pucOut) || (!puiOutLen))
    {
        return M_WE_LIB_ERR_INVALID_PARAMETER;
    }

    if((16 != stKey.iKeyLen)||(8 != stKey.iIvLen))
    {
        return M_WE_LIB_ERR_KEY_LENGTH;
    }

    if(!(uiInLen % 8) && (*puiOutLen >= uiInLen))
    {
        *puiOutLen = uiInLen;
    }
    else
    {
        return M_WE_LIB_ERR_BUFFER_TOO_SMALL;
    }
    stRc5Key.iRounds = iRounds;
    We_LibSetRC5Key(&stRc5Key, stKey);
    (WE_VOID)WE_MEMCPY(aucIv,stKey.pucIv,8);
    We_RC5CryptCBC(pucIn, pucOut, uiInLen, &stRc5Key, \
                    stKey.pucIv, M_WE_RC5_DEC);
    
    return iRv;
}
/*==================================================================================================
FUNCTION: 
    We_libRC5Encrypt1
CREATE DATE:
    2006-07-07
AUTHOR:
    steven ding
DESCRIPTION:
    
ARGUMENTS PASSED:
    St_WeCipherCrptKey     stKey[IN]:
    const WE_UCHAR*         pucIn[IN]:
    WE_INT32                iInLen[IN]:
    WE_UCHAR*               pucOut[OUT]:
    WE_INT32*               piOutLen[IN/OUT]:
RETURN VALUE:
    ERROR CODE
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    self
IMPORTANT NOTES:
    None
==================================================================================================*/
WE_INT32 We_LibRC5Encrypt1 (WE_INT32 iRounds,
                                   St_WeCipherCrptKey stKey, 
                                   const WE_UCHAR* pucIn, 
                                   WE_UINT32 uiInLen, 
                                   WE_UCHAR* pucOut, 
                                   WE_UINT32* puiOutLen)
{
    St_WeRC5Key    stRc5Key    = {0};
    WE_INT32        iRv         = M_WE_LIB_ERR_OK;
    WE_UINT32       uiLen       = 0; 
    WE_UCHAR        aucIv[8]    = {0};

    if(!stKey.pucKey || !stKey.pucIv || (0 >= stKey.iKeyLen) || \
        (stKey.iIvLen < 8) || (0 == uiInLen) || (!pucIn) || (!pucOut) || (!puiOutLen))
    {
        return M_WE_LIB_ERR_INVALID_PARAMETER;
    }

    if((16 != stKey.iKeyLen)||(8 != stKey.iIvLen))
    {
        return M_WE_LIB_ERR_KEY_LENGTH;
    }

    uiLen = uiInLen % 8;
    if((*puiOutLen >= ((1 + uiInLen / 8) * 8)))
    {
        *puiOutLen = ((1 + uiInLen / 8) * 8);
    }
    else
    {
        return M_WE_LIB_ERR_BUFFER_TOO_SMALL;
    }
    stRc5Key.iRounds = iRounds;
    We_LibSetRC5Key(&stRc5Key, stKey);

    (WE_VOID)WE_MEMCPY((WE_UCHAR*)aucIv, stKey.pucIv, 8);

    We_RC5CryptCBC(pucIn, pucOut, uiInLen - uiLen, &(stRc5Key),\
                    (WE_UCHAR*)aucIv, M_WE_RC5_ENC);

    {
        WE_UCHAR aucData[8] = {0};
        (WE_VOID)WE_MEMSET(aucData,(WE_UCHAR)(8-uiLen),8);
        if(uiLen)
        {
            (WE_VOID)WE_MEMCPY(aucData,(pucIn + uiInLen - uiLen),uiLen);
        }
        
        We_RC5CryptCBC(aucData, (pucOut + uiInLen - uiLen), 8, \
            &(stRc5Key), (WE_UCHAR*)aucIv, M_WE_RC5_ENC);
    }
    
    return iRv;
}
/*==================================================================================================
FUNCTION: 
    We_libRC5Decrypt1
CREATE DATE:
    2006-07-07
AUTHOR:
    steven ding
DESCRIPTION:
    
ARGUMENTS PASSED:
    St_WeCipherCrptKey     stKey[IN]:
    const WE_UCHAR*         pucIn[IN]:
    WE_INT32                iInLen[IN]:
    WE_UCHAR*               pucOut[OUT]:
    WE_INT32*               piOutLen[IN/OUT]:
RETURN VALUE:
    ERROR CODE
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    self
IMPORTANT NOTES:
    None
==================================================================================================*/
WE_INT32 We_LibRC5Decrypt1 (WE_INT32 iRounds,
                                   St_WeCipherCrptKey stKey, 
                                   const WE_UCHAR* pucIn, 
                                   WE_UINT32 uiInLen, 
                                   WE_UCHAR* pucOut, 
                                   WE_UINT32* puiOutLen)
{
    St_WeRC5Key    stRc5Key    = {0};
    WE_INT32        iRv         = M_WE_LIB_ERR_OK;
    WE_UINT32       uiLen       = 0; 
    WE_UCHAR        aucIv[8]    = {0};

    if(!stKey.pucKey || !stKey.pucIv || (0 >= stKey.iKeyLen) || \
        (stKey.iIvLen < 8) || (0 == uiInLen) || (!pucIn) || (!pucOut) || (!puiOutLen))
    {
        return M_WE_LIB_ERR_INVALID_PARAMETER;
    }

    if((16 != stKey.iKeyLen)||(8 != stKey.iIvLen))
    {
        return M_WE_LIB_ERR_KEY_LENGTH;
    }

    if(!(uiInLen % 8) && (*puiOutLen >= uiInLen))
    {
        *puiOutLen = uiInLen;
    }
    else
    {
        return M_WE_LIB_ERR_BUFFER_TOO_SMALL;
    }
    stRc5Key.iRounds = iRounds;
    We_LibSetRC5Key(&stRc5Key, stKey);
    (WE_VOID)WE_MEMCPY(aucIv,stKey.pucIv,8);
    We_RC5CryptCBC(pucIn, pucOut, uiInLen, &stRc5Key, \
                    stKey.pucIv, M_WE_RC5_DEC);
    uiLen = *(pucOut + uiInLen - 1);
    if(uiLen&&(uiLen <= 8))
    {
        WE_UINT32   uiLoop = 0;
        for(; uiLoop < uiLen; uiLoop++)
        {
            if(uiLen != *(pucOut + uiInLen - 1 - uiLoop))
            {
                break;
            }
        }
        if(uiLoop == uiLen)
        {
            *puiOutLen -= uiLen;
        }
    }
    
    return iRv;
}

/*idea*/
/*******************************************************************************
*   Type Define Section
*******************************************************************************/
typedef struct tagSt_WeIDEAParameter
{
    WE_UINT32       uiType;
    WE_UINT16       ausSecret[52];
    WE_UCHAR        aucIv[WE_IDEA_BLOCK_SIZE];
    WE_UCHAR        aucBuf[WE_IDEA_BLOCK_SIZE];
    WE_INT32        iBufLen;
}St_WeIDEAParameter;

/*******************************************************************************
*   Prototype Declare Section
*******************************************************************************/

static WE_INT32 We_LibIDEAEncryptCBC
(
    const WE_UCHAR* pucIn, 
    WE_UINT32 uiInLen, 
    WE_UCHAR* pucOut, 
    WE_HANDLE handle
);

static WE_INT32 We_LibIDEAEncrypt
(
    const WE_UCHAR* pucIn, 
    WE_UINT32 uiInLen, 
    WE_UCHAR* pucOut, 
    WE_HANDLE handle
);

static WE_INT32 We_LibIDEADecryptCBC
(
    const WE_UCHAR* pucIn, 
    WE_UINT32 uiInLen, 
    WE_UCHAR* pucOut, 
    WE_HANDLE handle
);

static WE_INT32 We_LibIDEADecrypt
(
    const WE_UCHAR* pucIn, 
    WE_UINT32 uiInLen, 
    WE_UCHAR* pucOut, 
    WE_HANDLE handle
);

static WE_INT32 We_LibIDEAKeyRotLeftMove(WE_UCHAR *pucKey);
static WE_UINT16 We_LibIDEAMul(WE_UINT16 usA, WE_UINT16 usB);
static WE_INT32 We_LibIDEAHandle(WE_UINT16 *pusData1, WE_UINT16 *pusSecret);
static WE_UINT16 We_LibIDEAMulInv(WE_UINT16 usX);

/*******************************************************************************
*   Function Define Section
*******************************************************************************/
/*==================================================================================================
FUNCTION: 
    We_LibIDEAEncryptNoPadding
CREATE DATE:
    2006-11-23
AUTHOR:
    stone An
DESCRIPTION:
    IDEA encrypt with no padding. The length of input is divided exactly by WE_IDEA_BLOCK_SIZE.
ARGUMENTS PASSED:
    St_WeCipherKey  stKey[IN]: secret key.
    const WE_UCHAR* pucIn[IN]: the data need to be encrypted.
    WE_UINT32       uiInLen[IN]: length of input data.
    WE_UCHAR*       pucOut[OUT]: the encrypted data.
    WE_UINT32*      puiOutLen[IN/OUT]: the length of output buffer when IN, 
                                       the length of encrypted data when OUT.
RETURN VALUE:
    M_WE_LIB_ERR_OK: success.
    other : fail.
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    omit.
IMPORTANT NOTES:
    None
==================================================================================================*/
WE_INT32 We_LibIDEAEncryptNoPadding
(
    St_WeCipherCrptKey  stKey, 
    const WE_UCHAR* pucIn, 
    WE_UINT32 uiInLen, 
    WE_UCHAR* pucOut, 
    WE_UINT32* puiOutLen
)
{
    WE_INT32     iRv      = M_WE_LIB_ERR_OK;
    WE_INT32     iRes     = M_WE_LIB_ERR_OK;
    WE_HANDLE    hHandle  = NULL;
    
    if( !stKey.pucKey || (0 >= stKey.iKeyLen) || 
        (0 == uiInLen) || (!pucIn) || (!pucOut) || (!puiOutLen))
    {
        return M_WE_LIB_ERR_INVALID_PARAMETER;
    }
    
    iRes = We_LibIDEAEncryptInit( stKey, &hHandle);    
    if (iRes == M_WE_LIB_ERR_OK)
    {
        iRv = We_LibIDEAEncryptUpdate( pucIn, uiInLen, pucOut, puiOutLen, hHandle);
        if (iRv == M_WE_LIB_ERR_OK)
        {
            iRes = We_LibIDEAEncryptFinal( hHandle);
        }
        else
        {
            iRes = We_LibIDEAEncryptFinal( hHandle);
            return iRv;
        }
    }
    
    return iRes;
}

/*==================================================================================================
FUNCTION: 
    We_LibIDEAEncryptPadding
CREATE DATE:
    2006-11-23
AUTHOR:
    stone An
DESCRIPTION:
    IDEA encrypt with padding. The length of input is not divided exactly by WE_IDEA_BLOCK_SIZE.
ARGUMENTS PASSED:
    St_WeCipherKey  stKey[IN]: secret key.
    const WE_UCHAR* pucIn[IN]: the data need to be encrypted.
    WE_UINT32       uiInLen[IN]: length of input data.
    WE_UCHAR*       pucOut[OUT]: the encrypted data.
    WE_UINT32*      puiOutLen[IN/OUT]: the length of output buffer when IN, 
                                       the length of encrypted data when OUT.
RETURN VALUE:
    M_WE_LIB_ERR_OK: success.
    other : fail.
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    omit.
IMPORTANT NOTES:
    None
==================================================================================================*/
WE_INT32 We_LibIDEAEncryptPadding
(
    St_WeCipherCrptKey  stKey, 
    const WE_UCHAR* pucIn, 
    WE_UINT32 uiInLen, 
    WE_UCHAR* pucOut, 
    WE_UINT32* puiOutLen
)
{
    WE_INT32     iRv      = M_WE_LIB_ERR_OK;
    WE_INT32     iRes     = M_WE_LIB_ERR_OK;
    WE_HANDLE    hHandle  = NULL;
    WE_UINT32    uiTmpLen = 0;
    
    if( !stKey.pucKey || (0 >= stKey.iKeyLen) || 
        (0 == uiInLen) || (!pucIn) || (!pucOut) || (!puiOutLen))
    {
        return M_WE_LIB_ERR_INVALID_PARAMETER;
    }
    uiTmpLen = *puiOutLen;
    
    iRes = We_LibIDEAEncryptInitPad( stKey, &hHandle);    
    if (iRes == M_WE_LIB_ERR_OK)
    {
        iRv = We_LibIDEAEncryptUpdatePad( pucIn, uiInLen, pucOut, puiOutLen, hHandle);
        uiTmpLen -= (*puiOutLen);
        if (iRv == M_WE_LIB_ERR_OK)
        {
            iRes = We_LibIDEAEncryptFinalPad( pucOut+(*puiOutLen), &uiTmpLen, hHandle);
        }
        else
        {
            iRes = We_LibIDEAEncryptFinalPad( pucOut+(*puiOutLen), &uiTmpLen, hHandle);
            return iRv;
        }
    }

    (*puiOutLen) += uiTmpLen;
    
    return iRes;
}


/*==================================================================================================
FUNCTION: 
    We_LibIDEAEncryptInit
CREATE DATE:
    2006-11-23
AUTHOR:
    stone An
DESCRIPTION:
    initialize IDEA algorithm without padding.
ARGUMENTS PASSED:
    St_WeCipherKey  stKey[IN]: secret key.
    WE_HANDLE*      pHandle[OUT]: the handle of IDEA.
RETURN VALUE:
    M_WE_LIB_ERR_OK: success.
    other : fail.
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    omit.
IMPORTANT NOTES:
    None
==================================================================================================*/
WE_INT32 We_LibIDEAEncryptInit
(
    St_WeCipherCrptKey stKey,
    WE_HANDLE* pHandle
)
{
    St_WeIDEAParameter*   pstPara   = NULL;    
    WE_INT32 iIndexI = 0;
    WE_INT32 iIndexJ = 0;
    WE_UCHAR  *pucTmpSecret = NULL;
    WE_UINT16 *pusTmpKey = NULL;
    
    if ((stKey.iKeyLen != 16) || (stKey.pucKey == NULL)||!pHandle)
    {
        return M_WE_LIB_ERR_INVALID_PARAMETER;
    }

    if(WE_IDEA_BLOCK_SIZE != stKey.iIvLen)
    {
        return M_WE_LIB_ERR_KEY_LENGTH;
    } 

    pusTmpKey = (WE_UINT16 *)WE_MALLOC((WE_UINT32)(stKey.iKeyLen / 2) * sizeof(WE_UINT16));
    if (NULL == pusTmpKey)
    {
        return M_WE_LIB_ERR_INSUFFICIENT_MEMORY;
    }    
    (WE_VOID)WE_MEMCPY((WE_UINT8 *)pusTmpKey, stKey.pucKey, (WE_UINT32)stKey.iKeyLen);
    pucTmpSecret = (WE_UINT8 *)pusTmpKey;
    pstPara = (St_WeIDEAParameter *)WE_MALLOC(sizeof(St_WeIDEAParameter));
    if (NULL == pstPara)
    {
        WE_FREE(pucTmpSecret);
        return M_WE_LIB_ERR_INSUFFICIENT_MEMORY;
    }
    (WE_VOID)WE_MEMSET(pstPara, 0, sizeof(St_WeIDEAParameter));
    /* expand the private key to 52 bytes. */
    for (iIndexI=0; iIndexI<6; iIndexI++)
    {
        for (iIndexJ=0; iIndexJ<8; iIndexJ++)
        {
            (pstPara->ausSecret)[8*iIndexI + iIndexJ] = pusTmpKey[iIndexJ];
        }
        
        (WE_VOID)We_LibIDEAKeyRotLeftMove(pucTmpSecret); /* the 128 bits secret move left rotate 25 bits.*/ 
    }    
    
    for (iIndexJ=0; iIndexJ<4; iIndexJ++)
    {
        (pstPara->ausSecret)[48 + iIndexJ] = pusTmpKey[iIndexJ];
    }
    
    pstPara->uiType = M_WE_IDEA_TYPE;
    (WE_VOID)WE_MEMCPY(pstPara->aucIv, stKey.pucIv, WE_IDEA_BLOCK_SIZE);
    *pHandle = pstPara;
    WE_FREE(pucTmpSecret);
    return M_WE_LIB_ERR_OK;
}

/*==================================================================================================
FUNCTION: 
    We_LibIDEAEncryptInitPad
CREATE DATE:
    2006-11-23
AUTHOR:
    stone An
DESCRIPTION:
    initialize IDEA algorithm using with padding.
ARGUMENTS PASSED:
    St_WeCipherKey  stKey[IN]: secret key.
    WE_HANDLE*      pHandle[OUT]: the handle of IDEA.
RETURN VALUE:
    M_WE_LIB_ERR_OK: success.
    other : fail.
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    omit.
IMPORTANT NOTES:
    None
==================================================================================================*/
WE_INT32 We_LibIDEAEncryptInitPad
(
    St_WeCipherCrptKey stKey,
    WE_HANDLE* pHandle
)
{
    return We_LibIDEAEncryptInit( stKey, pHandle);
}

/*==================================================================================================
FUNCTION: 
    We_LibIDEAEncryptUpdate
CREATE DATE:
    2006-11-23
AUTHOR:
    stone An
DESCRIPTION:
    encrypt data with IDEA algorithm.
ARGUMENTS PASSED:
    const WE_UCHAR* pucIn[IN]: the data need to be encrypted.
    WE_UINT32       uiInLen[IN]: length of input data.
    WE_UCHAR*       pucOut[OUT]: the encrypted data.
    WE_UINT32*      puiOutLen[IN/OUT]: the length of output buffer when IN, 
                                       the length of encrypted data when OUT.
    WE_HANDLE       Handle[IN]: the handle of IDEA.
RETURN VALUE:
    M_WE_LIB_ERR_OK: success.
    other : fail.
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    omit.
IMPORTANT NOTES:
    None
==================================================================================================*/
WE_INT32 We_LibIDEAEncryptUpdate
(
    const WE_UCHAR* pucIn, 
    WE_UINT32 uiInLen, 
    WE_UCHAR* pucOut, 
    WE_UINT32* puiOutLen,
    WE_HANDLE handle
)
{
    WE_UINT32             uiLen     = 0; 

    if ( (0 == uiInLen) || (!pucIn) || (!pucOut) || (!puiOutLen) || !handle)
    {
        return M_WE_LIB_ERR_INVALID_PARAMETER;
    }
    
    if (*puiOutLen >= uiInLen )
    {
        *puiOutLen = uiInLen;
    }
    else
    {
        return M_WE_LIB_ERR_BUFFER_TOO_SMALL;
    }

    uiLen = uiInLen % WE_IDEA_BLOCK_SIZE;
    if(0 == uiLen)
    {        
        (WE_VOID)We_LibIDEAEncryptCBC( pucIn, uiInLen, pucOut, handle);
    }
    else
    {
        return M_WE_LIB_ERR_DATA_LENGTH;
    }

    return M_WE_LIB_ERR_OK;    
}

/*==================================================================================================
FUNCTION: 
    We_LibIDEAEncryptUpdatePad
CREATE DATE:
    2006-11-23
AUTHOR:
    stone An
DESCRIPTION:
    encrypt data with IDEA algorithm using with padding.
ARGUMENTS PASSED:
    const WE_UCHAR* pucIn[IN]: the data need to be encrypted.
    WE_UINT32       uiInLen[IN]: length of input data.
    WE_UCHAR*       pucOut[OUT]: the encrypted data.
    WE_UINT32*      puiOutLen[IN/OUT]: the length of output buffer when IN, 
                                       the length of encrypted data when OUT.
    WE_HANDLE       Handle[IN]: the handle of IDEA.
RETURN VALUE:
    M_WE_LIB_ERR_OK: success.
    other : fail.
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    omit.
IMPORTANT NOTES:
    None
==================================================================================================*/
WE_INT32 We_LibIDEAEncryptUpdatePad
(
    const WE_UCHAR* pucIn, 
    WE_UINT32 uiInLen, 
    WE_UCHAR* pucOut, 
    WE_UINT32* puiOutLen,
    WE_HANDLE handle
)
{
    WE_UINT32             uiRealInLen = 0;
    WE_UINT32             uiIndex    = 0;
    St_WeIDEAParameter*   pstPara   = NULL;

    if( (0 == uiInLen) || (!pucIn) || (!pucOut) || (!puiOutLen) || !handle)
    {
        return M_WE_LIB_ERR_INVALID_PARAMETER;
    }
    
    pstPara = (St_WeIDEAParameter*)handle;
    uiIndex = (WE_UINT32)(pstPara->iBufLen);

    uiRealInLen = ((uiInLen + uiIndex) / WE_IDEA_BLOCK_SIZE) * WE_IDEA_BLOCK_SIZE;
    if (*puiOutLen >= uiRealInLen)
    {
        *puiOutLen = uiRealInLen;
    }
    else
    {
        return M_WE_LIB_ERR_BUFFER_TOO_SMALL;
    }

    pstPara->iBufLen = (WE_INT32)(uiInLen + uiIndex - uiRealInLen);
    (WE_VOID)WE_MEMCPY((pstPara->aucBuf + uiIndex), pucIn, (WE_UINT32)(WE_IDEA_BLOCK_SIZE - uiIndex));
    (WE_VOID)We_LibIDEAEncryptCBC( pstPara->aucBuf, WE_IDEA_BLOCK_SIZE, pucOut, handle);
    if (WE_IDEA_BLOCK_SIZE != *puiOutLen)
    {
        (WE_VOID)We_LibIDEAEncryptCBC( (pucIn + (WE_IDEA_BLOCK_SIZE - uiIndex)),            
                uiRealInLen-WE_IDEA_BLOCK_SIZE, pucOut+WE_IDEA_BLOCK_SIZE, handle);
    }
    uiIndex = uiInLen - (WE_UINT32)(pstPara->iBufLen);
    (WE_VOID)WE_MEMCPY(pstPara->aucBuf, pucIn + uiIndex, (WE_UINT32)(pstPara->iBufLen));
    
    return M_WE_LIB_ERR_OK;    
}

/*==================================================================================================
FUNCTION: 
    We_LibIDEAEncryptFinal
CREATE DATE:
    2006-11-23
AUTHOR:
    stone An
DESCRIPTION:
    the final handle of IDEA algorithm, release the handle of IDEA algorithm.
ARGUMENTS PASSED:
    WE_HANDLE       Handle[IN]: the handle of IDEA algorithm.
RETURN VALUE:
    M_WE_LIB_ERR_OK: success.
    other : fail.
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    omit.
IMPORTANT NOTES:
    None
==================================================================================================*/
WE_INT32 We_LibIDEAEncryptFinal(WE_HANDLE handle)
{
    St_WeIDEAParameter*   pstPara   = NULL;
    
    if(NULL == handle)
    {
        return M_WE_LIB_ERR_INVALID_PARAMETER;
    }
    
    pstPara = (St_WeIDEAParameter *)handle;
    WE_FREE(pstPara);    

    return M_WE_LIB_ERR_OK;
}

/*==================================================================================================
FUNCTION: 
    We_LibIDEAEncryptFinalPad
CREATE DATE:
    2006-11-23
AUTHOR:
    stone An
DESCRIPTION:
    the final handle with pad. Encryt the left plain data and then release the handle of IDEA algorthm.
ARGUMENTS PASSED:
    WE_UCHAR*       pucOut[OUT]: the encrypted data.
    WE_UINT32*      puiOutLen[IN/OUT]: the length of output buffer when IN, 
                                       the length of encrypted data when OUT.
    WE_HANDLE       Handle[IN]: the handle of IDEA algorithm.
RETURN VALUE:
    M_WE_LIB_ERR_OK: success.
    other : fail.
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    omit.
IMPORTANT NOTES:
    None
==================================================================================================*/
WE_INT32 We_LibIDEAEncryptFinalPad
(
    WE_UCHAR* pucOut, 
    WE_UINT32* puiOutLen,  
    WE_HANDLE handle
)
{
    St_WeIDEAParameter*   pstPara   = NULL;
    WE_UCHAR             aucData[WE_IDEA_BLOCK_SIZE]    = {0};
    
    if((NULL == handle) || (NULL == pucOut) || (NULL == puiOutLen))
    {
        return M_WE_LIB_ERR_INVALID_PARAMETER;
    }    

    pstPara = (St_WeIDEAParameter*)handle;
    
    if (pstPara->iBufLen != 0)
    {  
        if (*puiOutLen < WE_IDEA_BLOCK_SIZE)
        {
            return M_WE_LIB_ERR_BUFFER_TOO_SMALL;
        }
        *puiOutLen = WE_IDEA_BLOCK_SIZE;
        (WE_VOID)WE_MEMSET(aucData, (WE_UCHAR)(WE_IDEA_BLOCK_SIZE-pstPara->iBufLen), (WE_UINT32)WE_IDEA_BLOCK_SIZE);
        (WE_VOID)WE_MEMCPY(aucData, pstPara->aucBuf, (WE_UINT32)pstPara->iBufLen);
        (WE_VOID)We_LibIDEAEncryptCBC( aucData, WE_IDEA_BLOCK_SIZE, pucOut, handle);
    }
    else
    {
        *puiOutLen = 0;
    }
    
    WE_FREE(pstPara);    
    return M_WE_LIB_ERR_OK;
}

/*==================================================================================================
FUNCTION: 
    We_LibIDEAEncryptCBC
CREATE DATE:
    2006-11-23
AUTHOR:
    stone An
DESCRIPTION:
    Encryt the input data with IDEA algorthm.
ARGUMENTS PASSED:
    const WE_UCHAR* pucIn[IN]: the data need to be encrypted.
    WE_UINT32       uiInLen[IN]: length of input data.
    WE_UCHAR*       pucOut[OUT]: the encrypted data.
    WE_HANDLE       Handle[IN]: the handle of IDEA algorithm.
RETURN VALUE:
    M_WE_LIB_ERR_OK: success.
    other : fail.
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    omit.
IMPORTANT NOTES:
    None
==================================================================================================*/
static WE_INT32 We_LibIDEAEncryptCBC
(
    const WE_UCHAR* pucIn, 
    WE_UINT32 uiInLen, 
    WE_UCHAR* pucOut, 
    WE_HANDLE handle
)
{  
    
    WE_INT32  iLen = (WE_INT32)uiInLen;
    St_WeIDEAParameter*    pstPara = NULL;
    WE_INT32              iRes    = 0;
    WE_INT32              iIndex  = 0;
    WE_INT32              iIndexJ = 0; 
    WE_UCHAR              aucInTmp[WE_IDEA_BLOCK_SIZE] = {0}; 
    
    if( (0 == uiInLen) || (!pucIn) || (!pucOut) || !handle)
    {
        return M_WE_LIB_ERR_INVALID_PARAMETER;
    }

    pstPara = (St_WeIDEAParameter*)handle;
    
    for (iIndexJ=0; iIndexJ<iLen; iIndexJ+=WE_IDEA_BLOCK_SIZE)
    {
        for (iIndex=0; iIndex<WE_IDEA_BLOCK_SIZE; iIndex++)
        {
            aucInTmp[iIndex] = pucIn[iIndex+iIndexJ];
            aucInTmp[iIndex] ^= ((pstPara->aucIv)[iIndex]);
        }
        iRes = We_LibIDEAEncrypt( aucInTmp, WE_IDEA_BLOCK_SIZE, &pucOut[iIndexJ], handle);
        if (iRes != M_WE_LIB_ERR_OK)
        {
            return iRes;
        }

        (WE_VOID)WE_MEMCPY(pstPara->aucIv, &pucOut[iIndexJ], WE_IDEA_BLOCK_SIZE);
    }

    return M_WE_LIB_ERR_OK;        
}

/*==================================================================================================
FUNCTION: 
    We_LibIDEAEncrypt
CREATE DATE:
    2006-11-23
AUTHOR:
    stone An
DESCRIPTION:
    Encryt the input data which is one WE_IDEA_BLOCK_SIZE with IDEA algorthm.
ARGUMENTS PASSED:
    const WE_UCHAR* pucIn[IN]: the data need to be encrypted.
    WE_UINT32       uiInLen[IN]: length of input data, which is must be WE_IDEA_BLOCK_SIZE.
    WE_UCHAR*       pucOut[OUT]: the encrypted data.
    WE_HANDLE       Handle[IN]: the handle of IDEA algorithm.
RETURN VALUE:
    M_WE_LIB_ERR_OK: success.
    other : fail.
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    omit.
IMPORTANT NOTES:
    None
==================================================================================================*/
static WE_INT32 We_LibIDEAEncrypt
(
    const WE_UCHAR* pucIn, 
    WE_UINT32 uiInLen, 
    WE_UCHAR* pucOut, 
    WE_HANDLE handle
)
{
    St_WeIDEAParameter*   pstPara   = (St_WeIDEAParameter *)handle;
    WE_INT32              iIndexI = 0;
    WE_INT32              iRes = 0;
    WE_UINT16             usTmp = 0;
    WE_UINT16             *pusData = NULL;
    
    if ( (WE_IDEA_BLOCK_SIZE != uiInLen) || (!pucIn) || (!pucOut) || !handle)
    {
        return M_WE_LIB_ERR_INVALID_PARAMETER;
    }

    (WE_VOID)WE_MEMCPY(pucOut, pucIn, WE_IDEA_BLOCK_SIZE);
    
    pusData = (WE_UINT16 *)WE_MALLOC(WE_IDEA_BLOCK_SIZE / 2 * sizeof(WE_UINT16));
    if (NULL == pusData)
    {
        return M_WE_LIB_ERR_BUFFER_TOO_SMALL;
    }
    //pusTmp = pusData;
    (WE_VOID)WE_MEMCPY((WE_UCHAR *)pusData, pucOut, WE_IDEA_BLOCK_SIZE);
    
    for (iIndexI=0; iIndexI<8; iIndexI++)
    {
        iRes = We_LibIDEAHandle(pusData, &(pstPara->ausSecret)[6*iIndexI]);
        if (iRes != M_WE_LIB_ERR_OK)
        {
            WE_FREE(pusData);
            return iRes;
        }
    }
    usTmp = pusData[1];
    pusData[1] = pusData[2];
    pusData[2] = usTmp;
    
    pusData[0] = We_LibIDEAMul(pusData[0], (pstPara->ausSecret)[48]);
    pusData[1] += pstPara->ausSecret[49];
    pusData[2] += pstPara->ausSecret[50];
    pusData[3] = We_LibIDEAMul(pusData[3], (pstPara->ausSecret)[51]);
    
    (WE_VOID)WE_MEMCPY(pucOut, (WE_UCHAR *)pusData, WE_IDEA_BLOCK_SIZE);
    WE_FREE(pusData);
    
    return M_WE_LIB_ERR_OK;    
}


/*==================================================================================================
FUNCTION: 
    We_LibIDEADecryptNoPadding
CREATE DATE:
    2006-11-23
AUTHOR:
    stone An
DESCRIPTION:
    IDEA decrypt with no padding. The length of input is divided exactly by WE_IDEA_BLOCK_SIZE.
ARGUMENTS PASSED:
    St_WeCipherKey  stKey[IN]: secret key.
    const WE_UCHAR* pucIn[IN]: the data need to be decrypted.
    WE_UINT32       uiInLen[IN]: length of input data.
    WE_UCHAR*       pucOut[OUT]: the decrypted data.
    WE_UINT32*      puiOutLen[IN/OUT]: the length of output buffer when IN, 
                                       the length of decrypted data when OUT.
RETURN VALUE:
    M_WE_LIB_ERR_OK: success.
    other : fail.
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    omit.
IMPORTANT NOTES:
    None
==================================================================================================*/
WE_INT32 We_LibIDEADecryptNoPadding
(
    St_WeCipherCrptKey  stKey, 
    const WE_UCHAR* pucIn, 
    WE_UINT32 uiInLen, 
    WE_UCHAR* pucOut, 
    WE_UINT32* puiOutLen
)
{
    WE_INT32     iRv      = M_WE_LIB_ERR_OK;
    WE_INT32     iRes     = M_WE_LIB_ERR_OK;
    WE_HANDLE    hHandle  = NULL;
    
    if ( !stKey.pucKey || (0 >= stKey.iKeyLen) || 
        (0 == uiInLen) || (!pucIn) || (!pucOut) || (!puiOutLen))
    {
        return M_WE_LIB_ERR_INVALID_PARAMETER;
    }

    if (0 != uiInLen % WE_IDEA_BLOCK_SIZE)
    {
        return M_WE_LIB_ERR_DATA_LENGTH;
    }
    
    iRes = We_LibIDEADecryptInit( stKey, &hHandle);
    if (iRes == M_WE_LIB_ERR_OK)
    {
        iRv = We_LibIDEADecryptUpdate( pucIn, uiInLen, pucOut, puiOutLen, hHandle);
        if (iRv == M_WE_LIB_ERR_OK)
        {
            iRes = We_LibIDEADecryptFinal( hHandle);
        }
        else
        {
            iRes = We_LibIDEADecryptFinal( hHandle);
            return iRv;
        }
    }
    
    return iRes;
}

/*==================================================================================================
FUNCTION: 
    We_LibIDEADecryptPadding
CREATE DATE:
    2006-11-23
AUTHOR:
    stone An
DESCRIPTION:
    IDEA decrypt with padding. The length of input is not divided exactly by WE_IDEA_BLOCK_SIZE.
ARGUMENTS PASSED:
    St_WeCipherKey  stKey[IN]: secret key.
    const WE_UCHAR* pucIn[IN]: the data need to be decrypted.
    WE_UINT32       uiInLen[IN]: length of input data.
    WE_UCHAR*       pucOut[OUT]: the decrypted data.
    WE_UINT32*      puiOutLen[IN/OUT]: the length of output buffer when IN, 
                                       the length of decrypted data when OUT.
RETURN VALUE:
    M_WE_LIB_ERR_OK: success.
    other : fail.
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    omit.
IMPORTANT NOTES:
    None
==================================================================================================*/
WE_INT32 We_LibIDEADecryptPadding
(
    St_WeCipherCrptKey  stKey, 
    const WE_UCHAR* pucIn, 
    WE_UINT32 uiInLen, 
    WE_UCHAR* pucOut, 
    WE_UINT32* puiOutLen
)
{
    WE_INT32     iRv      = M_WE_LIB_ERR_OK;
    WE_INT32     iRes     = M_WE_LIB_ERR_OK;
    WE_HANDLE    hHandle  = NULL;
    WE_UINT32    uiTmpLen = 0;
    
    if( !stKey.pucKey || (0 >= stKey.iKeyLen) || 
        (0 == uiInLen) || (!pucIn) || (!pucOut) || (!puiOutLen))
    {
        return M_WE_LIB_ERR_INVALID_PARAMETER;
    }
    
    iRes = We_LibIDEADecryptInitPad( stKey, &hHandle);    
    if (iRes == M_WE_LIB_ERR_OK)
    {
        iRv = We_LibIDEADecryptUpdatePad( pucIn, uiInLen, pucOut, puiOutLen, hHandle);
        if (iRv == M_WE_LIB_ERR_OK)
        {
            iRes = We_LibIDEADecryptFinalPad( pucOut+(*puiOutLen), &uiTmpLen, hHandle);
        }
        else
        {
            iRes = We_LibIDEADecryptFinalPad( pucOut+(*puiOutLen), &uiTmpLen, hHandle);
            return iRv;
        }
    }

    (*puiOutLen) -= uiTmpLen;
    
    return iRes;
}

/*==================================================================================================
FUNCTION: 
    We_LibIDEADecryptInit
CREATE DATE:
    2006-11-23
AUTHOR:
    stone An
DESCRIPTION:
    initialize IDEA algorithm without padding.
ARGUMENTS PASSED:
    St_WeCipherKey  stKey[IN]: secret key.
    WE_HANDLE*      pHandle[OUT]: the handle of IDEA.
RETURN VALUE:
    M_WE_LIB_ERR_OK: success.
    other : fail.
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    omit.
IMPORTANT NOTES:
    None
==================================================================================================*/
WE_INT32 We_LibIDEADecryptInit
( 
    St_WeCipherCrptKey stKey,
    WE_HANDLE* pHandle
)
{
    WE_INT32  iIndexI = 0;
    WE_UINT8 aucKeyPosChange[52] = {48,49,50,51,46,47, 
        42,44,43,45,40,41,
        36,38,37,39,34,35,
        30,32,31,33,28,29,
        24,26,25,27,22,23,
        18,20,19,21,16,17,
        12,14,13,15,10,11,
        6, 8, 7, 9, 4, 5,
        0, 1, 2, 3 
        };
    WE_UINT8 aucMulRevPos[18] = {
        0,3,6,9,12,15,18,21,24,27,30,33,36,39,42,45,48,51}; 
    WE_UINT8 aucAddRevPos[18] ={
            1,2,7,8,13,14,19,20,25,26,31,32,37,38,43,44,49,50}; 
    St_WeIDEAParameter*    pstPara   = NULL;
    WE_UINT16              ausEncyptSecret[52] = {0};
    WE_INT32               iRes = M_WE_LIB_ERR_OK;    
            
    if (NULL == pHandle)
    {
        return M_WE_LIB_ERR_INVALID_PARAMETER;
    }
    
    iRes = We_LibIDEAEncryptInit( stKey, pHandle);
    if (iRes != M_WE_LIB_ERR_OK)
    {
        return iRes;
    }
    
    pstPara = (St_WeIDEAParameter *)(*pHandle);
    for (iIndexI=0 ; iIndexI<52 ; iIndexI++)
    { 
        ausEncyptSecret[iIndexI] = (pstPara->ausSecret)[iIndexI];
    }

    for (iIndexI=0 ; iIndexI<52 ; iIndexI++)
    { 
        (pstPara->ausSecret)[iIndexI] = ausEncyptSecret[aucKeyPosChange[iIndexI]];
    }
    
    for (iIndexI=0 ; iIndexI<18 ; iIndexI++) /* reverse of add mod 65536. */
    {
        (pstPara->ausSecret)[aucAddRevPos[iIndexI]] = (WE_UINT16)(65536 - (pstPara->ausSecret)[aucAddRevPos[iIndexI]]);
    }

    for (iIndexI=0 ; iIndexI<18 ; iIndexI++) /* reverse of multiply mod 65537. */
    {
        (pstPara->ausSecret)[aucMulRevPos[iIndexI]] = We_LibIDEAMulInv((pstPara->ausSecret)[aucMulRevPos[iIndexI]]);
    }
    
    return M_WE_LIB_ERR_OK;
}

/*==================================================================================================
FUNCTION: 
    We_LibIDEADecryptInitPad
CREATE DATE:
    2006-11-23
AUTHOR:
    stone An
DESCRIPTION:
    initialize IDEA algorithm using with padding.
ARGUMENTS PASSED:
    St_WeCipherKey  stKey[IN]: secret key.
    WE_HANDLE*      pHandle[OUT]: the handle of IDEA.
RETURN VALUE:
    M_WE_LIB_ERR_OK: success.
    other : fail.
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    omit.
IMPORTANT NOTES:
    None
==================================================================================================*/
WE_INT32 We_LibIDEADecryptInitPad
( 
    St_WeCipherCrptKey stKey,
    WE_HANDLE* pHandle
)
{
    return We_LibIDEADecryptInit( stKey, pHandle);
}

/*==================================================================================================
FUNCTION: 
    We_LibIDEADecryptUpdate
CREATE DATE:
    2006-11-23
AUTHOR:
    stone An
DESCRIPTION:
    decrypt data with IDEA algorithm.
ARGUMENTS PASSED:
    const WE_UCHAR* pucIn[IN]: the data need to be decrypted.
    WE_UINT32       uiInLen[IN]: length of input data.
    WE_UCHAR*       pucOut[OUT]: the decrypted data.
    WE_UINT32*      puiOutLen[IN/OUT]: the length of output buffer when IN, 
                                       the length of encrypted data when OUT.
    WE_HANDLE       Handle[IN]: the handle of IDEA.
RETURN VALUE:
    M_WE_LIB_ERR_OK: success.
    other : fail.
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    omit.
IMPORTANT NOTES:
    None
==================================================================================================*/
WE_INT32 We_LibIDEADecryptUpdate
(
    const WE_UCHAR* pucIn, 
    WE_UINT32 uiInLen, 
    WE_UCHAR* pucOut, 
    WE_UINT32* puiOutLen,
    WE_HANDLE handle
)
{
    WE_UINT32             uiLen     = 0; 

    if( (0 == uiInLen) || (!pucIn) || (!pucOut) || (!puiOutLen) || !handle)
    {
        return M_WE_LIB_ERR_INVALID_PARAMETER;
    }
    
    if(*puiOutLen >= uiInLen )
    {
        *puiOutLen = uiInLen;
    }
    else
    {
        return M_WE_LIB_ERR_BUFFER_TOO_SMALL;
    }

    uiLen = uiInLen % WE_IDEA_BLOCK_SIZE;
    if(0 == uiLen)
    {        
        (WE_VOID)We_LibIDEADecryptCBC( pucIn, uiInLen, pucOut, handle);
    }
    else
    {
        return M_WE_LIB_ERR_DATA_LENGTH;
    }

    return M_WE_LIB_ERR_OK;    
}

/*==================================================================================================
FUNCTION: 
    We_LibIDEADecryptUpdatePad
CREATE DATE:
    2006-11-23
AUTHOR:
    stone An
DESCRIPTION:
    decrypt data with IDEA algorithm using with padding.
ARGUMENTS PASSED:
    const WE_UCHAR* pucIn[IN]: the data need to be decrypted.
    WE_UINT32       uiInLen[IN]: length of input data.
    WE_UCHAR*       pucOut[OUT]: the decrypted data.
    WE_UINT32*      puiOutLen[IN/OUT]: the length of output buffer when IN, 
                                       the length of encrypted data when OUT.
    WE_HANDLE       Handle[IN]: the handle of IDEA.
RETURN VALUE:
    M_WE_LIB_ERR_OK: success.
    other : fail.
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    omit.
IMPORTANT NOTES:
    None
==================================================================================================*/
WE_INT32 We_LibIDEADecryptUpdatePad
(
    const WE_UCHAR* pucIn, 
    WE_UINT32 uiInLen, 
    WE_UCHAR* pucOut, 
    WE_UINT32* puiOutLen,
    WE_HANDLE handle
)
{
    WE_UINT32             uiRealInLen = 0;
    WE_UINT32             uiIndex     = 0;
    St_WeIDEAParameter*   pstPara     = NULL;
    WE_INT32              iRes        = 0;

    if( (0 == uiInLen) || (!pucIn) || (!pucOut) || (!puiOutLen) || !handle)
    {
        return M_WE_LIB_ERR_INVALID_PARAMETER;
    }
    
    pstPara = (St_WeIDEAParameter*)handle;
    uiIndex = (WE_UINT32)(pstPara->iBufLen);

    uiRealInLen = ((uiInLen + uiIndex) / WE_IDEA_BLOCK_SIZE) * WE_IDEA_BLOCK_SIZE;
    if (*puiOutLen >= uiRealInLen)
    {
        *puiOutLen = uiRealInLen;
    }
    else
    {
        return M_WE_LIB_ERR_BUFFER_TOO_SMALL;
    }
    
    pstPara->iBufLen = (WE_INT32)(uiInLen + uiIndex - uiRealInLen);
    (WE_VOID)WE_MEMCPY((pstPara->aucBuf + uiIndex), pucIn, (WE_UINT32)(WE_IDEA_BLOCK_SIZE - uiIndex));
    iRes = We_LibIDEADecryptCBC( pstPara->aucBuf, WE_IDEA_BLOCK_SIZE, pucOut, handle);
    if (WE_IDEA_BLOCK_SIZE != *puiOutLen)
    {
        iRes = We_LibIDEADecryptCBC( (pucIn + (WE_IDEA_BLOCK_SIZE - uiIndex)),            
                uiRealInLen-WE_IDEA_BLOCK_SIZE, pucOut+WE_IDEA_BLOCK_SIZE, handle);
    }
    uiIndex = uiInLen - (WE_UINT32)(pstPara->iBufLen);
    (WE_VOID)WE_MEMCPY(pstPara->aucBuf, pucIn + uiIndex, (WE_UINT32)(pstPara->iBufLen));
    
    return iRes;
}

/*==================================================================================================
FUNCTION: 
    We_LibIDEADecryptFinal
CREATE DATE:
    2006-11-23
AUTHOR:
    stone An
DESCRIPTION:
    the final handle of IDEA algorithm, release the handle of IDEA algorithm.
ARGUMENTS PASSED:
    WE_HANDLE       Handle[IN]: the handle of IDEA algorithm.
RETURN VALUE:
    M_WE_LIB_ERR_OK: success.
    other : fail.
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    omit.
IMPORTANT NOTES:
    None
==================================================================================================*/
WE_INT32 We_LibIDEADecryptFinal(WE_HANDLE handle)
{
    return We_LibIDEAEncryptFinal( handle);
}

/*==================================================================================================
FUNCTION: 
    We_LibIDEADecryptFinalPad
CREATE DATE:
    2006-11-23
AUTHOR:
    stone An
DESCRIPTION:
    the final handle with pad. compute the length of padding data.
ARGUMENTS PASSED:
    WE_UCHAR*       pucOut[OUT]: the end of decrypted data.
    WE_UINT32*      puiOutLen[IN/OUT]: the length of padding data.
    WE_HANDLE       Handle[IN]: the handle of IDEA algorithm.
RETURN VALUE:
    M_WE_LIB_ERR_OK: success.
    other : fail.
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    omit.
IMPORTANT NOTES:
    None
==================================================================================================*/
WE_INT32 We_LibIDEADecryptFinalPad
(
    WE_UCHAR* pucOut, 
    WE_UINT32* puiOutLen,  
    WE_HANDLE handle
)
{
    St_WeIDEAParameter*   pstPara = NULL;
    WE_UINT32             uiLen = 0;
    
    if((NULL == handle) || (NULL == pucOut) || (NULL == puiOutLen))
    {
        return M_WE_LIB_ERR_INVALID_PARAMETER;
    }
    pstPara = (St_WeIDEAParameter*)handle;
    
    *puiOutLen = 0;
    uiLen = *(pucOut - 1);
    if ((uiLen > 0) && (uiLen < WE_IDEA_BLOCK_SIZE))
    {
        WE_UINT32   uiLoop = 0;
        for (; uiLoop < uiLen; uiLoop++)
        {
            if (uiLen != *(pucOut - 1 - uiLoop))
            {
                break;
            }
        }
        if (uiLoop == uiLen)
        {
            *puiOutLen = uiLen;
        }
    }
    
    WE_FREE(pstPara);    
    return M_WE_LIB_ERR_OK;
}

/*==================================================================================================
FUNCTION: 
    We_LibIDEADecryptCBC
CREATE DATE:
    2006-11-23
AUTHOR:
    stone An
DESCRIPTION:
    Decryt the input data with IDEA algorthm.
ARGUMENTS PASSED:
    const WE_UCHAR* pucIn[IN]: the data need to be decrypted.
    WE_UINT32       uiInLen[IN]: length of input data.
    WE_UCHAR*       pucOut[OUT]: the decrypted data.
    WE_HANDLE       Handle[IN]: the handle of IDEA algorithm.
RETURN VALUE:
    M_WE_LIB_ERR_OK: success.
    other : fail.
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    omit.
IMPORTANT NOTES:
    None
==================================================================================================*/
static WE_INT32 We_LibIDEADecryptCBC
(
    const WE_UCHAR* pucIn, 
    WE_UINT32 uiInLen, 
    WE_UCHAR* pucOut, 
    WE_HANDLE handle
)
{  
    
    WE_INT32  iLen = (WE_INT32)uiInLen;
    St_WeIDEAParameter*    pstPara = NULL;
    WE_INT32              iRes    = 0;
    WE_INT32              iIndex  = 0;
    WE_INT32              iIndexJ = 0; 
    WE_UCHAR              aucInTmp[WE_IDEA_BLOCK_SIZE] = {0};  
    
    if( (0 == uiInLen) || (!pucIn) || (!pucOut) || !handle)
    {
        return M_WE_LIB_ERR_INVALID_PARAMETER;
    }
    
    pstPara = (St_WeIDEAParameter*)handle;
    
    
    for (iIndexJ=0; iIndexJ<iLen; iIndexJ+=WE_IDEA_BLOCK_SIZE)
    {
        for (iIndex=iIndexJ; iIndex<iIndexJ+WE_IDEA_BLOCK_SIZE; iIndex++)
        {
            aucInTmp[iIndex-iIndexJ] = pucIn[iIndex];
        }
        iRes = We_LibIDEADecrypt( aucInTmp, WE_IDEA_BLOCK_SIZE, &pucOut[iIndexJ], handle);
        if (iRes != M_WE_LIB_ERR_OK)
        {
            return iRes;
        }
        for (iIndex=0; iIndex<WE_IDEA_BLOCK_SIZE; iIndex++)
        {
            pucOut[iIndexJ+iIndex] ^= ((pstPara->aucIv)[iIndex]);
        }
        
        (WE_VOID)WE_MEMCPY(pstPara->aucIv, &pucIn[iIndexJ], WE_IDEA_BLOCK_SIZE);
    }
    
    return M_WE_LIB_ERR_OK;        
}

/*==================================================================================================
FUNCTION: 
    We_LibIDEADecrypt
CREATE DATE:
    2006-11-23
AUTHOR:
    stone An
DESCRIPTION:
    Decryt the input data which is one WE_IDEA_BLOCK_SIZE with IDEA algorthm.
ARGUMENTS PASSED:
    const WE_UCHAR* pucIn[IN]: the data need to be decrypted.
    WE_UINT32       uiInLen[IN]: length of input data.
    WE_UCHAR*       pucOut[OUT]: the decrypted data.
    WE_HANDLE       Handle[IN]: the handle of IDEA algorithm.
RETURN VALUE:
    M_WE_LIB_ERR_OK: success.
    other : fail.
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    omit.
IMPORTANT NOTES:
    None
==================================================================================================*/
static WE_INT32 We_LibIDEADecrypt
(
    const WE_UCHAR* pucIn, 
    WE_UINT32 uiInLen, 
    WE_UCHAR* pucOut, 
    WE_HANDLE handle
)
{
    return We_LibIDEAEncrypt( pucIn, uiInLen, pucOut, handle);
}

/*==================================================================================================
FUNCTION: 
    We_LibIDEAKeyRotLeftMove
CREATE DATE:
    2006-11-23
AUTHOR:
    stone An
DESCRIPTION:
    rotate left 25 bits.
ARGUMENTS PASSED:
    WE_UCHAR *pucKey[IN/OUT]: the secret need to rotate and the rotated result.
RETURN VALUE:
    M_WE_LIB_ERR_OK: success.
    other : fail.
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    omit.
IMPORTANT NOTES:
    None
==================================================================================================*/
static WE_INT32 We_LibIDEAKeyRotLeftMove(WE_UCHAR *pucKey)
{ 
    WE_INT32 iIndexI = 0;
    WE_INT32 iIndexJ = 0;
    WE_UCHAR ucCy = 0;
    WE_UCHAR ucFirstCy = 0;
    WE_UCHAR ucTmp = 0;

    if (NULL == pucKey)
    {
        return M_WE_LIB_ERR_INVALID_PARAMETER;
    }

    ucFirstCy = (pucKey[0] >> 7) & 0x01;

    /* rotate one bit. */
    for (iIndexI=0; iIndexI<15; iIndexI++)
    {
        ucCy = (pucKey[iIndexI+1] >> 7) & 0x01;
        pucKey[iIndexI] = (WE_UINT8)((pucKey[iIndexI]<<1) | (ucCy));
    }
    pucKey[15] = (WE_UINT8)((pucKey[15]<<1) | (ucFirstCy));

    /* rotate 3*8 bits. */
    for (iIndexI=0; iIndexI<3; iIndexI++)
    {
        ucTmp = pucKey[0];
        for (iIndexJ=0; iIndexJ<15; iIndexJ++)
        {
            pucKey[iIndexJ] = pucKey[iIndexJ+1];
        }
        pucKey[15] = ucTmp;
    }
    
    return M_WE_LIB_ERR_OK;
}

/*==================================================================================================
FUNCTION: 
    We_LibIDEAHandle
CREATE DATE:
    2006-11-23
AUTHOR:
    stone An
DESCRIPTION:
    encrypt or decrypt data using secret.
ARGUMENTS PASSED:
    WE_UINT16 *pusData[IN/OUT]: the data to be encrypted or decrypted when IN.
                                the encrypted or decrypted data when OUT.
    WE_UINT16 *pusSecret[IN]: the secret used for handling.
RETURN VALUE:
    M_WE_LIB_ERR_OK: success.
    other : fail.
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    omit.
IMPORTANT NOTES:
    None
==================================================================================================*/
static WE_INT32 We_LibIDEAHandle(WE_UINT16 *pusData1, WE_UINT16 *pusSecret)
{
    WE_UINT16 usD1 = 0;
    WE_UINT16 usD2 = 0;
    WE_UINT16 usD3 = 0;
    WE_UINT16 usD4 = 0;
    
    WE_UINT16 usD57 = 0;
    WE_UINT16 usD68 = 0;
    WE_UINT16 *pusData = pusData1;
    
    if ((NULL == pusSecret) || (NULL == pusData))
    {
        return M_WE_LIB_ERR_INVALID_PARAMETER;
    } 

    usD1 = pusData[0];
    usD2 = pusData[1];
    usD3 = pusData[2];
    usD4 = pusData[3];

    usD1 = We_LibIDEAMul(usD1, pusSecret[0]);
    usD2 += pusSecret[1];
    usD3 += pusSecret[2];
    usD4 = We_LibIDEAMul(usD4, pusSecret[3]);
    usD57 = usD1 ^ usD3;
    usD68 = usD2 ^ usD4;
    usD57 = We_LibIDEAMul(usD57, pusSecret[4]);
    usD68 += usD57;
    usD68 = We_LibIDEAMul(usD68, pusSecret[5]);
    usD57 += usD68;
    usD1 ^= usD68;
    usD3 ^= usD68;
    usD2 ^= usD57;
    usD4 ^= usD57;
    
    pusData[0] = usD1;
    pusData[1] = usD3; /* change 1 and 2. */
    pusData[2] = usD2;
    pusData[3] = usD4;
    
    return M_WE_LIB_ERR_OK;
}

/*==================================================================================================
FUNCTION: 
    We_LibIDEAMul
CREATE DATE:
    2006-11-23
AUTHOR:
    stone An
DESCRIPTION:
    multiply mod 65537.
ARGUMENTS PASSED:
    WE_UINT16 usA: one factor.
    WE_UINT16 usB: another factor.
RETURN VALUE:
    result of multiply.
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    omit.
IMPORTANT NOTES:
    None
==================================================================================================*/
static WE_UINT16 We_LibIDEAMul(WE_UINT16 usA, WE_UINT16 usB)
{
    WE_UINT32 uiRes = 0;

    if ((0 == usA) && (0 == usB))
    {
        uiRes = 0;
    }
    else if (0 == usA)
    {
        uiRes = (65536) * (WE_UINT32)usB; 
    }
    else if (0 == usB)
    {
        uiRes = (65536) * (WE_UINT32)usA; 
    }
    else
    {
        uiRes = (WE_UINT32)usA * (WE_UINT32)usB;
    }

    return (WE_UINT16)(uiRes % (65537));
}

#define LOW16(x) ((x) & 0xffff)
/*==================================================================================================
FUNCTION: 
    We_LibIDEAMulInv
CREATE DATE:
    2006-11-23
AUTHOR:
    stone An
DESCRIPTION:
    reverse of multiply mod 65537.
ARGUMENTS PASSED:
    WE_UINT16 usX: one factor.
RETURN VALUE:
    result of reverse of multiply.
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    omit.
IMPORTANT NOTES:
    None
==================================================================================================*/
static WE_UINT16 We_LibIDEAMulInv(WE_UINT16 usX)
{
    WE_UINT16 usT0 = 0;
    WE_UINT16 usT1 = 0;    
    WE_UINT16 usQ = 0;
    WE_UINT16 usY = 0;
    
    if (usX <= 1)        
    {       
        return usX; 
    }
    
    usT1 = (WE_UINT16)(0x10001L / usX);    
    usY = (WE_UINT16)(0x10001L % usX);    
    if (usY == 1)        
    {        
        return LOW16(1-usT1);        
    }
    
    usT0 = 1 ;    
    do    
    {        
        usQ = usX / usY; 
        usX %= usY;        
        usT0 += (WE_UINT16)(usQ * usT1);        
        if( usX == 1)            
        {            
            return usT0;            
        }        
        usQ = usY / usX;        
        usY %= usX;        
        usT1 += (WE_UINT16)(usQ * usT0);        
    }while(usY != 1);
    
    return LOW16(1-usT1);    
}



















































































