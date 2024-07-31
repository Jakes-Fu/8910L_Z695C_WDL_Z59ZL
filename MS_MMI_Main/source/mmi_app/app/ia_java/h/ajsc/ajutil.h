/*
 * Copyright 2002-2003 Aplix Corporation. All rights reserved.
 */
/** 
 * @file ajutil.h
 * AJMS Utility Header
 */
/*=========================================================================
 * KVM
 *=========================================================================
 * SYSTEM:    KVM
 * SUBSYSTEM: AJUTIL
 * FILE:      ajutil.h
 * OVERVIEW:  Utilities for common usage
 * VERSION:   2004/10/20
 *=======================================================================*/
#ifndef AJ_UTILITY_HEADER
#define AJ_UTILITY_HEADER

#ifdef __cplusplus
extern "C"{
#endif

#include "aj_type.h"

/** @addtogroup utility_aci 
 *  @{ 
 */
 
/** 
 * @addtogroup ajutil Utility ACI
 * @{
 */


/**
 * \brief The structure type for containing the URL information
 *        parsed by #ajutil_parseURL().
 */
typedef struct _AJURLInfoR2
{
    /** \brief Start pointer of the url. */
    AJ_BYTE     *uri;
    /** \brief Length of the <i>url</i>. */
    AJ_INT      uriLength;

    /** \brief Start pointer of the host name. */
    AJ_BYTE     *hostname;
    /** \brief Length of the <i>hostname</i>. */
    AJ_INT      hostnameLength;

    /** \brief Value of port number if exist. If 
     *         port does not exist in URL, it assigned
     *         80 if URL start with "http" and assigned
     *         443 if start with "https". In any other
     *         protocol, the port number is 0.
     */
    AJ_INT      port;

    /** \brief Start pointer of the path after the
     *         host name.
     */
    AJ_BYTE     *path;
    /** \brief Length of the <i>path</i>. */
    AJ_INT      pathLength;

    /** \brief Reserved field */
    AJ_BYTE     *query;
    /** \brief Reserved field */
    AJ_INT      queryLength;
} AJURLInfoR2;

/**
 *   \brief Converts a Unicode character string to a UTF-8 byte array.
 *   
 *   @param in
 *     A pointer to the memory area containing the Unicode character string.
 *   @param inChars
 *     The number of characters in the Unicode character string.
 *   @param out
 *     A pointer to the memory area for receiving the UTF-8 byte array resulting from 
 *     conversion.
 *   @param outBytes
 *     The number of bytes that can be output to the memory area designated in the 
 *     out parameter.<br>
 *     The number of characters resulting from conversion of a Unicode character 
 *     string to a UTF-8 byte array can be obtained by calling #ajutil_UniToUtf8Size().
 *     When priority is given to speed of execution, the value designated in inChars 
 *     times 3 bytes is an estimate on the safe side and can be used here.
 *   @return
 *     The number of Unicode characters converted to a UTF-8 byte array is returned.
 *
 *     If the return value is identical to that designated in the inChars parameter, 
 *     this means the entire Unicode character string was converted.<br>
 *     If not all the Unicode character string could be processed due to lack of 
 *     output space, -1 is returned.<br>
 *
 *   Even if the Unicode character string contains a termination character ('\\0'), 
 *   the designated number of bytes are processed. '\\0' is output as is.
 *   '\\0' is not added at the end of the UTF-8 byte array after conversion.
 */
AJ_S32 ajutil_UniToUtf8(const AJ_U16* in,  AJ_S32 inChars,
                              AJ_U8*  out, AJ_S32 outBytes );



/**
 *   \brief Returns the number of bytes resulting from conversion of a Unicode character 
 *   string to a UTF-8 byte array.
 *   
 *   @param in
 *     A pointer to the memory area containing the Unicode character string.
 *   @param inChars
 *     The number of characters in the Unicode character string.
 *   
 *   @return
 *     The number of bytes resulting from conversion of a Unicode character string to 
 *     a UTF-8 byte array is returned.
 *     This byte count does not include '\\0' termination.<br>
 *     If #AJ_NULL is designated for the in parameter, or a negative value for 
 *     inChars, -1 is returned indicating an illegal parameter.
 *   
 */
AJ_S32 ajutil_UniToUtf8Size(const AJ_U16* in, AJ_S32 inChars);

/**
 *   \brief Converts a UTF-8 byte array to a Unicode character string.
 *   
 *   @param in
 *     A pointer to the memory area containing the UTF-8 byte array.
 *   @param inBytes
 *     The number of bytes in the UTF-8 byte array.
 *   @param out
 *     A pointer to the memory area for receiving the Unicode character string 
 *     resulting from conversion.
 *   @param outChars
 *     The number of characters that can be output to the memory area designated by 
 *     the out parameter. <br>
 *     The number of characters resulting from conversion of a UTF-8 byte array to a 
 *     Unicode character string can be obtained by calling #ajutil_Utf8ToUniSize().
 *     When priority is given to speed of execution, the value designated in inBytes 
 *     is an estimate on the safe side and can be used here.
 *   
 *   @return
 *     The number of UTF-8 bytes converted to a Unicode character string is 
 *     returned.<br>
 *     If the return value is identical to that designated in the inBytes parameter, 
 *     this means the entire UTF-8 byte array was converted.<br>
 *     If not all the UTF-8 byte array could be processed due to lack of output 
 *     space, -1 is returned.<br>
 *     If #AJ_NULL is designated for either the in or out parameters, -2 is returned 
 *     indicating an illegal parameter.<br> 
 *   
 *   Even if the UTF-8 byte array contains a termination character ('\\0'),  
 *   the designated number of bytes are processed. '\\0' is output as is.
 *   If the UTF-8 byte array contains UCS4 character codes of 0x10000 and above,
 *   these characters are not converted but are replaced by the alternate character 
 *   designated in the substitute parameter and counted in the characters that could 
 *   not be converted.<br>
 *   UTF-8 converted to an illegal byte length is considered an illegal code under 
 *   RFC2279, but conversion proceeds in this function nonetheless.
 *   Example:: U+0000 in UTF-8 is the 1 byte 0x00, but conversion processing takes 
 *   place even it is encoded as 2 or 3 bytes (0xC0 0x80 or 0xE0 0x80 0x80).
 *   
 *
 */
AJ_S32 ajutil_Utf8ToUni(const AJ_U8*  in,  AJ_S32 inBytes,
                              AJ_U16* out, AJ_S32 outChars );



/**
 *   \brief Returns the number of characters resulting from conversion of a UTF-8 byte array 
 *   to a Unicode character string.
 *   
 *   @param in
 *     A pointer to the memory area containing the UTF-8 byte array.
 *   @param inBytes
 *     The number of bytes in the UTF-8 byte array.
 *   
 *   @return
 *     The number of characters resulting from conversion of a UTF-8 byte array to a 
 *     Unicode character string is returned. 
 *     This character count does not include the '\\0' terminating the converted Unicode 
 *     character string.
 *     If #AJ_NULL is designated for the in parameter, or if a negative value is 
 *     designated for inBytes, -1 is returned indicating an illegal parameter.
 *   
 *   If conversion is interrupted while processing a multi-byte character, that 
 *   character is not included in the count.
 */
AJ_S32 ajutil_Utf8ToUniSize(const AJ_U8* in, AJ_S32 inBytes);

/**
 *  \brief Finds the first occurrence of string in buffer.
 *
 *  \param  src                The buffer contains original data.
 *  \param  key                The character to search for.
 *  \param  srcSize            Size of the original buffer.
 *
 *  \return If it's successful to get the value according to the specific key,
 *          return a pointer to the first occurrence.
 *          Otherwise, return #AJ_NULL.
 *
 */
AJ_BYTE *ajutil_findStringInBuffer(const AJ_BYTE *src,
                                   const AJ_BYTE *key,
                                   const AJ_U32  srcSize);

/**
 *  \brief  Case-insensitively compare two strings with specified length
 *
 *  \param  str1            String 1 (null-terminated)
 *  \param  str2            String 2 (null-terminated)
 *  \param  maxLen          Maximum length to compare
 *
 *  \return If two string are case-insensitively same, return 0.
 *          Otherwise, return 1 if str1 > str2, -1 if str2 > str1.
 *
 */
AJ_S32 ajutil_stricmp(const AJ_U8 *str1, 
                      const AJ_U8 *str2, 
                      const AJ_S32 maxLen);

/**
 *  \brief  Compare two block of memory with specified length
 *
 *  \param  str1            String 1 (null-terminated)
 *  \param  strlen1         Length of str1
 *  \param  str2            String 2 (null-terminated)
 *  \param  strlen2         Length of str2
 *
 *  \return If two block memory are the same, return 0.
 *          Otherwise, return 1 if str1 > str2, -1 if str2 > str1.
 *          The comparation rule is same as ASCII string comparation
 *          strcmp().
 *
 */
AJ_S32 ajutil_memicmp(const AJ_U8  *str1, 
                            AJ_S32  strlen1, 
                      const AJ_U8  *str2, 
                            AJ_S32  strlen2);

/**
 *  \brief  Parse the URL string
 *
 *  \param  url             URL string
 *  \param  urlLength       The length of URL string     
 *  \param  urlInfo         The pointer to the structure AJURLInfo for parsed data
 *
 *  \return If the URL is successfully parsed, return 0.
 *          Otherwise, return -1.
 *
 **/
AJ_INT ajutil_parseURL(const AJ_BYTE *url, AJ_INT urlLength, AJURLInfoR2 *urlInfo);


/**
 * \brief To encode the data using Base64 algorithm
 *
 * @param	inbuf		input data(in)
 * @param    inbuf_len   length of input data(in)
 * @param	outbuf		output data in Base64, size of 
  *                     outbuf MUST > (4 * inbuf_len / 3 + 3) bytes (out)
 *
 * @return	size of data in outbuf
 */
AJ_S32 ajutil_b64_encode( AJ_U8* inbuf, AJ_S32 inbuf_len, AJ_U8* outbuf );

/**
 * \brief To decode the data using Base64 algorithm
 *
 * @param	inbuf		input Base64 data(in)
 * @param   inbuf_len   length of input data(in)
 * @param	outbuf		output data after decode, size of 
 *                      outbuf MUST > (3 * inbuf_len / 4) bytes (out)
 *
 * @return	size of data in outbuf
 */
AJ_S32 ajutil_b64_decode( AJ_U8* inbuf, AJ_S32 inbuf_len, AJ_U8* outbuf );

/** @} */
/** @} */

#ifdef __cplusplus
}
#endif 

#endif /* AJ_UTILITY_HEADER */
