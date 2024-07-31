/*
 * Copyright 2010-2015 Myriad Group AG. All Rights Reserved.
 * $Id$
 */

#ifndef CPL_MIDLETINFO
#define CPL_MIDLETINFO

#include <jbed_types.h>
/**
 * Common error codes for native MIDlet info functions
 */
typedef enum
{
   CPL_MI_FAILURE,
   CPL_MI_SUCCESS,
}
CPL_MI_RESULT;

/**
 * MIDlet suite information item IDs
 */
typedef enum
{
    MI_NAME,
    MI_ROOT,
#ifdef AMS_FOLDERS
    MI_FOLDER,
#endif
    MI_VENDOR,
    MI_VERSION,
    MI_PREINSTALLED,
    MI_MIDLET,
    MI_CLASS,
    MI_DOMAIN,
    MI_FILE_ROOT,
    MI_REMOVABLE,
#ifdef MICROSOFT_WNS_SUPPORT
    MI_WNS_IDENTIFIER,
#endif
    SI_JAD_URL,
    SI_JAR_URL,
    SI_JAR_SIZE,
    SI_VERSION,
    SI_VENDOR,
#ifdef AMS_FOLDERS
    MI_END_FOLDER,
#endif
    MI_LAST
}
CPL_MI_ITEM;

/**
 * Suite type: preinstalled (= preloaded) / downloaded
 */
typedef enum
{
    DUMMYTYPE    = 0x00000000,
    PREINSTALLED = 0x00000001,
    DOWNLOADED   = 0x00000002,
}
CPL_MI_SUITE_TYPE;

/**
 * Structure representing a midlet info
 * UTF-16 string
 */
typedef struct
{
    uint16_t *data;
    int32_t   length;
}
CPL_MI_STRING;

#define MAX_MIDLET_NUM 32

/**
 * The public suite information structure
 */
typedef struct
{
    CPL_MI_STRING     name;
    CPL_MI_STRING     version;
#ifdef AMS_FOLDERS
    CPL_MI_STRING     folder;
#endif
    CPL_MI_STRING     vendor;
    CPL_MI_STRING     root;
    CPL_MI_STRING     domain;
    CPL_MI_STRING     file_root;
    uint32_t          removable; // TRUE or FALSE
#ifdef MICROSOFT_WNS_SUPPORT
    CPL_MI_STRING     wns_identifier;
#endif
    CPL_MI_STRING     jad_url;
    CPL_MI_STRING     jar_url;
    int32_t           jarSize;
    uint32_t          type; // PREINSTALLED or DOWNLOADED
    uint32_t          numMidlet;
    CPL_MI_STRING     midlet[MAX_MIDLET_NUM];
    CPL_MI_STRING     classname[MAX_MIDLET_NUM];
}
CPL_MI_suiteInfo_s;

/**
 * The internal suite representation in the native list
 */
typedef struct _CPL_midletInfo_suiteEntry_s
{
    int32_t                              id;
    CPL_MI_suiteInfo_s                   info;
    struct _CPL_midletInfo_suiteEntry_s *next;
}
CPL_MI_suiteEntry_s;

/**
 * Initializes the module.
 *
 * @return CPL_MI_SUCCESS if the initialization has been successful,
 * CPL_MI_FAILURE
 */
CPL_MI_RESULT CPL_midletInfo_initialize(void);

/**
 * Adds a suite to the native list
 *
 * @param suiteID ID of the suite being added
 * @param info    information of the suite being added
 *
 * @return CPL_MI_SUCCESS if adding has been successful, CPL_MI_FAILURE
 * otherwise
 */
CPL_MI_RESULT CPL_midletInfo_addSuite(int32_t suiteID, CPL_MI_suiteInfo_s *info);

/**
 * Updates a suite in the native list
 *
 * @param suiteID ID of the suite being added
 * @param newInfo new information of the suite being updated
 *
 * @return CPL_MI_SUCCESS if update has been successful, CPL_MI_FAILURE
 * otherwise
 */
