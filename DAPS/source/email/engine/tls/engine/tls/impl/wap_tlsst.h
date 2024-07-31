/*==================================================================================================
    FILE NAME   : wap_tlsst.h
    MODULE NAME : TLS

    GENERAL DESCRIPTION
        Define structure of tls module.
    
    TechFaith Software Confidential Proprietary
    Copyright (c) 2006-2007 by TechFaith Software. All Rights Reserved.
====================================================================================================
    Revision History

    Modification Tracking
    Date        Author        Number       Description of changes
    --------------------------------------------------------------------------------
    2007-06-28  wangshuhua      None         Initialize   
==================================================================================================*/
    
#ifndef WAP_TLS_ST_H
#define WAP_TLS_ST_H

/***************************************************************************************************
*   Type Define Section
***************************************************************************************************/

typedef struct tagSt_WapTlsSessionId
{
    WE_UINT8          ucLength;     /*the length of session Id*/
    WE_UCHAR          aucId[32];    /*session identifier*/
    WE_UINT8        aucReserved[3]; /*conform to 4 bytes*/
} St_WapTlsSessionId;

typedef WE_UCHAR      T_aucWapTlsRandom[32];
typedef WE_UCHAR      T_aucWapTlsCipherSuite[2];

typedef struct tagSt_WapTlsProVer
{
    WE_UINT8  ucMajor;      /*the major verison of the protocol being employed*/
    WE_UINT8  ucMinor;      /*the minor verison of the protocol being employed*/
    WE_UINT16 usReserverd;  /*conform to 4 bytes*/
} St_WapTlsProVer;

typedef struct tagSt_WapTlsRecord 
{
    struct tagSt_WapTlsRecord  *pstNext;     
    WE_UINT32         uiSeqnum;         /*The sequence number for this record*/
    WE_UCHAR          *pucFragment;     /*the application data*/
    St_WapTlsProVer    stProVer;         /*The protocol being employed for this record*/
     WE_UINT16         usLength;         /*the length of the record without the record's header*/
    WE_UINT8          ucContentType;    /*the record's content type*/
    WE_UINT8          ucReserved;       /*conform to 4 bytes*/

} St_WapTlsRecord;

typedef struct tagSt_WapTlsClientHello
{
    WE_UINT8                ucNumCompressMethods;   /*the length of compressMethods,usually is 0*/
    WE_UINT8                aucCompressMethods[1];  /*This is a list of the compression methods supported by the client,usually is NULL*/
    WE_UINT16               usCipherSuitLen;        /*the length of the cipher suite*/
    T_aucWapTlsRandom       taucRandom;             /*A client -generated random structure*/
    WE_UCHAR                *pucCipherSuit;         /*This is a list of the cryptographic options supported by the client*/
    St_WapTlsProVer         stClientVer;  /*The version of the TLS protocol by which the client wishes to communicate during this session*/
    St_WapTlsSessionId      stSessionId;  /*ID of a session the client wishes to use for this connection, it should be empty 
                                            if none is available or the client wishes to generate new security parameters*/
} St_WapTlsClientHello;

typedef struct tagSt_WapTlsServerHello
{
    St_WapTlsProVer         stServerVer;        /*The protocol verison that decided by server.It is the lower of that suggested by the client
                                                    in the client hello and the highest supported by the server.*/
    T_aucWapTlsRandom       taucRandom;         /*A client -generated random structure*/
    St_WapTlsSessionId      stSessionId;        /*This is the identity of the session corresponding to this connection.*/
    T_aucWapTlsCipherSuite  taucCipherSuite;    /*The single cipher suite selected by the server 
                                             clienthello.cipher_suites*/
  WE_UINT8                  ucCompressMethod;/*The single compression algorithm selected by the server 
                                                    from the list in clienthello.cipher_suites*/
  WE_UINT8                    ucReserved; /*Conform to 4 bytes*/
} St_WapTlsServerHello;

typedef struct tagSt_WapTlsServerCert
{
    WE_UINT8      ucCertNum;                /*the number of the certificates sent by server*/
    St_SecTlsAsn1Certificate*   pstCert;    /*This is a sequence (chain) of X.509v3 certificates sent by server.*/
    WE_UINT8      aucReserverd[3];          /*conform to 4 bytes*/
} St_WapTlsServerCert;

typedef struct tagSt_WapTlsServerKeyExch
{    
    St_SecTlsKeyExchangeParams    stParams; /*The server key exchange structure defined by SEC,
                                                including the server's key exchange parameters and the server's certificate*/
} St_WapTlsServerKeyExch;

