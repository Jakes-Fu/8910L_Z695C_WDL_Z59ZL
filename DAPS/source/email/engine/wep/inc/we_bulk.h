/*=====================================================================================
    HEADER NAME : we_bulk.h
    MODULE NAME : WE

    GENERAL DESCRIPTION
        In this file,define the struct and function prototype. 

    TechFaith Software Confidential Proprietary
    Copyright (c) 2006-2007 by TechFaith Software. All Rights Reserved.
=======================================================================================
    Revision History
    
    Modification              Tracking
    Date            Author      Number    Description of changes
    ---------- -------------- --------- --------------------------------------
    2006-11-03       Tang         none      Init
  
=====================================================================================*/
/*******************************************************************************
*   Multi-Include-Prevent Section
*******************************************************************************/
#ifndef  WE_BULK_H
#define  WE_BULK_H


/*******************************************************************************
*   Macro Define Section
*******************************************************************************/
/*des*/
#define BLOCKSIZE   (8)
#define KEYSIZE     (8)
#define ROUND_KEY_SIZE  (128)

/*rc2*/
#define M_WE_RC2_TYPE       0x13532453
#define WE_RC2_BLOCK_SIZE   8

/*aes*/
/* direction */
#define AES_ENCRYPT 1
#define AES_DECRYPT 0

/* block and maximum NR */
#define AES_MAXNR       14
#define AES_BLOCK_SIZE  16

#define GETU32(pt) (((WE_UINT32)(pt)[0] << 24) ^ ((WE_UINT32)(pt)[1] << 16) ^ ((WE_UINT32)(pt)[2] <<  8) ^ ((WE_UINT32)(pt)[3]))
#define PUTU32(ct, st) { (ct)[0] = (WE_UINT8)((st) >> 24); (ct)[1] = (WE_UINT8)((st) >> 16); (ct)[2] = (WE_UINT8)((st) >>  8); (ct)[3] = (WE_UINT8)(st); }

/*idea*/
#define M_WE_IDEA_TYPE           0x135324
#define WE_IDEA_BLOCK_SIZE       8


/*******************************************************************************
*   Type Define Section
*******************************************************************************/
/*des*/
typedef struct tagSt_WeBlockMAtchS *pstBlockMach;
typedef WE_VOID (*blockfn)(pstBlockMach m, WE_UINT32 len, WE_UINT8 *in, WE_UINT8 *out);
typedef WE_VOID (*dcfn)(WE_UINT32 *data, WE_VOID *pvRoundKey[]);

typedef enum tagE_WeBsapiModeOp
{
    BSAPI_ENCRYPT, 
    BSAPI_DECRYPT
} E_WeBsapiModeOp; 


typedef enum tagE_WeBsapiModeCBC
{
    BSAPI_ECB, 
    BSAPI_CBC
} E_WeBsapiModeCBC;

typedef enum tagE_WeDesOr3Des
{
    BSAPI_SINGLE,
    BSAPI_TRIPLE
} E_WeDesOr3Des;

typedef enum tagE_WeDesKeyType
{
    BSAPI_KEY, 
    BSAPI_SCHEDULE
} E_WeDesKeyType;

typedef struct tagSt_BsapiMode 
{
    E_WeBsapiModeOp     eOp;
    E_WeBsapiModeCBC    eMode;
    E_WeDesOr3Des       eTriple;
    E_WeDesKeyType      eKeyType;
}St_WeLibBsapiMode ;

typedef struct tagSt_WeBlockMAtchS 
{
    WE_VOID         *pvRoundKey[3];
    WE_UINT8        aucData8[BLOCKSIZE];
    WE_UINT32       iv[3][BLOCKSIZE / 4];
    blockfn         fn;
    dcfn            corefn;
    E_WeBsapiModeOp eOp;
    WE_UINT32       iBufLen;
    WE_UINT32       uiType; 
}St_WeBlockMAtchS;

/*aes*/
typedef struct tagSt_WeAesKey 
{
    WE_UINT32 auiRoundKey[4 *(AES_MAXNR + 1)];
    WE_INT32  iNRounds;
}St_WeAesKey;

