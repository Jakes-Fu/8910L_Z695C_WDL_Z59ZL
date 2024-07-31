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
 * �����G���R�[�f�B���O�`���Ƃ��āAUTF-8 ���w�肷��
 */
#define JC_UTF8         0

/**
 * �����G���R�[�f�B���O�`���Ƃ��āAISO-8859-1 ���w�肷��
 */
#define JC_ISO8859_1    1

/**
 * �����G���R�[�f�B���O�`���Ƃ��āAISO-8859-2 ���w�肷��
 */
#define JC_ISO8859_2    2

/**
 * �����G���R�[�f�B���O�`���Ƃ��āAISO-8859-3 ���w�肷��
 */
#define JC_ISO8859_3    3

/**
 * �����G���R�[�f�B���O�`���Ƃ��āAISO-8859-4 ���w�肷��
 */
#define JC_ISO8859_4    4

/**
 * �����G���R�[�f�B���O�`���Ƃ��āAISO-8859-5 ���w�肷��
 */
#define JC_ISO8859_5    5

/**
 * �����G���R�[�f�B���O�`���Ƃ��āAISO-8859-6 ���w�肷��
 */
#define JC_ISO8859_6    6

/**
 * �����G���R�[�f�B���O�`���Ƃ��āAISO-8859-7 ���w�肷��
 */
#define JC_ISO8859_7    7

/**
 * �����G���R�[�f�B���O�`���Ƃ��āAISO-8859-8 ���w�肷��
 */
#define JC_ISO8859_8    8

/**
 * �����G���R�[�f�B���O�`���Ƃ��āAISO-8859-9 ���w�肷��
 */
#define JC_ISO8859_9    9

/**
 * �����G���R�[�f�B���O�`���Ƃ��āAISO-8859-13 ���w�肷��
 */
#define JC_ISO8859_13  13

/**
 * �����G���R�[�f�B���O�`���Ƃ��āAISO-8859-15 ���w�肷��
 */
#define JC_ISO8859_15  15

/**
 * �����G���R�[�f�B���O�`���Ƃ��āASJIS ���w�肷��
 */
#define JC_SJIS        20

/**
 * �����G���R�[�f�B���O�`���Ƃ��āACP950 ���w�肷��
 */
#define JC_CP950       21

/**
 * �����G���R�[�f�B���O�`���Ƃ��āABIG5 ���w�肷��
 */
#define JC_BIG5        22

/**
 * �����G���R�[�f�B���O�`���Ƃ��āAEUC-CN ���w�肷��
 */
#define JC_EUC_CN      23

/**
 * �����G���R�[�f�B���O�`���Ƃ��āATIS620 ���w�肷��
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