typedef struct tagSt_WapTlsCertReq
{ 
    St_SecTlsDistingubshedName *pstDN;  /*A list of the distinguished names of acceptable certificate authorities.*/
    WE_UCHAR    *pucCertType;           /*This field is a list of the types of certificates requested.*/
    WE_UINT8    ucCertAuthNum;          /*the number of DN returnd by server*/
    WE_UINT8    ucCertTypeNum;          /*the certificate's type count*/
    WE_UINT16   usReserved;             /*conform to 4 bytes*/
} St_WapTlsCertReq;

typedef struct tagSt_WapTlsClientCert
{
    WE_UINT8                    ucCertNum;  /*The number of the certificates that are suitable for server certificate request*/
    St_SecTlsAsn1Certificate    *pstCert;   /*A list of the certificates will send to server.*/
    WE_UINT8      aucReserverd[3];          /*conform to 4 bytes*/
} St_WapTlsClientCert;

typedef struct tagSt_WapTlsCertVerify
{
    WE_UCHAR        *pucSignature;  /*The signature to provide explicit verification of a client certificate.*/
    WE_UINT16       usSiglen;       /*The length of the signature*/
    WE_UINT16       usReserverd;    /*conform to 4 bytes*/
} St_WapTlsCertVerify;

typedef struct tagSt_WapTlsCipherSuiteInfo
{
    WE_UINT16   usCipherSuite;      /*The cipher suite*/
    WE_UINT8    ucExportAble;       /*Whether the cipher algorithm is exportable*/
    WE_UINT8    ucKeyExcMethod;     /*The key exchange method*/
    WE_UINT8    ucCipherAlg;        /*The cipher algorithm*/
    WE_UINT8    ucHashAlg;          /*The hash algorithm*/    
    WE_UINT16   usReserved;         /*conform to 4 bytes*/
} St_WapTlsCipherSuiteInfo;

typedef struct tagSt_WapTlsCipherInfo
{
    WE_UINT8    ucCipherAlg;        /*The cipher algorithm*/
    WE_UINT8    ucExportAble;       /*Whether the cipher algorithm is exportable*/
    WE_UINT8    ucType;             /*Cipher is stream or block*/
    WE_UINT8    ucKeyMaterialLen;   /*The number of bytes from the key_block that are used for generating the write keys*/
    WE_UINT8    ucKeySize;          /*How much entropy material is in the key material being fed into the encryption routines.*/
    WE_UINT8    ucIvSize;           /*How much data needs to be generated for the initialization vector.*/
    WE_UINT8    ucBlockSize;        /*The amount of data a block cipher enciphers in one chunk,
                                        Zero for stream ciphers; equal to the block size for  block ciphers*/
    WE_UINT8    ucReserved;         /*conform to 4 bytes*/
} St_WapTlsCipherInfo;

typedef struct tagSt_WapTlsHashInfo
{
    WE_UINT8 ucHashAlg;             /*Hash algorithm*/
    WE_UINT8 ucHashSize;            /*Hash size*/
    WE_UINT8 ucWhateever;           /*Padding size*/
    WE_UINT8 ucReserved;            /*conform to 4 bytes*/
} St_WapTlsHashInfo;

typedef struct tagSt_WapTlsHandshake
{
    WE_UINT8                ucState;  /*Hand shake state,eg.M_WAP_TLS_HM_STATE_WAIT_SERVER_HELLO*/
    WE_UINT8                ucFlags;  /*Flag of this handshake,eg.M_WAP_TLS_FULL_HANDSHAKE*/
    WE_UINT8                ucSecurityClass;/* Class of security */
    WE_UINT8                ucKeyExcMethod; /*Key exchange algorithm*/
    WE_UINT16               usCipherSuitesLen;/*Length of cipher suites*/
    WE_UINT16               usCipherSuite; /*Cipher suites*/
    WE_UINT16               usPubKeyHashLen;/*Public key hash length, from SEC*/
    WE_UINT16               usKeyExchPubValueLen;/*Data length sent to server side in client key exchange*/
    T_aucWapTlsRandom       taucClientRandom; /*Client random*/
    T_aucWapTlsRandom       taucServerRandom; /*Client random*/
    WE_INT32                iSecurityId;/*Provides a unique indentifier for a connection, be used to 
                                 match requests for session information to correct cache slots*/
    WE_UINT32               uiMsgs; /*Flag of message send to or receive from server side*/
#ifdef WAP_CONFIG_APP_MANAGE_CERTIFICATE
    WE_UINT32               uiAlert; 
    WE_UINT32               uiAlertDesc;
#endif
    WE_UCHAR                *pucCipherSuites; /*cipher suites*/
    WE_UCHAR                *pucPubKeyHash; /*Public key hash, from SEC*/
    WE_UCHAR                *pucKeyExchPubValue;/*Data sent to server side in client key exchange*/
    WE_UCHAR                aucServerVerifyData[12]; /*For verify server finished*/
    WE_UCHAR                aucSSLServerVerifyData[36]; /*For verify server finished*/
    WE_HANDLE               hClientMd5Handle; /*For create client finished*/
    WE_HANDLE               hClientShaHandle;
    WE_HANDLE               hServerMd5Handle; /*For check server finished*/
    WE_HANDLE               hServerShaHandle;
    WE_HANDLE               hVerifyMd5Handle; /*For cert sign*/
    WE_HANDLE               hVerifyShaHandle;
    St_WapTlsSessionId      stSessionId; /*Structure of session id*/
    St_WapTlsServerCert     stServerCert; /*Structure of server cert*/
    St_WapTlsServerKeyExch  stServerKeyExch;/*Structure of server key exchange*/
    St_WapTlsCertReq        stCertReq;     /*Structure of cert request*/   
} St_WapTlsHandshake;