typedef struct tagSt_WeAESParameter
{
    St_WeAesKey     stAESCipherKeyEx;
    WE_INT32        iType;
    WE_UCHAR        aucIv[16];
    WE_UCHAR        aucBuf[16];
    WE_UINT32       uiBufLen;
}St_WeAESParameter;

/*******************************************************************************
*   Prototype Declare Section
*******************************************************************************/
WE_VOID We_LibBlock3
(
    dcfn core,  
    WE_UINT32 *puiData, 
    WE_VOID *pavKs[]
);

WE_VOID We_LibBlockCBC
(
    pstBlockMach pstM, 
    WE_UINT32 uiLen, 
    WE_UINT8 *pucIn, 
    WE_UINT8 *pucOut
);

WE_INT32 We_LibAppendBlock
(
    WE_UINT8 *pucData, 
    WE_UINT32 uiLength, 
    WE_UCHAR *pucOut,
    WE_VOID *handle
);

WE_INT32 We_LibAppendDes
(
    WE_UINT8 *pucData, 
    WE_UINT32 uiLength,
    WE_UCHAR *pucOut,
    WE_VOID *pHandle
);

WE_VOID We_LibKillDes(WE_HANDLE hDESHandle);

WE_INT32 We_LibStartTripleDes
(
    St_WeLibBsapiMode *pstMode,  
    St_WeCipherCrptKey stKey, 
    WE_HANDLE *pHandle
);


WE_INT32 We_LibAppendTripleDes
(
    WE_UCHAR *pucData, 
    WE_UINT32 uiLength,
    WE_UCHAR *pucOut, 
    WE_VOID *pHandle
);


/***************************************************************************************************
*   DES Encrypt                                                                    
***************************************************************************************************/
/* No Padding */
WE_INT32 We_LibDesEncrypt
(
    St_WeCipherCrptKey stKey, 
    const WE_UCHAR* pucIn, 
    WE_UINT32 uiInLen, 
    WE_UCHAR* pucOut, 
    WE_UINT32* puiOutLen
);

WE_INT32 We_LibDesEncryptInit
(
    St_WeCipherCrptKey stKey,
    WE_HANDLE* pHandle
);

WE_INT32 We_LibDesEnCryptUpdate
(
    const WE_UCHAR* pucIn, 
    WE_UINT32 uiInLen,  
    WE_UCHAR* pucOut, 
    WE_UINT32* puiOutLen, 
    WE_HANDLE handle
);

WE_INT32 We_LibDesEnCryptFinal
(
    WE_HANDLE handle
);

/* RFC2630 Padding */
WE_INT32 We_LibDesEncrypt1
(
    St_WeCipherCrptKey stKey, 
    const WE_UCHAR* pucIn, 
    WE_UINT32 uiInLen, 
    WE_UCHAR* pucOut, 
    WE_UINT32* puiOutLen
);

WE_INT32 We_LibDesEncryptInit1
(
    St_WeCipherCrptKey stKey,
    WE_HANDLE* pHandle
);

WE_INT32 We_LibDesEnCryptUpdate1
(
    const WE_UCHAR* pucIn, 
    WE_UINT32 uiInLen,  
    WE_UCHAR* pucOut, 
    WE_UINT32* puiOutLen, 
    WE_HANDLE handle
);

WE_INT32 We_LibDesEnCryptFinal1
(
    WE_UCHAR* pucOut,
    WE_UINT32* puiOutLen, 
    WE_HANDLE handle
);


/***************************************************************************************************
*   DES Decrypt                                                                    
***************************************************************************************************/
/* No Padding */
WE_INT32 We_LibDesDecrypt
(
    St_WeCipherCrptKey stKey, 
    const WE_UCHAR* pucIn, 
    WE_UINT32 uiInLen, 
    WE_UCHAR* pucOut, 
    WE_UINT32* puiOutLen
);

WE_INT32 We_LibDesDeCryptInit
(
    St_WeCipherCrptKey stKey,
    WE_HANDLE* pHandle
);

WE_INT32 We_LibDesDeCryptUpdate
(
    const WE_UCHAR* pucIn, 
    WE_UINT32 uiInLen,  
    WE_UCHAR* pucOut, 
    WE_UINT32* puiOutLen, 
    WE_HANDLE handle
);

WE_INT32 We_LibDesDeCryptFinal
(
    WE_HANDLE handle
);


