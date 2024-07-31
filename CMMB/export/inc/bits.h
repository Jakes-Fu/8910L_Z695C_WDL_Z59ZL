/******************************************************************************
 ** File Name:      bits.h                                                    *
 ** Author:        					                                                  *
 ** Date:           2010/11/13                                                *
 ** Copyright:      2010 Spreadtrum, Incoporated. All Rights Reserved.        *
 ** Description:    define memory bits and bytes operations                   *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ******************************************************************************/

#ifndef _BITS_H_
#define _BITS_H_

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif
#if 0
#ifndef max
#define max(a,b)    (((a) > (b)) ? (a) : (b))
#endif

#ifndef min
#define min(a,b)    (((a) < (b)) ? (a) : (b))
#endif
#endif
//p is the pointer to a byte stream
//v is the value to write
#define VAL8(p)				((p)[0])
#define VAL16(p)			((VAL8(p)<<8)+VAL8((p)+1))
#define VAL24(p)			((VAL16(p)<<8)+VAL8((p)+2))
#define VAL32(p)			((VAL16(p)<<16)+VAL16((p)+2))
#define PUT8(p,v)			do{(p)[0]=(unsigned char)(v);}while(0)
#define PUT16(p,v)			do{PUT8((p),(v)>>8);PUT8((p)+1,(v));}while(0)
#define PUT24(p,v)			do{PUT16((p),(v)>>8);PUT8((p)+2,(v));}while(0)
#define PUT32(p,v)			do{PUT16((p),(v)>>16);PUT16((p)+2,(v));}while(0)

//p is the pointer to a byte stream
//s is the start bit, in LSB, ie. BIT_VAL8(p,0,2) will result in p[0]=3;
//n is the bit count
//v is the value to write
#if 0//for the silly lint e506
#define REALS(s,width)              min(max((s),0),(width-1))      // REALS is between 0 to width-1
#define REALN(s,n,width)            min((width)-REALS(s,width),max((n), 0)) // REALN is between 0 to width-REALS
#else
#define REALS(s,width)              s
#define REALN(s,n,width)            n
#endif
#define BIT_VAL8(p,s,n)             ((VAL8(p) >> REALS(s,8)) & (0xFF >> (8-REALN(s,n,8))))
#define BIT_VAL16(p,s,n)            ((VAL16(p) >> REALS(s,16)) & (0xFFFF >> (16-REALN(s,n,16))))
#define BIT_VAL32(p,s,n)            ((VAL32(p) >> REALS(s,32)) & (0xFFFFFFFF >> (32-REALN(s,n,32))))

#define BIT_PUT8(p,s,n,v)  \
do { \
    if (n > 0) \
    { \
        unsigned char val = VAL8(p); \
        unsigned char msk = 0xFF >> (8-REALN(s,n,8)); \
        SCI_ASSERT((v) <= msk); \
        val &= ~(msk<<REALS(s,8)); \
        val |= (v)<<REALS(s,8); \
        PUT8(p,val); \
    } \
} while(0)

#define BIT_PUT16(p,s,n,v)  \
do { \
    if (n > 0) \
    { \
        unsigned short val = VAL16(p); \
        unsigned short msk = 0xFFFF >> (16-REALN(s,n,16)); \
        SCI_ASSERT((v) <= msk); \
        val &= ~(msk<<REALS(s,16)); \
        val |= (v)<<REALS(s,16); \
        PUT16(p,val); \
    } \
} while(0)

#define BIT_PUT32(p,s,n,v)  \
do { \
    if (n > 0) \
    { \
        unsigned int val = VAL32(p); \
        unsigned int msk = 0xFFFFFFFF >> (32-REALN(s,n,32)); \
        SCI_ASSERT((v) <= msk); \
        val &= ~(msk<<REALS(s,32)); \
        val |= (v)<<REALS(s,32); \
        PUT32(p,val); \
    } \
} while(0)


/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    }
#endif

#endif /*_DAL_DV_SC6800AD_H_*/

