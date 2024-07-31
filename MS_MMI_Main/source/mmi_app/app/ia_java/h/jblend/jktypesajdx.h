/*
 *   Copyright 2009 Aplix Corporation. All rights reserved.
 */
#ifndef ___JKTYPESAJDX_H
#define ___JKTYPESAJDX_H

#ifdef __cplusplus
extern "C" {
#endif

/* Ignore alpha data */
#define AJDX_TRANSPARENT_NONE       0

/* 2-level alpha: visible or invisible */
#define AJDX_TRANSPARENT_FULL       1

/* 256-level alpha blending */
#define AJDX_TRANSPARENT_ALPHA      2

typedef JKUint16  AJDX_PIXEL;


typedef struct
{
    AJDX_PIXEL      *data;
    JKSint          width;
    JKSint          height;
    JKSint          transType; /* AJDX_TRANSPARENT_XXX */
    JKBool          isMutable;
} AJDX_IMAGE;

typedef struct
{
    JKUint32        *data;
    JKSint          width;
    JKSint          height;
    JKSint          transType; /* AJDX_TRANSPARENT_XXX */
    JKBool          isMutable;
} AJDX_ARGB_IMAGE;

typedef struct
{
    JKSint32        x;
    JKSint32        y;
    JKSint32        width;
    JKSint32        height;
} AJDX_CLIP;




#define CONST_CONFIG_TYPE_POLYCOM_POINT_STRUCT_SIZE (1)


#ifdef __cplusplus
}
#endif

#endif /* ___JKTYPESAJDX_H */