/* RFC2630 Padding */
WE_INT32 We_LibDesDecrypt1
(
    St_WeCipherCrptKey stKey, 
    const WE_UCHAR* pucIn, 
    WE_UINT32 uiInLen, 
    WE_UCHAR* pucOut, 
    WE_UINT32* puiOutLen
);

WE_INT32 We_LibDesDecryptInit1
(
    St_WeCipherCrptKey stKey,
    WE_HANDLE* pHandle
);

WE_INT32 We_LibDesDeCryptUpdate1
(
    const WE_UCHAR* pucIn, 
    WE_UINT32 uiInLen,  
    WE_UCHAR* pucOut, 
    WE_UINT32* puiOutLen, 
    WE_HANDLE handle
);

WE_INT32 We_LibDesDeCryptFinal1
(
    WE_UCHAR* pucOut,
    WE_UINT32* puiOutLen, 
    WE_HANDLE handle
);

/***************************************************************************************************
*   Tripartite DES Encrypt                                                                    
***************************************************************************************************/
/* No Padding */
WE_INT32 We_Lib3DesEncrypt
(
    St_WeCipherCrptKey stKey, 
    const WE_UCHAR* pucIn, 
    WE_UINT32 uiInLen, 
    WE_UCHAR* pucOut, 
    WE_UINT32* puiOutLen
);

WE_INT32 We_Lib3DesEncryptInit
(
    St_WeCipherCrptKey stKey,
    WE_HANDLE* pHandle
);

WE_INT32 We_Lib3DesEnCryptUpdate
(
    const WE_UCHAR* pucIn, 
    WE_UINT32 uiInLen,
    WE_UCHAR* pucOut, 
    WE_UINT32* puiOutLen, 
    WE_HANDLE handle
);

WE_INT32 We_Lib3DesEnCryptFinal
(
    WE_HANDLE handle
); 

/* RFC2630 Padding */
WE_INT32 We_Lib3DesEncrypt1
(
    St_WeCipherCrptKey stKey, 
    const WE_UCHAR* pucIn, 
    WE_UINT32 uiInLen, 
    WE_UCHAR* pucOut, 
    WE_UINT32* puiOutLen
);

WE_INT32 We_Lib3DesEncryptInit1
(
    St_WeCipherCrptKey stKey,
    WE_HANDLE* pHandle
);

WE_INT32 We_Lib3DesEnCryptUpdate1
(
    const WE_UCHAR* pucIn, 
    WE_UINT32 uiInLen,  
    WE_UCHAR* pucOut, 
    WE_UINT32* puiOutLen, 
    WE_HANDLE handle
);

WE_INT32 We_Lib3DesEnCryptFinal1
(
    WE_UCHAR* pucOut,
    WE_UINT32* puiOutLen, 
    WE_HANDLE handle
);


/***************************************************************************************************
*   Tripartite DES Decrypt                                                                    
***************************************************************************************************/
/* No Padding */
WE_INT32 We_Lib3DesDecrypt
(
    St_WeCipherCrptKey stKey, 
    const WE_UCHAR* pucIn, 
    WE_UINT32 uiInLen, 
    WE_UCHAR* pucOut, 
    WE_UINT32* puiOutLen
);


WE_INT32 We_Lib3DesDecryptInit
(
    St_WeCipherCrptKey stKey,
    WE_HANDLE* pHandle
);

WE_INT32 We_Lib3DesDeCryptUpdate
(
    const WE_UCHAR* pucIn, 
    WE_UINT32 uiInLen,
    WE_UCHAR* pucOut, 
    WE_UINT32* puiOutLen, 
    WE_HANDLE handle
);

WE_INT32 We_Lib3DesDeCryptFinal
(
    WE_HANDLE handle
); 



/* RFC2630 Padding */
WE_INT32 We_Lib3DesDecrypt1
(
    St_WeCipherCrptKey stKey, 
    const WE_UCHAR* pucIn, 
    WE_UINT32 uiInLen, 
    WE_UCHAR* pucOut, 
    WE_UINT32* puiOutLen
);


WE_INT32 We_Lib3DesDeCryptInit1
(
    St_WeCipherCrptKey stKey,
    WE_HANDLE* pHandle
);

