/* -*- Mode: c++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*-
 *
 * Copyright (C) 1995-2011 Opera Software AS.  All rights reserved.
 *
 * This file is part of the Opera web browser.
 * It may not be distributed under any circumstances.
 */

#ifndef OPDEV_UTIL_H
#define OPDEV_UTIL_H

#include "optype.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/**
 * @def OD_MIN
 * @brief Utility macro for finding the smallest of two values.
 *
 * @see OD_MAX
 *
 * @param[in] x the first value.
 * @param[in] y the second value.
 *
 * @return x if x is smaller than y, y otherwise.
 */
#define OD_MIN(x, y) (((x) < (y)) ? (x) : (y))

/**
 * @def OD_MAX
 * @brief Utility macro for finding the biggest of two values.
 *
 * @see OD_MIN
 *
 * @param[in] x the first value.
 * @param[in] y the second value.
 *
 * @return x if x is bigger than y, y otherwise.
 */
#define OD_MAX(x, y) (((x) > (y)) ? (x) : (y))

/**
 * @def od_countof
 * @brief Utility macro for getting the elements count of an array.
 *
 * @param[in] _Array the array.
 *
 * @return the elements count of the given array.
 */
#if !defined(__cplusplus) || (defined(_MSC_VER) && _MSC_VER < 1300)
/* VC6 doesn't support using template to get the count of an array. */
#define od_countof(_Array) (sizeof(_Array) / sizeof(_Array[0]))
#else
extern "C++"
{
	template <typename T, unsigned int N>
	char (*__od_countof_helper(T (&_Array)[N]))[N];
	#define od_countof(_Array) sizeof(*__od_countof_helper(_Array))
}
#endif /* __cplusplus */

/**
 * @def od_offsetof
 * @brief Utility macro for finding the offset of a field in a structure.
 *
 * @param[in] s the container structure.
 * @param[in] m the field to look the offset for.
 *
 * @return the offset of the given field in the structure, in bytes.
 */
#define od_offsetof(s, m)   ((unsigned int)&(((s*)0)->m))

/**
 * Compares two given multi byte strings for equality, ignoring case
 * differences.
 *
 * @warn NO checks are done on the input pointers.  An assertion will
 *       trigger in debug mode but it <b>WILL</b> crash in release mode
 *       if invalid data is being passed.
 *
 * @warn Case differences will be ignored only for the ASCII range of
 *       character, namely from A to Z.  Case differences between
 *       characters like U+00C0..U+00DE and others will still cause
 *       comparisons to fail.
 *
 * Comparisons are made in lexicographical order, favouring character
 * order rather than length.
 *
 * @param[in] s1 the first string.
 * @param[in] s2 the second string.
 *
 * @return a positive integer if s1 is greater than s2, zero if s1 and s2
 *         match, or a negative integer if s2 is greater than s1.
 */
OD_ALL_TASK int od_stricmp(const char *s1, const char *s2);

/**
 * Implementation of standard strnicmp function
 */
OD_ALL_TASK int od_strnicmp(const char *s1, const char *s2, unsigned int n);

/************************************************************************/
/* wchar                                                                */
/************************************************************************/

/**
 * Returns the length of a given wide-char string.
 *
 * @warn NO checks are done on the input pointer.  An assertion will
 *       trigger in debug mode but it <b>WILL</b> crash in release mode
 *       if invalid data is being passed.
 *
 * @param[in] s the string to get the length for.
 *
 * @return the string length, in characters.
 */
OD_ALL_TASK unsigned int od_wcslen(const OD_WCHAR* s);

/**
 * Copies a wide-character string into another, up to a given number of
 * characters.
 *
 * @warn NO checks are done on the input pointers.  An assertion will
 *       trigger in debug mode but it <b>WILL</b> crash in release mode
 *       if invalid data is being passed.
 *
 * @param[in] dest the buffer containing the string copy.
 * @param[in] src the source string.
 * @param[in] n the maximum amount of characters to copy.
 *
 * @return a pointer to the destination string.
 */
OD_ALL_TASK OD_WCHAR * od_wcsncpy(OD_WCHAR *dest, const OD_WCHAR *src, unsigned int n);

/**
 * Concatenates a wide-character string to another, up to a given number
 * of characters.
 *
 * @warn NO checks are done on the input pointers.  An assertion will
 *       trigger in debug mode but it <b>WILL</b> crash in release mode
 *       if invalid data is being passed.
 *
 * @param[in] dest the buffer containing the concatenated string.
 * @param[in] src the source string.
 * @param[in] n the maximum amount of characters to copy.
 *
 * @return the string length, in characters.
 */
OD_ALL_TASK OD_WCHAR * od_wcsncat(OD_WCHAR *dest, const OD_WCHAR *src, unsigned int n);

/**
 * Finds the first instance of the given wide character in the given
 * wide-char string.
 *
 * @warn NO checks are done on the input pointer.  An assertion will
 *       trigger in debug mode but it <b>WILL</b> crash in release mode
 *       if invalid data is being passed.
 *
 * @param[in] s the string to look into.
 * @param[in] c0 the character to look for.
 *
 * @return a pointer of the first instance of c0 in s, or NULL if
 *         not found.
 */
