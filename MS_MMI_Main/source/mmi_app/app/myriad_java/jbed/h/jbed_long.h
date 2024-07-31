/*
 * Copyright 2001-2008 Esmertec AG. All Rights Reserved.
 * $Id$
 */

/**
 * @file
 * Jbed Porting Layer: 64-bit operations (VM-independent)
 *
 * This file defines macros that allow 64 bit operations to be
 * performed on C compilers that that may have or may not have
 * 64 bit primitive integer types.
 *
 * Also uses or defines COMPILER_SUPPORTS_LONG and NEED_LONG_ALIGNMENT for
 * KVM compatibility.
 */

#ifndef _JBED_LONG_INCLUDED_
#define _JBED_LONG_INCLUDED_

#ifdef JBED_BIG_ENDIAN
struct  int64_struct {  int32_t hi; uint32_t lo; };
struct uint64_struct { uint32_t hi; uint32_t lo; };
#else
struct  int64_struct { uint32_t lo;  int32_t hi; };
struct uint64_struct { uint32_t lo; uint32_t hi; };
#endif


/* jbed_types.h should have been included already, so this file uses the
 * presence of a definition of INT64_MAX to indicate whether or not the
 * compiler supports 64 bit long operations.
 */
#ifndef INT32_MAX
#error jbed_types.h has not been included
#endif

#ifdef INT64_MAX /* { */

#define COMPILER_SUPPORTS_LONG  1

/* Access =================================================================*/

#define int64_hi(v) ((int32_t)((v) >> 32))
#define int64_lo(v) ((uint32_t)(v))

#define uint64_hi(v) ((uint32_t)((v) >> 32))
#define uint64_lo(v) ((uint32_t)(v))

/* Initialisation and assignment ==========================================*/

/** Initialise via assignment (high word and low word) */
#define int64_make(hi, lo)          (((int64_t)(hi) << 32) | (uint32_t)(lo))
#define uint64_make(hi, lo)         (((uint64_t)(hi) << 32) | (uint32_t)(lo))

/** Initialise an int64_t to the given values (high word and low word) */
#define int64_init(dst, hi, lo)     \
    ((void)((dst) = ((int64_t)(hi) << 32) | (uint32_t)(lo)))

#define uint64_init(dst, hi, lo)     \
    ((void)((dst) = ((uint64_t)(hi) << 32) | (uint32_t)(lo)))

/**
 * Assigns an int64_t to another int64_t. If src is of type uint64_t, use
 * int64_from_uint64(dst, src) instead.
 */
#define int64_assign(dst, s)        ((void)(dst = s))

/**
 * Assigns an uint64_t to another uint64_t. If src is of type int64_t, use
 * uint64_from_int64(dst, src) instead.
 */
#define uint64_assign(dst, s)       ((void)(dst = s))

#define int64_const(s)              (s)
#define uint64_const(s)             (s)

/** Initialise a value to zero */
#define int64_zero(dst)             ((void)((dst) = 0))
#define uint64_zero(dst)            ((void)((dst) = 0))

/* Conversions ============================================================*/

/** Explicit casting to int64_t/uint64_t */
#define int64_cast(s)               (int64_t)(s)
#define uint64_cast(s)              (uint64_t)(s)

/** Convert an int64_t into an int32_t, truncating the top 32 bits */
#define int32_from_int64(dst, src)  ((void)((dst) = (int32_t)(src)))

/** Convert an int to an int64_t */
#define int64_from_int32(dst, src)  ((void)((dst) = (int64_t)(int32_t)(src)))

/** Convert an unsigned int to an int64_t */
#define int64_from_uint32(dst, src) ((void)((dst) = (int64_t)(uint32_t)(src)))

/** Convert an unsigned int to a uint64_t */
#define uint64_from_uint32(dst,src) ((void)((dst) = (uint64_t)(uint32_t)(src)))

/** Convert an int to a uint64_t */
#define uint64_from_int32(dst, src) ((void)((dst) = (uint64_t)(int32_t)(src)))

/** Convert a uint64_t to a int64_t */
#define int64_from_uint64(dst, src) ((void)((dst) = (int64_t)(src)))

/** Convert a int64_t to a uint64_t */
#define uint64_from_int64(dst, src) ((void)((dst) = (uint64_t)(src)))

