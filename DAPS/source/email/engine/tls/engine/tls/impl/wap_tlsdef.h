/*==================================================================================================
    FILE NAME   : wap_tlsdef.h
    MODULE NAME : TLS

    GENERAL DESCRIPTION
        Define tls marco.
    
    TechFaith Software Confidential Proprietary
    Copyright (c) 2006-2007 by TechFaith Software. All Rights Reserved.
====================================================================================================
    Revision History

    Modification Tracking
    Date        Author        Number       Description of changes
    --------------------------------------------------------------------------------
    2007-06-28  wangshuhua      None         Initialize   
==================================================================================================*/
    
#ifndef WAP_TLS_DEF_H
#define WAP_TLS_DEF_H

/***************************************************************************************************
*   Macro Define Section
***************************************************************************************************/
/*Connection State*/
#define M_WAP_TLS_STATE_NULL                            0x00 /*Init state*/
#define M_WAP_TLS_STATE_HANDSHAKE_INITIATED             0x01 /*Prepare handshake*/
#define M_WAP_TLS_STATE_HANDSHAKE_IN_PROGRESS           0x02 /*Handshake is in process*/
#define M_WAP_TLS_STATE_OPEN                            0x03 /*after finished handshake*/
#define M_WAP_TLS_STATE_SHUTDOWN                        0x04 /*socket shut down*/
#define M_WAP_TLS_STATE_CLOSING                         0x05  /*socket error*/

/*TLS Data process state*/
#define M_WAP_TLS_INPUT_STATE_NULL                      0 /*Init state*/
#define M_WAP_TLS_INPUT_STATE_START                     1 /*Start read*/
#define M_WAP_TLS_INPUT_STATE_READING_HEADER            2 /*Get Header according to the header size define in protocol*/
#define M_WAP_TLS_INPUT_STATE_READING_DATA              3 /*Get data according to the data size define in header*/
#define M_WAP_TLS_INPUT_STATE_WAIT_CCS_PROCESSED        4 /*Wait Server CCS process or client CCS send*/
#define M_WAP_TLS_INPUT_STATE_WAIT_HTTP_READY           5 /*Wait Http read data from TLS module*/
/*************************************************************************************************
  TLS HM
***************************************************************************************************/
/*TLS HM Handshake data process state*/
#define M_WAP_TLS_HM_INPUT_STATE_START                  1 /*Start read*/
#define M_WAP_TLS_HM_INPUT_STATE_READING_HEADER         2 /*Get header according to the header size define in protocol*/
#define M_WAP_TLS_HM_INPUT_STATE_READING_DATA           3 /*Get data according to the data size define in header*/

/*TLS HM handshake state*/
#define M_WAP_TLS_HM_STATE_WAIT_SERVER_HELLO            0x01 /*Wait server Hello, after send client hello*/
#define M_WAP_TLS_HM_STATE_WAIT_CERTIFICATE             0x02 /*Wait server cert, after receive server hello during full handshake*/
#define M_WAP_TLS_HM_STATE_WAIT_SERVER_KEY_EXCHANGE     0x03 /*Wait server key exchange, after receive cert during full handshake*/
#define M_WAP_TLS_HM_STATE_WAIT_CERTIFICATE_REQUEST     0x04 /*Wait cert request, after receive server key exchange during full handshake*/
#define M_WAP_TLS_HM_STATE_WAIT_SERVER_HELLO_DONE       0x05 /*Wait server hello done,after receive cert request during full handshake*/
#define M_WAP_TLS_HM_STATE_WAIT_SERVER_CCS              0x06 /*Wait server ccs, after send client finished during full handshake
                                                               after receive server hello during simple handshake*/
#define M_WAP_TLS_HM_STATE_WAIT_FINISHED                0x07 /*Wait server finished, after Receive server ccs */

/*state wait sec response*/
#define M_WAP_TLS_HM_STATE_WAIT_PEER_LOOKUP_RESP        0x10 /*Wait search peer response*/
#define M_WAP_TLS_HM_STATE_WAIT_CERT_VERIFY_RESULT      0x11 /*Wait cert verify response*/
#define M_WAP_TLS_HM_STATE_WAIT_CREATE_KEY_EXCH_RESP    0x12 /*Wait create key exchange response*/
#define M_WAP_TLS_HM_STATE_WAIT_GET_CLIENT_CERT_RESP    0x13 /*Wait get client cert response*/
#define M_WAP_TLS_HM_STATE_WAIT_CERT_SIGNATURE_RESP     0x14 /*Wait cert signature response*/
#define M_WAP_TLS_HM_STATE_WAIT_SESSION_FETCH_RESP      0x15 /*Wait fetch session response*/
#define M_WAP_TLS_HM_STATE_WAIT_GET_CIPHER_SUITE_RESP   0x16 /*Wait get cipher suites response*/
/*Create MAC Secret,IV,encrypt key and decrypt key*/
#define M_WAP_TLS_HM_STATE_CREATE_KEYEXPANS             0x17 /*Wait calc key_block response*/
#define M_WAP_TLS_HM_STATE_CREATE_SWKEY                 0x18 /*Wait calc final_client_write_key respinse*/
#define M_WAP_TLS_HM_STATE_CREATE_CWKEY                 0x19 /*Wait calc final_server_write_key respinse*/
#define M_WAP_TLS_HM_STATE_CREATE_IVBLOCK               0x1a /*Wait calc IV_block respinse*/

