/*=====================================================================================
    FILE NAME   : we_hash.c
    MODULE NAME : WE

    GENERAL DESCRIPTION
        This file define several functions to realize the MD2 algorithm.

    TechFaith Software Confidential Proprietary
    Copyright (c) 2006-2007 by TechFaith Software. All Rights Reserved.
=======================================================================================
    Revision History

    Modification                   Tracking
    Date              Author       Number       Description of changes
    ----------   --------------   ---------   --------------------------------------
    2006-11-03         Tang         None          Init

=====================================================================================*/

/*=====================================================================================
*   Include File Section
*=====================================================================================*/
#include "we_def.h"
#include "we_libalg.h"
#include "we_mem.h"
#include "we_hash.h"   
             
/*******************************************************************************
*   md2
*******************************************************************************/

/*******************************************************************************
*   Type Define Section
*******************************************************************************/
typedef struct tagSt_WeHashMD2Context
{
    WE_UINT8    aucCheckSum[16]; 
    WE_UINT8    aucState[16];    
    WE_UINT8    aucTrailing[16]; 
    WE_UINT8    ucTlen;         
    WE_UINT8    ucL;            
} St_WeHashMD2Context;

/*******************************************************************************
*   Prototype Declare Section
*******************************************************************************/
static WE_INT32 We_LibHashUpdataMD2
(
    WE_UINT8            *pucData, 
    St_WeHashMD2Context *pstHashMD2Ctx
);

static WE_INT32 We_LibMD2UpdateChecksum
(
    WE_UINT8            *pucData, 
    St_WeHashMD2Context *pstHashMD2Ctx
);

static WE_INT32 We_LibMD2UpdateState
(
    WE_UINT8            *pucData, 
    St_WeHashMD2Context *pstHashMD2Ctx
);

static St_WeLibHashDigest We_LibHashOutputMD2( WE_VOID *pvHashID );

/*******************************************************************************
*   File Static Variable Define Section
*******************************************************************************/
static WE_UINT8 aucPerm[256] = 
{
    41, 46, 67, 201, 162, 216, 124, 1, 61, 54, 84, 161, 236, 240, 6,
    19, 98, 167, 5, 243, 192, 199, 115, 140, 152, 147, 43, 217, 188,
    76, 130, 202, 30, 155, 87, 60, 253, 212, 224, 22, 103, 66, 111, 24,
    138, 23, 229, 18, 190, 78, 196, 214, 218, 158, 222, 73, 160, 251,
    245, 142, 187, 47, 238, 122, 169, 104, 121, 145, 21, 178, 7, 63,
    148, 194, 16, 137, 11, 34, 95, 33, 128, 127, 93, 154, 90, 144, 50,
    39, 53, 62, 204, 231, 191, 247, 151, 3, 255, 25, 48, 179, 72, 165,
    181, 209, 215, 94, 146, 42, 172, 86, 170, 198, 79, 184, 56, 210,
    150, 164, 125, 182, 118, 252, 107, 226, 156, 116, 4, 241, 69, 157,
    112, 89, 100, 113, 135, 32, 134, 91, 207, 101, 230, 45, 168, 2, 27,
    96, 37, 173, 174, 176, 185, 246, 28, 70, 97, 105, 52, 64, 126, 15,
    85, 71, 163, 35, 221, 81, 175, 58, 195, 92, 249, 206, 186, 197,
    234, 38, 44, 83, 13, 110, 133, 40, 132, 9, 211, 223, 205, 244, 65,
    129, 77, 82, 106, 220, 55, 200, 108, 193, 171, 250, 36, 225, 123,
    8, 12, 189, 177, 74, 120, 136, 149, 139, 227, 99, 232, 109, 233,
    203, 213, 254, 59, 0, 29, 57, 242, 239, 183, 14, 102, 88, 208, 228,
    166, 119, 114, 248, 235, 117, 75, 10, 49, 68, 80, 180, 143, 237,
    31, 26, 219, 153, 141, 51, 159, 17, 131, 20
};
  
/*******************************************************************************
*   Function Define Section
*******************************************************************************/
/*=====================================================================================
FUNCTION: 
    We_LibHashStartMd2
CREATE DATE: 
    2006-12-04
AUTHOR: 
    Sam
DESCRIPTION:
    Hash init with MD2 algorithm type.
ARGUMENTS PASSED:
    uiSize[IN]: ;
    pvModulus[IN]: ;
    pstKey[IN]: ;
    hHandle[OUT]: data handle.
RETURN VALUE:
    M_WE_LIB_ERR_OK if success,others error code.
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    Omit
IMPORTANT NOTES:
====================================================================================*/
WE_UINT32 We_LibHashStartMd2(WE_HANDLE *hHandle)
{
    St_WeHashMD2Context *pstNew = NULL;

    CHECK_FOR_NULL_ARG(hHandle, 0x08);
    
    pstNew = WE_MALLOC(sizeof(St_WeHashMD2Context));
    if(!pstNew)
    {
        return 0x04;
    }
    (WE_VOID)WE_MEMSET(pstNew->aucCheckSum, 0, 16);
    (WE_VOID)WE_MEMSET(pstNew->aucState,    0, 16);
    (WE_VOID)WE_MEMSET(pstNew->aucTrailing, 0, 16);
    pstNew->ucTlen = 0;
    pstNew->ucL    = 0;

    (*hHandle) = (WE_HANDLE)pstNew;
    return 0x00;
}