/** Convert a float to an int64_t */
#define int64_from_float(dst, src)  ((void)((dst) = (int64_t)(src)))

/** Convert a double to an int64_t */
#define int64_from_double(dst, src) ((void)((dst) = (int64_t)(src)))

/** Convert an int64_t to float */
#define float_from_int64(dst, src)  ((void)((dst) = (float)(int64_t)(src)))

/** Convert an int64_t to double */
#define double_from_int64(dst, src) ((void)((dst) = (double)(int64_t)(src)))

/* Basic arithmetic =======================================================*/

/** Add two int64_t values together, and store the result */
#define int64_add(dst, s1, s2)  ((void)((dst) = (s1) + (s2)))

/** Subtract one int64_t from another, and store the result */
#define int64_sub(dst, s1, s2)  ((void)((dst) = (s1) - (s2)))

/** Negate a int64_t value */
#define int64_neg(dst, src)     ((void)((dst) = -(src)))

/** Multiply two int64_t values together, and store the result */
#define int64_mul(dst, s1, s2)  ((void)((dst) = (int64_t)(s1) * (int64_t)(s2)))

/** Divide int64_t by another, and store the result */
#define int64_div(dst, s1, s2)  ((void)((dst) = (int64_t)(s1) / (int64_t)(s2)))

/** Divide uint64_t by another, and store the result */
#define uint64_div(dst, s1, s2) ((void)((dst) = (uint64_t)(s1) / (uint64_t)(s2)))

/** Calulate the remainder, and store the result */
#define int64_rem(dst, s1, s2)  ((void)((dst) = (int64_t)(s1) % (int64_t)(s2)))
#define uint64_rem(dst, s1, s2) ((void)((dst) = (uint64_t)(s1) % (uint64_t)(s2)))

/* Bitwise operations =====================================================*/

/** logical NOT of int64_t value, and store the result */
#define int64_not(dst, s)       ((void)((dst) ~= (s)))

/** AND two int64_t values together, and store the result */
#define int64_and(dst, s1, s2)  ((void)((dst) = (int64_t)(s1) & (int64_t)(s2)))

/** OR two int64_t values together, and store the result */
#define int64_or( dst, s1, s2)  ((void)((dst) = (int64_t)(s1) | (int64_t)(s2)))

/** XOR two int64_t values together, and store the result */
#define int64_xor(dst, s1, s2)  ((void)((dst) = (int64_t)(s1) ^ (int64_t)(s2)))

/** Logical shift right an int64_t value, and store the result */
#define int64_lshr(dst, s1, s2) ((void)((dst) = (uint64_t)(s1) >> (s2)))
#define uint64_lshr(dst, s1, s2) ((void)((dst) = (uint64_t)(s1) >> (s2)))

/** Arithmetic shift right an int64_t value, and store the result */
#define int64_ashr(dst, s1, s2) ((void)((dst) =  (int64_t)(s1) >> (s2)))

/** Logical shift left an int64_t value, and store the result */
#define int64_lshl(dst, s1, s2) ((void)((dst) =  (int64_t)(s1) << (s2)))
#define uint64_lshl(dst, s1, s2) ((void)((dst) = (uint64_t)(s1) << (s2)))

/* Comparisons ============================================================*/

#define int64_eq_zero(s1)   ((s1) == 0) /* Equal to zero */
#define int64_ne_zero(s1)   ((s1) != 0) /* Not equal to zero */
#define int64_lt_zero(s1)   ((s1) <  0) /* Less than zero */
#define int64_gt_zero(s1)   ((s1) >  0) /* Greater than zero */
#define int64_le_zero(s1)   ((s1) <= 0) /* Less than or equal to zero */
#define int64_ge_zero(s1)   ((s1) >= 0) /* Greater than or equal to zero */

#define int64_eq(s1, s2)    ((s1) == (s2)) /* Equal */
#define int64_ne(s1, s2)    ((s1) != (s2)) /* Not equal */
#define int64_lt(s1, s2)    ((s1) <  (s2)) /* s1 less than s2 */
#define int64_gt(s1, s2)    ((s1) >  (s2)) /* s1 greater than s2 */
#define int64_le(s1, s2)    ((s1) <= (s2)) /* s1 less than or equal to s2 */
#define int64_ge(s1, s2)    ((s1) >= (s2)) /* s1 greater than or equal to s2 */