typedef struct tagSt_WapTlsConnState
{
    WE_UINT8                ucCipherAlg;        /*cipher algorithm*/
    WE_UINT8                ucHashAlg;          /*hash algorithm*/
    WE_UINT8                ucHashSize;         /*hash size*/
    WE_UINT8                ucCompressionAlg;   /*compression method*/
    WE_INT32                iSeqnum;            /*The sequence number for this connection*/
    WE_INT32                iMasterSecretId;    /*The identification of the master secret*/
    WE_UCHAR                aucMacSecret[M_WAP_TLS_MAX_HASH_KEY_SIZE];  /*store the MAC secret*/
    WE_UCHAR                aucEncryptionKey[M_WAP_TLS_MAX_KEY_SIZE];   /*store the write_key*/
    WE_UCHAR                aucIV[M_WAP_TLS_MAX_IV_SIZE];               /*Store the IV,for block ciphers only*/
    WE_HANDLE               hCipherHandle;      /*the handle of stream encryption, for stream*/
} St_WapTlsConnState;   

typedef struct tagSt_WapTlsMsgList
{
    struct tagSt_WapTlsMsgList *pstNext;
    St_TlsMsg  stMsg;
} St_WapTlsMsgList;


typedef struct tagSt_WapTlsData
{
    WE_CHAR  *pcBuffer;
    WE_INT32 iBuffLength;
    struct tagSt_WapTlsData *pstNext;       
}St_WapTlsData,*P_St_WapTlsData;
typedef struct tagSt_WapTlsSocketList 
{
    St_WapTlsData   *pstHeader;
    St_WapTlsData   *pstTail;
} St_WapTlsSocketList,P_St_WapTlsSocketList;