WE_INT32 We_Lib3DesDeCryptUpdate1
(
    const WE_UCHAR* pucIn, 
    WE_UINT32 uiInLen,  
    WE_UCHAR* pucOut, 
    WE_UINT32* puiOutLen, 
    WE_HANDLE handle
);

WE_INT32 We_Lib3DesDeCryptFinal1
(
    WE_UCHAR* pucOut,
    WE_UINT32* puiOutLen, 
    WE_HANDLE handle
);

WE_INT32 We_LibStartDesCore
(
 St_WeLibBsapiMode *pstMode,  
 St_WeCipherCrptKey *pstKey, 
 WE_HANDLE *pHandle
 );  

/*******************************************************************************
*   Prototype Declare Section
*******************************************************************************/
WE_INT32 We_LibRC2EncryptNoPadding
(
    St_WeCipherCrptKey  stKey, 
    const WE_UCHAR* pucIn, 
    WE_UINT32 uiInLen, 
    WE_UCHAR* pucOut, 
    WE_UINT32* puiOutLen
);

WE_INT32 We_LibRC2EncryptPadding
(
    St_WeCipherCrptKey  stKey, 
    const WE_UCHAR* pucIn, 
    WE_UINT32 uiInLen, 
    WE_UCHAR* pucOut, 
    WE_UINT32* puiOutLen
);

WE_INT32 We_LibRC2EncryptInit
(
    St_WeCipherCrptKey stKey,
    WE_UINT32 uiBits,
    WE_HANDLE* pHandle
);

WE_INT32 We_LibRC2EncryptInitPad
(
    St_WeCipherCrptKey stKey,
    WE_UINT32 uiBits,
    WE_HANDLE* pHandle
);

WE_INT32 We_LibRC2EncryptUpdate
(
    const WE_UCHAR* pucIn, 
    WE_UINT32 uiInLen, 
    WE_UCHAR* pucOut, 
    WE_UINT32* puiOutLen,
    WE_HANDLE handle
);

WE_INT32 We_LibRC2EncryptUpdatePad
(
    const WE_UCHAR* pucIn, 
    WE_UINT32 uiInLen, 
    WE_UCHAR* pucOut, 
    WE_UINT32* puiOutLen,
    WE_HANDLE handle
);

WE_INT32 We_LibRC2EncryptFinal(WE_HANDLE handle);

WE_INT32 We_LibRC2EncryptFinalPad
( 
    WE_UCHAR* pucOut, 
    WE_UINT32* puiOutLen,
    WE_HANDLE handle
);

WE_INT32 We_LibRC2DecryptNoPadding
( 
    St_WeCipherCrptKey  stKey, 
    const WE_UCHAR* pucIn, 
    WE_UINT32 uiInLen, 
    WE_UCHAR* pucOut, 
    WE_UINT32* puiOutLen
);

WE_INT32 We_LibRC2DecryptPadding
(
    St_WeCipherCrptKey  stKey, 
    const WE_UCHAR* pucIn, 
    WE_UINT32 uiInLen, 
    WE_UCHAR* pucOut, 
    WE_UINT32* puiOutLen
);

WE_INT32 We_LibRC2DecryptInit
(
    St_WeCipherCrptKey stKey,
    WE_UINT32 uiBits,
    WE_HANDLE* pHandle
);

WE_INT32 We_LibRC2DecryptInitPad
(
    St_WeCipherCrptKey stKey,
    WE_UINT32 uiBits,
    WE_HANDLE* pHandle
);

WE_INT32 We_LibRC2DecryptUpdate
(
    const WE_UCHAR* pucIn, 
    WE_UINT32 uiInLen, 
    WE_UCHAR* pucOut, 
    WE_UINT32* puiOutLen,
    WE_HANDLE handle
);

WE_INT32 We_LibRC2DecryptUpdatePad
(
    const WE_UCHAR* pucIn, 
    WE_UINT32 uiInLen, 
    WE_UCHAR* pucOut, 
    WE_UINT32* puiOutLen,
    WE_HANDLE handle
);

WE_INT32 We_LibRC2DecryptFinal(WE_HANDLE handle);

WE_INT32 We_LibRC2DecryptFinalPad
(
    WE_UCHAR* pucOut, 
    WE_UINT32* puiOutLen,
    WE_HANDLE handle
);