/*=====================================================================================
FUNCTION: 
    We_LibHashAppendMd2
CREATE DATE: 
    2006-12-04
AUTHOR: 
    Sam
DESCRIPTION:
    Hashed data with MD2 algorithm type.
ARGUMENTS PASSED:
    pucData[IN]: data to be hashed;
    uiNbits[IN]: length in bit(8*bytes);
    pvEid[IN]: data handle.
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
WE_UINT32 We_LibHashAppendMd2
(
    WE_UINT8    *pucData, 
    WE_UINT32   uiNbits, 
    WE_VOID     *pvEid
)
{
    St_WeHashMD2Context   *pstM = (St_WeHashMD2Context *)pvEid;
    WE_UINT32   uiLen = (WE_UINT32) (uiNbits >> 3);

    if ((NULL == pstM) || (NULL == pucData))
    {
        return 0x08;
    }
    
    if (uiLen + pstM->ucTlen < 16)
    {
        (WE_VOID)WE_MEMCPY (pstM->aucTrailing+pstM->ucTlen, pucData, uiLen);
        pstM->ucTlen = (WE_UINT8) (pstM->ucTlen + uiLen);  
        return 0x00;
    }
    (WE_VOID)WE_MEMCPY (pstM->aucTrailing+pstM->ucTlen, pucData, 16-pstM->ucTlen);
    pucData+=(16-pstM->ucTlen);
    uiLen -=(16-pstM->ucTlen);    
    
    (WE_VOID)We_LibHashUpdataMD2(pstM->aucTrailing, pstM);    
    
    while(uiLen > 15)
    {
        (WE_VOID)We_LibHashUpdataMD2(pucData, pstM);
        pucData += 16;
        uiLen -= 16;
    }
    
    (WE_VOID)WE_MEMCPY(pstM->aucTrailing, pucData, uiLen);
    pstM->ucTlen = (WE_UINT8) uiLen;
    return 0x00;
}

/*=====================================================================================
FUNCTION: 
    We_LibHashFinishMd2
CREATE DATE: 
    2006-12-04
AUTHOR: 
    Sam
DESCRIPTION:
    Get the hash value with MD2 algorithm type.
ARGUMENTS PASSED:
    pvEid[IN]: data handle.
    pstHashDigest[OUT]: hash value.
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
WE_UINT32 We_LibHashFinishMd2(WE_VOID *pvEid, St_WeLibHashDigest *pstHashDigest)
{
    St_WeLibHashDigest  stRval={{0},0};

    CHECK_FOR_NULL_ARG(pvEid, 0x08);
    CHECK_FOR_NULL_ARG(pstHashDigest, 0x08);

    stRval = We_LibHashOutputMD2(pvEid);
    (*pstHashDigest) = stRval;
    
    WE_LIB_FREE(pvEid);
    return 0x00;
}

/*=====================================================================================
FUNCTION: 
    We_LibHashOutputMD2
CREATE DATE: 
    2006-12-04
AUTHOR: 
    Sam
DESCRIPTION:
    Out put the hashed data and free memory.
ARGUMENTS PASSED:
    pvEid[IN]: data handle.
RETURN VALUE:
    Structure of hash value.
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    Omit
IMPORTANT NOTES:
=====================================================================================*/
static St_WeLibHashDigest We_LibHashOutputMD2(WE_VOID * pvEid)
{
    St_WeHashMD2Context *pstM = NULL;
    St_WeLibHashDigest stRval = {{0},0};

    if (NULL == pvEid)
    {
        return stRval;
    }
    pstM = (St_WeHashMD2Context *)pvEid;
    
    stRval.uiHashSize = 16;    
    (WE_VOID)WE_MEMSET(pstM->aucTrailing+pstM->ucTlen, 16-pstM->ucTlen, 16-pstM->ucTlen);    
    (WE_VOID)We_LibHashUpdataMD2(pstM->aucTrailing, pstM);    
    (WE_VOID)We_LibMD2UpdateState(pstM->aucCheckSum, pstM);    
    (WE_VOID)WE_MEMCPY(stRval.aucHashValue, pstM->aucState, 16);    
    /* reset. */
    (WE_VOID)WE_MEMSET(pstM->aucCheckSum, 0, 16);
    (WE_VOID)WE_MEMSET(pstM->aucState, 0, 16);
    (WE_VOID)WE_MEMSET(pstM->aucTrailing, 0, 16);
    pstM->ucTlen = 0;
    pstM->ucL = 0;  
    
    return stRval;
}

/*=====================================================================================
FUNCTION: 
    We_LibHashUpdataMD2
CREATE DATE: 
    2006-12-04
AUTHOR: 
    Sam
DESCRIPTION:
    Update the checksum and state.
ARGUMENTS PASSED:
    pucIn[IN]: trailing data;
    pstM[IN]: MD2 context;
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
static WE_INT32 We_LibHashUpdataMD2(WE_UINT8 *pucIn, St_WeHashMD2Context *pstM)
{
    (WE_VOID)We_LibMD2UpdateChecksum(pucIn, pstM);
    (WE_VOID)We_LibMD2UpdateState(pucIn, pstM);
    return 0x00;
}


/*=====================================================================================
FUNCTION: 
    We_LibMD2UpdateChecksum
CREATE DATE: 
    2006-12-04
AUTHOR: 
    Sam
DESCRIPTION:
    Updata the checksum.
ARGUMENTS PASSED:
    pucIn[IN]: trailing data;
    pstM[IN]: MD2 context;
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
static WE_INT32 We_LibMD2UpdateChecksum(WE_UINT8 *pucin, St_WeHashMD2Context *pstM)
{
    WE_UINT32 iIndex=0;

    if (NULL == pstM||!pucin)
    {
        return M_WE_LIB_ERR_INVALID_PARAMETER;
    }
    
    for(iIndex = 0; iIndex < 16; iIndex++)
    {
        pstM->aucCheckSum[iIndex] ^= aucPerm[*(pucin+iIndex) ^ (pstM->ucL)];
        pstM->ucL = pstM->aucCheckSum[iIndex];
    }
    return 0x00;
}

/*=====================================================================================
FUNCTION: 
    We_LibMD2UpdateState
CREATE DATE: 
    2006-12-04
AUTHOR: 
    Sam
DESCRIPTION:
    Update the state.
ARGUMENTS PASSED:
    pucIn[IN]: trailing data;
    pstM[IN]: MD2 context;
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
static WE_INT32 We_LibMD2UpdateState(WE_UINT8 *pucIn, St_WeHashMD2Context *pstM)
{ 
    WE_UINT32   uiJ=0;     
    WE_UINT32   uiK=0;     
    WE_UINT8    ucT = 0; 
    WE_UINT8    aucX[48]; 

    CHECK_FOR_NULL_ARG(pucIn, 0x08);
    
    (WE_VOID)WE_MEMCPY(aucX, pstM->aucState, 16);
    (WE_VOID)WE_MEMCPY(aucX+16, pucIn, 16);
    for (uiJ=0; uiJ<16; uiJ++)
    {
        aucX[uiJ+32] = (WE_UINT8) (((pstM->aucState[uiJ]) ^ (pucIn[uiJ])));
    }
    
    for (uiJ = 0; uiJ < 18; uiJ++)
    {
        for (uiK = 0; uiK < 48; uiK++)
        {
            ucT = (WE_UINT8) (aucX[uiK] ^ aucPerm[ucT]);
            aucX[uiK] = ucT;
        }
        ucT = ((ucT+(WE_UINT8)uiJ) & 255);
    }
    
    (WE_VOID)WE_MEMCPY(pstM->aucState, aucX, 16);
    return 0x00;
}


/*md5*/

/*******************************************************************************
*   Macro Define Section
*******************************************************************************/         
#define F(x,y,z) (((x) & (y)) | (~(x) & (z)))
#define G(x,y,z) (((x) & (z)) | ((y) & ~(z)))
#define H(x,y,z) ((x) ^ (y) ^ (z))
#define I(x,y,z) ((y) ^ ((x) | ~(z)))

#define R(a,s) ((a << (s)) + (a >> (32 - (s))))

#define FF(a,b,c,d,m,s,t) a += F(b,c,d) + m + t; a = b + R(a,s);
#define GG(a,b,c,d,m,s,t) a += G(b,c,d) + m + t; a = b + R(a,s);
#define HH(a,b,c,d,m,s,t) a += H(b,c,d) + m + t; a = b + R(a,s);
#define II(a,b,c,d,m,s,t) a += I(b,c,d) + m + t; a = b + R(a,s);


/*******************************************************************************
*   Type Define Section
*******************************************************************************/
typedef struct tagSt_WeLibMD5Context
{
    WE_UINT32   uiDigest[4];
    WE_UINT32   uiMessage[16];
    WE_UINT32   uiSofar;
    WE_UINT32   uiNBits;
} St_WeLibMD5Context;


/*******************************************************************************
*   Function prototype Section
*******************************************************************************/
WE_VOID We_LibHashOutputToMd5
(
    St_WeLibHashDigest *pstHashDigest, 
    WE_VOID* pvHashID 
);


