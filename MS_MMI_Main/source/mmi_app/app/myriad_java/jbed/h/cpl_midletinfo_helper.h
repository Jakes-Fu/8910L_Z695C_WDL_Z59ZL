/**
 * Copyright 2010-2011 Myriad Group AG. All Rights Reserved.
 * $Id$
 */
#ifndef CPL_MIDLETINFO_HELPER
#define CPL_MIDLETINFO_HELPER

#include <cpl_midletinfo.h>
#include <jbed_jbni.h>

/**
 * Helper to set midlet info string value
 */
#define SET_MI_STRING(pstr, d, size) {                                                           \
                                           CPL_MI_STRING *ptstr   = (pstr);                      \
                                           size_t        tmpSize = (size_t)(size);             \
                                           uint16_t      *tmpData = (d);                         \
                                           ptstr->data = (tmpSize > 0) ? ((uint16_t *)SCI_ALLOCA((tmpSize + 1) * sizeof(uint16_t))) : NULL; \
                                           if (ptstr->data != NULL)                              \
                                           {                                                     \
                                               CRTL_memcpy(ptstr->data, tmpData, tmpSize * sizeof(uint16_t)); \
                                               ptstr->data[tmpSize] = 0;                         \
                                               ptstr->length = (int32_t)tmpSize;                 \
                                           }                                                     \
                                           else                                                  \
                                           {                                                     \
                                               ptstr->length = 0;                                \
                                           }                                                     \
                                     }

/**
 * Helper to check whether the midlet information string is empty or not
 */
#define IS_EMPTY_MI_STRING(pstr) ((pstr)->data == NULL)


/**
 * Helper to free the midlet information string
 */
#define FREE_MI_STRING(str) {CPL_MI_STRING *ptstr = (str); \
                             if (ptstr->data != NULL)      \
                                SCI_FREE(ptstr->data);    \
                             ptstr->data = NULL;           \
                             ptstr->length = 0;}

/**
 * Helper to convert MI string into simple string
 */
#define MI_STRING_TO_STRING(dst, d, size) {                                                     \
                                            uint16_t      *tmpData = (d);                       \
                                            int32_t        tmpSize = (int32_t)(size);           \
                                            char         **pdst    = (dst);                     \
                                            *pdst = (tmpSize > 0) ? ((char *) SCI_ALLOCA((tmpSize + 1) * sizeof(char))) : NULL;\
                                            if (*pdst != NULL)                                  \
                                            {                                                   \
                                                CRTL_memset(tmpData, 0, (tmpSize + 1));         \
                                                Jbni_convertUcs2ToAscii(tmpData, CRTL_wstrlen(tmpData), *pdst, tmpSize); \
                                            }                                                   \
                                          }

/**
 * Deallocates memory of all the suite info items that were dynamically.
 * allocated. The memory occupied by the CPL_MI_suiteInfo structure itself
 * is not deallocated, to do so, one must call CPL_midletInfo_deallocateSuiteInfo.
 * NOTE: This function is eligible for calling for the suite info structure
 * allocated on stack. This MUST not be used outside the CPL midlet info module,
 * CPL_midletInfo_allocateSuiteInfo() / CPL_midletInfo_deallocateSuiteInfo()
 * pair must be used outside.
 *
 * @param suiteInfo pointer to memory that is being de-allocated
 */
void CPL_midletInfo_deallocateSuiteInfoItems(CPL_MI_suiteInfo_s *suiteInfo);

/**
 * Allocates memory for suite info and sets all the info fields
 * to their default values. Any external code must use this function
 * to allocate suite info structures.
 *
 * @param pointer to the newly allocated structure if succeeded,
 * NULL otherwise
 */
CPL_MI_suiteInfo_s *CPL_midletInfo_allocateSuiteInfo(void);

/**
 * Deallocates memory of suite info previously allocated by
 * CPL_midletInfo_allocateSuiteInfo().
 *
 * @param suiteInfo pointer to memory that is being de-allocated
 */
void CPL_midletInfo_deallocateSuiteInfo(CPL_MI_suiteInfo_s *suiteInfo);

/**
 * Checks whether the given suite (candidate) matches to the given template (matcher)
 * The candidate matches to the matcher if all the matcher fields that are set, are
 * equal to the corresponding candidate fields.
 *
 * @param candidate
 * @param matcher
 * @return TRUE if the candidate matches the matcher, FALSE otherwise
 */
bool_t CPL_midletInfo_suiteMatches(const CPL_MI_suiteInfo_s *candidate, const CPL_MI_suiteInfo_s *matcher);

#endif /* CPL_MIDLETINFO_HELPER */