#define uint64_eq_zero(s1)  ((s1) == 0) /* Equal to zero */
#define uint64_ne_zero(s1)  ((s1) != 0) /* Not equal to zero */
#define uint64_gt_zero(s1)  ((s1) >  0) /* Greater than zero */

#else /* } { Compiler doesn't support 64 bit ints */

#define COMPILER_SUPPORTS_LONG  0

typedef struct  int64_struct  int64_t;
typedef struct uint64_struct uint64_t;


/* Boundaries */
extern const int64_t INT64_MAX;
extern const int64_t INT64_MIN;

/* Access =================================================================*/

#define int64_hi(v) (v).hi
#define int64_lo(v) (v).lo

#define uint64_hi(v) (v).hi
#define uint64_lo(v) (v).lo

/* Initialisation and assignment ==========================================*/

/** Initialise an int64_t via assignment (high word and low word) */
#ifdef JBED_BIG_ENDIAN
#define int64_make(hi, lo) { hi, (uint32_t)(lo) }
#define uint64_make(hi, lo) { (uint32_t)(hi), (uint32_t)(lo) }
#else
#define int64_make(hi, lo) { (uint32_t)(lo), hi }
#define uint64_make(hi, lo) { (uint32_t)(lo), (uint32_t)(hi) }
#endif

/** Initialise an int64_t to the given values (high word and low word) */
#define int64_init(dst, high, low) do {                                     \
        (dst).hi = (high);                                                  \
        (dst).lo = (uint32_t)(low);                                         \
    } while (0)

#define uint64_init(dst, high, low) do {                                    \
        (dst).hi = (uint32_t)(high);                                        \
        (dst).lo = (uint32_t)(low);                                         \
    } while (0)

/**
 * Assigns an int64_t to another int64_t. If src is of type uint64_t, use
 * int64_from_uint64(dst, src) instead.
 */
#define int64_assign(dst, src) ((void)(dst = src))

/**
 * Assigns an uint64_t to another uint64_t. If src is of type int64_t, use
 * uint64_from_int64(dst, src) instead.
 */
#define uint64_assign(dst, src) ((void)(dst = src))

extern int64_t int64_const(const int32_t c);
extern uint64_t uint64_const(const uint32_t c);

/** Initialise a value to zero */
#define int64_zero(dst)  int64_init(dst, 0, 0)
#define uint64_zero(dst) uint64_init(dst, 0, 0)

/* Conversions ============================================================*/

/** Explicit casting integer variable to int64_t */
#define int64_cast(src)            int64_const(src)

/** Explicit casting integer variable to int64_t */
#define uint64_cast(src)           uint64_const(src)

/** Convert a unint64_t to a int64_t */
#define int64_from_uint64(dst, src) do {                                  \
        uint64_t tmp = (src);                                             \
        (dst).hi = (int32_t) tmp.hi;                                      \
        (dst).lo = tmp.lo;                                                \
    } while (0)

/** Convert a int64_t to a uint64_t */
#define uint64_from_int64(dst, src) do {                                  \
        int64_t tmp = (src);                                              \
        (dst).hi = (uint32_t) tmp.hi;                                     \
        (dst).lo = tmp.lo;                                                \
    } while (0)

/** Convert an int64_t into an int32_t, truncating the top 32 bits */
#define int32_from_int64(dst, src)    (dst = (src).lo)

/** Convert an int to an int64_t */
#define int64_from_int32(dst, src) do {                                     \
        dst.hi = 0;                                                         \
        dst.lo = src;                                                       \
        if ((src) < 0) {                                                    \
            dst.hi--;                                                       \
        }                                                                   \
    } while (0)                                                             \

/** Convert an unsigned int to an int64_t */
#define int64_from_uint32(dst, src) do {                                    \
        dst.hi = 0;                                                         \
        dst.lo = src;                                                       \
    } while (0)

/** Convert an int to a uint64_t */
#define uint64_from_int32(dst, src) do {                                    \
        dst.hi = 0;                                                         \
        dst.lo = src;                                                       \
    } while (0)

