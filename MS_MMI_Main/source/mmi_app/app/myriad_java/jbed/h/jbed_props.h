/*
 * Copyright 2005-2011 Myriad Group AG. All Rights Reserved.
 * $Id$
 */

#ifndef _JBED_PROPS_H_
#define _JBED_PROPS_H_

#if defined(__cplusplus)
extern "C" {
#endif

#define DEFAULT_FILE_BASEDIR "/Installed/"
#define DEFAULT_FILE_ROOTDIR "/"

/* Return the prop has been intialized or not? */
bool_t isPropInit(void);

/** Initialise the properties subsystem. */
void Jbed_props_startup(void);

/** Finalise and free off any properties. */
void Jbed_props_shutdown(void);

/**
 * Search for a property in the system properties (publicly visible ones)
 * @param key   Key (property name) to look for (UTF8)
 * @return Returns ptr to UTF8 value of property, or NULL if not found.
 */
const char *Jbed_getProperty(const char* key);

/**
 * Set a property value (publicly visible ones)
 * @param key   Key name of the property to set (UTF8)
 * @param val   Value of property to set (UTF8)
 * @return Returns the new copy of val on success, and NULL on failure
 */
const char *Jbed_setProperty(const char* key, const char* val);

/**
 * Convienience function to set a property if not already set.
 * @param key   Key name of the property to set (UTF8)
 * @param val   Value of property to set (UTF8)
 * @return Returns the new value on success, or NULL on failure
 */
const char *Jbed_setPropertyDefault(const char *key, const char *value);

/* FDAC Also has the notion of internal properties that cannot be
 * retrieved by using System.getProperty(), but are used for internal
 * configuration of the JVM and libraries
 */

/**
 * Search for a property in the system properties (internal properties)
 * @param key   Key (property name) to look for (UTF8)
 * @return Returns ptr to UTF8 value of property, or NULL if not found.
 */
const char *Jbed_getInternalProperty(const char* key);

/**
 * Set a property value (internal properties)
 * @param key   Key name of the property to set
 * @param val   Value of property to set
 * @return Returns the new copy of val on success, and NULL on failure
 */
const char *Jbed_setInternalProperty(const char* key, const char* val);

/**
 * del a property in the system properties (publicly visible ones)
 * @param key:  Key (property name) to look for (UTF8)
 * @return Returns:if delete key successfully,return TRUE, else return FALSE.
 */
bool_t Jbed_deleteProperty(const char* key);

void Jbed_setVmBuildID(void);

void setAppPropertyValue(const char *key, const char *val);

#ifdef JBED_CDCA

/** Callback prototype. Called once for each key/value pair in the
 * properties database, in search order.
 * May generate multiple entries for a given key if the database has
 * duplicates.
 * The context pointer is passed every time unchanged.
 * @return Zero to continue enumeration, non-zero error code otherwise.
 */
typedef int (*propsCallback)(void *context,
                        const char *key, const char *value);

/**
 * Call the callback function once for each key/value pair.
 * Pass the context pointer with each callback.
 *
 * @param fn        call back function
 * @param context   data pointer to pass to callback
 * @return 0 if all enumerated OK, or error code from callback
 */
int Jbed_enumerateProperties(propsCallback fn, void *context);

#endif  /* JBED_CDCA */

/* Backward compatibility interfaces
 *
 * These interfaces are deprecated, and will be removed in later version.
 */
#ifdef JVM_FBCC
#define jbed_props_init()                   Jbed_props_startup()
#define jbed_props_destroy()                Jbed_props_shutdown()
#define jbed_props_getProperty(key)         Jbed_getProperty(key)
#define jbed_props_setProperty(k, v)        Jbed_setProperty(k, v)
#define jbed_props_delProperty(key)         Jbed_deleteProperty(key)
#define jbed_props_setPropertyDefault(k, v) Jbed_setPropertyDefault(k, v)

#endif /* JVM_FBCC */

#if defined(__cplusplus)
}
#endif

#endif  /* _JBED_PROPS_H_ */
