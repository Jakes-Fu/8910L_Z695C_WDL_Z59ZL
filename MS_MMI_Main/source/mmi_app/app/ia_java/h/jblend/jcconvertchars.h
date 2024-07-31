/*
 * Copyright 2004 Aplix Corporation. All rights reserved.
 */
/**
 * @file JcConvertChars.h
 */

#ifndef ___JC_CONVERT_CHARS_H
#define ___JC_CONVERT_CHARS_H

#include "jktypes.h"

#ifdef __cplusplus
extern "C" {
#endif


/**
 * 文字エンコーディング形式として、UTF-8 を指定する
 */
#define JC_UTF8         0

/**
 * 文字エンコーディング形式として、ISO-8859-1 を指定する
 */
#define JC_ISO8859_1    1

/**
 * 文字エンコーディング形式として、ISO-8859-2 を指定する
 */
#define JC_ISO8859_2    2

/**
 * 文字エンコーディング形式として、ISO-8859-3 を指定する
 */
#define JC_ISO8859_3    3

/**
 * 文字エンコーディング形式として、ISO-8859-4 を指定する
 */
#define JC_ISO8859_4    4

/**
 * 文字エンコーディング形式として、ISO-8859-5 を指定する
 */
#define JC_ISO8859_5    5

/**
 * 文字エンコーディング形式として、ISO-8859-6 を指定する
 */
#define JC_ISO8859_6    6

/**
 * 文字エンコーディング形式として、ISO-8859-7 を指定する
 */
#define JC_ISO8859_7    7

/**
 * 文字エンコーディング形式として、ISO-8859-8 を指定する
 */
#define JC_ISO8859_8    8

/**
 * 文字エンコーディング形式として、ISO-8859-9 を指定する
 */
#define JC_ISO8859_9    9

/**
 * 文字エンコーディング形式として、ISO-8859-13 を指定する
 */
#define JC_ISO8859_13  13

/**
 * 文字エンコーディング形式として、ISO-8859-15 を指定する
 */
#define JC_ISO8859_15  15

/**
 * 文字エンコーディング形式として、SJIS を指定する
 */
#define JC_SJIS        20

/**
 * 文字エンコーディング形式として、CP950 を指定する
 */
#define JC_CP950       21

/**
 * 文字エンコーディング形式として、BIG5 を指定する
 */
#define JC_BIG5        22

/**
 * 文字エンコーディング形式として、EUC-CN を指定する
 */
#define JC_EUC_CN      23

/**
 * 文字エンコーディング形式として、TIS620 を指定する
 */
#define JC_TIS620      24


/**
 */
#define JC_INSUFFICIENT_BUFFER   -1

/**
 */
#define JC_ILLEGAL_ARGUMENT      -2

/**
 */
#define JC_UNSUPPORTED_ENCODING  -3


/**
 *
 */
JKSint32  JcConvertCharUnicodeTo( JKSint32 charset_id,
                                  JKWChar unicode,
                                  JKUint8* out, JKSint32 outBytes );

/**
 *
 */
JKSint32  JcConvertCharUnicodeFrom( JKSint32 charset_id,
                                    const JKUint8* in, JKSint32 inBytes,
                                    JKWChar* unicode );


/**
 *
 */
JKSint32 JcGetStringBytesUnicodeTo( JKSint32 charset_id,
                                    const JKWChar* in, JKSint32 inChars,
                                    const JKUint8* substitute, JKSint32 substituteLen );


/**
 *
 */
JKSint32 JcGetCharCountUnicodeFrom( JKSint32 charset_id,
                                    const JKUint8* in, JKSint32 inBytes,
                                    JKWChar substitute );

/**
 *
 */
JKSint32 JcConvertStringUnicodeTo( JKSint32 charset_id,
                                   const JKWChar* in, JKSint32 inChars,
                                   JKUint8* out, JKSint32 outBytes,
                                   JKSint32* outNextOff, JKSint32* badChars,
                                   const JKUint8* substitute, JKSint32 substituteLen );


/**
 *
 */
JKSint32 JcConvertStringUnicodeFrom( JKSint32 charset_id,
                                     const JKUint8* in, JKSint32 inBytes,
                                     JKWChar* out, JKSint32 outChars,
                                     JKSint32* outNextOff, JKUint8* saveState, JKSint32* badChars,
                                     JKWChar substitute );



#ifdef __cplusplus
}
#endif

#endif /* ___JC_CONVERT_CHARS_H */