/** Convert an unsigned int to a uint64_t */
#define uint64_from_uint32(dst, src) do {                                   \
        dst.hi = 0;                                                         \
        dst.lo = src;                                                       \
    } while (0)

/** Convert a float to an int64_t */
#define int64_from_float(dst, src) do {                                       \
        dst.hi = (int32_t)(src / (65536.0 * 65536));                          \
        dst.lo = (int32_t) (src - (((float) dst.hi) * (65536.0 * 65536.0)));  \
    } while (0)

/** Convert a double to an int64_t */
#define int64_from_double(dst, src) do {                                      \
        dst.hi = (int32_t)(src / (65536.0 * 65536));                          \
        dst.lo = (int32_t) (src - (((double) dst.hi) * (65536.0 * 65536.0))); \
    } while (0)

/** Convert an int64_t to float*/
#define float_from_int64(dst, src) do {                                     \
        dst = ((float)src.hi) * (65536.0 * 65536);                          \
        dst += (float)src.lo;                                               \
    } while (0)

/** Convert an int64_t to double*/
#define double_from_int64(dst, src) do {                                    \
        dst = ((double)src.hi) * (65536.0 * 65536);                         \
        dst += (double) src.lo;                                             \
    } while (0)

/* Basic arithmetic =======================================================*/

/** Add two int64_t values, and store the result */
#define int64_add(dst, s1, s2) do {                                         \
        int64_t rv64;                                                       \
        rv64.hi = s1.hi + s2.hi;                                            \
        if (((rv64.lo = s1.lo + s2.lo) < s1.lo) || (rv64.lo < s2.lo)) {     \
            /* dst of an add can only be lower than one of the values if */ \
            /* we had an overflow, so increment the hi-order 32 bits */     \
            rv64.hi++;                                                      \
        }                                                                   \
        (dst).hi = rv64.hi;                                                 \
        (dst).lo = rv64.lo;                                                 \
    } while (0)

/** Subtract two int64_t values, and store the result */
#define int64_sub(dst, s1, s2) do {                                         \
        int64_t rv64;                                                       \
        rv64.hi = s1.hi - s2.hi;                                            \
        if ( (rv64.lo = s1.lo - s2.lo) > s1.lo ) {                          \
            /* The only we can subtract and get a larger number is */       \
            /* if a borrow from the hi-order word occurred */               \
            rv64.hi--;                                                      \
        }                                                                   \
        (dst).hi = rv64.hi;                                                 \
        (dst).lo = rv64.lo;                                                 \
    } while (0)

/** Negate a int64_t value, and store the result */
#define int64_neg(r, a) ((r).lo = -(int32_t)(a).lo,                         \
    (r).hi = -(int32_t)(a).hi - ((r).lo != 0))

/** Multiply two int64_t values, and store the result */
#define int64_mul(r, a, b) do {                                             \
        int64_t _a, _b;                                                     \
        int64_init(_a, a.hi, a.lo);                                         \
        int64_init(_b, b.hi, b.lo);                                         \
        int64_multiply32(r, _a.lo, _b.lo);                                  \
        (r).hi += _a.hi * _b.lo + _a.lo * _b.hi;                            \
    } while (0)

#define int64_lo16(a)        ((a) & (((uint32_t)1 << 16) - 1))
#define int64_hi16(a)        ((a) >> 16)

#define int64_multiply32(r, a, b) do {                                      \
        uint32_t _a1, _a0, _b1, _b0, _y0, _y1, _y2, _y3;                    \
        _a1 = int64_hi16(a), _a0 = int64_lo16(a);                           \
        _b1 = int64_hi16(b), _b0 = int64_lo16(b);                           \
        _y0 = _a0 * _b0;                                                    \
        _y1 = _a0 * _b1;                                                    \
        _y2 = _a1 * _b0;                                                    \
        _y3 = _a1 * _b1;                                                    \
        _y1 += int64_hi16(_y0);        /* can't carry */                    \
        _y1 += _y2;                    /* might carry */                    \
        if (_y1 < _y2)                                                      \
            _y3 += (uint32_t)((uint32_t)1 << 16);  /* propagate */          \
        (r).lo = (int64_lo16(_y1) << 16) + int64_lo16(_y0);                 \
        (r).hi = _y3 + int64_hi16(_y1);                                     \
    } while (0)