CPL_MI_RESULT CPL_midletInfo_updateSuite(int32_t suiteID, const CPL_MI_suiteInfo_s *newInfo);

/**
 * Removes a suite from the native list
 *
 * @param suiteID ID of the suite being removed
 *
 * @return CPL_MI_SUCCESS if update has been successful, CPL_MI_FAILURE
 * otherwise
 */
CPL_MI_RESULT CPL_midletInfo_removeSuite(int32_t suiteID);

/**
 * Gets a specified suite infor item value for a specified currently installed suite.
 *
 * @param suiteID   suite ID.
 * @param itemID suite info item ID.
 * @infoBuffer the pointer to the pointer of the output buffer.
 *
 * @return CPL_MI_SUCCESS if get suite info item has been successful, CPL_MI_FAILURE
 * otherwise
 */
CPL_MI_RESULT CPL_midletInfo_getSuiteInfoItem(int32_t suiteID, CPL_MI_ITEM itemID, void **infoBuffer);

/**
 * Gets all suite infromation items for a specified currently installed suite.
 *
 * @param suiteID   ID of the sutie
 * @param suiteInfo output structure
 */
CPL_MI_RESULT CPL_midletInfo_getSuiteInfo(int32_t suiteID, CPL_MI_suiteInfo_s *suiteInfo);

/**
 * Gets the number of currently installed suites.
 *
 * @param type   the type of the MIDlet suite to be returned: any combination of
 * PREINSTALLED and DOWNLOADED.
 */
uint32_t CPL_midletInfo_getMIDletCount(const CPL_MI_suiteInfo_s *matcher);

/**
 * Gets the list of suite id for the currently installed Availed suites.
 *
 * @param list   out parameter - the list of the currently installed MIDlet suites
 * @param length the length of the list (in terms of int32_t) values.
 * @param type   the type of the MIDlet suite to be returned: any combination of
 * PREINSTALLED and DOWNLOADED.
 *
 * @return real number of suites returned
 */

uint32_t CPL_midletInfo_getAvailedSuiteIDList(int32_t *list, uint32_t length, const CPL_MI_suiteInfo_s *matcher);

/**
 * Gets the list of suite id for the currently installed suites.
 *
 * @param list   out parameter - the list of the currently installed MIDlet suites
 * @param length the length of the list (in terms of int32_t) values.
 * @param type   the type of the MIDlet suite to be returned: any combination of
 * PREINSTALLED and DOWNLOADED.
 *
 * @return real number of suites returned
 */
uint32_t CPL_midletInfo_getSuiteIDList(int32_t *list, uint32_t length, const CPL_MI_suiteInfo_s *matcher);

/**
 * Finalizes the module.
 */
void CPL_midletInfo_finalize(void);

/**
 *  Get a copy of current midlet suite list into the pMidlet pointed structure.
 *  the pMidlet must not be NULL.
 */
CPL_MI_RESULT CPL_midletInfo_get(CPL_MI_suiteEntry_s *pMidlet);

/** Get midletList. */
CPL_MI_suiteEntry_s *CPL_midletInfo_getMidletList(void);

/**
 * Release the midlet suite list got from CPL_midletInfo_get
 */
CPL_MI_RESULT CPL_midletInfo_release(CPL_MI_suiteEntry_s *pMidlet);

/**
 * Return TRUE if preload mark is found in selector.utf
 */
int32_t CPL_midletInfo_getPreloadMarkStatus(void);

#if defined(NANDBOOT_SUPPORT) && defined(DYNAMIC_MAINMENU_SUPPORT)
/* Set needCreateShortcut flag for nand flash preinstall.*/
void CPL_midletInfo_setNeedShortcutFlag(bool_t flag);

/* Return the flag of needCreateShortcut.*/
bool_t CPL_midletInfo_getNeedShortcutFlag(void);
#endif
#endif /* CPL_MIDLETINFO */