typedef struct tagSt_WapTlsConn 
{
    WE_UCHAR            ucState;                /*connection state*/
    WE_UCHAR            ucFlags;                /*connection flags*/
    WE_UCHAR            ucTcpSendReady;         /*TRUE: Can receive data from socket */
    WE_UCHAR            ucHttpSendProcessing;   /*TRUE: Can send Application data*/
    WE_UCHAR            ucHttpSendSelect;       /*determine by last parameter*/
    WE_UCHAR            ucInputState;           /*connection read data state*/
    WE_UCHAR            ucHmInputState;         /*handshake layer read data state*/
    WE_UINT16           usRecvRecStartPos;      /*http receive application data start position*/
    
    M_WAP_TLS_UINT24    tiHmInputMsgLen;        /*handshake layer data length*/
    WE_INT32            iHmInputBytesRequired;  /*the length of data need by handshake layer*/
    WE_INT32            iHmInputBytesRead;      /*the length of handshake layer data already read*/
    WE_INT32            iConnId;         /*connection id*/
    WE_INT32            iCidx;
    WE_INT32            iObjectId;              /*to identify the invoker is TLS, for SEC*/

    WE_INT32            iSecId;
    WE_INT32            iDstInWap;              /**/
    WE_INT32            iInputBytesRequired;    /*bytes requierd to parse*/
    WE_INT32            iInputBytesRead;        /*bytes have read*/
    WE_INT32            iInputBytesLeft;        /*data left in the input buffer*/
    WE_INT32            iInputBufPos;           /*current pos for input buffer*/
    WE_INT32            iOutputBytesLeft;       /*bytes left in the ouput buffer*/
    WE_INT32            iOutputBufPos;          /*current position for output buffer*/
    WE_INT32            iDataSource;            /**/
    WE_UCHAR            aucInputHeaderBuf[M_WAP_TLS_RECORD_HEADER_SIZE];        /*store Record header to parse*/
    WE_UCHAR            aucHmInputHeaderBuf[M_WAP_TLS_HANDSHAKE_HEADER_SIZE];   /*store Handshake header*/
    WE_UCHAR            *pucHmInputMsg;         /*the handshake layer data,without record layer header*/
    WE_UCHAR            *pucInputBuf;           /*used to receive data from socket*/
    WE_UCHAR            *pucOutputBuf;          /*used to send data to socket*/
    
    St_WapTlsProVer         stProtocolVersion;  /*the protocol verison using of this connection*/
    St_WapTlsRecord         stRecvRec;          /*the record layer data have reveived*/
    St_WapTlsConnState      stRead;             /*the read connection state*/
    St_WapTlsConnState      stWrite;            /*the write connection state*/
    St_WapTlsConnState      *pstPendingRead;    /*the pending read connection state*/
    St_WapTlsConnState      *pstPendingWrite;   /*the pending write connection state*/
    St_WapTlsHandshake      *pstHandshake;      /*the handshake information of this connection*/
    St_WapTlsRecord         *pstSendRecs;       /*the record layer data to send to socket*/
    St_WapTlsMsgList        *pstSavedMsgFirst;  /*the first node of TLS message list*/
    St_WapTlsMsgList        *pstSavedMsgLast;   /*the last node of TLS message list*/

    St_WapTlsSocketList     stSocketData; 

    /*store parameter for start handshake*/
    WE_BOOL  bUseProxy;
    WE_UCHAR *pucAddr;
    WE_INT32  iAddrLen;
    WE_UCHAR* pucDomain;
    WE_UINT16  usDomainLen;
    WE_UINT16 usPort;
    WE_INT32  iOverride;
    
    WE_UINT8      ucReserverd;

} St_WapTlsConn;
typedef struct tagSt_WapTlsCertNode
{   
    WE_INT32 iCertType;
    WE_INT32 iLength;     /* the length of pvBuffer */
    WE_VOID* pvBuffer;  /* pointer to certificate */
    struct tagSt_WapTlsCertNode* pstNext;
}St_WapTlsCertNode;
typedef struct tagSt_WapTlsCertList
{                                                                                                                                                    
    WE_INT32 iNew;     /* TRUE/FALSE */
    St_WapTlsCertNode* pstHead;    
}St_WapTlsCertList;

#ifdef WAP_TLS_SUPPORT_VIEW_SVR_CERT
typedef struct tagSt_WapTlsSvrCert{
 WE_CHAR *pcHost;
 WE_VOID *pvCertContent;
 WE_UINT32 uiCertLen;
}St_WapTlsSvrCert;
#endif

typedef struct tagSt_WapTlsHandle                               
{ 
    WE_HANDLE       hMsgHandle;                 /*store message handle*/
    WE_HANDLE       hSecHandle;                 /*store SEC module handle*/
    WE_HANDLE       hLibAlgHandle;              /*store the algorithm handle*/
    WE_INT32        iOpenConnNumber;            /*store the number of opened connection */
    St_WapTlsConn  *apstWapTlsConnTable[G_WAP_CFG_MAX_CONNECTIONS]; /*TLS connection table*/
    St_WapTlsCipherSuiteInfo    *pstCipherSuTab;                    /*Store the cipher suite table*/
    St_WapTlsCipherInfo         *pstCipherInfoTab;                  /*Store the cipher Info table*/
    St_WapTlsHashInfo           *pstHashInfoTab;                    /*Store the Hash Info table*/
    
    WE_HANDLE   hPrivData; /*for app callback*/
    WE_HANDLE   hcbRespunc;/*app callback*/
    St_WapTlsCertList *pstRootCert;/*root cert list*/
#ifdef WAP_TLS_SUPPORT_USER_ASSIGN_PRO_VERSION
    WE_UINT8     ucVerMinor;
    WE_UINT8     ucVerMajor;
    WE_UINT16    usReserverd;
#endif    
#ifdef WAP_TLS_SUPPORT_VIEW_SVR_CERT
        St_WapTlsSvrCert * apstServerCert[MAX_SERVER_CERT_NUM];
#endif

}St_WapTlsHandle;

#endif
