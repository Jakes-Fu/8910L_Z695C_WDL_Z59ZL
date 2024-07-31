/**
 * Copyright 2007 Esmertec AG. All Rights Reserved.
 * $Id$
 */

 /**
 * @file
 * Base64 encode and decode.
 */

#ifndef _JBEDBASE64_H__
#define _JBEDBASE64_H__

#include <jbed_types.h>

#if defined(__cplusplus)
extern "C" {
#endif
/**
 * Encode the given raw data to Base64 string.
 *
 * @param input data to be encoded
 * @param inputLen the length of the input data
 * @return pointer to a Null-terminated Base64 encoded string,
 *         or NULL if error occures. The returned char array should be
 *         explicitly freed by base64Free if no longer needed.
 */
uint8_t * Jbed_base64Encode(uint8_t * input, int32_t inputLen);

/**
 * Decode the given Base64 string to raw data.
 *
 * @param input Null-terminated Base64 string to be decoded, 4 bytes aligned by padding is not required.
 * @param output decoded binary data;
 *        The returned char array should be explicitly freed by base64Free if no longer needed.
 * @return the length of the decoded data or -1 if error occured.
 */
int32_t Jbed_base64Decode(uint8_t * input, uint8_t ** output);

/**
 * Free the char array returned by base64Encode/base64Decode.
 * @param toFree the memory to free, which was allocated by method Jbed_base64Encode or Jbed_base64Decode.
 */
void Jbed_base64Free(uint8_t * toFree);

#if defined(__cplusplus)
}
#endif

#endif /* _JBEDBASE64_H__ */
