/*==================================================================================================
    FILE NAME : sec_uecst.h
    MODULE NAME : SEC
    
    GENERAL DESCRIPTION
    define UE event
        
    TechFaith Software Confidential Proprietary
    Copyright (c) 2006-2007  by TechFaith Software. All Rights Reserved.
====================================================================================================
    Revision History
    
    Modification                           Tracking
    Date            Author                Number      Description of changes
    ---------- -------------- --------- --------------------------------------
    2006-07-22      Bird                                create sec_uecst.h
    
==================================================================================================*/

/***************************************************************************************************
*   multi-Include-Prevent Section
***************************************************************************************************/
#ifndef SEC_UECST_H
#define SEC_UECST_H

/*Move from I interface head by bird 20061208*/
/***************************************************************************************************
*   Macro define section 
***************************************************************************************************/
/********************************************
*   Button string constants
********************************************/
/*"Ok"*/
#define G_SEC_ID_OK                                   ( 0x00) 
/*"Cancel"*/
#define G_SEC_ID_CANCEL                           ( 0x01) 
/*" "*/
#define G_SEC_ID_NONE                               ( 0x03) 

/********************************************
*   Warning constrants
********************************************/
/*"Not enough memory to perform."*/
#define G_SEC_ID_WARNING_NOTENOUGH_MEMORY   ( 0x05) 
/*"An error occured."*/
#define G_SEC_ID_WARNING_GENERAL_ERROR       ( 0x06) 
/*"Wrong input parameter."*/
#define G_SEC_ID_WARNING_WRONG_PARAMETER            ( 0x07) 

#ifndef M_SEC_CFG_CONN_WITHOUT_CERT_UE

#define G_Evt_WARNING       ( 0x34 )

/************************************************
*   confirmation constants 
************************************************/
/* "No CA certificate is found. Connect without server or client 
   authentication?" */
#define G_SEC_ID_CONFIRM_CONNECT_NO_AUTHENTICATION       ( 0x0e) 

/* "No matched CA certificate is found. Continue?" */
#define G_SEC_ID_CONFIRM_NO_MATCHED_CA_CERT              ( 0x10) 

/* "Error in certificate. Continue?" */
#define G_SEC_ID_CONFIRM_ERROR_IN_CERT                   ( 0x11) 

/* "Certificate is expired. Continue?" */
#define G_SEC_ID_CONFIRM_EXPIRED_CERT                    ( 0x12) 

/* "No certificate is received from gateway. Connect without gateway 
   or client authentication?" */
#define G_SEC_ID_CONFIRM_NO_GATEWAY_CERT                 ( 0x13) 

#endif

/********************************************
*   Title 
********************************************/
/* Button constants */
#define M_SEC_ID_OK             G_SEC_ID_OK
#define M_SEC_ID_CANCEL         G_SEC_ID_CANCEL
#define M_SEC_ID_NONE           G_SEC_ID_NONE


#ifndef M_SEC_CFG_CONN_WITHOUT_CERT_UE
/* confirmation constants */
#define M_SEC_ID_CONFIRM_1       G_SEC_ID_CONFIRM_CONNECT_NO_AUTHENTICATION
#define M_SEC_ID_CONFIRM_8       G_SEC_ID_CONFIRM_NO_MATCHED_CA_CERT
#define M_SEC_ID_CONFIRM_10     G_SEC_ID_CONFIRM_ERROR_IN_CERT
#define M_SEC_ID_CONFIRM_11     G_SEC_ID_CONFIRM_EXPIRED_CERT
#define M_SEC_ID_CONFIRM_12     G_SEC_ID_CONFIRM_NO_GATEWAY_CERT
#endif

/* Session  */
#define M_SEC_ID_SESSION_WTLS       G_SEC_ID_SESSION_WTLS  

#define M_SEC_ID_SESSION_MD5        G_SEC_ID_SESSION_MD5
#define M_SEC_ID_SESSION_SHA1       G_SEC_ID_SESSION_SHA1

#define M_SEC_ID_SESSION_BULK_NULL            G_SEC_ID_SESSION_BULK_NULL
#define M_SEC_ID_SESSION_BULK_RC5              G_SEC_ID_SESSION_BULK_RC5
#define M_SEC_ID_SESSION_BULK_RC5_40        G_SEC_ID_SESSION_BULK_RC5_40
#define M_SEC_ID_SESSION_BULK_RC5_56        G_SEC_ID_SESSION_BULK_RC5_56
#define M_SEC_ID_SESSION_BULK_RC5_64        G_SEC_ID_SESSION_BULK_RC5_64
#define M_SEC_ID_SESSION_BULK_3DES            G_SEC_ID_SESSION_BULK_3DES
 
#define M_SEC_ID_SESSION_KEYEXCH_NULL               G_SEC_ID_SESSION_KEYEXCH_NULL
#define M_SEC_ID_SESSION_KEYEXCH_RSA_ANON           G_SEC_ID_SESSION_KEYEXCH_RSA_ANON
#define M_SEC_ID_SESSION_KEYEXCH_RSA_ANON_512       G_SEC_ID_SESSION_KEYEXCH_RSA_ANON_512
#define M_SEC_ID_SESSION_KEYEXCH_RSA_ANON_768       G_SEC_ID_SESSION_KEYEXCH_RSA_ANON_768
#define M_SEC_ID_SESSION_KEYEXCH_RSA                G_SEC_ID_SESSION_KEYEXCH_RSA
#define M_SEC_ID_SESSION_KEYEXCH_RSA_512            G_SEC_ID_SESSION_KEYEXCH_RSA_512
#define M_SEC_ID_SESSION_KEYEXCH_RSA_768            G_SEC_ID_SESSION_KEYEXCH_RSA_768

#ifndef M_SEC_CFG_CONN_WITHOUT_CERT_UE
/* Warning constrants */
#define M_SEC_ID_WARNING_INSUFFICIENT_MEMORY        G_SEC_ID_WARNING_NOTENOUGH_MEMORY
#define M_SEC_ID_WARNING_GENERAL_ERROR              G_SEC_ID_WARNING_GENERAL_ERROR
#define M_SEC_ID_WARNING_INVALID_PARAMETER          G_SEC_ID_WARNING_WRONG_PARAMETER 
#endif

/* for using the new format:"dd/mm/yyyy" */
/* Month  */
/*"January"*/
#define M_SEC_ID_JAN                (1) 
/*"February"*/
#define M_SEC_ID_FEB                (2) 
/*"March"*/
#define M_SEC_ID_MARCH           (3) 
/*"April"*/
#define M_SEC_ID_APR                (4) 
/*"May"*/
#define M_SEC_ID_MAY                (5) 
/*"June"*/
#define M_SEC_ID_JUNE               (6) 
/*"July"*/
#define M_SEC_ID_JULY               (7)
/*"August"*/ 
#define M_SEC_ID_AUG                (8) 
/*"September"*/
#define M_SEC_ID_SEP                (9) 
/*"October"*/
#define M_SEC_ID_OCT                (10) 
/*"November"*/
#define M_SEC_ID_NOV                (11) 
/*"December"*/
#define M_SEC_ID_DEC                (12) 

#ifndef M_SEC_CFG_CONN_WITHOUT_CERT_UE
/***************************************************************************************************
*   type define section 
***************************************************************************************************/
typedef struct tagSt_Warning
{
    WE_INT32 iWarningContent;
}St_Warning;

#endif

#endif