/***************************************************************************************************
*  AES Prototype Declare Section
***************************************************************************************************/
WE_INT32 We_AESSetEncryptKey
(
    const WE_UINT8  *pucUserKey, 
    const WE_INT32  iNBits,
    St_WeAesKey     *pstAESKey
);

WE_INT32 We_AESSetDecryptKey
(
    const WE_UINT8  *pucUserKey, 
    const WE_INT32  iNBits,
    St_WeAesKey     *pstAESKey
);

WE_VOID We_AESEncrypt
(
    const WE_UINT8  *pucInData, 
    WE_UINT8        *pucOutData,
    const St_WeAesKey *pstAESKey
);


WE_VOID We_AESDecrypt
(
    const WE_UINT8  *pucInData, 
    WE_UINT8        *pucOutData,
    const St_WeAesKey *pstAESKey
);

/* encrypt and decrypt data with the same function, so we have the last param. */
WE_VOID We_AESCBCEncrypt
(
    const St_WeAesKey   *pstAESKey,
    WE_UINT8            *pucIVec, 
    const WE_INT32      enc, 
    const WE_UINT8      *pucInData, 
    const WE_UINT32     uiInDataLen, 
    WE_UINT8            *pucOutData,
    WE_UINT32           *puiOutDataLen
);

WE_VOID We_AESCBCDecrypt
(
    const St_WeAesKey   *pstAESKey,
    WE_UINT8            *pucIVec, 
    const WE_INT32      enc, 
    const WE_UINT8      *pucInData, 
    const WE_UINT32     uiInDataLen, 
    WE_UINT8            *pucOutData,
    WE_UINT32           *puiOutDataLen
);

/* interface */
WE_INT32 We_LibAESEncrypt
(
    St_WeCipherCrptKey  stKey, 
    const WE_UCHAR      *pucIn, 
    WE_UINT32           uiInLen, 
    WE_UCHAR            *pucOut, 
    WE_UINT32           *puiOutLen
);

WE_INT32 We_LibAESEncryptInit
(
    St_WeCipherCrptKey  stKey, 
    WE_HANDLE           *hAESHandle
);

WE_INT32 We_LibAESEncryptUpdate
(
    const WE_UCHAR  *pucIn, 
    WE_UINT32       uiInLen, 
    WE_UCHAR        *pucOut, 
    WE_UINT32       *puiOutLen, 
    WE_HANDLE       hAESHandle
);

WE_INT32 We_LibAESEncryptFinal
(
    WE_HANDLE hAESHandle
);

WE_INT32 We_LibAESEncrypt1
(
    St_WeCipherCrptKey  stKey, 
    const WE_UCHAR* pucIn, 
    WE_UINT32 uiInLen, 
    WE_UCHAR* pucOut, 
    WE_UINT32* puiOutLen
);

WE_INT32 We_LibAESEncryptInit1
(
    St_WeCipherCrptKey  stKey, 
    WE_HANDLE *hAESHandle
);

WE_INT32 We_LibAESEncryptUpdate1
(
    const WE_UCHAR* pucIn, 
    WE_UINT32 uiInLen, 
    WE_UCHAR* pucOut, 
    WE_UINT32* puiOutLen, 
    WE_HANDLE hAESHandle
);

WE_INT32 We_LibAESEncryptFinal1
(
    WE_UCHAR    *pucOut, 
    WE_UINT32   *puiOutLen,
    WE_HANDLE   hAESHandle
);

WE_INT32 We_LibAESDecrypt
(
    St_WeCipherCrptKey  stKey, 
    const WE_UCHAR* pucIn, 
    WE_UINT32 uiInLen, 
    WE_UCHAR* pucOut, 
    WE_UINT32* puiOutLen
);

WE_INT32 We_LibAESDecryptInit
(
    St_WeCipherCrptKey  stKey, 
    WE_HANDLE *hAESHandle
);

WE_INT32 We_LibAESDecryptUpdate
(
    const WE_UCHAR* pucIn, 
    WE_UINT32 uiInLen, 
    WE_UCHAR* pucOut, 
    WE_UINT32* puiOutLen, 
    WE_HANDLE hAESHandle
);

WE_INT32 We_LibAESDecryptFinal
(
    WE_HANDLE hAESHandle
);