/** Divide helper */
extern void int64_udivmod(uint64_t *qp, uint64_t *rp, uint64_t a, uint64_t b);

/** Divide two int64_t values, and store the result */
#define int64_div(r, a, b) do {                                             \
        uint64_t _r, _a, _b;                                                \
        uint32_t _negative = (a).hi < 0? 1U: 0U;                            \
        if (_negative) {                                                    \
            int64_neg(_a, a);                                               \
        } else {                                                            \
            int64_init(_a, a.hi, a.lo);                                     \
        }                                                                   \
        if ((int32_t)(b).hi < 0) {                                          \
            _negative ^= 1;                                                 \
            int64_neg(_b, b);                                               \
        } else {                                                            \
            int64_init(_b, b.hi, b.lo);                                     \
        }                                                                   \
        int64_init(_r, r.hi, r.lo);                                         \
        int64_udivmod(&(_r), 0, _a, _b);                                    \
        if (_negative)                                                      \
            int64_neg(_r, _r);                                              \
        int64_init(r, _r.hi, _r.lo);                                        \
    } while (0)

/** Divide uint64_t by another, and store the result */
#define uint64_div(dst, s1, s2) int64_udivmod(&(dst), 0, (s1), (s2))


/** Divide two int64_t values, and store the remainder */
#define int64_rem(r, a, b) do {                                             \
        uint64_t _r, _a, _b;                                                \
        uint32_t _negative = (a).hi < 0? 1U: 0U;                            \
        if (_negative) {                                                    \
            int64_neg(_a, a);                                               \
        } else {                                                            \
            int64_init(_a, a.hi, a.lo);                                     \
        }                                                                   \
        if ((int32_t)(b).hi < 0) {                                          \
            int64_neg(_b, b);                                               \
        } else {                                                            \
            int64_init(_b, b.hi, b.lo);                                     \
        }                                                                   \
        int64_init(_r, r.hi, r.lo);                                         \
        int64_udivmod(0, &(_r), _a, _b);                                    \
        if (_negative)                                                      \
            int64_neg(_r, _r);                                              \
        int64_init(r, _r.hi, _r.lo);                                        \
    } while (0)

/** Divide two uint64_t values, and store the remainder */
#define uint64_rem(dst, s1, s2) int64_udivmod(0, &(dst), (s1), (s2))

/* Bitwise operations =====================================================*/

/** logical NOT of int64_t value, and store the result */
#define int64_not(dst, s) do {                                              \
        (dst).hi = ~(s).hi;                                                 \
        (dst).lo = ~(s).lo;                                                 \
    } while (0)

/** AND two int64_t values together, and store the result */
#define int64_and(dst, s1, s2) do {                                         \
        (dst).hi = (s1).hi & (s2).hi;                                       \
        (dst).lo = (s1).lo & (s2).lo;                                       \
    } while (0)

/** OR two int64_t values together, and store the result */
#define int64_or( dst, s1, s2) do {                                         \
        (dst).hi = (s1).hi | (s2).hi;                                       \
        (dst).lo = (s1).lo | (s2).lo;                                       \
    } while (0)

/** XOR two int64_t values together, and store the result */
#define int64_xor(dst, s1, s2) do {                                         \
        (dst).hi = (s1).hi ^ (s2).hi;                                       \
        (dst).lo = (s1).lo ^ (s2).lo;                                       \
    } while (0)

/** Logical shift right an int64_t value, and store the result */
#define int64_lshr(dst, s, n) do {                                          \
        if ( (n & 32) != 0) {                                               \
            /* Shifting 32-bits or more - move hi to lo */                  \
            dst.lo = s.hi;                                                  \
            dst.hi = 0;                                                     \
        } else {                                                            \
            int64_assign(dst, s);                                           \
        }                                                                   \
        if ( (n & 31) != 0) {                                               \
            dst.lo >>= (n & 31);                                            \
            dst.lo |= dst.hi << (32 - (n & 31));                            \
            dst.hi = ((uint32_t) dst.hi) >> (n & 31);                       \
        }                                                                   \
    } while (0)