#define M_WAP_TLS_HM_STATE_WAIT_CCS_PROCESSED           0x1b /*Wait server CCS processed or send client CCS*/
#define M_WAP_TLS_HM_STATE_CHECK_SERVER_FINISHED        0x1c /*After receive server finished*/
#define M_WAP_TLS_HM_STATE_CREATE_CLIENT_FINISHED       0x1d /*Before send client finished*/
#define M_WAP_TLS_HM_STATE_WAIT_PEER_LOOKUP_RESP_1      0x1e /*For make sure SSL will search peer once only*/

#define M_WAP_TLS_HM_SSL_WAIT_FOR_CREATE_KEY            0x1f 
#define M_WAP_TLS_HM_SSL_WAIT_FOR_CREATE_CLIENT_CERT_VERIFY            0x20  
#define M_WAP_TLS_HM_SSL_WAIT_FOR_CREATE_CLIENT_FINISHED         0x21
#define M_WAP_TLS_HM_SSL_WAIT_FOR_PROC_SERVER_FINISHED     0x22

/*State wait client choose*/
#define M_WAP_TLS_HM_WAIT_CLIENT_CONFIRM     0x23
#define M_WAP_TLS_HM_WAIT_CLIENT_USER_CERT   0x24


/*Alert Level*/
#define M_WAP_TLS_ALERT_LEVEL_WARNING                   1 /*Waring level*/
#define M_WAP_TLS_ALERT_LEVEL_FATAL                     2 /*Fatal level*/

/*Alert Description*/
#define M_WAP_TLS_ALERT_DESC_CLOSE_NOTIFY               0
#define M_WAP_TLS_ALERT_DESC_UNEXPECTED_MESSAGE         10
#define M_WAP_TLS_ALERT_DESC_BAD_RECORD_MAC             20
#define M_WAP_TLS_ALERT_DESC_DECRYPTION_FAILED          21
#define M_WAP_TLS_ALERT_DESC_RECORD_OVERFLOW            22
#define M_WAP_TLS_ALERT_DESC_DECOMPRESSION_FAILURE      30
#define M_WAP_TLS_ALERT_DESC_HANDSHAKE_FAILURE          40
#define M_WAP_SSL_ALERT_DESC_NO_CERTIFICATE             41
#define M_WAP_TLS_ALERT_DESC_BAD_CERTIFICATE            42
#define M_WAP_TLS_ALERT_DESC_UNSUPPORTED_CERTIFICATE    43
#define M_WAP_TLS_ALERT_DESC_CERTIFICATE_REVOKED        44
#define M_WAP_TLS_ALERT_DESC_CERTIFICATE_EXPIRED        45
#define M_WAP_TLS_ALERT_DESC_CERTIFICATE_UNKNOWN        46
#define M_WAP_TLS_ALERT_DESC_ILLEGAL_PARAMETER          47
#define M_WAP_TLS_ALERT_DESC_UNKNOWN_CA                 48
#define M_WAP_TLS_ALERT_DESC_ACCESS_DENIED              49
#define M_WAP_TLS_ALERT_DESC_DECODE_ERROR               50
#define M_WAP_TLS_ALERT_DESC_DECRYPT_ERROR              51
#define M_WAP_TLS_ALERT_DESC_EXPORT_RESTRICTION         60
#define M_WAP_TLS_ALERT_DESC_PROTOCOL_VERSION           70
#define M_WAP_TLS_ALERT_DESC_INSUFFICIENT_SECURITY      71
#define M_WAP_TLS_ALERT_DESC_INTERNAL_ERROR             80
#define M_WAP_TLS_ALERT_DESC_USER_CANCELED              90
#define M_WAP_TLS_ALERT_DESC_NO_RENEGOTIATION           100

/*Content Type*/
#define M_WAP_TLS_CONTENT_TYPE_CCS                      20  /*Change Cipher Spec*/
#define M_WAP_TLS_CONTENT_TYPE_ALERT                    21  /*Alert PDU*/
#define M_WAP_TLS_CONTENT_TYPE_HANDSHAKE                22   /*HandShake*/
#define M_WAP_TLS_CONTENT_TYPE_DATA                     23   /*Application*/

/*HandShake Type*/
#define M_WAP_TLS_HANDSHK_HELLO_REQUEST                 0 
#define M_WAP_TLS_HANDSHK_CLIENT_HELLO                  1
#define M_WAP_TLS_HANDSHK_SERVER_HELLO                  2
#define M_WAP_TLS_HANDSHK_CERTIFICATE                   11
#define M_WAP_TLS_HANDSHK_SERVER_KEY_EXCHANGE           12
#define M_WAP_TLS_HANDSHK_CERTIFICATE_REQUEST           13
#define M_WAP_TLS_HANDSHK_SERVER_HELLO_DONE             14
#define M_WAP_TLS_HANDSHK_CERTIFICATE_VERIFY            15
#define M_WAP_TLS_HANDSHK_CLIENT_KEY_EXCHANGE           16
#define M_WAP_TLS_HANDSHK_FINISHED                      20

