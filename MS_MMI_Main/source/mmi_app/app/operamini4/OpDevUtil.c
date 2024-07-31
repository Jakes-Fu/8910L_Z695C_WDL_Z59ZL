/* -*- Mode: c++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*-
 *
 * Copyright (C) 1995-2011 Opera Software AS.  All rights reserved.
 *
 * This file is part of the Opera web browser.
 * It may not be distributed under any circumstances.
 */

#include "opcfg.h"
#include "opdevinternal.h"
#include "opimport.h"

static int _od_tolower(int c);

int od_stricmp(const char *s1, const char *s2)
{
	OD_ASSERT( s1 != NULL && s2 != NULL );

	while (*s1 && *s2 && (*s1 == *s2 || _od_tolower(*s1) == _od_tolower(*s2)))
		++s1, ++s2;
	return _od_tolower(*s1) - _od_tolower(*s2);
}

int od_strnicmp(const char *s1, const char *s2, size_t n)
{
	while (n > 0 && *s1 && *s2 && (*s1 == *s2 || _od_tolower(*s1) == _od_tolower(*s2)))
	{
		++s1, ++s2, --n;
	}

	return n > 0 ? _od_tolower(*s1) - _od_tolower(*s2) : 0;
}

unsigned int od_wcslen(const OD_WCHAR* s)
{
    const OD_WCHAR *t = s;

    OD_ASSERT( s != NULL );

    while (*t++ != 0);
    return t - s - 1;
}

OD_WCHAR * od_wcsncpy(OD_WCHAR *dest, const OD_WCHAR *src, unsigned int n)
{
    OD_WCHAR *dest0 = dest;

    OD_ASSERT( dest != NULL && src != NULL );

    while (n > 0 && *src != 0)
    {
        *dest++ = *src++;
        --n;
    }

    /* We are required to add a trailing nul, even when n is zero, which
    means that the n passed as parameter has to be one less than the
    remaining space in dest (C99 7.21.3.2.2)  */

    *dest = 0;

    return dest0;
}

OD_WCHAR * od_wcsncat(OD_WCHAR *dest, const OD_WCHAR *src, unsigned int n)
{
    OD_WCHAR *dest0 = dest;

    OD_ASSERT( dest != NULL && src != NULL );

    while (*dest != 0)
        dest++;

    while (n > 0 && *src != 0)
    {
        *dest++ = *src++;
        --n;
    }

    /* We are required to add a trailing nul, even when n is zero, which
    means that the n passed as parameter has to be one less than the
    remaining space in dest (C99 7.21.3.2.2)  */

    *dest = 0;

    return dest0;
}

const OD_WCHAR * od_wcschr(const OD_WCHAR *s, int c0)
{
    OD_WCHAR c = (OD_WCHAR)c0;

    OD_ASSERT( s != NULL );

    while (*s && *s != c)
        ++s;
    return *s == c ? s : NULL;
}

const OD_WCHAR * od_wcsrchr(const OD_WCHAR *s, int c0)
{
    OD_WCHAR c = (OD_WCHAR)c0;
    const OD_WCHAR * p = NULL;

    OD_ASSERT( s != NULL );

    do
    {
        if (*s == c)
            p = s;
    } while (*s++ != 0);
    return (OD_WCHAR *)p;
}

OD_WCHAR * od_wcsstr(const OD_WCHAR * haystack, const OD_WCHAR * needle0)
{
    OD_ASSERT( haystack != NULL && needle0 != NULL );

    if (!*needle0)
        return (OD_WCHAR *) haystack;

    if (*haystack)
    {
        const OD_WCHAR *needle = (const OD_WCHAR *)needle0;
        do
        {
            if (*needle == *haystack)
            {
                /* Continue scanning if we find a match */
                ++ needle;

                /*
                 * Return pointer to beginning of string when we have
                 * scanned to the end of needle.
                 */
                if (!*needle)
                    return (OD_WCHAR *) haystack - ((const OD_WCHAR *) needle - needle0) + 1;
            }
            else if (needle != (const OD_WCHAR *) needle0)
            {
                /* Else backtrack to the start of the current substring. */
                haystack -= ((const OD_WCHAR *) needle - needle0);
                needle = (const OD_WCHAR *) needle0;
            }

            ++ haystack;
        } while (*haystack);
    }

    return NULL;
}

int od_wcscmp(const OD_WCHAR *s1, const OD_WCHAR *s2)
{
    OD_ASSERT( s1 != NULL && s2 != NULL );

    while (*s1 && *s1 == *s2)
        ++s1, ++s2;
    return (int)*s1 - (int)*s2;
}

/* Notes: When n is 0, return 0. */
int od_wcsncmp(const OD_WCHAR *s1, const OD_WCHAR *s2, int n)
{
	OD_ASSERT( s1 != NULL && s2 != NULL );
	OD_ASSERT( n >= 0);

	while (n > 0 && *s1 && *s1 == *s2)
		--n, ++s1, ++s2;
	return n == 0 ? 0 : ((int)*s1 - (int)*s2);
}

static int _od_tolower(int c)
{
    if (c >= 'A' && c <= 'Z')
        return c + 'a' - 'A';

    return c;
}

int od_wcsicmp(const OD_WCHAR *s1, const OD_WCHAR *s2)
{
    OD_ASSERT( s1 != NULL && s2 != NULL );

    while (*s1 && *s2 && (*s1 == *s2 || _od_tolower(*s1) == _od_tolower(*s2)))
        ++s1, ++s2;
    return _od_tolower(*s1) - _od_tolower(*s2);
}

OD_BOOL od_wcsendwith(const OD_WCHAR *s1, const OD_WCHAR* s2)
{
	int len1, len2;

	len1 = od_wcslen(s1);
	len2 = od_wcslen(s2);
	if(len1 < len2)
		return OD_FALSE;
	else
		return 0 == od_wcsncmp(s1 + len1 - len2, s2, len2);
}