WE_INT32 We_LibAESDecrypt1
(
    St_WeCipherCrptKey  stKey, 
    const WE_UCHAR* pucIn, 
    WE_UINT32 uiInLen, 
    WE_UCHAR* pucOut, 
    WE_UINT32* puiOutLen
);

WE_INT32 We_LibAESDecryptInit1
(
    St_WeCipherCrptKey  stKey, 
    WE_HANDLE *hAESHandle
);

WE_INT32 We_LibAESDecryptUpdate1
(
    const WE_UCHAR* pucIn, 
    WE_UINT32 uiInLen, 
    WE_UCHAR* pucOut, 
    WE_UINT32* puiOutLen, 
    WE_HANDLE hAESHandle
);

WE_INT32 We_LibAESDecryptFinal1
(
    WE_UCHAR* pucOutEndp, 
    WE_UINT32* puiPadLen, 
    WE_HANDLE hAESHandle
);

/*************************************************************************************************
      RC5 function declare
*************************************************************************************************/
WE_INT32 We_LibRC5Encrypt(WE_INT32 iRounds,
                                  St_WeCipherCrptKey stKey, 
                                  const WE_UCHAR* pucIn, 
                                  WE_UINT32 uiInLen, 
                                  WE_UCHAR* pucOut, 
                                  WE_UINT32* puiOutLen);
                           
WE_INT32 We_LibRC5Decrypt(WE_INT32 iRounds,
                                  St_WeCipherCrptKey stKey, 
                                  const WE_UCHAR*pucIn, 
                                  WE_UINT32 uiInLen, 
                                  WE_UCHAR* pucOut, 
                                  WE_UINT32* puiOutLen);

WE_INT32 We_LibRC5Encrypt1(WE_INT32 iRounds,
                                  St_WeCipherCrptKey stKey, 
                                  const WE_UCHAR* pucIn, 
                                  WE_UINT32 uiInLen, 
                                  WE_UCHAR* pucOut, 
                                  WE_UINT32* puiOutLen);

WE_INT32 We_LibRC5Decrypt1(WE_INT32 iRounds,
                                  St_WeCipherCrptKey stKey, 
                                  const WE_UCHAR*pucIn, 
                                  WE_UINT32 uiInLen, 
                                  WE_UCHAR* pucOut, 
                                  WE_UINT32* puiOutLen);

WE_INT32 We_LibRC5CryptInit(WE_INT32 iRounds,
                                      St_WeCipherCrptKey stKey,
                                      WE_HANDLE* pHandle);

WE_INT32 We_LibRC5EncryptInit(WE_INT32 iRounds,
                                      St_WeCipherCrptKey stKey,
                                      WE_HANDLE* pHandle);

WE_INT32 We_LibRC5EncryptUpdate(const WE_UCHAR* pucIn, 
                                        WE_UINT32 uiInLen, 
                                        WE_UCHAR* pucOut, 
                                        WE_UINT32* puiOutLen,
                                        WE_HANDLE handle);

WE_INT32 We_LibRC5EncryptFinal(WE_HANDLE handle); 

WE_INT32 We_LibRC5EncryptInit1(WE_INT32 iRounds,
                                       St_WeCipherCrptKey stKey,
                                       WE_HANDLE* pHandle);

WE_INT32 We_LibRC5EncryptUpdate1(const WE_UCHAR* pucIn, 
                                         WE_UINT32 uiInLen, 
                                         WE_UCHAR* pucOut, 
                                         WE_UINT32* puiOutLen,
                                         WE_HANDLE handle);

WE_INT32 We_LibRC5EncryptFinal1(WE_UCHAR* pucOut, 
                                        WE_UINT32* puiOutLen, WE_HANDLE handle); 

WE_INT32 We_LibRC5DecryptInit(WE_INT32 iRounds,
                                      St_WeCipherCrptKey stKey,
                                      WE_HANDLE* pHandle);

WE_INT32 We_LibRC5DecryptUpdate(const WE_UCHAR* pucIn, 
                                         WE_UINT32 uiInLen, 
                                         WE_UCHAR* pucOut, 
                                         WE_UINT32* puiOutLen,
                                         WE_HANDLE handle);

WE_INT32 We_LibRC5DecryptFinal(WE_HANDLE handle); 