/*Flag*/
#define M_WAP_TLS_FLAG_FULL_HANDSHAKE                   0x01 /*Perform a full handshake*/
#define M_WAP_TLS_FLAG_NEED_CERT_VERIFY                 0x02 /*Need cert verify PDU*/
#define M_WAP_TLS_FLAG_IS_SSL                           0x04 /*SSL Session*/
#define M_WAP_TLS_FLAG_SSL_CLIENT_HELLO                 0x08 /*SSL Session*/
#define M_WAP_TLS_FLAG_ESTABLISHED                      0x10 /*Finish handshake*/
#define M_WAP_TLS_FLAG_LAST_PACKET                      0x20

/*Mark handshake data type*/
#define M_BIT_CLIENT_HELLO                              0x1   /*ClientHello*/
#define M_BIT_SERVER_HELLO                              0x2   /*ServerHello*/
#define M_BIT_SERVER_CERTIFICATE                        0x4   /*Certificate*/
#define M_BIT_SERVER_KEY_EXCHANGE                       0x8   /*ServerKeyExchange*/
#define M_BIT_SERVER_CERTIFICATE_REQUEST                0x10  /*CertificateRequest*/
#define M_BIT_SERVER_HELLO_DONE                         0x20  /*ServerHelloDone*/
#define M_BIT_CLIENT_CERTIFICATE                        0x40  /*Certificate*/
#define M_BIT_CLIENT_KEY_EXCHANGE                       0x80  /*ClientKeyExchange*/
#define M_BIT_CLIENT_CERTIFICATE_VERIFY                 0x100 /*ChangeCipherSpec*/
#define M_BIT_CLIENT_FINISHED                           0x200 /*Finished*/
#define M_BIT_SERVER_FINISHED                           0x400 /*Finished*/

/*Cipher algorithm*/
#define M_WAP_TLS_CIPHER_NULL                           G_SEC_ALG_CIPHER_NULL
#define M_WAP_TLS_CIPHER_IDEA_CBC                       G_SEC_ALG_CIPHER_IDEA_CBC
#define M_WAP_TLS_CIPHER_RC2_CBC_40                     G_SEC_ALG_CIPHER_RC2_CBC_40
#define M_WAP_TLS_CIPHER_RC4_40                         G_SEC_ALG_CIPHER_RC4_40
#define M_WAP_TLS_CIPHER_RC4_128                        G_SEC_ALG_CIPHER_RC4_128
#define M_WAP_TLS_CIPHER_DES40_CBC                      G_SEC_ALG_CIPHER_DES_CBC_40
#define M_WAP_TLS_CIPHER_DES_CBC                        G_SEC_ALG_CIPHER_DES_CBC
#define M_WAP_TLS_CIPHER_3DES_EDE_CBC                   G_SEC_ALG_CIPHER_3DES_CBC_EDE

/*Hash algorithm*/
#define M_WAP_TLS_HASH_NULL                             G_SEC_HASH_NULL
#define M_WAP_TLS_HASH_MD5                              G_SEC_HASH_MD5
#define M_WAP_TLS_HASH_SHA                              G_SEC_HASH_SHA

/*Compress algorithm*/
#define M_WAP_TLS_COMPRESSION_NULL                      0

/*Cipher type*/
#define M_WAP_TLS_CIPHER_TYPE_STREAM                   1
#define M_WAP_TLS_CIPHER_TYPE_BLOCK                    2

/*Cipher parameter*/
#define M_WAP_TLS_MAX_KEY_SIZE                         24/*Max size of encrypt key*/
#define M_WAP_TLS_MAX_IV_SIZE                          8 /*Max size of IV*/
#define M_WAP_TLS_MAX_HASH_SIZE                        20 /*Max value of hash value*/
#define M_WAP_TLS_MAX_HASH_KEY_SIZE                    20 /*Max size of MAC secret*/

#define M_WAP_TLS_BUF_SIZE                             1024*5 /*Max size of buffer to store data from socket*/
#define M_WAP_TLS_RECORD_HEADER_SIZE                   5 /*1 content-type 2 version,2 length*/
#define M_WAP_TLS_HANDSHAKE_HEADER_SIZE                4 /*1 handshke-type 3 length*/

/*Protocol version*/
#define M_WAP_TLS_PROTOCOL_VERSION_MAJOR                3   /*the major protocol version of TLS*/
#define M_WAP_TLS_PROTOCOL_VERSION_MINOR                1   /*the minor protocol version of TLS*/

#define M_WAP_TLS_UINT24                              WE_UINT32

#define M_WAP_TLS_CONN_GET(handle, cidx) \
       ((((cidx) < 0) || ((cidx) >= G_WAP_CFG_MAX_CONNECTIONS)) ? NULL : handle->apstWapTlsConnTable[cidx])




#endif