/*******************************************************************************
*   Function Define Section
*******************************************************************************/
/*=====================================================================================
FUNCTION: 
    We_LibMD5Compress
CREATE DATE: 
    2006-12-04
AUTHOR: 
    Sam
DESCRIPTION:
    MD5 basic transformation, Transforms state based on block.
ARGUMENTS PASSED:
    puiDigest[IN]: state;
    puiMessage[IN]: message block;
RETURN VALUE:
    None.
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    Omit
IMPORTANT NOTES:
=====================================================================================*/
static WE_VOID We_LibMD5Compress( WE_UINT32 *puiDigest, WE_UINT32 *puiMessage )
{
    WE_UINT32 a, b, c, d;

    if (!(puiDigest)) 
    {
        return;
    }
    if (!(puiMessage)) 
    {
        return;
    }

    a = puiDigest[0];
    b = puiDigest[1];
    c = puiDigest[2];
    d = puiDigest[3];

    FF(a,b,c,d,puiMessage[ 0], 7, 0xd76aa478)
    FF(d,a,b,c,puiMessage[ 1],12, 0xe8c7b756)
    FF(c,d,a,b,puiMessage[ 2],17, 0x242070db)
    FF(b,c,d,a,puiMessage[ 3],22, 0xc1bdceee)
    FF(a,b,c,d,puiMessage[ 4], 7, 0xf57c0faf)
    FF(d,a,b,c,puiMessage[ 5],12, 0x4787c62a)
    FF(c,d,a,b,puiMessage[ 6],17, 0xa8304613)
    FF(b,c,d,a,puiMessage[ 7],22, 0xfd469501)
    FF(a,b,c,d,puiMessage[ 8], 7, 0x698098d8)
    FF(d,a,b,c,puiMessage[ 9],12, 0x8b44f7af)
    FF(c,d,a,b,puiMessage[10],17, 0xffff5bb1)
    FF(b,c,d,a,puiMessage[11],22, 0x895cd7be)
    FF(a,b,c,d,puiMessage[12], 7, 0x6b901122)
    FF(d,a,b,c,puiMessage[13],12, 0xfd987193)
    FF(c,d,a,b,puiMessage[14],17, 0xa679438e)
    FF(b,c,d,a,puiMessage[15],22, 0x49b40821)

    GG(a,b,c,d,puiMessage[ 1], 5, 0xf61e2562)
    GG(d,a,b,c,puiMessage[ 6], 9, 0xc040b340)
    GG(c,d,a,b,puiMessage[11],14, 0x265e5a51)
    GG(b,c,d,a,puiMessage[ 0],20, 0xe9b6c7aa)
    GG(a,b,c,d,puiMessage[ 5], 5, 0xd62f105d)
    GG(d,a,b,c,puiMessage[10], 9, 0x02441453)
    GG(c,d,a,b,puiMessage[15],14, 0xd8a1e681)
    GG(b,c,d,a,puiMessage[ 4],20, 0xe7d3fbc8)
    GG(a,b,c,d,puiMessage[ 9], 5, 0x21e1cde6)
    GG(d,a,b,c,puiMessage[14], 9, 0xc33707d6)
    GG(c,d,a,b,puiMessage[ 3],14, 0xf4d50d87)
    GG(b,c,d,a,puiMessage[ 8],20, 0x455a14ed)
    GG(a,b,c,d,puiMessage[13], 5, 0xa9e3e905)
    GG(d,a,b,c,puiMessage[ 2], 9, 0xfcefa3f8)
    GG(c,d,a,b,puiMessage[ 7],14, 0x676f02d9)
    GG(b,c,d,a,puiMessage[12],20, 0x8d2a4c8a)

    HH(a,b,c,d,puiMessage[ 5], 4, 0xfffa3942)
    HH(d,a,b,c,puiMessage[ 8],11, 0x8771f681)
    HH(c,d,a,b,puiMessage[11],16, 0x6d9d6122)
    HH(b,c,d,a,puiMessage[14],23, 0xfde5380c)
    HH(a,b,c,d,puiMessage[ 1], 4, 0xa4beea44)
    HH(d,a,b,c,puiMessage[ 4],11, 0x4bdecfa9)
    HH(c,d,a,b,puiMessage[ 7],16, 0xf6bb4b60)
    HH(b,c,d,a,puiMessage[10],23, 0xbebfbc70)
    HH(a,b,c,d,puiMessage[13], 4, 0x289b7ec6)
    HH(d,a,b,c,puiMessage[ 0],11, 0xeaa127fa)
    HH(c,d,a,b,puiMessage[ 3],16, 0xd4ef3085)
    HH(b,c,d,a,puiMessage[ 6],23, 0x04881d05)
    HH(a,b,c,d,puiMessage[ 9], 4, 0xd9d4d039)
    HH(d,a,b,c,puiMessage[12],11, 0xe6db99e5)
    HH(c,d,a,b,puiMessage[15],16, 0x1fa27cf8)
    HH(b,c,d,a,puiMessage[ 2],23, 0xc4ac5665)

    II(a,b,c,d,puiMessage[ 0], 6, 0xf4292244)
    II(d,a,b,c,puiMessage[ 7],10, 0x432aff97)
    II(c,d,a,b,puiMessage[14],15, 0xab9423a7)
    II(b,c,d,a,puiMessage[ 5],21, 0xfc93a039)
    II(a,b,c,d,puiMessage[12], 6, 0x655b59c3)
    II(d,a,b,c,puiMessage[ 3],10, 0x8f0ccc92)
    II(c,d,a,b,puiMessage[10],15, 0xffeff47d)
    II(b,c,d,a,puiMessage[ 1],21, 0x85845dd1)
    II(a,b,c,d,puiMessage[ 8], 6, 0x6fa87e4f)
    II(d,a,b,c,puiMessage[15],10, 0xfe2ce6e0)
    II(c,d,a,b,puiMessage[ 6],15, 0xa3014314)
    II(b,c,d,a,puiMessage[13],21, 0x4e0811a1)
    II(a,b,c,d,puiMessage[ 4], 6, 0xf7537e82)
    II(d,a,b,c,puiMessage[11],10, 0xbd3af235)
    II(c,d,a,b,puiMessage[ 2],15, 0x2ad7d2bb)
    II(b,c,d,a,puiMessage[ 9],21, 0xeb86d391)

    puiDigest[0] += a;
    puiDigest[1] += b;
    puiDigest[2] += c;
    puiDigest[3] += d;
}



