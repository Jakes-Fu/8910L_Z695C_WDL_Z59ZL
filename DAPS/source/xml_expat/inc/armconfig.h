/*================================================================
** Copyright 2000, Clark Cooper
** All rights reserved.
**
** This is free software. You are permitted to copy, distribute, or modify
** it under the terms of the MIT/X license (contained in the COPYING file
** with this distribution.)
**
*/

#ifndef ARMCONFIG_H
#define ARMCONFIG_H


/* 1234 = LIL_ENDIAN, 4321 = BIGENDIAN */
#if defined(__BIG_ENDIAN) || defined(__BigEndian)
	#define BYTEORDER  4321 
#else
	#define BYTEORDER  1234
#endif

/* Define to 1 if you have the `bcopy' function. */
#undef HAVE_BCOPY

/* Define to 1 if you have the `memmove' function. */
#define HAVE_MEMMOVE

/* Define to specify how much context to retain around the current parse point. */
#undef XML_CONTEXT_BYTES

/* Define to make parameter entity parsing functionality available. */
//#undef XML_DTD
#define XML_DTD 1

/* Define to make XML Namespaces functionality available. */
//#undef XML_NS
#define XML_NS 1

// #define XML_UNICODE

//#define unsigned long int size_t

#endif /* ifndef ARMCONFIG_H */