OD_ALL_TASK const OD_WCHAR * od_wcschr(const OD_WCHAR *s, int c0);

/**
 * Finds the last instance of the given wide character in the given
 * wide-char string.
 *
 * @warn NO checks are done on the input pointer.  An assertion will
 *       trigger in debug mode but it <b>WILL</b> crash in release mode
 *       if invalid data is being passed.
 *
 * @param[in] s the string to look into.
 * @param[in] c0 the character to look for.
 *
 * @return a pointer of the last instance of c0 in s, or NULL if
 *         not found.
 */
OD_ALL_TASK const OD_WCHAR * od_wcsrchr(const OD_WCHAR *s, int c0);

/**
 * Finds the first instance of a given wide-char substring in a given
 * string.
 *
 * @warn NO checks are done on the input pointers.  An assertion will
 *       trigger in debug mode but it <b>WILL</b> crash in release mode
 *       if invalid data is being passed.
 *
 * @param[in] haystack the string to look into.
 * @param[in] needle0 the string to look for.
 *
 * @return a pointer of the first instance of needle0 in haystack, or
 *         NULL if not found.
 */
OD_ALL_TASK OD_WCHAR * od_wcsstr(const OD_WCHAR *haystack, const OD_WCHAR *needle0);

/**
 * Compares two given wide-char strings for equality.
 *
 * @warn NO checks are done on the input pointers.  An assertion will
 *       trigger in debug mode but it <b>WILL</b> crash in release mode
 *       if invalid data is being passed.
 *
 * Comparisons are made in lexicographical order, favouring character
 * order rather than length.
 *
 * @param[in] s1 the first string.
 * @param[in] s2 the second string.
 *
 * @return a positive integer if s1 is greater than s2, zero if s1 and s2
 *         match, or a negative integer if s2 is greater than s1.
 */
OD_ALL_TASK int od_wcscmp(const OD_WCHAR *s1, const OD_WCHAR *s2);

/* Notes: When n is 0, return 0. */
OD_ALL_TASK int od_wcsncmp(const OD_WCHAR *s1, const OD_WCHAR *s2, int n);

/**
 * Compares two given wide-char strings for equality, ignoring case
 * differences.
 *
 * @warn NO checks are done on the input pointers.  An assertion will
 *       trigger in debug mode but it <b>WILL</b> crash in release mode
 *       if invalid data is being passed.
 *
 * @warn Case differences will be ignored only for the ASCII range of
 *       character, namely from A to Z.  Case differences between
 *       characters like U+00C0..U+00DE and others will still cause
 *       comparisons to fail.
 *
 * Comparisons are made in lexicographical order, favouring character
 * order rather than length.
 *
 * @param[in] s1 the first string.
 * @param[in] s2 the second string.
 *
 * @return a positive integer if s1 is greater than s2, zero if s1 and s2
 *         match, or a negative integer if s2 is greater than s1.
 */
OD_ALL_TASK int od_wcsicmp(const OD_WCHAR *s1, const OD_WCHAR *s2);

OD_ALL_TASK OD_BOOL od_wcsendwith(const OD_WCHAR *s1, const OD_WCHAR* s2);

/**
 * @def OD_BREAKIF
 * @brief Utility macro for simple scope management
 *
 * This macro simply executes the given expression, and breaks from the
 * current scope if the return code (which should be of type OD_STATUS) is
 * an error condition according to OD_Error.
 *
 * @param[in] expression The expression to evaluate.
 */
#define OD_BREAKIF(X) {OD_STATUS t = (X); if (t != OD_SUCCESS) break;}

/**
 * @def OD_RETURNIF
 * @brief Utility macro for simple error management
 *
 * This macro simply executes the given expression, and returns from the
 * current function if the return code (which should be of type OD_STATUS)
 * is an error condition according to OD_Error.  The macro forces the
 * function to return with the same return code it got from the expression
 * being evaluated.
 *
 * @param[in] expression The expression to evaluate.
 */
#define OD_RETURNIF(X) {OD_STATUS t = (X); if (t != OD_SUCCESS) return t;}

#define OD_CHECK_ARGS(X, RET) { if(!(X)) { OD_ASSERT(X); return RET; }}
#define OD_CHECK_ARGS_RETURN_VOID(X) { if(!(X)) { OD_ASSERT(X); return; }}
#define OD_CHECK_ARGS_RETURN_STATUS(X) { if(!(X)) { OD_ASSERT(X); return OD_INVALID_PARAM; }}

#define OD_RETURNIFNOCTX(ctx) \
	do \
	{ \
		OD_ASSERT(ctx && "Missing context"); \
		if (!ctx) \
			return OD_NULL_POINTER; \
	} while (0)

#ifdef __cplusplus
};
#endif /* __cplusplus */

#endif /* OPDEV_SYSTEM_H */