/*=====================================================================================
FUNCTION: 
    We_LibHashStartMd5
CREATE DATE: 
    2006-12-04
AUTHOR: 
    Sam
DESCRIPTION:
    Hash init with MD5 algorithm type.
ARGUMENTS PASSED:
    uiSize[IN]: ;
    pvModulus[IN]: ;
    pstKey[IN]: ;
    hHandle[IN]: data handle.
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
WE_UINT32 We_LibHashStartMd5(WE_HANDLE *hHandle)
{
    WE_UINT32 uiRes = M_WE_LIB_ERR_OK;
    St_WeLibMD5Context *pstHashMD5Ctx = NULL;

    CHECK_FOR_NULL_ARG(hHandle, 0x08);

    pstHashMD5Ctx = (St_WeLibMD5Context *)WE_MALLOC(sizeof(St_WeLibMD5Context));
    if(!pstHashMD5Ctx)
    {
        return M_WE_LIB_ERR_INVALID_PARAMETER;
    }
    /* initialise */
    pstHashMD5Ctx->uiDigest[0] = 0x67452301UL;  /* MD buffer */
    pstHashMD5Ctx->uiDigest[1] = 0xefcdab89UL;
    pstHashMD5Ctx->uiDigest[2] = 0x98badcfeUL;
    pstHashMD5Ctx->uiDigest[3] = 0x10325476UL;
    pstHashMD5Ctx->uiSofar = 0UL;
    pstHashMD5Ctx->uiNBits = 0U;

    (*hHandle) = (WE_HANDLE)pstHashMD5Ctx;
    return uiRes; 
}
/*=====================================================================================
FUNCTION: 
    We_LibHashAppendMd5
CREATE DATE: 
    2006-12-04
AUTHOR: 
    Sam
DESCRIPTION:
    Hashed data with MD5 algorithm type.
ARGUMENTS PASSED:
    pucMessage[IN]: data to be hashed;
    uiNBits[IN]: length of data in bit(8*bytes);
    pvHashID[IN]: data handle.
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
#define AP(id,index,byte) switch((index) & 3) {\
  case 0:\
    (id)->uiMessage[(index) >> 2] = (byte);\
    break;\
  case 1:\
    (id)->uiMessage[(index) >> 2] += (byte) << 8;\
    break;\
  case 2:\
    (id)->uiMessage[(index) >> 2] += (byte) << 16;\
    break;\
  case 3:\
    (id)->uiMessage[(index) >> 2] += (byte) << 24;\
    break;\
  default:\
    break;\
  }
WE_UINT32 We_LibHashAppendMd5
(
    WE_UINT8    *pucMessage, 
    WE_UINT32   uiNBits, 
    WE_VOID     *pvHashID
)
{
    St_WeLibMD5Context *pstHashMD5Ctx = NULL;
    WE_UINT32   uiByteCount = 0;
    WE_UINT32   uiIndex = 0;
    WE_UINT32   uiBytes = 0;  

    if (!pvHashID||!pucMessage)
    {
        return M_WE_LIB_ERR_INVALID_PARAMETER;
    }
    if (uiNBits == 0)
    {
        return 0x00;
    }
    pstHashMD5Ctx = (St_WeLibMD5Context *)pvHashID;
    uiByteCount = pstHashMD5Ctx->uiNBits >> 3;
    uiBytes = uiNBits >> 3; /* transform to byte length. */

    for(uiIndex = 0; uiIndex < uiBytes; uiIndex++) 
    {
        AP(pstHashMD5Ctx,uiByteCount,(WE_UINT32) pucMessage[uiIndex])
        if(++uiByteCount == 64) 
        {
            /* compress the data with the unit of 64 bytes. */
            We_LibMD5Compress(pstHashMD5Ctx->uiDigest, pstHashMD5Ctx->uiMessage);
            pstHashMD5Ctx->uiSofar += 512UL;    /* flag of bit length. */
            uiByteCount = 0;
        }
    }
    /* record of data length. */
    pstHashMD5Ctx->uiNBits = uiByteCount << 3;
    return 0;
}
/*=====================================================================================
FUNCTION: 
    We_LibHashFinishMd5
CREATE DATE: 
    2006-12-04
AUTHOR: 
    Sam
DESCRIPTION:
    Get the hash value with MD5 algorithm type.
ARGUMENTS PASSED:
    pstHashDigest[IN]: hash value;
    pvHashID[IN]: data handle.
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
WE_UINT32 We_LibHashFinishMd5( WE_VOID* pvHashID, St_WeLibHashDigest *pstHashDigest)
{
    St_WeLibHashDigest  stOutHashDigest = {{0},0};
    
    if (!(pvHashID) || (!pstHashDigest)) 
    {
        return M_WE_LIB_ERR_INVALID_PARAMETER;
    }
    We_LibHashOutputToMd5(&stOutHashDigest, pvHashID);

    (*pstHashDigest) = stOutHashDigest;
    WE_LIB_FREE(pvHashID);
    return 0;
}
/*=====================================================================================
FUNCTION: 
    We_LibHashOutputToMd5
CREATE DATE: 
    2006-12-04
AUTHOR: 
    Sam
DESCRIPTION:
    Encode and output.
ARGUMENTS PASSED:
    pucDigest[IN]: state;
    pvHashID[IN]: data handle.
RETURN VALUE:
    None.
USED GLOBAL VARIABLES:
    None
USED STATIC VARIABLES:
    None
CALL BY:
    Omit
IMPORTANT NOTES:
=====================================================================================*/
void We_LibHashOutputToMd5( St_WeLibHashDigest *pstHashDigest, WE_VOID* pvHashID )
{
    St_WeLibMD5Context *pstHashMD5Ctx = NULL;
    WE_UINT32   uiByteCount = 0;
    WE_UINT32   uiIndex;

    if (!(pvHashID)||!pstHashDigest) 
    {
        return;
    }
    pstHashMD5Ctx = (St_WeLibMD5Context *)pvHashID;
    uiByteCount = pstHashMD5Ctx->uiNBits >> 3;

    pstHashMD5Ctx->uiSofar += (WE_UINT32) pstHashMD5Ctx->uiNBits;
    AP(pstHashMD5Ctx,uiByteCount,128UL) /* a single "1" bit and seven "0" bits are padded to the message. */
    uiByteCount++;

    if(uiByteCount > 56) 
    {
        if(uiByteCount <= 60)
        {
            pstHashMD5Ctx->uiMessage[15] = 0UL;
        }
        We_LibMD5Compress(pstHashMD5Ctx->uiDigest, pstHashMD5Ctx->uiMessage);
        
        for(uiIndex = 0; uiIndex < 14; uiIndex++)
        {
            pstHashMD5Ctx->uiMessage[uiIndex] = 0UL;
        }
    } 
    else
    {
        for(uiIndex = (uiByteCount + 3) >> 2; uiIndex < 14; uiIndex++)
        {
            /* multiple "0" bits are appended so that the length in bits of
            * the padded message becomes congruent to 448, modulo 512.*/
            pstHashMD5Ctx->uiMessage[uiIndex] = 0UL;
        }
    }
    /* pad the last eight bytes. */
    pstHashMD5Ctx->uiMessage[14] = pstHashMD5Ctx->uiSofar;
    pstHashMD5Ctx->uiMessage[15] = 0UL;
    We_LibMD5Compress(pstHashMD5Ctx->uiDigest, pstHashMD5Ctx->uiMessage);

    for(uiIndex = 0; uiIndex < 4; uiIndex++) 
    {
        /* get the digest value. */
        WE_UINT32 uiTmp = pstHashMD5Ctx->uiDigest[uiIndex];
        WE_UINT32 dind = uiIndex << 2;

        pstHashDigest->aucHashValue[dind++] = (WE_UINT8) (uiTmp & 255);
        pstHashDigest->aucHashValue[dind++] = (WE_UINT8) ((uiTmp >>= 8) & 255);
        pstHashDigest->aucHashValue[dind++] = (WE_UINT8) ((uiTmp >>= 8) & 255);
        pstHashDigest->aucHashValue[dind] = (WE_UINT8) ((uiTmp >>= 8) & 255);
    }
    pstHashDigest->uiHashSize = uiIndex << 2;
    /* reset MD buffer. */
    pstHashMD5Ctx->uiDigest[0] = 0x67452301UL;
    pstHashMD5Ctx->uiDigest[1] = 0xefcdab89UL;
    pstHashMD5Ctx->uiDigest[2] = 0x98badcfeUL;
    pstHashMD5Ctx->uiDigest[3] = 0x10325476UL;
    pstHashMD5Ctx->uiSofar = 0UL;
    pstHashMD5Ctx->uiNBits = 0U;
}




#undef F
#undef G
#undef H
#undef I
#undef R
#undef FF
#undef GG
#undef HH
#undef II
#undef AP


/***********************************************sha*********************************/
/*=====================================================================================
    FILE   NAME : we_sha.c
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
    2006-11-03         Sam         None          Init

=====================================================================================*/


