/*
 * Copyright 2002-2003 Aplix Corporation. All rights reserved.
 */

/***************************************************************************
 * FILE: kjava_sys_zlib.h
 *
 * DESCRIPTION:
 *      The interface is used to abstract zlib operations.
 *
 * UPDATED DATE:
 *      2004/10/21
 *
 * REVISION:
 *      2004/10/27,  Jack,      first version.
 *
 ***************************************************************************/

#ifndef KJAVA_SYS_ZLIB_HEADER_H
#define KJAVA_SYS_ZLIB_HEADER_H

/**
 * @file kjava_sys_zlib.h
 *
 */

#ifdef __cplusplus
extern "C" {
#endif




/** \addtogroup kjava_sys
 *  @{
 */
/** \defgroup kjava_sys_z zlib interface
 *
 *  Interfaces listed in the group are used to abstract zlib interfaces,
 *  including inflation and deflation.
 *
 *  Please refer to following link to get more information about zlib:
 *      - http://www.gzip.org/zlib/manual.html
 *
 *  \author Jack Chung
 *  @{
 */

/*-----------------------------------------------------------------------------
 * Marcos, Constants, Types, and Structures
 *----------------------------------------------------------------------------*/

#ifndef KSI_NEED_BASIC_DEFLATION_FUNCTIONALITIES
/** \brief Flag for the need of only basic functions of zlib */
#define KSI_NEED_BASIC_DEFLATION_FUNCTIONALITIES    0
#endif

#ifndef KSI_NEED_ADVANCED_FUNCTIONALITIES
/** \brief Flag for the need of advanced functions of zlib */
#define KSI_NEED_ADVANCED_FUNCTIONALITIES           0
#endif


/** \brief no flush */
#define KJAVA_Z_NO_FLUSH              0
/** \brief partial flush */
#define KJAVA_Z_PARTIAL_FLUSH         1
/** \brief synchrounus flush */
#define KJAVA_Z_SYNC_FLUSH            2
/** \brief full flush */
#define KJAVA_Z_FULL_FLUSH            3
/** \brief operation finished */
#define KJAVA_Z_FINISH                4


/** \brief operation okay */
#define KJAVA_Z_OK                    0
/** \brief end of stream */
#define KJAVA_Z_STREAM_END            1
/** \brief need the dictionary */
#define KJAVA_Z_NEED_DICT             2


/** \brief error number */
#define KJAVA_Z_ERRNO                 (-1)
/** \brief stream error */
#define KJAVA_Z_STREAM_ERROR          (-2)
/** \brief invalid data */
#define KJAVA_Z_DATA_ERROR            (-3)
/** \brief invalid memory or invalid memory operation */
#define KJAVA_Z_MEM_ERROR             (-4)
/** \brief buffer error */
#define KJAVA_Z_BUF_ERROR             (-5)
/** \brief version incompatible */
#define KJAVA_Z_VERSION_ERROR         (-6)


/** \brief no compression */
#define KJAVA_Z_NO_COMPRESSION        0
/** \brief best speed */
#define KJAVA_Z_BEST_SPEED            1
/** \brief best compression */
#define KJAVA_Z_BEST_COMPRESSION      9
/** \brief default compression */
#define KJAVA_Z_DEFAULT_COMPRESSION   (-1)


/** \brief filtered strategy */
#define KJAVA_Z_FILTERED              1
/** \brief huffman encoding only */
#define KJAVA_Z_HUFFMAN_ONLY          2
/** \brief default strategy */
#define KJAVA_Z_DEFAULT_STRATEGY      0


/** \brief binary content */
#define KJAVA_Z_BINARY                0
/** \brief ASCII content */
#define KJAVA_Z_ASCII                 1
/** \brief unknown content */
#define KJAVA_Z_UNKNOWN               2

/** \brief deflated */
#define KJAVA_Z_DEFLATED              8

/** \brief NULL */
#define KJAVA_Z_NULL                  0


/**
 * \brief z_stream
 */
typedef struct _KJavaZStreamTag
{
    /** \brief next input byte */
    unsigned char * next_in;

    /** \brief number of bytes available at next_in */
    unsigned int    avail_in;

    /** \brief total nb of input bytes read so far */
    unsigned long   total_in;

    /** \brief next output byte should be put there */
    unsigned char * next_out;

    /** \brief remaining free space at next_out */
    unsigned int    avail_out;

    /** \brief total nb of bytes output so far */
    unsigned long   total_out;

    /** \brief private data object passed to allocFunc and freeFunc */
    void *          opaque;

    /** \brief closure */
    void *          closure;
}
KJavaZStream;

/** \brief handle for z_stream */
typedef KJavaZStream * KJavaZHandle;

/** \brief function pointer prototype for memory allocation */
typedef void* (* PFNC_KJAVA_Z_MALLOC) (void *params, unsigned int items, unsigned int size);

/** \brief function pointer prototype for releasing allocated memory */
typedef void  (* PFNC_KJAVA_Z_FREE) (void *params, void *address);


/*-----------------------------------------------------------------------------
 * Global Function(s) Prototype
 *----------------------------------------------------------------------------*/

/** \brief initialization for zlib interface */
int kjava_z_init(void);


/** \brief finalization for zlib interface */
int kjava_z_fini(void);


/**
 * \brief get the version of zlib.
 *
 * \return the string of zlib version.
 */
const char * kjava_z_version(void);


/** \brief create a KJavaZHandle without any designated
 *         type (neither inflation nor deflation)
 *
 * \param allocFunc
 *                  function pointer for memory allocation.
 *
 * \param freeFunc
 *                  function pointer for releasing allocated memory.
 *
 * \return the handle for zlib operation.
 */
KJavaZHandle kjava_z_createStream(PFNC_KJAVA_Z_MALLOC allocFunc, PFNC_KJAVA_Z_FREE freeFunc);


/**
 * \brief release the created KJavaZHandle.
 *
 * \param hStream
 *                  the designated handle to be released.
 *
 * \return if the operation is successful, returns 1. Otherwise, returns 0.
 */
int kjava_z_releaseStream(KJavaZHandle hStream);



/** \defgroup kjava_sys_z_basic basic functions
 *
 *  \brief basic funtionalities that VM needs. (only inflation is MUST)
 *
 *  Interfaces listed in the group for inflation are necessary for PNG decoding.
 *
 *  @{
 */

#if KSI_NEED_BASIC_DEFLATION_FUNCTIONALITIES
/**
 * \brief Initializes the internal stream state for compression.
 *
 * \param hStream
 *                  the handle for KJavaZStream.
 *
 * \param level
 *                  the compression level. it must be #KJAVA_Z_DEFAULT_COMPRESSION, or
 *                  between 0 and 9: 1 gives best speed, 9 gives best
 *                  compression, 0 gives no compression at all
 *                  (the input data is simply copied a block at a time).
 *
 * \return possible return values:
 *          - #KJAVA_Z_OK , if success.
 *          - #KJAVA_Z_MEM_ERROR , if there was not enough memory.
 *          - #KJAVA_Z_STREAM_ERROR , if level is not a valid compression level
 *          - #KJAVA_Z_VERSION_ERROR , zlib library version incompatible.
 */
int kjava_z_deflateInit(KJavaZHandle hStream, int level);

/**
 * \brief Compress data as much as possible.
 *
 * The function compresses as much data as possible, and stops when the input
 * buffer becomes empty or the output buffer becomes full. It may introduce
 * some output latency (reading input without producing any output) except
 * when forced to flush.
 *
 * \param hStream
 *                  the handle for KJavaZStream.
 *
 * \param flush
 *                  flush flag. possible values:
 *                      - KJAVA_Z_NO_FLUSH
 *                      - KJAVA_Z_PARTIAL_FLUSH
 *                      - KJAVA_Z_SYNC_FLUSH
 *                      - KJAVA_Z_SYNC_FLUSH
 *                      - KJAVA_Z_FINISH
 *
 * \return
 */
int kjava_z_deflate(KJavaZHandle hStream, int flush);

/**
 * \brief End the compression process.
 *
 * \param hStream
 *                  the handle for KJavaZStream.
 *
 * \return
 */
int kjava_z_deflateEnd(KJavaZHandle hStream);

#endif /* KSI_NEED_BASIC_DEFLATION_FUNCTIONALITIES */

/**
 * \brief Initialize the decompression process.
 *
 * \param hStream
 *                  the handle for KJavaZStream.
 *
 *
 * Please see zlib documentation for more detailed information.
 */
int kjava_z_inflateInit(KJavaZHandle hStream);

/**
 * \brief Initialize the decompression process.
 *
 * \param hStream
 *                  the handle for KJavaZStream.
 *
 * \param flush
 *                  flush flag. possible values:
 *                      - KJAVA_Z_NO_FLUSH
 *                      - KJAVA_Z_PARTIAL_FLUSH
 *                      - KJAVA_Z_SYNC_FLUSH
 *                      - KJAVA_Z_SYNC_FLUSH
 *                      - KJAVA_Z_FINISH
 *
 *
 * Please see zlib documentation for more detailed information.
 */
int kjava_z_inflate(KJavaZHandle hStream, int flush);

/**
 * \brief End the decompression process.
 *
 * \param hStream
 *                  the handle for KJavaZStream.
 *
 *
 * Please see zlib documentation for more detailed information.
 */
int kjava_z_inflateEnd(KJavaZHandle hStream);

/** @} */


#if KSI_NEED_ADVANCED_FUNCTIONALITIES
/** \defgroup kjava_sys_z_advance advanced functions
 *
 *  \brief All interfaces listed in the group are optional.
 *
 *  @{
 */
int kjava_z_deflateInit2(KJavaZHandle hStream, int level, int method, int windowBits, int memLevel, int strategy);

int kjava_z_deflateSetDictionary(KJavaZHandle hStream, const unsigned char *dictionary, unsigned int dictLength);

int kjava_z_deflateCopy(KJavaZHandle hStreamDst, KJavaZHandle hStreamSrc);

int kjava_z_deflateReset(KJavaZHandle hStream);

int kjava_z_deflateParams(KJavaZHandle hStream, int level, int strategy);

int kjava_z_inflateInit2(KJavaZHandle hStream, int windowBits);

int kjava_z_inflateSetDictionary(KJavaZHandle hStream, const unsigned char *dictionary, unsigned int dictLength);

int kjava_z_inflateSync(KJavaZHandle hStream);

int kjava_z_inflateReset(KJavaZHandle hStream);

/** @} */
#endif /* KSI_NEED_ADVANCED_FUNCTIONALITIES */


/** @} */

#ifdef __cplusplus
}
#endif

#endif /* KJAVA_SYS_ZLIB_HEADER_H */
