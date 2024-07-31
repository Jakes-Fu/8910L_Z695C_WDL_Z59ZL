#ifndef _DH_LIB_H
#define _DH_LIB_H

#ifdef M_SEC_CFG_DHE_RSA

WE_INT32 SEClib_generateDHKeyPair(WE_HANDLE   hSecHandle,WE_UCHAR *pucDhP, WE_INT32 iDhPLen,
                                  WE_UCHAR *pucDhG, WE_INT32 iDhGLen, WE_UCHAR *pucDhYs, WE_INT32 iDhYsLen, 
                                  WE_UCHAR **ppucClientPubKey, WE_INT32 *piClientPubkeyLen,
                                  WE_UCHAR **ppucSharedKey, WE_INT32 *piSharedKeyLen );
#endif
#endif