/*******************************************************************************
*   Macro Define Section
*******************************************************************************/          
#define F(x,y,z) (((x) & (y)) | (~(x) & (z)))
#define G(x,y,z) ((x) ^ (y) ^ (z))
#define H(x,y,z) (((x) & (y)) | ((x) & (z)) | ((y) & (z)))
#define I(x,y,z) ((x) ^ (y) ^ (z))

#define R(a,s) ((a << (s)) + (a >> (32 - (s))))


/*******************************************************************************
*   Type Define Section
*******************************************************************************/
typedef struct tagSt_WeHashSHA1Context
{
    WE_UINT32   uiDigest[5];
    WE_UINT32   uiMessage[80];
    WE_UINT32   uiSofar;
    WE_UINT32   uiNBits;
} St_WeHashSHA1Ctx;


/*******************************************************************************
*   Prototype Declare Section
*******************************************************************************/
static WE_VOID We_LibHashOutputToSha( St_WeLibHashDigest *pstHashDigest, WE_VOID* pvHashID );

/*******************************************************************************
*   Function Define Section
*******************************************************************************/
/*=====================================================================================
FUNCTION: 
    We_LibSHACompress
CREATE DATE: 
    2006-12-04
AUTHOR: 
    Sam
DESCRIPTION:
    Process the message stored in the message block array.
ARGUMENTS PASSED:
    puiDigest[IN]: state;
    puiMessage[IN]: message block;
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
#define NW(j) t = puiMessage[j - 3] ^ puiMessage[j - 8] ^ puiMessage[j - 14] ^ puiMessage[j - 16]; puiMessage[j] = R(t,1)

WE_VOID We_LibSHACompress( WE_UINT32 *puiDigest, WE_UINT32 *puiMessage )
{
    WE_UINT32 a, b, c, d, e;
    WE_UINT32 t;
    WE_INT32  i; 

    if (!(puiDigest))
    {
        return;
    }

    if (!(puiMessage)) 
    {
        return;
    }

    a = puiDigest[0];
    b = puiDigest[1];
    c = puiDigest[2];
    d = puiDigest[3];
    e = puiDigest[4];

    for(i = 0; i < 16; i++) 
    {
        t = R(a,5) + F(b,c,d) + e + puiMessage[i] + 0x5a827999;
        e = d;
        d = c;
        c = R(b,30);
        b = a;
        a = t;
    }

    for(i = 16; i < 20; i++) 
    {
        NW(i); 
        t = R(a,5) + F(b,c,d) + e + puiMessage[i] + 0x5a827999; 
        e = d;
        d = c; 
        c = R(b,30); 
        b = a; 
        a = t;
    }

    for(i = 20; i < 40; i++) 
    {
        NW(i); 
        t = R(a,5) + G(b,c,d) + e + puiMessage[i] + 0x6ed9eba1; 
        e = d; 
        d = c; 
        c = R(b,30); 
        b = a; 
        a = t;
    }

    for(i = 40; i < 60; i++) 
    {
        NW(i); 
        t = R(a,5) + H(b,c,d) + e + puiMessage[i] + 0x8f1bbcdc; 
        e = d; 
        d = c; 
        c = R(b,30); 
        b = a; 
        a = t;
    }

    for(i = 60; i < 80; i++) 
    {
        NW(i); 
        t = R(a,5) + I(b,c,d) + e + puiMessage[i] + 0xca62c1d6; 
        e = d; 
        d = c; 
        c = R(b,30); 
        b = a; 
        a = t;
    }

    puiDigest[0] += a;
    puiDigest[1] += b;
    puiDigest[2] += c;
    puiDigest[3] += d;
    puiDigest[4] += e;
}
#undef NW

/*=====================================================================================
FUNCTION: 
    We_LibHashStartSha
CREATE DATE: 
    2006-12-04
AUTHOR: 
    Sam
DESCRIPTION:
    Set the init information.
ARGUMENTS PASSED:
    uiSize[IN]: ;
    pvModulus[IN]: ;
    pstKey[IN]: ;
    phHandle[IN]: data handle.
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
WE_UINT32 We_LibHashStartSha(WE_HANDLE *phHandle)
{
    WE_UINT32 uiRes = M_WE_LIB_ERR_OK;
    St_WeHashSHA1Ctx *pstHashSHA1Ctx = NULL;

    CHECK_FOR_NULL_ARG(phHandle, M_WE_LIB_ERR_INVALID_PARAMETER);
    
    pstHashSHA1Ctx = (St_WeHashSHA1Ctx *)WE_MALLOC(sizeof(St_WeHashSHA1Ctx));    
    if(!pstHashSHA1Ctx) 
    {
        return M_WE_LIB_ERR_INSUFFICIENT_MEMORY;
    }

    pstHashSHA1Ctx->uiDigest[0] = 0x67452301UL;
    pstHashSHA1Ctx->uiDigest[1] = 0xefcdab89UL;
    pstHashSHA1Ctx->uiDigest[2] = 0x98badcfeUL;
    pstHashSHA1Ctx->uiDigest[3] = 0x10325476UL;
    pstHashSHA1Ctx->uiDigest[4] = 0xc3d2e1f0UL;
    pstHashSHA1Ctx->uiSofar = 0UL;
    pstHashSHA1Ctx->uiNBits = 0U;

    (*phHandle) = (WE_HANDLE)pstHashSHA1Ctx;
    return uiRes;
}
/*=====================================================================================
FUNCTION: 
    We_LibHashAppendSha
CREATE DATE: 
    2006-12-04
AUTHOR: 
    Sam
DESCRIPTION:
    Hashed data with SHA1 algorithm type.
ARGUMENTS PASSED:
    pucMessage[IN]: message block to be hashed;
    uiNBits[IN]: length of message in bit(8*byte);
    pvHashID[IN]: data handle.
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
#define AP(id,index,byte) switch((index) & 3) {\
  case 0:\
    (id)->uiMessage[(index) >> 2] = (byte) << 24;\
    break;\
  case 1:\
    (id)->uiMessage[(index) >> 2] += (byte) << 16;\
    break;\
  case 2:\
    (id)->uiMessage[(index) >> 2] += (byte) << 8;\
    break;\
  case 3:\
    (id)->uiMessage[(index) >> 2] += (byte);\
    break;\
  default:\
    break;\
  }

WE_UINT32 We_LibHashAppendSha
(
    WE_UINT8    *pucMessage, 
    WE_UINT32   uiNBits, 
    WE_VOID*    pvHashID
)
{
    St_WeHashSHA1Ctx *pstHashSHA1Ctx = NULL;
    WE_UINT32   uiByteCount = 0;
    WE_UINT32   uiIndex;
    WE_UINT32   uiBytes = 0;/*/8*/  
    WE_UINT32 uiRes = M_WE_LIB_ERR_OK;

    if (!pvHashID||!pucMessage)
    {
        return M_WE_LIB_ERR_INVALID_PARAMETER;
    }
    if (uiNBits == 0)
    {
        return 0x00;
    }
    pstHashSHA1Ctx = (St_WeHashSHA1Ctx *)pvHashID;
    uiByteCount = pstHashSHA1Ctx->uiNBits >> 3;
    uiBytes = uiNBits >> 3;/* 8 */  
    
    for(uiIndex = 0; uiIndex < uiBytes; uiIndex++) 
    {
        AP(pstHashSHA1Ctx,uiByteCount,(WE_UINT32) pucMessage[(WE_UINT32) uiIndex])
        if(++uiByteCount == 64) 
        {
            We_LibSHACompress(pstHashSHA1Ctx->uiDigest, pstHashSHA1Ctx->uiMessage);
            pstHashSHA1Ctx->uiSofar += 512UL;
            uiByteCount = 0;
        }
    }
    pstHashSHA1Ctx->uiNBits = uiByteCount << 3;
    
    return uiRes; 
}
/*=====================================================================================
FUNCTION: 
    We_LibHashFinishSha
CREATE DATE: 
    2006-12-04
AUTHOR: 
    Sam
DESCRIPTION:
    Get the hash value with SHA1 algorithm type.
ARGUMENTS PASSED:
    pstHashDigest[OUT]: pointer to hash value;
    pvHashID[IN]: data handle.
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
WE_UINT32 We_LibHashFinishSha( WE_VOID* pvHashID, St_WeLibHashDigest *pstHashDigest)
{
    WE_UINT32 uiRes = M_WE_LIB_ERR_OK;
    St_WeLibHashDigest stOutHashDigest = {{0},0};

    if (!(pvHashID)||(!pstHashDigest)) 
    {
        return M_WE_LIB_ERR_INVALID_PARAMETER;
    }
    We_LibHashOutputToSha(&stOutHashDigest, pvHashID);
    
    (*pstHashDigest) = stOutHashDigest;
    WE_LIB_FREE(pvHashID);
    return uiRes;
}
/*=====================================================================================
FUNCTION: 
    We_LibHashOutputToSha
CREATE DATE: 
    2006-12-04
AUTHOR: 
    Sam
DESCRIPTION:
    Encode and out put hash result.
ARGUMENTS PASSED:
    pucDigest[IN]: state;
    pvHashID[IO]: data handle.
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
static WE_VOID We_LibHashOutputToSha( St_WeLibHashDigest *pstHashDigest, WE_VOID* pvHashID )
{
    St_WeHashSHA1Ctx *pstHashSHA1Ctx = NULL;
    WE_UINT32   uiByteCount = 0;
    WE_UINT32   uiIndex = 0;

    if (!(pvHashID)||!pstHashDigest)
    {
        return;
    }
    pstHashSHA1Ctx = (St_WeHashSHA1Ctx *)pvHashID;
    uiByteCount = pstHashSHA1Ctx->uiNBits >> 3;

    pstHashSHA1Ctx->uiSofar += (WE_UINT32) pstHashSHA1Ctx->uiNBits;
    AP(pstHashSHA1Ctx,uiByteCount,128UL)
    uiByteCount++;

    if(uiByteCount > 56) 
    {
        if(uiByteCount <= 60)
        {
            pstHashSHA1Ctx->uiMessage[15] = 0UL;
        }
        We_LibSHACompress(pstHashSHA1Ctx->uiDigest, pstHashSHA1Ctx->uiMessage);
        
        for(uiIndex = 0; uiIndex < 14; uiIndex++)
        {
            pstHashSHA1Ctx->uiMessage[uiIndex] = 0UL;
        }
    } 
    else
    {
        for(uiIndex = (uiByteCount + 3) >> 2; uiIndex < 14; uiIndex++)
        {
            pstHashSHA1Ctx->uiMessage[uiIndex] = 0UL;
        }
    }

    pstHashSHA1Ctx->uiMessage[14] = 0UL;
    pstHashSHA1Ctx->uiMessage[15] = pstHashSHA1Ctx->uiSofar;
    We_LibSHACompress(pstHashSHA1Ctx->uiDigest, pstHashSHA1Ctx->uiMessage);

    for(uiIndex = 0; uiIndex < 5; uiIndex++) 
    {
        WE_UINT32 uiTmp = pstHashSHA1Ctx->uiDigest[uiIndex];
        WE_UINT32 dind  = uiIndex << 2;

        pstHashDigest->aucHashValue[dind+3] = (WE_UINT8) (uiTmp & 255);
        pstHashDigest->aucHashValue[dind+2] = (WE_UINT8) ((uiTmp >>= 8) & 255);
        pstHashDigest->aucHashValue[dind+1] = (WE_UINT8) ((uiTmp >>= 8) & 255);
        pstHashDigest->aucHashValue[dind+0] = (WE_UINT8) ((uiTmp >>= 8) & 255);
    }
    pstHashDigest->uiHashSize = uiIndex << 2;
    pstHashSHA1Ctx->uiDigest[0] = 0x67452301UL;
    pstHashSHA1Ctx->uiDigest[1] = 0xefcdab89UL;
    pstHashSHA1Ctx->uiDigest[2] = 0x98badcfeUL;
    pstHashSHA1Ctx->uiDigest[3] = 0x10325476UL;
    pstHashSHA1Ctx->uiDigest[4] = 0xc3d2e1f0UL;
    pstHashSHA1Ctx->uiSofar = 0UL;
    pstHashSHA1Ctx->uiNBits = 0U;

}


#undef F
#undef G
#undef I
#undef H
#undef R
#undef AP

/******************************************hmac*******************************************/
/*=====================================================================================
    FILE   NAME : we_libhmac.c
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

=====================================================================================*/

