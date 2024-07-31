/*
 * Copyright 2004-2008 Esmertec AG. All Rights Reserved.
 * $Id$
 */

/**
 * @file
 * Jbed Porting Layer: PIM support (VM-independent).
 * This interface provides PIM support for the Jbed VM.
 */

#ifndef _CPL_PIM_H__
#define _CPL_PIM_H__

#include <jbed_global.h>
#include <jbed_jbni.h>
#include <javainstance.h>

#if defined(__cplusplus)
extern "C" {
#endif
/**
 * @name Return values for the PIM CPL functions
 */

/*@{*/

/** Indicates successful completion of an operation. */
#define CPL_PIM_SUCCESS            0
/** Indicates that an operation failed. */
#define CPL_PIM_ERROR             -128
/** Shows no limit value. */
#define CPL_PIM_NO_LIMIT          -1
/** Indicates that an operation is implemented asynchronously, and the caller
 *  will be notified by an ASYNC_notify() when the results are made available
 *  in the asynchronous IO buffers.
 */
#define CPL_PIM_EWOULDBLOCK      -129

/*@}*/

/**
 * @subsection PIMLists Constants representing the PIM Lists
 * @name Constants representing the PIM Lists
 */

/*@{*/

/** Constant representing a Contact List. */
#define CONTACT_LIST 1
/** Constant representing an Event List. */
#define EVENT_LIST 2
/** Constant representing a ToDo List. */
#define TODO_LIST 3

/*@}*/

/**
 * @subsection RRuleFields Repeat Rule fields
 * @name Repeat Rule fields
 */

/*@{*/

/** Field specifying the frequency of the Repeat. */
#define REPEAT_RULE_FREQUENCY  0
/** Field specifying the number of times the Event item repeats including
 *  the first time, starting from the first time the event starts and
 *  continuing to the last date of the repeat.
 */
#define REPEAT_RULE_COUNT  32
/** Field specifying the number of iterations of the frequency between
 *  occurring dates, or how often the frequency repeats.
 */
#define REPEAT_RULE_INTERVAL  128
/** Field specifying the ending date of the repeating event. */
#define REPEAT_RULE_END  64
/** Field specifying the month in which an event occurs. */
#define REPEAT_RULE_MONTH_IN_YEAR  8
/** Field specifying the days of the week an Event occurs. */
#define REPEAT_RULE_DAY_IN_WEEK  2
/** Field specifying which week in a month a particular event occurs. */
#define REPEAT_RULE_WEEK_IN_MONTH  16
/** Field specifying the day of the month an Event occurs. */
#define REPEAT_RULE_DAY_IN_MONTH  1
/** Field specifying the day of the year an Event occurs. */
#define REPEAT_RULE_DAY_IN_YEAR  4

/*@}*/

/**
 * @subsection RRules Repeat Rule constants describing patterns for Event items
 * @name Repeat Rule constants describing patterns for Event items
 */

/*@{*/

/** Used for frequency when the Event happens every day. */
#define REPEAT_RULE_DAILY  16
/** Used for frequency when the Event happens every week. */
#define REPEAT_RULE_WEEKLY  17
/** Used for frequency when the Event happens every month. */
#define REPEAT_RULE_MONTHLY  18
/** Used for frequency when the Event happens every year. */
#define REPEAT_RULE_YEARLY  19

/*@}*/

/**
 * @subsection ContactFields Contact fields
 * @name Contact fields
 */

/*@{*/

/** Field specifying the name for this contact. */
#define CONTACT_NAME 106
/** Field specifying an address for this Contact. */
#define CONTACT_ADDR 100
/** Field for an e-mail address. */
#define CONTACT_EMAIL 103
/** Field represents a formatted version of a name for the Contact entry. */
#define CONTACT_FORMATTED_NAME 105
/** Field represents a formatted version of a complete address for the
 *  Contact entry.
 */
#define CONTACT_FORMATTED_ADDR 104
/** Field where the data represents a nickname. */
#define CONTACT_NICKNAME 107
/** Field specifying supplemental information or a comment associated with
 *  a Contact.
 */
#define CONTACT_NOTE 108
/** Field specifying the organization name or units associated with a
 *  Contact.
 */
#define CONTACT_ORG 109
/** Field for a voice telephone number. */
#define CONTACT_TEL 115
/** Field specifying the job title for a Contact. */
#define CONTACT_TITLE 116
/** Field specifying a unique ID for a Contact. */
#define CONTACT_UID 117
/** Field specifying the uniform resource locator for a Contact. */
#define CONTACT_URL 118
/** Field for the birthday of the Contact. */
#define CONTACT_BIRTHDAY 101
/** Field specifying the last modification date and time of a Contact item. */
#define CONTACT_REVISION 114
/** Field specifying a photo for a Contact. */
#define CONTACT_PHOTO 110
/** Field specifying the public encryption key for a Contact. */
#define CONTACT_PUBLIC_KEY 112
/** Field specifying a photo of a Contact. */
#define CONTACT_PHOTO_URL 111
/** Field specifying the public encryption key for a Contact. */
#define CONTACT_PUBLIC_KEY_STRING 113
/** Field specifying the desired access class for this contact. */
#define CONTACT_CLASS 102

/*@}*/

/**
 * @subsection ContactAttrs Contact attributes
 * @name Contact attributes
 */

/*@{*/

/** Attribute classifying a data value as related to an ASSISTANT. */
#define CONTACT_ATTR_ASST 1
/** Attribute classifying a data value as related to AUTO. */
#define CONTACT_ATTR_AUTO 2
/** Attribute classifying a data value as related to FAX. */
#define CONTACT_ATTR_FAX 4
/** Attribute classifying a data value as related to HOME. */
#define CONTACT_ATTR_HOME 8
/** Attribute classifying a data value as related to MOBILE. */
#define CONTACT_ATTR_MOBILE 16
/** Attribute classifying a data value as "OTHER". */
#define CONTACT_ATTR_OTHER 32
/** Attribute classifying a data value as related to PAGER. */
#define CONTACT_ATTR_PAGER 64
/** Attribute classifying a data value with preferred status
 *  for retrieval or display purposes (platform specific).
 */
#define CONTACT_ATTR_PREF 128
/** Attribute classifying a data value as related to SMS. */
#define CONTACT_ATTR_SMS 256
/** Attribute classifying a data value as related to WORK. */
#define CONTACT_ATTR_WORK 512
/** Attribute classifying a data value as related to
  * TEL field for video telephony.
  */
#define CONTACT_ATTR_VIDEO_CALL 16777216

/*@}*/

/**
 * @subsection ContactIdxAddr Contact indexes into the string array for an address field
 * @name Contact indexes into the string array for an address field
 */

/*@{*/

/** Index into the string array for an address field, where the data at
 *  this index represents the post office box of a particular address.
 */
#define CONTACT_ADDR_POBOX 0
/** Index into the string array for an address field, where the data at
 *  this index represents any extra info of a particular address.
 */
#define CONTACT_ADDR_EXTRA 1
/** Index into the string array for an address field, where the data at
 *  this index represents the street information of a particular address.
 */
#define CONTACT_ADDR_STREET 2
/** Index into the string array for an address field, where the data at
 *  this index represents the locality (for example, a city) of a
 *  particular address.
 */
#define CONTACT_ADDR_LOCALITY 3
/** Index into the string array for an address field, where the data at
 *  this index represents the region (for example, a province, state, or
 *  territory) of a particular address.
 */
#define CONTACT_ADDR_REGION 4
/** Index into the string array for an address field, where the data at
 *  this index represents the postal code (for example, a zip code) of
 *  a particular address.
 */
#define CONTACT_ADDR_POSTALCODE 5
/** Index into the string array for an address field, where the data at
 *  this index represents the country of a particular address.
 */
#define CONTACT_ADDR_COUNTRY 6

/*@}*/

/**
 * @subsection ContactIdxName Contact indexes into the string array for a name field
 * @name Contact indexes into the string array for a name field
 */

/*@{*/

/** Index into the string array for a name field, where the data at
 *  this index represents the family name.
 */
#define CONTACT_NAME_FAMILY 0
/** Index into the string array for a name field, where the data at
 *  this index represents the given name.
 */
#define CONTACT_NAME_GIVEN 1
/** Index into the string array for a name field, where the data at
 *  this index represents other alternate name or names.
 */
#define CONTACT_NAME_OTHER 2
/** Index into the string array for a name field, where the data at
 *  this index represents a prefix to a name.
 */
#define CONTACT_NAME_PREFIX 3
/** Index into the string array for a name field, where the data at
 *  this index represents a suffix to a name.
 */
#define CONTACT_NAME_SUFFIX 4
/** Index into the string array of a name field, where the data at
 *  this index represents the reading of a name.
 */
#define CONTACT_NAME_READING 5

/*@}*/

/**
 * @subsection EventFields Event fields
 * @name Event fields
 */

/*@{*/

/** Field identifying the venue for this Event. */
#define EVENT_LOCATION 103
/** Field specifying a more complete description
 *  than the EVENT_SUMMARY for this Event.
 */
#define EVENT_NOTE 104
/** Field specifying the summary or subject for this Event. */
#define EVENT_SUMMARY 107
/** Field specifying a unique ID for an Event. */
#define EVENT_UID 108
/** Field specifying the non-inclusive date and time a single Event ends. */
#define EVENT_END 102
/** Field specifying the last modification date and time of an Event item. */
#define EVENT_REVISION 105
/** Field specifying the inclusive date and time a single Event starts. */
#define EVENT_START 106
/** Field specifying a relative time for an Alarm for this Event. */
#define EVENT_ALARM 100
/** Field specifying the desired access class for this contact. */
#define EVENT_CLASS 101

/*@}*/

/**
 * @subsection ToDoFields ToDo fields
 * @name ToDo fields
 */

/*@{*/

/** Field specifying a more complete description than the SUMMARY
 *  for this ToDo.
 */
#define TODO_NOTE 104
/** Field specifying the summary or subject for this ToDo. */
#define TODO_SUMMARY 107
/** Field specifying a unique ID for a ToDo. */
#define TODO_UID 108
/** Field specifying the desired access class for this contact. */
#define TODO_CLASS 100
/** Field specifying the priority of this ToDo. */
#define TODO_PRIORITY 105
/** Field ID indicating a ToDo has been completed on the date
 *  indicated by this field.
 */
#define TODO_COMPLETION_DATE 102
/** Field indicating the date a ToDo is due. The data for this field
 *  is expressed in the same long value format as java.util.Date, which
 *  is milliseconds since the epoch (00:00:00 GMT, January 1, 1970).
 */
#define TODO_DUE 103
/** Field specifying the last modification date and time of a ToDo item. */
#define TODO_REVISION 106
/** Field ID indicating a ToDo has been completed. */
#define TODO_COMPLETED 101

/*@}*/

/**
 * Structure to store operation results.
 */
typedef struct CPL_pim_asyncResult_s {
    /* The async result code */
    int32_t result;
    int32_t moreResult;
    int32_t strAddr;
    int32_t strLen;
    uint8_t* byteArrAddr;
    int32_t byteArrLength;
} CPL_pim_asyncResult_t;

/**
 * Starts up the PIM module. The implementation can use this to do one time initialization
 * of the PIM module, but on MVM the implementation needs to count the calls as there will
 * be one call per isolate using the PIM module.
 *
 * @return #CPL_PIM_SUCCESS or #CPL_PIM_ERROR.
 */
int32_t CPL_pim_startUp(void);

/**
 * Shuts down the PIM module. The implementation should free the resources it allocated when
 * CPL_pim_startUp() was first called. On MVM, the calls need to be counted and the resources
 * can only be freed when the count reaches 0, i.e. when there has been as many calls to
 * CPL_pim_shutDown()than there were calls to CPL_pim_startUp().
 *
 * @return #CPL_PIM_SUCCESS or #CPL_PIM_ERROR.
 */
int32_t CPL_pim_shutDown(void);

 /**
  * Returns a list of all PIM List names for the given PIM list type.
  * If there are
  * no Lists for that list type, a zero length byte array is returned. The first
  * name in the list is the name of the default List for a type.
  *
  * The name list should be put in a byte array of the following format:
  * <ul>
  * <li> 4 bytes - number of names in this list</li>
  * <li> 4 bytes - length m of the first name in this list</li>
  * <li> m bytes - first name in this list</li>
  * <li> 4 bytes - length n of the next name in  this list</li>
  * <li> n bytes - next name in this list</li>
  * <li> ...</li>
  * </ul>
  *
  * @param pimListType Type of PIM list.
  * @return #CPL_PIM_EWOULDBLOCK, #CPL_PIM_ERROR or pointer to a byte array in the
  * format specified above.
  */
int32_t CPL_pim_listPIMLists(int32_t pimListType);

/**
 * Used to fetch the next PIM item from a PIM list.
 * The current item is represented by <code>curItem</code>.
 * If <code>curItem</code> is NULL, then the first item
 * in <code>pimList</code> is returned.
 * The next PIM item is returned in a byte array
 * of the following format:
 * <ul>
 * <li> 4 bytes - length m of PIM item in vCard or vCalendar format</li>
 * <li> m bytes - vCard or vCalendar representation of PIM item</li>
 * </ul>
 *
 * NOTE that if the returned byte array contains the data of an
 * <code>Event</code> item, the value of <code>Event.ALARM</code>
 * field MUST be an absolute date, not the relative time from the
 * <code>Event.START</code>. If there is no <code>Event.START</code>
 * in the item to compute the absolute time, the implementation can:
 *
 * - return #CPL_PIM_ERROR to indicate the failure in the computation
 * of the alarm date.
 * - Silently drop the <code>ALARM</code> field.
 * - Silently create the missing <code>START</code> field.
 *
 * The supported vCard version is 2.1, the supported vCalendar version is 1.0.
 * The following extended fields are added for Contact fields
 * that are not included in vCard 2.1:
 * <ul>
 * <li> CATEGORIES - extended field is "X-JBED-CATEGORIES"</li>
 * <li> PUBLIC_KEY_STRING - extended field is "X-JBED-KEY-STR"</li>
 * <li> CLASS - extended field is "X-JBED-CLASS";</li>
 * </ul>
 * The following extended fields are added for Event fields
 * that are not included in vCalendar 1.0:
 * <ul>
 * <li> NOTE - equals suitable field is "DESCRIPTION"</li>
 * <li> REVISION - equals suitable field is "LAST-MODIFIED"</li>
 * </ul>
 * The following extended fields are added for ToDo fields
 * that are not included in vCalendar 1.0:
 * <ul>
 * <li> NOTE - equals suitable field is "DESCRIPTION"</li>
 * <li> REVISION - equals suitable field is "LAST-MODIFIED"</li>
 * <li> COMPLETED - equals suitable field is "STATUS".</li>
 * Correct values for this property are TRUE and FALSE.
 * </ul>
 *
 * @param curItem         Current PIM item.
 * @param pimListHandle   PIM list handle.
 * @return #CPL_PIM_EWOULDBLOCK, #CPL_PIM_ERROR or pointer to byte array in the
 * format specified above.
 */
int32_t CPL_pim_getNextItem(
                            struct INSTANCE_com_jbed_pim_PIMItemImpl_s * curItem,
                            int32_t pimListHandle);

/**
 * Returns the categories defined for the PIM list.
 * If there are no categories defined
 * for the PIM list or categories are unsupported for the list,
 * then a zero length
 * array is returned.
 * Categories should be put in a byte array of the following format:
 * <ul>
 * <li> 4 bytes - number of categories</li>
 * <li> 4 bytes - length m of the first category</li>
 * <li> m bytes - first category in categories</li>
 * <li> 4 bytes - length n of the next category</li>
 * <li> n bytes - next category in categories</li>
 * <li> ...</li>
 * </ul>
 *
 * @param pimListHandle PIM list handle.
 * @return #CPL_PIM_EWOULDBLOCK, #CPL_PIM_ERROR or pointer to byte array in the
 * format specified above.
 */
int32_t CPL_pim_getCategories(int32_t pimListHandle);

/**
 * Returns the categories defined for the specified PIM item.
 * If there are no categories defined for the specified PIM item
 * or categories are unsupported for the PIM list, then a zero
 * length array is returned.
 * Categories should be put in a byte array of the following format:
 * <ul>
 * <li> 4 bytes - number of categories</li>
 * <li> 4 bytes - length m of the first category</li>
 * <li> m bytes - first category in categories</li>
 * <li> 4 bytes - length n of the next category</li>
 * <li> n bytes - next category in categories</li>
 * <li> ...</li>
 * </ul>
 *
 * @param pimItem The instance of specified PIM item.
 * @return #CPL_PIM_EWOULDBLOCK, #CPL_PIM_ERROR or pointer to byte array in the
 * format specified above. NULL can be returned to indicate that there was no
 * changes made to the categories of this item since the last successful
 * CPL_pim_getNextItem() or CPL_pim_getItemCategories().
 */
int32_t CPL_pim_getItemCategories(struct INSTANCE_com_jbed_pim_PIMItemImpl_s * pimItem);

/**
 * Persists the data in the PIM item to the underlying data storage for
 * its PIM list and returns the item's UID and revision.
 * UID and revision should be put in <code>CPL_pim_asyncResult_t</code> structure.
 * Fields byteArrAddr and byteArrLength should contain pointer and length
 * of byte array of the following format:
 * <ul>
 * <li> 8 bytes - long value of revision, (-1) if REVISION is unsupported.</li>
 * <li> (byteArrayLength - 8) bytes - byte array representation of UID</li>
 * </ul>
 *
 * The data content of the PIM item is extracted from the <code>pimItem</code>
 * pointer.
 *
 * The PIM item fields are extracted directly from the fields member of
 * <code>pimItem</code>:
 * @code
 * REFARRAY fields;
 * struct INSTANCE_com_jbed_pim_Field_s * field;
 * int i = 0;
 * fields = pimItem->fields;
 * field = (struct INSTANCE_com_jbed_pim_Field_s *)fields->data[i];
 * @endcode
 *
 * To access data within a field, the field structure has to be casted
 * to its specific type, either
 * <code>struct INSTANCE_com_jbed_pim_StringArrayField_s, struct
 * INSTANCE_com_jbed_pim_StringField_s, struct INSTANCE_com_jbed_pim_DateField_s,
 * struct INSTANCE_com_jbed_pim_BinaryField_s,</code> or <code>struct
 * INSTANCE_com_jbed_pim_IntField_s.</code>
 * In order to determine the type of a field, the field ID is used:
 * @code
 * int32_t fieldID = field->id;
 * @endcode
 * The <code>fieldID</code> will be one of the ID's defined
 * in @ref ContactFields, @ref EventFields, or @ref ToDoFields.
 * NOTE that the number of valid data in the data array of each field is indicated
 * by the <code>field->countValues</code>, not the length of the data array.


 * Every type-specific field structure has a field <code>dataValues</code>,
 * which represents an array containing field data of a specific type.
 * To extract data from the particular field, you should get one data element
 * of the <code>dataValues</code> array and cast it to the suitable type.
 * For example,
 * given that <code>strField</code> is of type <code>struct
 * INSTANCE_com_jbed_pim_StringField_s</code>:
 * @code
 * STRING_INSTANCE email = (STRING_INSTANCE)(strField->dataValues->data[0]);
 * @endcode
 *
 * Another example on how to get data from a StringArray field is:
 * @code
 * REFARRAY name = strArrField->dataValues->data[0];
 * STRING_INSTANCE nFamily, nGiven, nOther, nPrefix, nSuffix;
 * if ((name != NULL) && (name->length > 0))
 * {
 *     nFamily = (STRING_INSTANCE)(name->data[0]);
 *     nGiven = (STRING_INSTANCE)(name->data[1]);
 *     nOther = (STRING_INSTANCE)(name->data[2]);
 *     nPrefix = (STRING_INSTANCE)(name->data[3]);
 *     nSuffix = (STRING_INSTANCE)(name->data[4]);
 * }
 * @endcode
 *
 * An example on how to access PIM item categories goes as follows:
 * @code
 * STRING_INSTANCE category;
 * REFARRAY categories = pimItem->categories->elementData;
 * if (categories->length > 0)
 * {
 *    category = (STRING_INSTANCE)(categories->data[0]);
 * }
 * @endcode
 *
 * The size of pimItem->categories->elementData may be bigger that
 * the maximum allowed properties, when categories were added by another
 * application. Implemention MUST resolve the issue either by throwing away
 * the extra categories or returning an error.
 *
 * An Example how to access the list handle on which the item should be stored:
 *
 * @code
 * int32_t pimListHandle = pimItem->pimList->handle;
 * @endcode
 *
 * A final example, this time on how to access RepeatRule fields from
 * an Event item:
 * @code
 * struct INSTANCE_javax_microedition_pim_RepeatRule_s * repeatRule = event->repeatRule;
 * int32_t frequency = -1, count = -1, int32_t interval = -1,
 * monthInYear = -1, dayInWeek = -1, weekInMonth = -1, dayInMonth = -1,
 * dayInYear = -1;
 * int64_t end = -1;
 * if (repeatRule != NULL)
 * {
 *     frequency = repeatRule->frequency;
 *     count = repeatRule->count;
 *     interval = repeatRule->interval;
 *     monthInYear = repeatRule->monthInYear;
 *     dayInWeek = repeatRule->dayInWeek;
 *     weekInMonth = repeatRule->weekInMonth;
 *     dayInMonth = repeatRule->dayInMonth;
 *     dayInYear = repeatRule->dayInYear;
 *     end = repeatRule->end;
 * }
 * @endcode
 *
 * Note that if the platform's native format is vCard 2.1 or vCalendar 1.0 and
 * the JBED_PIM_STOREITEM_VDATA build symbol is defined, the vCard or vCalendar
 * data for the PIMItem is stored in a additional BYTEARRAY vData field of the
 * structure INSTANCE_com_jbed_pim_PIMItemImpl_s.
 *
 * Note that the <code>ALARM</code> field of a <code>PIMItem</code>
 * is relative to the date of the <code>START</code> field. It may
 * happen that the <code>START</code> field is missing and that could cause
 * problems for implementations that need to store the <code>ALARM</code>
 * field as an absolute date. There are four ways to handle this and other
 * similar issues:
 *
 * - The platform cannot store An <code>Event.ALARM</code> without an
 * <code>Event.START</code> and the <code>Event.ALARM</code> value(s) are
 * silently dropped.
 * - The platform cannot store An <code>Event.ALARM</code> without an
 * <code>Event.START</code>, and a <code>PIMException</code> is thrown.
 * - The platform cannot store An <code>Event.ALARM</code> without an
 * <code>Event.START</code>, and an <code>Event.START</code> with a
 * platform-specific value is automatically created.
 * - The platform can store an <code>Event.ALARM</code> without an
 * <code>Event.START</code> (i.e. it stores only the relative time of the
 * <code>ALARM</code> to the <code>START</code>) and the <code>ALARM</code>
 * field is stored normally.
 *
 * @param pimItem PIM item instance.
 * @return #CPL_PIM_EWOULDBLOCK, #CPL_PIM_ERROR or pointer to
 * <code>CPL_pim_asyncResult_t</code> structure.
 */
int32_t CPL_pim_storeItem(struct INSTANCE_com_jbed_pim_PIMItemImpl_s * pimItem);

/**
 * Opens a PIM list with the list type and name.
 * Returns PIM list's name and handle.
 * Name and handle should be put in <code>CPL_pim_asyncResult_t</code> structure.
 * Field res of this structure should contain PIM list handle.
 * Fields byteArrAddr and byteArrLength should contain pointer
 * to byte array data of list name and length conformably;
 *
 * @param pimListType PIM list type.
 * @param pimListName Name of PIM list. If name equals null then the default
 * list should be opened.
 * @return #CPL_PIM_EWOULDBLOCK, #CPL_PIM_ERROR or pointer to
 * <code>CPL_pim_asyncResult_t</code> structure.
 *
 */
int32_t CPL_pim_openPIMList(int32_t pimListType,
                                     STRING_INSTANCE pimListName);

/**
 * Closes a PIM list.
 *
 * @param pimListHandle PIM list handle.
 * @return #CPL_PIM_EWOULDBLOCK, #CPL_PIM_ERROR or #CPL_PIM_SUCCESS.
 */
int32_t CPL_pim_closePIMList(int32_t pimListHandle);

/**
 * Adds the provided category to the PIM list. If the given category already exists
 * for the list, the method does not add another category and considers that this
 * method call is successful and returns.
 *
 * @param pimListHandle    PIM list handle.
 * @param category Category to be added.
 * @return #CPL_PIM_EWOULDBLOCK, #CPL_PIM_ERROR or #CPL_PIM_SUCCESS.
 */
int32_t CPL_pim_addCategory(int32_t pimListHandle,
                                     STRING_INSTANCE category);

/**
 * Deletes the indicated category from the PIM list. If the indicated category is not
 * in the PIM list, this method is treated as successfully completing.
 *
 * @param pimListHandle PIM list handle.
 * @param category      Category to be deleted.
 * @return #CPL_PIM_EWOULDBLOCK, #CPL_PIM_ERROR or #CPL_PIM_SUCCESS.
 */
int32_t CPL_pim_deleteCategory(int32_t pimListHandle,
                                         STRING_INSTANCE category);

/**
 * Renames a category from an old name to a new name, and make sure no category
 * with old name exists. All items associated with the old category name are changed
 * to reference the new category name after this function returns. If the new
 * category name is already an existing category, then the items associated with
 * the old category name are associated with the existing category.
 *
 * @param pimListHandle PIM list handle.
 * @param oldCategory   Old category name.
 * @param newCategory   New category name.
 * @return #CPL_PIM_EWOULDBLOCK or #CPL_PIM_SUCCESS.
 * @return #CPL_PIM_ERROR if <code>oldCategory</code> does not exist.
 */
int32_t CPL_pim_renameCategory(int32_t  pimListHandle,
                                          STRING_INSTANCE oldCategory,
                                          STRING_INSTANCE newCategory);

/**
 * Returns the maximum number of categories that this list can have.
 *
 * @param pimListHandle    PIM list handle.
 * @return #CPL_PIM_EWOULDBLOCK, #CPL_PIM_ERROR or number of categories supported
 * by this list. 0 indicates no category support, #CPL_PIM_NO_LIMIT (-1) indicates
 * there is no limit the the number of categories that this list can have.
 */
int32_t CPL_pim_maxCategories(int32_t pimListHandle);

/**
 * Indicates the total number of data values that a particular field supports in
 * this list.
 *
 * @param pimListHandle PIM list handle.
 * @param fieldID       Field ID.
 * @return #CPL_PIM_EWOULDBLOCK, #CPL_PIM_ERROR or integer value indicating the number
 * of values that can be stored in the field. 0 indicates that the field is not
 * supported by this list, #CPL_PIM_NO_LIMIT (-1) indicates this field supports
 * having an unlimited number of added values in it.
 */
int32_t CPL_pim_maxValues(int32_t pimListHandle,
                                    int32_t fieldID);

/**
 * Gets all fields that are supported in this list. All fields supported by this list,
 * including both standard and extended, are returned in this array.
 *
 * @param pimListHandle PIM list handle.
 * @return #CPL_PIM_EWOULDBLOCK, #CPL_PIM_ERROR or pointer to int array of supported
 * fields.
 */
int32_t CPL_pim_getSuppFields(int32_t pimListHandle);

/**
 * Returns the Repeat Rule fields that are supported by this EventList for the
 * provided Repeat Rule frequency. Repeat Rule frequencies are defined in @ref RRules.
 * The int values in the array represent a bitwise combination of @ref RRuleFields
 * that are valid for the given frequency. For example, if #REPEAT_RULE_MONTHLY was
 * the frequency and this event list supported specifying monthly repeat rules by
 * either the day of the month or by a day in a specific week (e.g. Thursday of the
 * 3rd week in the month), then the returned int array would contain two elements with
 * one value being #REPEAT_RULE_DAY_IN_MONTH and the other being
 * #REPEAT_RULE_DAY_IN_WEEK | #REPEAT_RULE_WEEK_IN_MONTH. All possible field
 * combinations that are valid are returned in the array.
 *
 * If the given frequency is not supported by this list, a zero length array is
 * returned. If the frequency is supported but no fields are supported by this list
 * for that frequency, a one item array containing the integer 0 is returned. In
 * these conditions, any field that has a value set will be ignored by this event list.
 *
 * @param pimListHandle PIM list handle.
 * @param frequency Frequency.
 * @return #CPL_PIM_EWOULDBLOCK, #CPL_PIM_ERROR or pointer to int array of supported
 * repeat fields.
 */
int32_t CPL_pim_getSuppRRuleFields(int32_t pimListHandle,
                                               int32_t frequency);

/**
 * Returns an integer array containing all of the supported attributes for the given
 * field. All attributes supported by this list, including both standard and extended,
 * are returned in this array. The attributes are provided one attribute per entry
 * in the returned integer array.
 *
 * @param pimListHandle PIM list handle.
 * @param fieldID       Field ID.
 * @return #CPL_PIM_EWOULDBLOCK, #CPL_PIM_ERROR or pointer to int array of supported
 * attributes. If there are no supported fields, a zero-length array is returned.
 */
int32_t CPL_pim_getSuppAttrs(int32_t pimListHandle, int32_t fieldID);

/**
 * Returns an integer array containing all of the supported elements of a string
 * array for the given field. The array elements are provided one element per entry
 * in the returned integer array.
 *
 * @param pimListHandle PIM list handle.
 * @param fieldID      Field ID.
 * @return #CPL_PIM_EWOULDBLOCK, #CPL_PIM_ERROR or pointer to int array of supported
 * array elements. If there are no supported array elements, a zero-length array is
 * returned.
 */
int32_t CPL_pim_getSuppArrElems(int32_t pimListHandle, int32_t fieldID);

/**
 * Returns the maximum number of categories that this item can be assigned to.
 * PIM list type and name are extracted from the <code>pimList</code> pointer:
 * @code
 * int32_t pimListType = pimList->pimListType;
 * STRING_INSTANCE pimListName = pimList->name;
 * @endcode
 *
 * @param pimItem PIM item.
 * @return #CPL_PIM_EWOULDBLOCK, #CPL_PIM_ERROR or maximum number of categories this
 * item can be assigned to. 0 indicates no category support #CPL_PIM_NO_LIMIT (-1)
 * indicates there is no limit to the number of categories that this item
 * can be assigned to.
 */
int32_t CPL_pim_itemMaxCategories(
                         struct INSTANCE_com_jbed_pim_PIMItemImpl_s * pimItem);

/**
 * Removes PIM item from PIM list.
 *
 * @param pimListHandle PIM list handle.
 * @param uid PIM       Item' UID.
 * @return #CPL_PIM_EWOULDBLOCK, #CPL_PIM_ERROR or #CPL_PIM_SUCCESS.
 */
int32_t CPL_pim_removePIMItem(int32_t pimListHandle,
                                         STRING_INSTANCE uid);

/**
  * Malloc memory for PIM LIST.Beacuse Spreadturm don't allow
  * using CRTL_malloc in proting layer.
  *
  * @return malloc memory size.
  */
void *jbedPimMallocMemory(int32_t size);

/**
  * Free memory for PIM LIST.Beacuse Spreadturm don't allow
  * using CRTL_free in proting layer.
  *
  */
void jbedPimFreeMemory(void *ptr);

#if defined(__cplusplus)
} /* extern "C" */
#endif

#endif/* __CPL_PIM_H__ */