#define uint64_lshr(dst, s, n) do {                                         \
        if ( (n & 32) != 0) {                                               \
            /* Shifting 32-bits or more - move hi to lo */                  \
            dst.lo = s.hi;                                                  \
            dst.hi = 0;                                                     \
        } else {                                                            \
            uint64_assign(dst, s);                                          \
        }                                                                   \
        if ( (n & 31) != 0) {                                               \
            dst.lo >>= (n & 31);                                            \
            dst.lo |= dst.hi << (32 - (n & 31));                            \
            dst.hi = dst.hi >> (n & 31);                                    \
        }                                                                   \
    } while (0)

/** Arithmetic shift right an int64_t value, and store the result */
#define int64_ashr(r, a, b) do {                                            \
        if (b) {                                                            \
            int64_t _a;                                                     \
            int64_init(_a, a.hi, a.lo);                                     \
            if ((b) < 32) {                                                 \
                (r).lo = (_a.hi << (32 - (b))) | (_a.lo >> ((b) & 31));     \
                (r).hi = (int32_t)_a.hi >> ((b) & 31);                      \
            } else {                                                        \
                (r).lo = (int32_t)_a.hi >> ((b) & 31);                      \
                (r).hi = (int32_t)_a.hi >> 31;                              \
            }                                                               \
        } else {                                                            \
            int64_init((r), (a).hi, (a).lo);                                \
        }                                                                   \
    } while (0)

/** Logical shift left a value, and store the result */
#define int64_lshl(dst, s, n) do {                                          \
        if ( (n & 32) != 0) {                                               \
            /* Shifting 32-bits or more - move lo to hi */                  \
            dst.hi = s.lo;                                                  \
            dst.lo = 0;                                                     \
        } else {                                                            \
            int64_init(dst, s.hi, s.lo);                                    \
        }                                                                   \
        if ( (n & 31) != 0) {                                               \
            dst.hi = ((uint32_t) dst.hi) << (n & 31);                       \
            dst.hi |= dst.lo >> (32 - (n & 31));                            \
            dst.lo <<= (n & 31);                                            \
        }                                                                   \
    } while (0)

#define uint64_lshl(dst, s, n) do {                                         \
        if ( (n & 32) != 0) {                                               \
            /* Shifting 32-bits or more - move lo to hi */                  \
            (dst).hi = (s).lo;                                              \
            (dst).lo = 0;                                                   \
        } else {                                                            \
            uint64_assign(dst, s);                                          \
        }                                                                   \
        if ( (n & 31) != 0) {                                               \
            (dst).hi = (dst).hi << (n & 31);                                \
            (dst).hi |= (dst).lo >> (32 - (n & 31));                        \
            (dst).lo <<= (n & 31);                                          \
        }                                                                   \
    } while (0)


/* Comparisons ============================================================*/

#define int64_eq_zero(s1)    ((s1.lo | s1.hi) == 0)        /* Equal to zero */
#define int64_ne_zero(s1)    ((s1.lo | s1.hi) != 0)        /* Not equal to zero */
#define int64_lt_zero(s1)    ((s1.hi) < 0)                 /* Less than zero */
#define int64_gt_zero(s1)    (((s1.hi) > 0) || ((s1.hi == 0) && (s1.lo != 0))) /* Greater than zero */
#define int64_le_zero(s1)    (((s1.hi) < 0) || ((s1.hi == 0) && (s1.lo == 0))) /* Less than or equal to zero */
#define int64_ge_zero(s1)    (((s1.hi) > 0) || ((s1.hi == 0) && (s1.lo >= 0))) /* Greater than or equal to zero */

#define int64_eq(a, b)       ((a.hi == b.hi) &&  (a.lo == b.lo))
#define int64_ne(a, b)       ((a.hi != b.hi) ||  (a.lo != b.lo))
#define int64_lt(a, b)       ((a.hi <  b.hi) || ((a.hi == b.hi) && (a.lo <  b.lo)))
#define int64_gt(a, b)       ((a.hi >  b.hi) || ((a.hi == b.hi) && (a.lo >  b.lo)))
#define int64_le(a, b)       ((a.hi <  b.hi) || ((a.hi == b.hi) && (a.lo <= b.lo)))
#define int64_ge(a, b)       ((a.hi >  b.hi) || ((a.hi == b.hi) && (a.lo >= b.lo)))