/*******************************************************************************
*   Type Define Section
*******************************************************************************/
typedef struct tagSt_WeHMAC 
{
    E_WeHashAlgType   eHashType;
    WE_UINT32         (*HashStart)(WE_HANDLE *hHandle);
    WE_UINT32         (*HashAppend)(WE_UINT8 *pucData, WE_UINT32 uiNBits, WE_VOID *pvHashID);
    WE_UINT32         (*HashFinish)(WE_VOID *pvHashID, St_WeLibHashDigest *pstHashDigest);
} St_WeHMAC;

typedef struct tagSt_WeLibHmacContext
{
    St_WeHMAC stHmac;
    WE_VOID *pvHid1;  
    WE_VOID *pvHid2;  
} St_HmacId;


/*******************************************************************************
*   Function prototype Section
*******************************************************************************/
static WE_UINT32  We_LibStartHmac
( 
    St_WeHMAC  *pstHmac,
    const WE_UINT8   *pucKey, 
    WE_UINT32  uiKeyLen, 
    WE_HANDLE  *hHMACHandle
);

static WE_UINT32 We_LibAppendHmac
(
    WE_UINT8    *pucData, 
    WE_UINT32   uiNBits, 
    WE_VOID     *pvHid
);

static WE_UINT32 We_LibFinishHmac
(
    WE_VOID * pvHid, 
    St_WeLibHashDigest *pstHashDigest
);


