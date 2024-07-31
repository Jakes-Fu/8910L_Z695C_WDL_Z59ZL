#ifdef KURO_SUPPORT

#ifndef BSWAP_H
#define BSWAP_H

/*================   Byte ordering   ==================*/
#ifndef _WIN32
#define LITTLE_ENDIAN 
#endif

#define INT1_H2N(v) (v)
#define INT1_N2H(v) (v)
#define INT4_H2N(v) UINT4_EX(v)
#define INT4_N2H(v) UINT4_EX(v)
#define INT2_H2N(v) UINT2_EX(v)
#define INT2_N2H(v) UINT2_EX(v)
#define INT8_H2N(v) UINT8_EX(v)

#ifdef LITTLE_ENDIAN	/* Intel X86 Family CPUs,etc */
	#define UINT4_EX(v)	((unsigned int) (((v & 0xFF) << 24) | ((v & 0xFF00) << 8) | ((v & 0xFF0000) >> 8) | ((v & 0xFF000000) >> 24)))
	#define UINT2_EX(v)	((unsigned short) (((v & 0xFF) << 8) | ((v & 0xFF00) >> 8)))
    #define UINT8_EX(v)  ((uint64_t)( \
		(uint64_t)(((uint64_t)(v) & (uint64_t)0x00000000000000ffULL) << 56) | \
		(uint64_t)(((uint64_t)(v) & (uint64_t)0x000000000000ff00ULL) << 40) | \
		(uint64_t)(((uint64_t)(v) & (uint64_t)0x0000000000ff0000ULL) << 24) | \
		(uint64_t)(((uint64_t)(v) & (uint64_t)0x00000000ff000000ULL) <<  8) | \
	        (uint64_t)(((uint64_t)(v) & (uint64_t)0x000000ff00000000ULL) >>  8) | \
		(uint64_t)(((uint64_t)(v) & (uint64_t)0x0000ff0000000000ULL) >> 24) | \
		(uint64_t)(((uint64_t)(v) & (uint64_t)0x00ff000000000000ULL) >> 40) | \
		(uint64_t)(((uint64_t)(v) & (uint64_t)0xff00000000000000ULL) >> 56) ))
#else
	#define UINT4_EX(v)	(v)
	#define UINT2_EX(v)	(v)
	#define UINT8_EX(v)	(v)
#endif/* LITTLE_ENDIAN */

#endif /* BSWAP_H */
#endif