#define uint64_eq_zero(s1)    ((s1.lo | s1.hi) == 0)        /* Equal to zero */
#define uint64_ne_zero(s1)    ((s1.lo | s1.hi) != 0)        /* Not equal to zero */
#define uint64_gt_zero(s1)    uint64_ne_zero(s1)

#endif /* } Compiler doesn't support 64 bit ints */



/* A small number of targets require that int64_t * is 64-bit aligned.
 * Set NEED_LONG_ALIGNMENT to 1 if this is the case, 0 otherwise.
 * Honour any setting that has been established externally, to allow
 * target or toolset to override.
 */

#ifdef NEED_LONG_ALIGNMENT
# if !COMPILER_SUPPORTS_LONG
#  error NEED_LONG_ALIGNMENT but !COMPILER_SUPPORTS_LONG
# endif

#else

/* The only one likely to be encountered is ARMv5 using LDRD / STRD
 * instructions. This can only happen if the stack is aligned to
 * 8 bytes. This this stack alignment does not itself imply that the compiler
 * is using LDRD, but we choose a safe default - can be overridden with an
 * explicit setting in a build file.
 */

# if COMPILER_SUPPORTS_LONG && defined(JBED_ARM_V5) && defined(JBED_ARM_STACKALIGN8)
#  define NEED_LONG_ALIGNMENT 1
# else
#  define NEED_LONG_ALIGNMENT 0
# endif

#endif /* defined(NEED_LONG_ALIGNMENT) */



/* Define types [u]int64_unaligned_t for use in structures.
 * Define [u]int64_{load,store}_unaligned for copying values between
 * a [u]int64_t and a [u]int64_unaligned_t *
 * Note that the _unaligned_ types must require no more than 32-bit
 * alignment.
 */

#if NEED_LONG_ALIGNMENT  /* implies COMPILER_SUPPORTS_LONG */

/* Represent the unaligned types as structures, and then load/store
 * must operate on the 32-bit pieces separately
 */

typedef struct  int64_struct  int64_unaligned_t;
typedef struct uint64_struct uint64_unaligned_t;

#define   int64_load_unaligned(val, src)  do { \
                 const int64_unaligned_t *p = src; \
                 val = int64_make(p->hi, p->lo); \
             } while(0)

#define  uint64_load_unaligned(val, src)  do { \
                 const uint64_unaligned_t *p = src; \
                 val = uint64_make(p->hi, p->lo); \
             } while(0)


#define  int64_store_unaligned(val, dst)    do { \
                 int64_t v = (val); \
                 int64_unaligned_t *p = dst; \
                 p->hi = int64_hi(v); \
                 p->lo = int64_lo(v); \
             } while(0)

#define  uint64_store_unaligned(val, dst)   do { \
                 uint64_t v = (val); \
                 uint64_unaligned_t *p = dst; \
                 p->hi = uint64_hi(v); \
                 p->lo = uint64_lo(v); \
             } while(0)

#else /* NEED_LONG_ALIGNMENT */

/* Either COMPILER_SUPPORTS_LONG && !NEED_LONG_ALIGNMENT,
 * or !COMPILER_SUPPORTS_LONG. For the latter case, the definitions
 * below turn into structure copies.
 *
 * Some COMPILER_SUPPORTS_LONG toolsets choose to align int64_t on a 64-bit
 * boundary, even if the underlying architecture does not require it. Therefore,
 * we choose to always represent [u]int64_unaligned_t as a structure, and we
 * use pointer casts for load/store.
 */

typedef struct  int64_struct  int64_unaligned_t;
typedef struct uint64_struct uint64_unaligned_t;


#define   int64_load_unaligned(val, src)          (val = *( int64_t*)(src))
#define  uint64_load_unaligned(val, src)          (val = *(uint64_t*)(src))

#define   int64_store_unaligned(val, dst)    (void)(*( int64_t*)(dst) = (val))
#define  uint64_store_unaligned(val, dst)    (void)(*(uint64_t*)(dst) = (val))

#endif /* NEED_LONG_ALIGNMENT */

#endif /* _JBED_LONG_INCLUDED_ */