/*******************************************************************************
*   Function Define Section
*******************************************************************************/
/************************************************************************
*   hmac-md2
************************************************************************/
/*=====================================================================================
FUNCTION: 
    We_LibHashStartHmacMd2
CREATE DATE: 
    2006-12-04
AUTHOR: 
    Sam
DESCRIPTION:
    Start hmac with hash algorithm of MD2.
ARGUMENTS PASSED:
    pucKey[IN]: cipher key used in hmac;
    uiKeyLen[IN]: length of key;
    hHMACHandle[IO]: data handle.
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
WE_UINT32 We_LibHashStartHmacMd2
(
    const WE_UINT8    *pucKey, 
    WE_UINT32   uiKeyLen, 
    WE_HANDLE   *hHMACHandle
)
{
    St_WeHMAC stHmacMD2;
    
    stHmacMD2.eHashType = E_WE_ALG_HASH_MD2;
    stHmacMD2.HashStart = We_LibHashStartMd2;
    stHmacMD2.HashAppend = We_LibHashAppendMd2;
    stHmacMD2.HashFinish = We_LibHashFinishMd2;
    
    return We_LibStartHmac(&stHmacMD2, pucKey, uiKeyLen, hHMACHandle);
}

/*=====================================================================================
FUNCTION: 
    We_LibHashAppendHmacMd2
CREATE DATE: 
    2006-12-04
AUTHOR: 
    Sam
DESCRIPTION:
    Append hmac with hash algorithm of MD2.
ARGUMENTS PASSED:
    pucData[IN]: data to be hmac;
    uiNBits[IN]: bit length of data;
    pvHid[IO]: data handle.
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
WE_UINT32 We_LibHashAppendHmacMd2
(
    WE_UINT8    *pucData, 
    WE_UINT32   uiNBits, 
    WE_VOID     *pvHid
)
{
    return We_LibAppendHmac(pucData, uiNBits, pvHid);
}

/*=====================================================================================
FUNCTION: 
    We_LibHashFinishHmacMd2
CREATE DATE: 
    2006-12-04
AUTHOR: 
    Sam
DESCRIPTION:
    Get hmac result with hash algorithm of MD2.
ARGUMENTS PASSED:
    pvHid[IO]: data handle.
    pstHashDigest[OUT]: hmac result;
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
WE_UINT32 We_LibHashFinishHmacMd2( WE_VOID * pvHid, St_WeLibHashDigest *pstHashDigest)
{
    return We_LibFinishHmac(pvHid, pstHashDigest);
}
/************************************************************************
*   hmac md5
************************************************************************/
/*=====================================================================================
FUNCTION: 
    We_LibHashStartHmacMd5
CREATE DATE: 
    2006-12-04
AUTHOR: 
    Sam
DESCRIPTION:
    Start hmac with hash algorithm of MD5.
ARGUMENTS PASSED:
    pucKey[IN]: cipher key used in hmac;
    uiKeyLen[IN]: length of key;
    hHMACHandle[IO]: data handle.
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
WE_UINT32 We_LibHashStartHmacMd5
(
    const WE_UINT8    *pucKey, 
    WE_UINT32   uiKeyLen, 
    WE_HANDLE   *hHMACHandle
)
{
    St_WeHMAC stHmacMD5;
    
    stHmacMD5.eHashType = E_WE_ALG_HASH_MD5;
    stHmacMD5.HashStart = We_LibHashStartMd5;
    stHmacMD5.HashAppend = We_LibHashAppendMd5;
    stHmacMD5.HashFinish = We_LibHashFinishMd5;
    
    return We_LibStartHmac(&stHmacMD5, pucKey, uiKeyLen, hHMACHandle);
}

/*=====================================================================================
FUNCTION: 
    We_LibHashAppendHmacMd5
CREATE DATE: 
    2006-12-04
AUTHOR: 
    Sam
DESCRIPTION:
    Append hmac with hash algorithm of MD5.
ARGUMENTS PASSED:
    pucData[IN]: data to be hmac;
    uiNBits[IN]: bit length of data;
    pvHid[IO]: data handle.
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
WE_UINT32 We_LibHashAppendHmacMd5
(
    WE_UINT8    *pucData, 
    WE_UINT32   uiNBits, 
    WE_VOID     *pvHid
)
{
    return We_LibAppendHmac(pucData, uiNBits, pvHid);
}

/*=====================================================================================
FUNCTION: 
    We_LibHashFinishHmacMd5
CREATE DATE: 
    2006-12-04
AUTHOR: 
    Sam
DESCRIPTION:
    Get hmac result with hash algorithm of MD5.
ARGUMENTS PASSED:
    pvHid[IO]: data handle.
    pstHashDigest[OUT]: hmac result;
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
WE_UINT32 We_LibHashFinishHmacMd5( WE_VOID * pvHid, St_WeLibHashDigest *pstHashDigest)
{
    return We_LibFinishHmac(pvHid, pstHashDigest);
}
/************************************************************************
*   hmac-sha
************************************************************************/
/*=====================================================================================
FUNCTION: 
    We_LibHashStartHmacsha
CREATE DATE: 
    2006-12-04
AUTHOR: 
    Sam
DESCRIPTION:
    Start hmac with hash algorithm of sha.
ARGUMENTS PASSED:
    pucKey[IN]: cipher key used in hmac;
    uiKeyLen[IN]: length of key;
    hHMACHandle[IO]: data handle.
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
WE_UINT32  We_LibHashStartHmacSha
( 
    const WE_UINT8   *pucKey, 
    WE_UINT32  uiKeyLen, 
    WE_HANDLE  *hHMACHandle
)
{
    St_WeHMAC stHmacSHA;
    
    stHmacSHA.eHashType = E_WE_ALG_HASH_SHA1;
    stHmacSHA.HashStart = We_LibHashStartSha;
    stHmacSHA.HashAppend = We_LibHashAppendSha;
    stHmacSHA.HashFinish = We_LibHashFinishSha;
                           
    return We_LibStartHmac(&stHmacSHA, pucKey, uiKeyLen, hHMACHandle);
}


/*=====================================================================================
FUNCTION: 
    We_LibHashAppendHmacsha
CREATE DATE: 
    2006-12-04
AUTHOR: 
    Sam
DESCRIPTION:
    Append hmac with hash algorithm of sha.
ARGUMENTS PASSED:
    pucData[IN]: data to be hmac;
    uiNBits[IN]: bit length of data;
    pvHid[IO]: data handle.
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
WE_UINT32 We_LibHashAppendHmacSha
(
    WE_UINT8    *pucData, 
    WE_UINT32   uiNBits, 
    WE_VOID     *pvHid
)
{
    return We_LibAppendHmac(pucData, uiNBits, pvHid);
}

/*=====================================================================================
FUNCTION: 
    We_LibHashFinishHmacsha
CREATE DATE: 
    2006-12-04
AUTHOR: 
    Sam
DESCRIPTION:
    Get hmac result with hash algorithm of sha.
ARGUMENTS PASSED:
    pvHid[IO]: data handle.
    pstHashDigest[OUT]: hmac result;
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
WE_UINT32 We_LibHashFinishHmacSha( WE_VOID * pvHid, St_WeLibHashDigest *pstHashDigest )
{
    return We_LibFinishHmac(pvHid, pstHashDigest);
}


/*=====================================================================================
FUNCTION: 
    We_LibHashStartHmac
CREATE DATE: 
    2006-12-04
AUTHOR: 
    Sam
DESCRIPTION:
    Start hmac with reference hash algorithm.
ARGUMENTS PASSED:
    pstHmac[IN]: hash alg flag and reference function pointer;
    pucKey[IN]: cipher key used in hmac;
    uiKeyLen[IN]: length of key;
    hHMACHandle[IO]: data handle.
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
static WE_UINT32  We_LibStartHmac
( 
    St_WeHMAC  *pstHmac,
    const WE_UINT8   *pucKey, 
    WE_UINT32  uiKeyLen, 
    WE_HANDLE  *hHMACHandle
)
{
    WE_UINT32   uiRes = 0x00;
    St_HmacId   *pstNew = NULL;
    WE_UINT32   uiIndex = 0;
    WE_UINT8    aucKeyData[M_HASH_BLOCK_SIZE] = {0};
    WE_UINT8    *pucKeyData = NULL;
    WE_UINT8 ucAlg = (WE_UINT8)pstHmac->eHashType;
    
    if((NULL==pucKey)||(0==uiKeyLen)||!hHMACHandle||0==ucAlg||ucAlg>3)
    {
        return 0x08;
    }
    (WE_VOID)WE_MEMSET(aucKeyData, 0x36, M_HASH_BLOCK_SIZE);
    
    pucKeyData = WE_MALLOC(uiKeyLen*sizeof(WE_UINT8));
    if (NULL == pucKeyData)
    {
        return M_WE_LIB_ERR_INSUFFICIENT_MEMORY;
    }
    (WE_VOID)WE_MEMCPY(pucKeyData, pucKey, uiKeyLen);
    if(uiKeyLen > M_HASH_BLOCK_SIZE) 
    { 
        WE_VOID *pvHid = NULL;
        St_WeLibHashDigest stHdig = {{0},0};

        uiRes = pstHmac->HashStart(&pvHid);
        if(0x00 != uiRes || !pvHid) 
        {
            WE_LIB_FREE(pucKeyData);
            return 0x01;
        }
        uiRes = pstHmac->HashAppend(pucKeyData, uiKeyLen * 8, pvHid);
        if(0x00 != uiRes) 
        {
            WE_LIB_FREE(pucKeyData);
            WE_LIB_FREE(pvHid);
            return uiRes;
        }
        uiRes = pstHmac->HashFinish(pvHid, &stHdig);
        if(uiRes != 0x00) 
        {
            WE_LIB_FREE(pucKeyData);
            WE_LIB_FREE(pvHid);
            return uiRes;
        }        
        for (uiIndex = 0; uiIndex < stHdig.uiHashSize; uiIndex ++)
        {
            aucKeyData[uiIndex] ^= stHdig.aucHashValue[uiIndex];
        }
    } 
    else 
    {
        for(uiIndex = 0; uiIndex < uiKeyLen; uiIndex++) 
        {
            aucKeyData[uiIndex] ^= pucKeyData[uiIndex];
        }
    }
    WE_LIB_FREE(pucKeyData);
    pstNew = (St_HmacId *)WE_MALLOC(sizeof(St_HmacId));
    if(!pstNew) 
    {
        return 0x04;
    }
    (void)WE_MEMSET( pstNew,0,sizeof(St_HmacId) );
    uiRes = pstHmac->HashStart(&(pstNew->pvHid1));
    if(!(pstNew->pvHid1) || 0x00 != uiRes)
    {
        WE_LIB_FREE(pstNew);
        return 0x01;
    }
    uiRes = pstHmac->HashAppend(aucKeyData, M_HASH_BLOCK_SIZE * 8, pstNew->pvHid1);
    if(0x00 != uiRes) 
    {
        WE_LIB_FREE(pstNew->pvHid1);
        WE_LIB_FREE(pstNew);
        return uiRes;
    }
    
    for(uiIndex = 0; uiIndex < M_HASH_BLOCK_SIZE; uiIndex++)
    {
        aucKeyData[uiIndex] ^= 0x6a; 
    }
    
    uiRes = pstHmac->HashStart(&(pstNew->pvHid2));
    if(0x00 != uiRes || !pstNew->pvHid2) 
    {
        WE_LIB_FREE(pstNew->pvHid1);
        WE_LIB_FREE(pstNew);
        return 0x01;
    }
    uiRes = pstHmac->HashAppend(aucKeyData, M_HASH_BLOCK_SIZE * 8, pstNew->pvHid2);
    if(0x00 != uiRes) 
    {
        WE_LIB_FREE(pstNew->pvHid1);
        WE_LIB_FREE(pstNew->pvHid2);
        WE_LIB_FREE(pstNew);
        return uiRes;
    }  
    pstNew->stHmac = (*pstHmac);
    (*hHMACHandle) = (WE_HANDLE)pstNew;
    return 0x00;
}

/*=====================================================================================
FUNCTION: 
    We_LibHashAppendHmac
CREATE DATE: 
    2006-12-04
AUTHOR: 
    Sam
DESCRIPTION:
    Append hmac with reference hash algorithm.
ARGUMENTS PASSED:
    pucData[IN]: data to be hmac;
    uiNBits[IN]: bit length of data;
    pvHid[IO]: data handle.
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
static WE_UINT32 We_LibAppendHmac
(
    WE_UINT8    *pucData, 
    WE_UINT32   uiNBits, 
    WE_VOID     *pvHid
)
{
    St_HmacId *pstSession = (St_HmacId *)pvHid;
    
    CHECK_FOR_NULL_ARG(pstSession, 0x08);
    if (!pstSession->stHmac.HashAppend)
    {
        WE_LIB_FREEIF(pstSession->pvHid1);
        WE_LIB_FREEIF(pstSession->pvHid2);
        WE_LIB_FREE(pstSession);
        return 0x08;
    }
    CHECK_FOR_NULL_ARG(pstSession->pvHid1, 0x08);
    
    return(pstSession->stHmac.HashAppend(pucData, uiNBits, pstSession->pvHid1));
}

/*=====================================================================================
FUNCTION: 
    We_LibHashFinishHmac
CREATE DATE: 
    2006-12-04
AUTHOR: 
    Sam
DESCRIPTION:
    Get hmac result with reference hash algorithm.
ARGUMENTS PASSED:
    pvHid[IO]: data handle.
    pstHashDigest[OUT]: hmac result;
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
static WE_UINT32 We_LibFinishHmac( WE_VOID * pvHid, St_WeLibHashDigest *pstHashDigest )
{
    WE_UINT32 uiRes = 0x00;
    St_HmacId *pstSession = (St_HmacId *)pvHid;
    St_WeLibHashDigest stRval={{0},0};
    
    if (!(pstSession) || (!pstSession->stHmac.HashAppend)||(!pstSession->stHmac.HashFinish)) 
    {
        return 0x08;
    }
    if (!pstHashDigest)
    {
        WE_LIB_FREEIF(pstSession->pvHid1);
        WE_LIB_FREEIF(pstSession->pvHid2);
        WE_LIB_FREE(pstSession);
        return 0x08;
    }
    
    uiRes = pstSession->stHmac.HashFinish(pstSession->pvHid1, &stRval);
    if(uiRes != 0x00) 
    {
        WE_LIB_FREEIF(pstSession->pvHid2);
        WE_LIB_FREE(pstSession);
        return uiRes;
    }  
    uiRes = pstSession->stHmac.HashAppend(stRval.aucHashValue, stRval.uiHashSize * 8, pstSession->pvHid2);
    if(0x00 != uiRes)
    {
        WE_LIB_FREEIF(pstSession->pvHid2);
        WE_LIB_FREE(pstSession);
        return uiRes;
    }
    
    uiRes = pstSession->stHmac.HashFinish(pstSession->pvHid2, &stRval);
    if(0x00 != uiRes) 
    {
        WE_LIB_FREE(pstSession);
        return uiRes;
    }
    WE_LIB_FREE(pstSession);

    (*pstHashDigest) = stRval;
    return uiRes;
}

/*=====================================================================================
FUNCTION: 
    We_LibRemoveHmac
CREATE DATE: 
    2006-12-04
AUTHOR: 
    Sam
DESCRIPTION:
    Free the memory of data handle.
ARGUMENTS PASSED:
    pvHmacId[IN]: data handle.
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
void We_LibRemoveHmac( WE_VOID *pvHmacId )
{    
    St_HmacId *pstSession = (St_HmacId *)pvHmacId;
    if (!(pstSession)) 
    {
        return;
    }

    WE_LIB_FREEIF(pstSession->pvHid1);
    WE_LIB_FREEIF(pstSession->pvHid2);
    WE_LIB_FREE(pstSession);
}