WE_INT32 We_LibRC5DecryptInit1(WE_INT32 iRounds,
                                       St_WeCipherCrptKey stKey,
                                       WE_HANDLE* pHandle);

WE_INT32 We_LibRC5DecryptUpdate1(const WE_UCHAR* pucIn, 
                                         WE_UINT32 uiInLen, 
                                         WE_UCHAR* pucOut, 
                                         WE_UINT32* puiOutLen,
                                         WE_HANDLE handle);

WE_INT32 We_LibRC5DecryptFinal1(WE_UCHAR* pucOut, 
                                        WE_UINT32* puiOutLen, WE_HANDLE handle);     

/*******************************************************************************
*   IDEA Prototype Declare Section
*******************************************************************************/
WE_INT32 We_LibIDEAEncryptNoPadding
(
    St_WeCipherCrptKey  stKey, 
    const WE_UCHAR* pucIn, 
    WE_UINT32 uiInLen, 
    WE_UCHAR* pucOut, 
    WE_UINT32* puiOutLen
);

WE_INT32 We_LibIDEAEncryptPadding
(
    St_WeCipherCrptKey  stKey, 
    const WE_UCHAR* pucIn, 
    WE_UINT32 uiInLen, 
    WE_UCHAR* pucOut, 
    WE_UINT32* puiOutLen
);

WE_INT32 We_LibIDEAEncryptInit
(
    St_WeCipherCrptKey stKey,
    WE_HANDLE* pHandle
);

WE_INT32 We_LibIDEAEncryptInitPad
(
    St_WeCipherCrptKey stKey,
    WE_HANDLE* pHandle
);

WE_INT32 We_LibIDEAEncryptUpdate
(
    const WE_UCHAR* pucIn, 
    WE_UINT32 uiInLen, 
    WE_UCHAR* pucOut, 
    WE_UINT32* puiOutLen,
    WE_HANDLE handle
);

WE_INT32 We_LibIDEAEncryptUpdatePad
(
    const WE_UCHAR* pucIn, 
    WE_UINT32 uiInLen, 
    WE_UCHAR* pucOut, 
    WE_UINT32* puiOutLen,
    WE_HANDLE handle
);

WE_INT32 We_LibIDEAEncryptFinal(WE_HANDLE handle);

WE_INT32 We_LibIDEAEncryptFinalPad
(
    WE_UCHAR* pucOut, 
    WE_UINT32* puiOutLen,  
    WE_HANDLE handle
);

WE_INT32 We_LibIDEADecryptNoPadding
(
    St_WeCipherCrptKey  stKey, 
    const WE_UCHAR* pucIn, 
    WE_UINT32 uiInLen, 
    WE_UCHAR* pucOut, 
    WE_UINT32* puiOutLen
);

WE_INT32 We_LibIDEADecryptPadding
(
    St_WeCipherCrptKey  stKey, 
    const WE_UCHAR* pucIn, 
    WE_UINT32 uiInLen, 
    WE_UCHAR* pucOut, 
    WE_UINT32* puiOutLen
);

WE_INT32 We_LibIDEADecryptInit
(
    St_WeCipherCrptKey stKey,
    WE_HANDLE* pHandle
);

WE_INT32 We_LibIDEADecryptInitPad
(
    St_WeCipherCrptKey stKey,
    WE_HANDLE* pHandle
);

WE_INT32 We_LibIDEADecryptUpdate
(
    const WE_UCHAR* pucIn, 
    WE_UINT32 uiInLen, 
    WE_UCHAR* pucOut, 
    WE_UINT32* puiOutLen,
    WE_HANDLE handle
);

WE_INT32 We_LibIDEADecryptUpdatePad
(
    const WE_UCHAR* pucIn, 
    WE_UINT32 uiInLen, 
    WE_UCHAR* pucOut, 
    WE_UINT32* puiOutLen,
    WE_HANDLE handle
);

WE_INT32 We_LibIDEADecryptFinal(WE_HANDLE handle);

WE_INT32 We_LibIDEADecryptFinalPad
( 
    WE_UCHAR* pucOut, 
    WE_UINT32* puiOutLen,
    WE_HANDLE handle
);
#endif /*WE_BULK_H*/

/*--------------------------END-----------------------------*/

