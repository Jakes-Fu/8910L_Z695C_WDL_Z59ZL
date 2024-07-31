/**
 * @file 
 */


#ifndef ___AM_PIM_UTIL_H
#define ___AM_PIM_UTIL_H

#ifdef __cplusplus
extern "C" {
#endif


#include "jktypes.h"
#include "jkpimcommon.h"
#include "jkpimrepeatrule.h"


/**
 * @addtogroup Module_PIM_ACCESS_JBI_variable_1
 *
 * @{
 */

/*	========== ========== ========== ========== ==========
	 @: Function for access to a string array (JKPIMStringArray).
	========== ========== ========== ========== ========== */
/**
 *  Creates a string array.
 *  
 *  @return
 *     A pointer to the created JKPIMStringArray structure: Succeeded<br>
 *     #JK_NULL: Failed
 *  
 *  This function creates and initializes a JKPIMStringArray structure representing a string array.
 */
JKPIMStringArray* AmPIMStringArrayCreate(void);

/**
 *  Frees the memory space added to a string array.
 *  
 *  @param *array 
 *         A pointer to the JKPIMStringArray structure containing a string array.
 *  @return 
 *     #JK_PIM_SUCCESS: Succeeded.<br>
 *     #JK_PIM_INVALID_PARAM_ERR: Invalid parameter was designated
 *
 *  This function frees the pointer (JKPIMString*) array
 *  indicated by the JKPIMStringArray structure <i>data</i> member,
 *  the structure memory space pointed to by the elements of that array,
 *  and the memory space pointed to by the <i>text</i> member.
 *  JKPIMStringArray structure member values are initialized as follows.
 *
 *   <i>size</i> = 0<br>
 *   <i>data</i> = #JK_NULL
 */
JKPIMErrCode AmPIMStringArrayClear(JKPIMStringArray* array);

/**
 * Deletes a JKPIMStringArray structure and frees its memory space.
 *
 * @param *array
 *         A pointer to the JKPIMStringArray structure containing a string array.
 * @return
 *     #JK_PIM_SUCCESS: Succeeded.
 *  
 *  This function frees the pointer (JKPIMString*) array
 *  indicated by the JKPIMStringArray structure <i>data</i> member,
 *  the structure memory space pointed to by the elements of that array,
 *  and the memory space pointed to by the <i>text</i> member,
 *  as well as the memory space of the JKPIMStringArray structure itself.
 *  
 *  After this function is called, the pointer passed in the <i>array</i> argument
 *  is no longer valid.
 * 
 */
JKPIMErrCode AmPIMStringArrayDestroy(JKPIMStringArray* array);

/**
 *  Adds a string to a string array.
 *  
 *  @param *array 
 *         A pointer to the JKPIMStringArray structure containing a string array.
 *  @param *text 
 *         A pointer to the buffer holding the string to be added.
 *  @param length 
 *         The number of the character in the string to be added.
 *  
 *  @return
 *     #JK_PIM_SUCCESS: Succeeded<br>
 *     #JK_PIM_MEMORY_ERR: Insufficient memory<br>
 *     #JK_PIM_INVALID_PARAM_ERR: Invalid parameter was designated
 *  
 *  This function adds a string to the array indicated by the <i>data</i> member of
 *  the JKPIMStringArray structure, and increments the value of the <i>size</i> member.
 *  
 *  This function creates a copy of the data indicated by the <i>text</i> parameter for use.
 */
JKPIMErrCode AmPIMStringArrayAddString(JKPIMStringArray* array,JKWChar* text,JKSint32 length);

/**
 *  Deletes from a string array the string designated by its index. 
 * 
 * @param *array 
 *         A pointer to the JKPIMStringArray structure containing the string array.
 * @param index 
 *         The index indicating the string to be deleted.
 *         Designate an index number between 0 and the number of elements -1. 
 * @return
 *     #JK_PIM_SUCCESS: Succeeded<br>
 *     #JK_PIM_MEMORY_ERR: Insufficient memory<br>
 *     #JK_PIM_INVALID_PARAM_ERR: Invalid parameter was designated<br>
 *     #JK_PIM_INDEX_ERR: Invalid index was designated
 *
 *  This function deletes a string in the array indicated by the <i>data</i> member of
 *  the JKPIMStringArray structure.
 */
JKPIMErrCode AmPIMStringArrayRemoveString(JKPIMStringArray* array,JKSint32 index);

/** @} end of group */

/**
 * @addtogroup Module_PIM_ACCESS_JBI_variable_2
 *
 * @{
 */

/*	========== ========== ========== ========== ==========
	 @: Fucntion for access to an int array (JKPIMIntArray).
	========== ========== ========== ========== ========== */
/**
 * Creates an integer array.
 *
 * @return 
 *     A pointer to the created JKPIMIntArray structure: Succeeded<br>
 *     #JK_NULL: Failed
 *
 *  This function creates and initializes a JKPIMIntArray structure 
 *  containing an array of 32-bit signed integers.
 */
JKPIMIntArray* AmPIMIntArrayCreate(void);

/**
 * Initializes an integer array.
 *  @param *array 
 *         A pointer to the JKPIMIntArray structure containing the integer array.
 *  @return 
 *     #JK_PIM_SUCCESS: Succeeded.<br>
 * 
 *  This function frees the memory space indicated by the <i>data</i> member of 
 *  a JKPIMIntArray structure, and initializes the JKPIMIntArray structure
 *  member values as follows.
 *  
 *   <i>size</i> = 0<br>
 *   <i>data</i> = #JK_NULL
 * 
 */
JKPIMErrCode AmPIMIntArrayClear(JKPIMIntArray* array);

/**
 * Deletes a JKPIMIntArray structure and frees its memory space. 
 * 
 *  @param *array 
 *         A pointer to the JKPIMIntArray structure containing an integers array.
 *  @return 
 *     #JK_PIM_SUCCESS: Succeeded.<br>
 * 
 *  This function frees the memory space indicated by the <i>data</i> member of 
 *  a JKPIMIntArray structure and the memory space of the JKPIMIntArray
 *  structure itself. 
 * 
 *  After this function is called, the pointer passed in the <i>array</i> argument
 *  is no longer valid.
 */
JKPIMErrCode AmPIMIntArrayDestroy(JKPIMIntArray* array);

/**
 *  Adds an interger to an interger array.
 * 
 * @param *dest
 *         A pointer to the JKPIMIntArray structure containing the integer array.
 * @param value
 *         An interger to be added.
 * @return
 *     #JK_PIM_SUCCESS: Succeeded<br>
 *     #JK_PIM_MEMORY_ERR: Insufficient memory<br>
 *     #JK_PIM_INVALID_PARAM_ERR: Invalid parameter was designated
 * 
 *  This function adds an integer to the end of an array indicated
 *  by the <i>data</i> member of a JKPIMIntArray structure, and increments the
 *  <i>size</i> member.
 */
JKPIMErrCode AmPIMIntArrayAddInt(JKPIMIntArray* dest,JKSint32 value);

/**
 * Adds an integer array to a JKPIMIntArray structure.
 * 
 * @param *dest
 *         A pointer to the JKPIMIntArray structure containing an integer array.
 * @param *src 
 *         A pointer to the buffer holding the integer array to be added.
 * @param size
 *         The number of elements in the integer array to be added.
 * @return
 *     #JK_PIM_SUCCESS: Succeeded<br>
 *     #JK_PIM_MEMORY_ERR: Insufficient memory<br>
 *     #JK_PIM_INVALID_PARAM_ERR: Invalid parameter was designated
 * 
 *  This function adds an integer array to the end of an array indicated
 *  by the <i>data</i> member of a JKPIMIntArray structure, and increments the
 *  <i>size</i> member.
 */
JKPIMErrCode AmPIMIntArrayAddIntArray(JKPIMIntArray* dest,JKSint32* src,JKSint32 size);

/** @} end of group */

/**
 * @addtogroup Module_PIM_ACCESS_JBI_string
 *
 * @{
 */
 
 
/*	========== ========== ========== ========== ==========
	 @: Function for access to a string (JKPIMString).
	========== ========== ========== ========== ========== */
/**
 *  Sets a string in a string structure. 
 * 
 *  @param string 
 *         A pointer to the JKPIMString structure to set a string.
 *  @param text 
 *         The string to be set.
 *  @param length 
 *         The number of the characters in the string indicated by <i>text</i>.
 *  @return 
 *    #JK_PIM_SUCCESS: Succeeded<br>
 *    #JK_PIM_MEMORY_ERR: Insufficient memory<br>
 *    #JK_PIM_INVALID_PARAM_ERR: Invalid parameter was designated
 * 
 */
JKPIMErrCode AmPIMStringSetString(JKPIMString* string,JKWChar* text,JKSint32 length);

/** @} end of group */


/**
 * @addtogroup Module_PIM_ACCESS_JBI_variable_3
 *
 * @{
 */
 
/*	========== ========== ========== ========== ==========
	 @: Function for access to a long array (JKPIMLongArray).
	========== ========== ========== ========== ========== */
/**
 *  Creates a Long array.
 * 
 * @return
 *     A pointer to the created JKPIMLongArray structure: Succeeded<br>
 *     #JK_NULL: Failed
 * 
 *  This function creates and initializes a JKPIMLongArray structure 
 *  containing an array of 64-bit integers.
 */
JKPIMLongArray* AmPIMLongArrayCreate(void);

/**
 *  Frees the memory space added to a Long array.
 * 
 * @param *array
 *         A pointer to the JKPIMLongArray structure containing the Long array.
 * @return
 *     #JK_PIM_SUCCESS: Succeeded.<br>
 *     #JK_PIM_INVALID_PARAM_ERR: Invalid parameter was designated
 * 
 *  This function frees the pointer (JKPIMLong*) array
 *  indicated by the JKPIMLongArray structure <i>data</i> member,
 *  frees the structure memory space pointed to by the elements of that array,
 *  and initializes JKPIMLongArray structure member values as follows.
 *   
 *   size = 0<br>
 *   data = #JK_NULL
 */
JKPIMErrCode AmPIMLongArrayClear(JKPIMLongArray* array);

/**
 * Deletes a JKPIMLongArray structure and frees its memory space.
 * 
 * @param *array
 *         A pointer to the JKPIMLongArray structure containing the Long array.
 * @return 
 *     #JK_PIM_SUCCESS: Succeeded.
 * 
 *  This function frees the pointer (JKPIMLong*) array
 *  indicated by the JKPIMLongArray structure <i>data</i> member,
 *  frees the structure memory space pointed to by the elements of that array,
 *  and frees the memory space of the JKPIMLongArray structure itself.
 *  
 *  After this function is called, the pointer passed in the <i>array</i> parameter
 *  is no longer valid.
 */
JKPIMErrCode AmPIMLongArrayDestroy(JKPIMLongArray* array);

/**
 * Adds a Long value to a Long array.
 * 
 * @param *array
 *         A pointer to the JKPIMLongArray structure containing the Long array.
 * @param high
 *         The higher 32 bits of the Long value to be added.
 * @param low
 *         The lower 32 bits of the Long value to be added.
 * @return			
 *     #JK_PIM_SUCCESS: Succeeded<br>
 *     #JK_PIM_MEMORY_ERR: Insufficient memory<br>
 *     #JK_PIM_INVALID_PARAM_ERR: Invalid parameter was designated
 * 
 *  This function creates a JKPIMLong structure representing a Long value 
 *  passed in parameters, adds the Long value to an array indicated by the 
 *  <i>data</i> member of the JKPIMLongArray structure, and increments the <i>size</i> member.
 */
JKPIMErrCode AmPIMLongArrayAddLong(JKPIMLongArray* array,JKUint32 high,JKUint32 low);

/**
 *  Deletes from a Long array the value designated by its index. 
 * @param *array
 *         A pointer to the JKPIMLongArray structure containing the Long array.
 * @param index 
 *         The index indicating the Long value to be deleted.
 *         Designate an index number between 0 and the number of elements -1. 
 * @return 
 *     #JK_PIM_SUCCESS: Succeeded<br>
 *     #JK_PIM_MEMORY_ERR: Insufficient memory<br>
 *     #JK_PIM_INVALID_PARAM_ERR: Invalid parameter was designated<br>
 *     #JK_PIM_INDEX_ERR: Invalid index was designated
 * 
 *  This function deletes a Long value in the array indicated by the <i>data</i> member of
 *  the JKPIMLongArray structure.
 */
JKPIMErrCode AmPIMLongArrayRemove(JKPIMLongArray* array,JKSint32 index);

/** @} end of group */

/**
 * @addtogroup Module_PIM_ACCESS_JBI_variable_4
 *
 * @{
 */

/*	========== ========== ========== ========== ==========
	 @: Function for access to a calendar array (JKPIMCalendarArray).
	========== ========== ========== ========== ========== */
/**
 *  Creates a calendar array.
 *	
 * @return
 *     A pointer to the created JKPIMCalendarArray structure: Succeeded<br>
 *     #JK_NULL: Failed
 * 
 *  This function creates and initializes a JKPIMCalendarArray structure 
 *  containing an array of calendar data.
 */
JKPIMCalendarArray* AmPIMCalendarArrayCreate(void);

/**
 *  Frees the memory space added to a calendar array.
 * 
 * @param *array
 *         A pointer to the JKPIMCalendarArray structure containing the calendar array.
 * @return 
 *     #JK_PIM_SUCCESS: Succeeded.<br>
 *     #JK_PIM_INVALID_PARAM_ERR: Invalid parameter was designated
 * 
 *  This function frees the pointer (JKPIMCalendar*) array
 *  indicated by the JKPIMCalendarArray structure <i>data</i> member,
 *  frees the structure memory space pointed to by the elements of that array,
 *  and initializes JKPIMCalendarArray structure member values as follows.
 *
 *   size = 0<br>
 *   data = #JK_NULL
 */
JKPIMErrCode AmPIMCalendarArrayClear(JKPIMCalendarArray* array);

/**
 * Deletes a JKPIMCalendarArray structure and frees its memory space.
 *
 * @param *array
 *         A pointer to the JKPIMCalendarArray structure containing the calendar array.
 * @return 
 *     #JK_PIM_SUCCESS: Succeeded.
 * 
 *  This function frees the pointer (JKPIMCalendar*) array
 *  indicated by the JKPIMCalendarArray structure <i>data</i> member,
 *  frees the structure memory space pointed to by the elements of that array,
 *  and frees the memory space of the JKPIMCalendarArray structure itself.
 *  
 *  After this function is called, the pointer passed in the <i>array</i> argument
 *  is no longer valid.
 */
JKPIMErrCode AmPIMCalendarArrayDestroy(JKPIMCalendarArray* array);

/**
 * Adds a calendar value to a calendar array.
 * @param *array
 *         A pointer to the JKPIMCalendarArray structure containing the calendar array.
 * @param *value
 *         A pointer to the buffer holding the calendar value to be added.
 * @return 
 *     #JK_PIM_SUCCESS: Succeeded<br>
 *     #JK_PIM_MEMORY_ERR: Insufficient memory<br>
 *     #JK_PIM_INVALID_PARAM_ERR: Invalid parameter was designated
 * 
 *  This function adds a calendar value to the array indicated by the <i>data</i> member of
 *  the JKPIMCalendarArray structure, and increments the value of the <i>size</i> member.
 *  
 *  This function creates a copy of the data indicated by the <i>value</i> parameter for use.
 */
JKPIMErrCode AmPIMCalendarArrayAddCalendar(JKPIMCalendarArray* array,JKPIMCalendar* value);

/** @} end of group */

/**
 * @addtogroup Module_PIM_ACCESS_JBI_variable_5
 *
 * @{
 */

/*	========== ========== ========== ========== ==========
	 @: Function for access to a date array (JKPIMDateArray).
	========== ========== ========== ========== ========== */
/**
 *  Creates a date array.
 *
 * @return
 *     A pointer to the created JKPIMDateArray structure: Succeeded<br>
 *     #JK_NULL: Failed
 * 
 *  This function creates and initializes a JKPIMDateArray structure 
 *  containing an array of date data.
 */
JKPIMDateArray* AmPIMDateArrayCreate(void);

/**
 *  Frees the memory space added to a date array.
 *  
 *  @param *array
 *         A pointer to the JKPIMDateArray structure containing the date array.
 *  @return
 *     #JK_PIM_SUCCESS: Succeeded<br>
 *     #JK_PIM_INVALID_PARAM_ERR: Invalid parameter was designated
 *  
 *  This function frees the pointer (JKPIMLong*) array
 *  indicated by the JKPIMDateArray structure <i>data</i> member,
 *  frees the structure memory space pointed to by the elements of that array,
 *  and initializes JKPIMDateArray structure member values as follows.
 *
 *   <i>size</i> = 0<br>
 *   <i>data</i> = #JK_NULL
 */
JKPIMErrCode AmPIMDateArrayClear(JKPIMDateArray* array);

/**
 * Deletes a JKPIMDateArray structure and frees its memory space.
 *
 * @param *array
 *         A pointer to the #JKPIMDateArray structure containing the date array.
 * @return 
 *     #JK_PIM_SUCCESS: Succeeded.
 * 
 *  This function frees the pointer (JKPIMLong*) array
 *  indicated by the JKPIMDateArray structure <i>data</i> member,
 *  frees the structure memory space pointed to by the elements of that array,
 *  and frees the memory space of the JKPIMDateArray structure itself.
 *  
 *  After this function is called, the pointer passed in the <i>array</i> argument
 *  is no longer valid.
 */
JKPIMErrCode AmPIMDateArrayDestroy(JKPIMDateArray* array);

/**
 * Adds a date value to a date array.
 *  
 *  @param *array
 *         A pointer to the #JKPIMDateArray structure containing the date array.
 *  @param *date
 *         A pointer to the buffer holding the date to be added.
 *         Designate the date as a value expressing the elapsed time in milliseconds 
 *         from 00:00:00.000, January 1, 1970 (GMT).
 *  @return
 *     #JK_PIM_SUCCESS: Succeeded<br>
 *     #JK_PIM_MEMORY_ERR: Insufficient memory<br>
 *     #JK_PIM_INVALID_PARAM_ERR: Invalid parameter was designated
 *  
 *  This function adds a date to the array indicated by the <i>data</i> member of
 *  the #JKPIMDateArray structure, and increments the value of the <i>size</i> member.
 *  Data added to the array are arranged in ascending order.
 *  If the same date already exists in the array, this function returns
 *  #JK_PIM_SUCCESS without performing any other action.
 */
JKPIMErrCode AmPIMDateArrayAddDate(JKPIMDateArray* array,JKPIMDate* value);

/**
 *  Deletes from a date array the value designated by its index. 
 *  
 *  @param *array
 *         A pointer to the #JKPIMDateArray structure containing the date array.
 *  @param *date
 *         A pointer to the buffer holding the <i>date</i> to be removed.
 *         Designate the date as a value expressing the elapsed time in milliseconds 
 *         from 00:00:00.000, January 1, 1970 (GMT).
 *  @return
 *     #JK_PIM_SUCCESS: Succeeded<br>
 *     #JK_PIM_MEMORY_ERR: Insufficient memory<br>
 *     #JK_PIM_INVALID_PARAM_ERR: Invalid parameter was designated
 *  
 *  This function deletes a date in the array indicated by the <i>data</i> member of
 *  the #JKPIMDateArray structure, and decrements the value of the <i>size</i> member.
 *  If the designated date does not exist in the array, this function returns
 *  #JK_PIM_SUCCESS without performing any other action.
 */
JKPIMErrCode AmPIMDateArrayRemove(JKPIMDateArray* array,JKPIMDate* value);

/** @} end of group */

/**
 * @addtogroup Module_PIM_ACCESS_JBI_variable_6
 *
 * @{
 */

/*	========== ========== ========== ========== ==========
	 @: Fucntion for access to a field definitions array (JKPIMFieldDedArray).
	========== ========== ========== ========== ========== */
/**
 *  Creates a field definition array.
 * 
 *  @return
 *     A pointer to the created JKPIMFieldDefArray structure: Succeeded<br>
 *     #JK_NULL: Failed
 * 
 *  This function creates and initializes structure JKPIMFieldDefArray 
 *  representing a field definition array.
 */
JKPIMFieldDefArray* AmPIMFieldDefArrayCreate(void);

/**
 *  Frees the memory space added to a field definition array.
 *  
 *  @param *array
 *         A pointer to the JKPIMFieldDefArray structure containing the field definition array.
 *  
 *  @return 
 *     #JK_PIM_SUCCESS: Succeeded.<br>
 *     #JK_PIM_INVALID_PARAM_ERR: Invalid parameter was designated
 *  
 *  This function frees the pointer (JKPIMFieldDef*) array
 *  indicated by the JKPIMFieldDefArray structure <i>data</i> member,
 *  frees the structure memory space pointed to by the elements of that array,
 *  and initializes JKPIMFieldDefArray structure member values as follows.
 *
 *   <i>size</i> = 0<br>
 *   <i>data</i> = #JK_NULL
 */
JKPIMErrCode AmPIMFieldDefArrayClear(JKPIMFieldDefArray* array);

/**
 * Deletes a JKPIMFieldDefArray structure and frees its memory space.
 * 
 * @param *array
 *         A pointer to the JKPIMFieldDefArray structure containing the field definition array.
 * @return 
 *     #JK_PIM_SUCCESS: Succeeded.
 * 
 *  This function frees the pointer (JKPIMFieldDef*) array
 *  indicated by the JKPIMFieldDefArray structure <i>data</i> member,
 *  frees the structure memory space pointed to by the elements of that array,
 *  and frees the memory space of the JKPIMFieldDefArray structure itself.
 *  
 *  After this function is called, the pointer passed in the <i>array</i> argument
 *  is no longer valid.
 */
JKPIMErrCode AmPIMFieldDefArrayDestroy(JKPIMFieldDefArray* array);

/**
 *  Adds a field definition to a field definition array.
 *  
 *  @param *array 
 *         A pointer to the JKPIMFieldDefArray structure containing the field definition array.
 *  @param fieldId 
 *         The field ID.
 *         Designate a value unique within the item. 
 *  @param *label 
 *         A pointer to the JKPIMString structure holding the label of the field.
 *  @param dataType
 *         Data type of the field.
 *  @param maxSize 
 *         The maximum number of the items that can be held by the field.
 *  @param attributes 
 *         One or combination of the bits of the attributes supported by the field.
 *         Designate the bitwise OR of the values defined as attribute identifiers.
 *  @return 
 *     #JK_PIM_SUCCESS: Succeeded.<br>
 *     #JK_PIM_MEMORY_ERR: Insufficient memory error.<br>
 *     #JK_PIM_INVALID_PARAM_ERR: Invalid parameter was designated
 *  
 *  This function sets the parameter-designated information in a JKPIMFieldDef
 *  structure, appends it to the array indicated by the <i>data</i> member to a JKPIMFieldDefArray 
 *  structure, and increments the <i>size</i> member value. 
 */
JKPIMErrCode AmPIMFieldDefArrayAddFieldDef(JKPIMFieldDefArray* array,JKSint32 fieldId,JKPIMString* label,JKPIMDataType dataType,JKSint32 maxSize,JKSint32 attributes);

/**
 *  Adds a extended field definition to a field definition array.
 *  
 *  @param *array 
 *         A pointer to the JKPIMFieldDefArray structure containing the field definition array.
 *  @param fieldId 
 *         The Field ID.
 *         Designate a value unique within the item. 
 *  @param *label 
 *         A pointer to the JKPIMString structure holding the label of the field.
 *  @param dataType 
 *         Data type of the field.
 *  @param maxSize 
 *         The maximum number of the items that can be held by the field.
 *  @param attributes 
 *         One or combination of the bits of the attributes supported by the field.
 *         Designate the bitwise OR of the values defined as attribute identifiers.
 *  @param *elementIds 
 *         A pointer to an array of field element IDs.
 *  @param *labels 
 *         A pointer to the JKPIMStringArray structure containing field element labels.
 *  @param arraySize
 *         The total count of the field elements.
 *  
 *  @return 
 *     #JK_PIM_SUCCESS: Succeeded.<br>
 *     #JK_PIM_MEMORY_ERR: Insufficient memory error.<br>
 *     #JK_PIM_INVALID_PARAM_ERR: Invalid parameter was designated
 *  
 *  This function sets the parameter-designated information in a JKPIMExFieldDef
 *  structure, appends it to the array indicated by the <i>data</i> member of the JKPIMFieldDefArray 
 *  structure, and increments the <i>size</i> member value. 
 *  
 *  In the <i>elementIds</i> parameter, designate the identifiers of
 *  <code>STRING_ARRAY</code> type data elements specified in the PIM optional
 *  package specification.
 *  
 *  In the <i>arraySize</i> parameter, designate the total count of the field elements.
 *  The count should be the sum of the number of the elements defined by the PIM optional 
 *  package and the number of the elements originally defined in the native system.
 *  If, for example, the field ID is #JK_PIM_CONTACT_ADDR, the count should be as follows.
 *  
 *  #JK_PIM_CONTACT_ADDR_ADDRSIZE + [The count of the element defined in the native system]
 *  
 *  This function creates a copy of the data indicated by pointer type parameters for use.
 */
JKPIMErrCode AmPIMFieldDefArrayAddExFieldDef(JKPIMFieldDefArray* array,JKSint32 fieldId,JKPIMString* label,JKPIMDataType dataType,JKSint32 maxSize,JKSint32 attributes,JKSint32* elementIds,JKPIMStringArray* labels,JKSint32 arraySize);

/** @} end of group */

/**
 * @addtogroup Module_PIM_ACCESS_JBI_variable_7
 *
 * @{
 */

/*	========== ========== ========== ========== ==========
	 @: Function for access to an attribute definitions array (JKPIMAttributeDefArray).
	========== ========== ========== ========== ========== */
/**
 *  Creates an attribute definition array.
 * 
 *  @return
 *     A pointer to the created JKPIMAttributeDefArray structure: Succeeded<br>
 *     #JK_NULL: Failed
 * 
 *  This function creates and initializes a JKPIMAttributeDefArray structure 
 *  representing an attribute definition array.
 */
JKPIMAttributeDefArray* AmPIMAttributeDefArrayCreate(void);

/**
 *  Frees the memory space added to an attribute definition array.
 *  
 *  @param *array 
 *         A pointer to the JKPIMAttributeDefArray structure containing the attribute definition array.
 *  @return
 *     #JK_PIM_SUCCESS: Succeeded.<br>
 *     #JK_PIM_INVALID_PARAM_ERR: Invalid parameter was designated
 *  
 *  This function frees the pointer (JKPIMAttributeDef*) array
 *  indicated by the JKPIMAttributeDefArray structure <i>data</i> member,
 *  frees the structure memory space pointed to by the elements of that array,
 *  and initializes JKPIMAttributeDefArray structure member values as follows.
 *
 *   <i>size</i> = 0<br>
 *   <i>data</i> = #JK_NULL
 */
JKPIMErrCode AmPIMAttributeDefArrayClear(JKPIMAttributeDefArray* array);

/**
 * Deletes a JKPIMAttributeDefArray structure and frees its memory space.
 * 
 * @param *array
 *         A pointer to the JKPIMAttributeDefArray structure containing the attribute definition array.
 * @return 
 *     #JK_PIM_SUCCESS: Succeeded.
 * 
 *  This function frees the pointer (JKPIMAttributeDef*) array
 *  indicated by the JKPIMAttributeDefArray structure <i>data</i> member,
 *  frees the structure memory space pointed to by the elements of that array,
 *  and frees the memory space of the JKPIMAttributeDefArray structure itself.
 *  
 *  After this function is called, the pointer passed in the <i>array</i> argument
 *  is no longer valid.
 */
JKPIMErrCode AmPIMAttributeDefArrayDestroy(JKPIMAttributeDefArray* array);

/**
 *  Adds an attribute definition to an attribute definition array.
 *  
 *  @param *array 
 *         A pointer to the JKPIMAttributeDefArray structure containing the attribute definition 
 *         array.
 *  @param attributeId 
 *         Attribute ID.
 *  @param *label 
 *         A pointer to the JKPIMString structure holding the label of the attribute.
 *  @return 
 *     #JK_PIM_SUCCESS: Succeeded.<br>
 *     #JK_PIM_MEMORY_ERR: Insufficient memory error.<br>
 *     #JK_PIM_INVALID_PARAM_ERR: Invalid parameter was designated
 *  
 *  This function sets the parameter-designated information in a JKPIMAttributeDef
 *  structure, appends it to the array indicated by the <i>data</i> member to a JKPIMAttributeDefArray 
 *  structure, and increments the <i>size</i> member value. 
 */
JKPIMErrCode AmPIMAttributeDefArrayAddAttributeDef(JKPIMAttributeDefArray* array,JKSint32 attributeId,JKPIMString* label);

/** @} end of group */

/**
 * @addtogroup Module_PIM_ACCESS_JBI_item_1
 *
 * @{
 */

/*	========== ========== ========== ========== ==========
	 @: Fucntion for access to an item (JKPIMItem).
	========== ========== ========== ========== ========== */
/**
 *  Creates an item.
 * 
 *  @param listType
 *         The type of the list to which the item belongs. 
 *         Designate one of the following.
 *         <ul>
 *          <li> #JK_PIM_CONTACT
 *          <li> #JK_PIM_EVENT
 *          <li> #JK_PIM_TODO
 *         </ul>
 *  @return
 *     A pointer to the created item (JKPIMItem or JKPIMEventItem): Succeeded<br>
 *     #JK_NULL: Failed
 * 
 *  If the <i>listType</i> parameter designates #JK_PIM_EVENT, this function creates and initializes a 
 *  JKPIMEventItem structure; if the <i>listType</i> parameter designates #JK_PIM_CONTACT or 
 *  #JK_PIM_TODO, this function creates and initializes a JKPIMItem structure.
 */
JKPIMItem* AmPIMItemCreate(JKPIMListType listType);

/**
 * Deletes a JKPIMItem or JKPIMEventItem structure and frees its memory space.
 * 
 * @param *item 
 *	A pointer to the JKPIMItem or JKPIMEventItem structure.
 * @return
 *     #JK_PIM_SUCCESS: Succeeded.
 * 
 *  This function frees the memory space indicated by all the pointer type data
 *  contained in the structure designated by the <i>item</i> parameter, and frees the
 *  memory space of the structure itself. 
 *  
 *  After this function is called, the pointer passed in the <i>item</i> argument
 *  is no longer valid.
 */
JKPIMErrCode AmPIMItemDestroy(JKPIMItem* item);

/**
 *  Adds to an item a field for holding data.
 *  @param *item
 *         A pointer to the JKPIMItem structure containing the item.
 *  @param fieldId
 *         The Field ID.
 *         Designate a value unique within the item. 
 *  @param dataType
 *         Data type of the field.
 *  @param fieldDef
 *         A pointer to the JKPIMFieldDef structure containing the field definition 
 *         array.
 *  @return
 *     #JK_PIM_SUCCESS: Succeeded.<br>
 *     #JK_PIM_MEMORY_ERR: Insufficient memory error.
 * 
 *  This function adds a field of the designated type to the item.
 *  
 *  This function uses the JKPIMFieldDef structure passed in the <i>fieldDef</i> 
 *  parameter as the field definition of the added field.
 *  
 *  Since the pointer passed in <i>fieldDef</i> is referenced by the item indicated
 *  in the <i>item</i> parameter, it must remain valid even after this function
 *  returns to its caller. What this means is that the responsibility for
 *  managing this pointer lies with the program managing the item indicated
 *  by <i>item</i>.
 *  If JBlend manages the item to which a field was added by this function,
 *  then when the field definition memory area pointed to by <i>fieldDef</i> is
 *  no longer needed, JBlend frees it using JkPIMFreePointer().
 */
JKPIMErrCode AmPIMItemPrepareField(JKPIMItem* item,JKSint32 fieldId,JKPIMDataType dataType,JKPIMFieldDef* fieldDef);

/**
 *  Searches an item for a field whose ID matches the designated field ID.
 *  
 *  @param *item 
 *         A pointer to the JKPIMItem structure containing the item to be searched.
 *  @param fieldId 
 *         The ID of the field to be searched for.
 *  @return 
 *     A pointer to a JKPIMField structure containing the search results: Succeeded<br>
 *     #JK_NULL: If no match was found.
 *  
 *  This function passes as the return value a pointer to a field data having the field ID
 *  designated by the <i>fieldId</i> parameter in the item designated by the <i>item</i> parameter. 
 *  
 *  The pointer returned by this function remains valid as long as the item designated
 *  by <i>item</i> exists.
 *  
 *  A typical use of this function is for comparing item data in the implementation of a 
 *  JSI function such as JkPIMGetResultSetByItem(). 
 */
JKPIMField* AmPIMItemFindField(JKPIMItem* item,JKSint32 fieldId);


/*	========== ========== ========== ========== ==========
     @: Functions for adding data to an item (JKPIMItem).
     If the item is a JKPIMEventItem, cast it to a JKPIMItem.
	========== ========== ========== ========== ========== */
/**
 *  Adds a category to an item.
 *  
 *  @param *item 
 *         A pointer to the JKPIMItem structure containing the item.
 *  @param *category 
 *         A pointer to the buffer holding the category name.
 *  @param length
 *         The length (number of characters) of the category name.
 *  @return
 *     #JK_PIM_SUCCESS: Succeeded.<br>
 *     #JK_PIM_MEMORY_ERR: Insufficient memory error.<br>
 *     #JK_PIM_INVALID_PARAM_ERR: Invalid parameter was designated
 *  
 *  This function adds a category assignment to the item designated by the <i>item</i> parameter. 
 *  This function does not perform a parameter check for duplicate values.
 */
JKPIMErrCode AmPIMItemAddCategory(JKPIMItem* item,JKWChar* category,JKSint32 length);

/** @} end of group */

/**
 * @addtogroup Module_PIM_ACCESS_JBI_item_2
 *
 * @{
 */

/**
 *  Appends binary data to the designated field of an item.
 *  
 *  @param *item 
 *         A pointer to the JKPIMItem structure containing the item.
 *  @param fieldId 
 *         The Field ID.
 *  @param attributes 
 *         One or combination of the bits of the attributes.
 *         Designate the bitwise OR of the values defined as attribute identifiers.
 *  @param size 
 *         The binary data size in bytes. 
 *  @param *data 
 *         A pointer to the memory area holding the binary data. 
 *  
 *  @return 
 *     #JK_PIM_SUCCESS: Succeeded.<br>
 *     #JK_PIM_MEMORY_ERR: Insufficient memory error.<br>
 *     #JK_PIM_INVALID_PARAM_ERR: Invalid parameter was designated<br>
 *     #JK_PIM_UNSUPPORTED_FIELD_ERR:
 *        The designated field does not exist, or the field data type is not #JK_PIM_BINARY.<br>
 *     #JK_PIM_INDEX_ERR: 
 *        The data count exceeded the maximum number of the data the field can contain.
 *  
 *  This function appends binary data to data in the designated field of the item designated 
 *  by the <i>item</i> parameter.
 * 
 *  This function creates a copy of the data indicated by the <i>data</i> parameter for use.
 */
JKPIMErrCode AmPIMItemAddBinary(JKPIMItem* item,JKSint32 fieldId,JKSint32 attributes,JKSint32 size,JKSint8* data);

/**
 *  Appends a boolean data to the designated field of an item.
 *  
 *  @param *item 
 *         A pointer to the JKPIMItem structure containing the item.
 *  @param fieldId 
 *         The Field ID.
 *  @param attributes 
 *         One or combination of the bits of the attributes.
 *         Designate the bitwise OR of the values defined as attribute identifiers.
 *  @param data 
 *         The boolean data.
 *  @return 
 *     #JK_PIM_SUCCESS: Succeeded.<br>
 *     #JK_PIM_MEMORY_ERR: Insufficient memory error.<br>
 *     #JK_PIM_INVALID_PARAM_ERR: Invalid parameter was designated<br>
 *     #JK_PIM_UNSUPPORTED_FIELD_ERR:
 *        The designated field does not exist, or the field data type is not #JK_PIM_BOOLEAN.<br>
 *     #JK_PIM_INDEX_ERR:
 *        The data count exceeded the maximum number of the data the field can contain.
 *  
 *  This function appends a boolean data to the data in a field of the item designated 
 *  by the <i>item</i> parameter.
 */
JKPIMErrCode AmPIMItemAddBoolean(JKPIMItem* item,JKSint32 fieldId,JKSint32 attributes,JKBool data);

/**
 *  Appends a date data to the designated field of an item.
 *  
 *  @param *item 
 *         A pointer to the JKPIMItem structure containing the item.
 *  @param fieldId 
 *         The Field ID.
 *  @param attributes 
 *         One or combination of the bits of the attributes.
 *         Designate the bitwise OR of the values defined as attribute identifiers.
 *  @param *data 
 *         A pointer to the memory area holding the date data. 
 *  @return 
 *     #JK_PIM_SUCCESS: Succeeded.<br>
 *     #JK_PIM_MEMORY_ERR: Insufficient memory error.<br>
 *     #JK_PIM_INVALID_PARAM_ERR: Invalid parameter was designated<br>
 *     #JK_PIM_UNSUPPORTED_FIELD_ERR:
 *        The designated field does not exist, or the field data type is not #JK_PIM_DATE.<br>
 *     #JK_PIM_INDEX_ERR:
 *        The data count exceeded the maximum number of the data the field can contain.
 *  
 *  This function appends date data to data in the designated field of the item designated 
 *  by the <i>item</i> parameter.
 *  
 *  This function creates a copy of the data indicated by the <i>value</i> parameter for use.
 */
JKPIMErrCode AmPIMItemAddDate(JKPIMItem* item,JKSint32 fieldId,JKSint32 attributes,JKPIMDate* value);

/**
 *  Appends integer data to the designated field of an item.
 *  
 *  @param *item 
 *         A pointer to the JKPIMItem structure containing the item.
 *  @param fieldId 
 *         The Field ID.
 *  @param attributes 
 *         One or combination of the bits of the attributes.
 *         Designate the bitwise OR of the values defined as attribute identifiers.
 *  @param data 
 *         The integer data.
 *  @return 
 *     #JK_PIM_SUCCESS: Succeeded.<br>
 *     #JK_PIM_MEMORY_ERR: Insufficient memory error.<br>
 *     #JK_PIM_INVALID_PARAM_ERR: Invalid parameter was designated
 *     #JK_PIM_UNSUPPORTED_FIELD_ERR:
 *        The designated field does not exist, or the field data type is not #JK_PIM_INT.<br>
 *     #JK_PIM_INDEX_ERR: 
 *        The data count exceeded the maximum number of the data the field can contain.
 *  
 *  This function appends integer data to data in a field of the item designated 
 *  by the <i>item</i> parameter.
 */
JKPIMErrCode AmPIMItemAddInt(JKPIMItem* item,JKSint32 fieldId,JKSint32 attributes,JKSint32 data);

/**
 *  Appends string data to the designated field of an item.
 *  
 *  @param *item 
 *         A pointer to the JKPIMItem structure containing the item.
 *  @param fieldId 
 *         The Field ID.
 *  @param attributes 
 *         One or combination of the bits of the attributes.
 *         Designate the bitwise OR of the values defined as attribute identifiers.
 *  @param *data 
 *         A pointer to the JKPIMString structure holding the string data. 
 *  @return 
 *     #JK_PIM_SUCCESS: Succeeded.<br>
 *     #JK_PIM_MEMORY_ERR: Insufficient memory error.<br>
 *     #JK_PIM_INVALID_PARAM_ERR: Invalid parameter was designated<br>
 *     #JK_PIM_UNSUPPORTED_FIELD_ERR:
 *        The designated field does not exist, or the field data type is not #JK_PIM_STRING.<br>
 *     #JK_PIM_INDEX_ERR: 
 *        The data count exceeded the maximum number of the data the field can contain.
 *  
 *  This function appends string data to data in a field of the item designated 
 *  by the </i>item</i> parameter.
 *  
 *  This function creates a copy of the data indicated by the <i>data</i> parameter for use.
 */
JKPIMErrCode AmPIMItemAddString(JKPIMItem* item,JKSint32 fieldId,JKSint32 attributes,JKPIMString* data);

/**
 *  Appends string array data to the designated field of an item.
 *  
 *  @param *item 
 *         A pointer to the JKPIMItem structure containing the item.
 *  @param fieldId 
 *         The Field ID.
 *  @param attributes 
 *         One or combination of the bits of the attributes.
 *         Designate the bitwise OR of the values defined as attribute identifiers.
 *  @param *data 
 *         A pointer to the JKPIMStringArray structure holding the string array data. 
 *  @return 
 *     #JK_PIM_SUCCESS: Succeeded.<br>
 *     #JK_PIM_MEMORY_ERR: Insufficient memory error.<br>
 *     #JK_PIM_INVALID_PARAM_ERR: Invalid parameter was designated<br>
 *     #JK_PIM_UNSUPPORTED_FIELD_ERR:
 *        The designated field does not exist, or the field data type is not #JK_PIM_STRING_ARRAY.<br>
 *     #JK_PIM_INDEX_ERR: 
 *        The data count exceeded the maximum number of the data the field can contain.
 *  
 *  This function appends string array data to the data in a field of the item designated 
 *  by the <i>item</i> parameter.
 *  
 *  This function creates a copy of the data indicated by the <i>data</i> parameter for use.
 */
JKPIMErrCode AmPIMItemAddStringArray(JKPIMItem* item,JKSint32 fieldId,JKSint32 attributes,JKPIMStringArray* data);

/**
 *  Appends calendar array data to the designated field of an item.
 * 
 *  @param *item 
 *         A pointer to the JKPIMItem structure containing the item.
 *  @param fieldId 
 *         The Field ID.
 *  @param attributes 
 *         One or combination of the bits of the attributes.
 *         Designate the bitwise OR of the values defined as attribute identifiers.
 *  @param *data 
 *         A pointer to the JKPIMCalendar structure holding the calendar data. 
 *  @return 
 *     #JK_PIM_SUCCESS: Succeeded.<br>
 *     #JK_PIM_MEMORY_ERR: Insufficient memory error.<br>
 *     #JK_PIM_INVALID_PARAM_ERR: Invalid parameter was designated<br>
 *     #JK_PIM_UNSUPPORTED_FIELD_ERR:
 *        The designated field does not exist, or the field data type is not #JK_PIM_CALENDAR.<br>
 *     #JK_PIM_INDEX_ERR: 
 *        The data count exceeded the maximum number of the data the field can contain.
 *  
 *  This function appends calendar data to the data in a field of the item designated 
 *  by the <i>item</i> parameter.
 *  
 *  This function creates a copy of the data indicated by the <i>value</i> parameter for use.
 */
JKPIMErrCode AmPIMItemAddCalendar(JKPIMItem* item,JKSint32 fieldId,JKSint32 attributes,JKPIMCalendar* value);


/*	========== ========== ========== ========== ==========
     @: Functions for setting data to an item (JKPIMItem).
     If the item is a JKPIMEventItem, cast it to a JKPIMItem.
	========== ========== ========== ========== ========== */
/**
 *  Updates binary data in the designated field of an item.
 *  
 *  @param *item 
 *         A pointer to the JKPIMItem structure containing the item.
 *  @param fieldId 
 *         The Field ID.
 *  @param index 
 *         The index of the data to be updated. 
 *         The valid index range is from 0 to the number of field data minus 1.
 *  @param attributes 
 *         One or combination of the bits of the attributes.
 *         Designate the bitwise OR of the values defined as attribute identifiers.
 *  @param size 
 *         The binary data size in bytes. 
 *  @param *data 
 *         A pointer to the memory area holding the binary data. 
 *  @return 
 *     #JK_PIM_SUCCESS: Succeeded.<br>
 *     #JK_PIM_MEMORY_ERR: Insufficient memory error.<br>
 *     #JK_PIM_INVALID_PARAM_ERR: Invalid parameter was designated<br>
 *     #JK_PIM_UNSUPPORTED_FIELD_ERR:
 *        The designated field does not exist, or the field data type is not #JK_PIM_BINARY.<br>
 *     #JK_PIM_INDEX_ERR: 
 *        Invalid index was designated.
 *  
 *  This function updates the field binary data located at the position indicated by <i>index</i>.
 *  
 *  This function creates a copy of the data indicated by the <i>data</i> parameter for use.
 */
JKPIMErrCode AmPIMItemSetBinary(JKPIMItem* item,JKSint32 fieldId,JKSint32 index,JKSint32 attributes,JKSint32 size,JKSint8* data);

/**
 *  Updates boolean data in the designated field of an item.
 *  
 *  @param *item 
 *         A pointer to the JKPIMItem structure containing the item.
 *  @param fieldId 
 *         The Field ID.
 *  @param index 
 *         The index of the data to be updated. 
 *         The valid index range is from 0 to the number of field data minus 1.
 *  @param attributes 
 *         One or combination of the bits of the attributes.
 *         Designate the bitwise OR of the values defined as attribute identifiers.
 *  @param data 
 *         The boolean data.
 *  @return 
 *     #JK_PIM_SUCCESS: Succeeded.<br>
 *     #JK_PIM_MEMORY_ERR: Insufficient memory error.<br>
 *     #JK_PIM_INVALID_PARAM_ERR: Invalid parameter was designated<br>
 *     #JK_PIM_UNSUPPORTED_FIELD_ERR:
 *        The designated field does not exist, or the field data type is not #JK_PIM_BINARY.<br>
 *     #JK_PIM_INDEX_ERR: Invalid index was designated.
 *  
 *  This function updates the field boolean data located at the position indicated by <i>index</i>.
 */
JKPIMErrCode AmPIMItemSetBoolean(JKPIMItem* item,JKSint32 fieldId,JKSint32 index,JKSint32 attributes,JKBool data);

/**
 *  Updates date data in the designated field of an item.
 *  
 *  @param *item 
 *         A pointer to the JKPIMItem structure containing the item.
 *  @param fieldId 
 *         The Field ID.
 *  @param index 
 *         The index of the data to be updated. 
 *         The valid index range is from 0 to the number of field data minus 1.
 *  @param attributes 
 *         One or combination of the bits of the attributes.
 *         Designate the bitwise OR of the values defined as attribute identifiers.
 *  @param *data 
 *         A pointer to the memory area holding the date data. 
 *  @return 
 *     #JK_PIM_SUCCESS: Succeeded.<br>
 *     #JK_PIM_MEMORY_ERR: Insufficient memory error.<br>
 *     #JK_PIM_INVALID_PARAM_ERR: Invalid parameter was designated<br>
 *     #JK_PIM_UNSUPPORTED_FIELD_ERR:
 *        The designated field does not exist, or the field data type is not #JK_PIM_DATE.<br>
 *     #JK_PIM_INDEX_ERR: Invalid index was designated.
 * 
 *  This function updates the field date data located at the position indicated by <i>index</i>.
 *  
 *  This function creates a copy of the data indicated by the <i>value</i> parameter for use.
 */
JKPIMErrCode AmPIMItemSetDate(JKPIMItem* item,JKSint32 fieldId,JKSint32 index,JKSint32 attributes,JKPIMDate* value);

/**
 *  Updates integer data in the designated field of an item.
 *  
 *  @param *item 
 *         A pointer to the JKPIMItem structure containing the item.
 *  @param fieldId 
 *         The Field ID.
 *  @param index 
 *         The index of the data to be updated. 
 *         The valid index range is from 0 to the number of field data minus 1.
 *  @param attributes 
 *         One or combination of the bits of the attributes.
 *         Designate the bitwise OR of the values defined as attribute identifiers.
 *  @param data 
 *         The integer data.
 *  @return 
 *     #JK_PIM_SUCCESS: Succeeded.<br>
 *     #JK_PIM_MEMORY_ERR: Insufficient memory error.<br>
 *     #JK_PIM_INVALID_PARAM_ERR: Invalid parameter was designated<br>
 *     #JK_PIM_UNSUPPORTED_FIELD_ERR:
 *        The designated field does not exist, or the field data type is not #JK_PIM_INT.<br>
 *     #JK_PIM_INDEX_ERR: Invalid index was designated.
 *  
 *  This function updates the field integer data located at the position indicated by <i>index</i>.
 */
JKPIMErrCode AmPIMItemSetInt(JKPIMItem* item,JKSint32 fieldId,JKSint32 index,JKSint32 attributes,JKSint32 data);

/**
 *  Updates string data in the designated field of an item.
 *  
 *  @param *item 
 *         A pointer to the JKPIMItem structure containing the item.
 *  @param fieldId 
 *         The Field ID.
 *  @param index 
 *         The index of the data to be updated. 
 *         The valid index range is from 0 to the number of field data minus 1.
 *  @param attributes 
 *         One or combination of the bits of the attributes.
 *         Designate the bitwise OR of the values defined as attribute identifiers.
 *  @param *data 
 *         A pointer to the memory area holding the string data. 
 *  @return 
 *     #JK_PIM_SUCCESS: Succeeded.<br>
 *     #JK_PIM_MEMORY_ERR: Insufficient memory error.<br>
 *     #JK_PIM_INVALID_PARAM_ERR: Invalid parameter was designated<br>
 *     #JK_PIM_UNSUPPORTED_FIELD_ERR:
 *        The designated field does not exist, or the field data type is not #JK_PIM_STRING.<br>
 *     #JK_PIM_INDEX_ERR: Invalid index was designated.
 * 
 *  This function updates the field string data located at the position indicated by <i>index</i>.
 *  
 *  This function creates a copy of the data indicated by the <i>data</i> parameter for use.
 */
JKPIMErrCode AmPIMItemSetString(JKPIMItem* item,JKSint32 fieldId,JKSint32 index,JKSint32 attributes,JKPIMString* data);

/**
 *  Updates string array data in the designated field of an item.
 *  
 *  @param *item 
 *         A pointer to the JKPIMItem structure containing the item.
 *  @param fieldId 
 *         The Field ID.
 *  @param index 
 *         The index of the data to be updated. 
 *         The valid index range is from 0 to the number of field data minus 1.
 *  @param attributes 
 *         One or combination of the bits of the attributes.
 *         Designate the bitwise OR of the values defined as attribute identifiers.
 *  @param *data 
 *         A pointer to the JKPIMStringArray structure holding the string array. 
 *  @return 
 *     #JK_PIM_SUCCESS: Succeeded.<br>
 *     #JK_PIM_MEMORY_ERR: Insufficient memory error.<br>
 *     #JK_PIM_INVALID_PARAM_ERR: Invalid parameter was designated<br>
 *     #JK_PIM_UNSUPPORTED_FIELD_ERR:
 *        The designated field does not exist, or the field data type is not #JK_PIM_STRING_ARRAY.<br>
 *     #JK_PIM_INDEX_ERR: Invalid index was designated.
 *  
 *  This function updates the field string array data located at the position indicated by <i>index</i>.
 *  
 *  This function creates a copy of the data indicated by the <i>data</i> parameter for use.
 */
JKPIMErrCode AmPIMItemSetStringArray(JKPIMItem* item,JKSint32 fieldId,JKSint32 index,JKSint32 attributes,JKPIMStringArray* data);

/**
 *  Updates calendar data in the designated field of an item.
 *  
 *  @param *item 
 *         A pointer to the JKPIMItem structure containing the item.
 *  @param fieldId 
 *         The Field ID.
 *  @param index 
 *         The index of the data to be updated. 
 *         The valid index range is from 0 to the number of field data minus 1.
 *  @param attributes 
 *         One or combination of the bits of the attributes.
 *         Designate the bitwise OR of the values defined as attribute identifiers.
 *  @param *data 
 *         A pointer to the JKPIMCalendar structure holding the calendar data. 
 *  @return 
 *     #JK_PIM_SUCCESS: Succeeded.<br>
 *     #JK_PIM_MEMORY_ERR: Insufficient memory error.<br>
 *     #JK_PIM_INVALID_PARAM_ERR: Invalid parameter was designated<br>
 *     #JK_PIM_UNSUPPORTED_FIELD_ERR:
 *        The designated field does not exist, or the field data type is not #JK_PIM_STRING_ARRAY.<br>
 *     #JK_PIM_INDEX_ERR: Invalid index was designated.
 *  
 *  This function updates the field string array data located at the position indicated by <i>index</i>.
 *  
 *  This function creates a copy of the data indicated by the <i>value</i> parameter for use.
 */
JKPIMErrCode AmPIMItemSetCalendar(JKPIMItem* item,JKSint32 fieldId,JKSint32 attributes,JKSint32 index,JKPIMCalendar* value);


/** @} end of group */

/**
 * @addtogroup Module_PIM_ACCESS_JBI_item_1
 *
 * @{
 */

/*	========== ========== ========== ========== ==========
     @: Functions for deleting data to an item (JKPIMItem).
     If the item is a JKPIMEventItem, cast it to a JKPIMItem.
	========== ========== ========== ========== ========== */
/**
 *  Deletes the designated category from an item.
 * 
 * @param *item
 *         A pointer to the JKPIMItem structure containing the item.
 * @param index
 *         The index of the data to be deleted. 
 *         The valid index range is from 0 to the number of category minus 1.
 * @return
 *     #JK_PIM_SUCCESS: Succeeded.<br>
 *     #JK_PIM_MEMORY_ERR: Insufficient memory error.<br>
 *     #JK_PIM_INVALID_PARAM_ERR: Invalid parameter was designated<br>
 *     #JK_PIM_INDEX_ERR: Invalid index was designated.
 * 
 */
JKPIMErrCode AmPIMItemRemoveCategory(JKPIMItem* item,JKSint32 index);

/** @} end of group */

/**
 * @addtogroup Module_PIM_ACCESS_JBI_item_2
 *
 * @{
 */

/**
 *  Deletes data from the designated field of an item.
 *  
 *  @param *item 
 *         A pointer to the JKPIMItem structure containing the item.
 *  @param fieldId 
 *         The Field ID.
 *  @param index 
 *         The index of the data to be deleted. 
 *         The valid index range is from 0 to the number of field data minus 1.
 *  @return 
 *     #JK_PIM_SUCCESS: Succeeded.<br>
 *     #JK_PIM_INVALID_PARAM_ERR: Invalid parameter was designated<br>
 *     #JK_PIM_MEMORY_ERR: Insufficient memory error.<br>
 *     #JK_PIM_UNSUPPORTED_FIELD_ERR:
 *       The designated field does not exist.<br>
 *     #JK_PIM_INDEX_ERR: Invalid index was designated.
 *  
 *  This function deletes the field data located at the position indicated by <i>index</i>.
 */
JKPIMErrCode AmPIMItemRemove(JKPIMItem* item,JKSint32 fieldId,JKSint32 index);


/** @} end of group */

/**
 * @addtogroup Module_PIM_ACCESS_JBI_item_1
 *
 * @{
 */

/*	========== ========== ========== ========== ==========
     @: Functions for getting data to an item (JKPIMItem).
     If the item is a JKPIMEventItem, cast it to a JKPIMItem.
	========== ========== ========== ========== ========== */
/**
 *         Gets the list type of an item.
 *  @param *item
 *         A pointer to the JKPIMItem structure containing the item.
 *  @param *listType
 *         A pointer to the memory area for receiving the list type.
 *  
 *  @return
 *     #JK_PIM_SUCCESS: Succeeded.<br>
 *     #JK_PIM_INVALID_PARAM_ERR: Invalid parameter was designated.
 *
 */
JKPIMErrCode AmPIMItemGetListType(JKPIMItem* item,JKPIMListType* listType);

/** @} end of group */

/**
 * @addtogroup Module_PIM_ACCESS_JBI_item_2
 *
 * @{
 */

/**
 *  Gets attributes from data in the designated field of an item.
 * 
 *  @param *item
 *         A pointer to the JKPIMItem structure containing the item.
 *  @param fieldId
 *         The Field ID.
 *  @param index
 *         The index of the data. 
 *         The valid index range is from 0 to the number of field data minus 1.
 *  @param *attributes
 *         A pointer to the memory area for receiving the attributes.
 *  @return 
 *     #JK_PIM_SUCCESS: Succeeded.<br>
 *     #JK_PIM_INVALID_PARAM_ERR: Invalid parameter was designated<br>
 *     #JK_PIM_UNSUPPORTED_FIELD_ERR:
 *       The designated field does not exist.<br>
 *     #JK_PIM_INDEX_ERR: Invalid index was designated.
 *  
 * This function gets attributes from the data designated by <i>index</i> in the designated field.
 * The attributes are set as a bitwise OR of the values defined as attribute identifiers.
 */
JKPIMErrCode AmPIMItemGetAttributes(JKPIMItem* item,JKSint32 fieldId,JKSint32 index,JKSint32* attributes);

/**
 *  Gets binary data from data in the designated field of an item.
 *  
 *  @param *item 
 *         A pointer to the JKPIMItem structure containing the item.
 *  @param fieldId 
 *         The Field ID.
 *  @param index 
 *         The index of the data. 
 *         The valid index range is from 0 to the number of field data minus 1.
 *  @param *attributes 
 *         A pointer to the memory area for receiving the attributes of the data.
 *  @param *size 
 *         A pointer to the buffer for receiving the size of the data in bytes.
 *  @param **data 
 *         A pointer to the buffer for receiving a pointer to the memory area holding the binary data.
 *  @return 
 *     #JK_PIM_SUCCESS: Succeeded.<br>
 *     #JK_PIM_INVALID_PARAM_ERR: Invalid parameter was designated<br>
 *     #JK_PIM_UNSUPPORTED_FIELD_ERR:
 *        The designated field does not exist, or the field data type is not #JK_PIM_BINARY.<br>
 *     #JK_PIM_INDEX_ERR: Invalid index was designated.
 *  
 * This function gets binary data and attributes from the data designated by <i>index</i> in the 
 * designated field.
 * The attributes are set as a bitwise OR of the values defined as attribute identifiers.
 *  
 *  The pointer this function puts in the buffer indicated by the <i>data</i>
 *  parameter remains valid as long as the item indicated by <i>item</i> exists and
 *  the binary data in the fields indicated by <i>fieldId</i> and <i>index</i> is not changed
 *  or deleted.
 */
JKPIMErrCode AmPIMItemGetBinary(JKPIMItem* item,JKSint32 fieldId,JKSint32 index,JKSint32* attributes,JKSint32* size,JKSint8** data);

/**
 *  Gets boolean data from data in the designated field of an item.
 *  
 *  @param *item 
 *         A pointer to the JKPIMItem structure containing the item.
 *  @param fieldId 
 *         The Field ID.
 *  @param index 
 *         The index of the data. 
 *         The valid index range is from 0 to the number of field data minus 1.
 *  @param *attributes 
 *         A pointer to the buffer for receiving the attributes of the data.
 *  @param *data 
 *         A pointer to the buffer for receiving the boolean data.
 *  @return 
 *     #JK_PIM_SUCCESS: Succeeded.<br>
 *     #JK_PIM_INVALID_PARAM_ERR: Invalid parameter was designated<br>
 *     #JK_PIM_UNSUPPORTED_FIELD_ERR:
 *        The designated field does not exist, or the field data type is not #JK_PIM_BOOLEAN.<br>
 *     #JK_PIM_INDEX_ERR: Invalid index was designated.
 * 
 * This function gets boolean data and attributes from the data designated by <i>index</i> in the 
 * designated field.
 * The attributes are set as a bitwise OR of the values defined as attribute identifiers.
 */
JKPIMErrCode AmPIMItemGetBoolean(JKPIMItem* item,JKSint32 fieldId,JKSint32 index,JKSint32* attributes,JKBool* data);

/**
 *  Gets date data from data in the designated field of an item.
 *  
 *  @param *item 
 *         A pointer to the JKPIMItem structure containing the item.
 *  @param fieldId 
 *         The Field ID.
 *  @param index 
 *         The index of the data. 
 *         The valid index range is from 0 to the number of field data minus 1.
 *  @param *attributes 
 *         A pointer to the buffer for receiving the attributes of the data.
 *  @param *data 
 *         A pointer to the buffer for receiving the date data.
 *  @return 
 *     #JK_PIM_SUCCESS: Succeeded.<br>
 *     #JK_PIM_INVALID_PARAM_ERR: Invalid parameter was designated<br>
 *     #JK_PIM_UNSUPPORTED_FIELD_ERR:
 *        The designated field does not exist, or the field data type is not #JK_PIM_DATE.<br>
 *     #JK_PIM_INDEX_ERR: Invalid index was designated.
 * 
 * This function gets date data and attributes from the data designated by <i>index</i> in the 
 * designated field.
 * The attributes are set as a bitwise OR of the values defined as attribute identifiers.
 */
JKPIMErrCode AmPIMItemGetDate(JKPIMItem* item,JKSint32 fieldId,JKSint32 index,JKSint32* attributes,JKPIMDate* value);

/**
 *  Gets integer data from data in the designated field of an item.
 *  
 *  @param *item 
 *         A pointer to the JKPIMItem structure containing the item.
 *  @param fieldId 
 *         The Field ID.
 *  @param index 
 *         The index of the data. 
 *         The valid index range is from 0 to the number of field data minus 1.
 *  @param *attributes 
 *         A pointer to the buffer for receiving the attributes of the data.
 *  @param *data 
 *         A pointer to the buffer for receiving the integer data.
 *  @return 
 *     #JK_PIM_SUCCESS: Succeeded.<br>
 *     #JK_PIM_INVALID_PARAM_ERR: Invalid parameter was designated<br>
 *     #JK_PIM_UNSUPPORTED_FIELD_ERR:
 *        The designated field does not exist, or the field data type is not #JK_PIM_INT.<br>
 *     #JK_PIM_INDEX_ERR: Invalid index was designated.
 * 
 * This function gets integer data and attributes from the data designated by <i>index</i> in the 
 * designated field.
 * The attributes are set as a bitwise OR of the values defined as attribute identifiers.
 */
JKPIMErrCode AmPIMItemGetInt(JKPIMItem* item,JKSint32 fieldId,JKSint32 index,JKSint32* attributes,JKSint32* data);

/**
 *  Gets string data from data in the designated field of an item.
 *  
 *  @param *item 
 *         A pointer to the JKPIMItem structure containing the item.
 *  @param fieldId 
 *         The Field ID.
 *  @param index 
 *         The index of the data. 
 *         The valid index range is from 0 to the number of field data minus 1.
 *  @param *attributes 
 *         A pointer to the buffer for receiving the attributes of the data.
 *  @param **data 
 *         A pointer to the buffer for receiving a pointer to the 
 *         JKPIMString structure holding the string data.
 *  @return 
 *     #JK_PIM_SUCCESS: Succeeded.<br>
 *     #JK_PIM_INVALID_PARAM_ERR: Invalid parameter was designated<br>
 *     #JK_PIM_UNSUPPORTED_FIELD_ERR:
 *        The designated field does not exist, or the field data type is not #JK_PIM_STRING.<br>
 *     #JK_PIM_INDEX_ERR: Invalid index was designated.
 * 
 * This function gets string data and attributes from the data designated by <i>index</i> in the 
 * designated field.
 * The attributes are set as a bitwise OR of the values defined as attribute identifiers.
 *  
 *  The pointer this function puts in the buffer indicated by the <i>data</i>
 *  parameter remains valid as long as the item indicated by <i>item</i> exists and
 *  the string data in the fields indicated by <i>fieldId</i> and <i>index</i> is not changed
 *  or deleted.
 */
JKPIMErrCode AmPIMItemGetString(JKPIMItem* item,JKSint32 fieldId,JKSint32 index,JKSint32* attributes,JKPIMString** data);

/**
 *  Gets string array data from data in the designated field of an item.
 *  
 *  @param *item 
 *         A pointer to the JKPIMItem structure containing the item.
 *  @param fieldId 
 *         The Field ID.
 *  @param index 
 *         The index of the data. 
 *         The valid index range is from 0 to the number of field data minus 1.
 *  @param *attributes 
 *         A pointer to the buffer for receiving the attributes of the data.
 *  @param **data 
 *         A pointer to the buffer for receiving a pointer to the JKPIMStringArray 
 *         structure holding the string array data.
 *  @return 
 *     #JK_PIM_SUCCESS: Succeeded.<br>
 *     #JK_PIM_INVALID_PARAM_ERR: Invalid parameter was designated<br>
 *     #JK_PIM_UNSUPPORTED_FIELD_ERR:
 *        The designated field does not exist, or the field data type is not #JK_PIM_STRING_ARRAY.<br>
 *     #JK_PIM_INDEX_ERR: Invalid index was designated.
 *  
 * This function gets string array data and attributes from the data designated by <i>index</i> in the 
 * designated field.
 * The attributes are set as a bitwise OR of the values defined as attribute identifiers.
 *  
 *  The pointer this function puts in the buffer indicated by the <i>data</i>
 *  parameter remains valid as long as the item indicated by <i>item</i> exists and
 *  the string array data in the fields indicated by <i>fieldId</i> and <i>index</i> is not changed
 *  or deleted.
 */
JKPIMErrCode AmPIMItemGetStringArray(JKPIMItem* item,JKSint32 fieldId,JKSint32 index,JKSint32* attributes,JKPIMStringArray** data);

/**
 *  Gets calendar data from data in the designated field of an item.
 *  
 *  @param *item 
 *         A pointer to the JKPIMItem structure containing the item.
 *  @param fieldId 
 *         The Field ID.
 *  @param index 
 *         The index of the data. 
 *         The valid index range is from 0 to the number of field data minus 1.
 *  @param *attributes 
 *         A pointer to the buffer for receiving the attributes of the data.
 *  @param *value
 *         A pointer to the buffer for receiving the calendar data.
 *  @return 
 *     #JK_PIM_SUCCESS: Succeeded.<br>
 *     #JK_PIM_INVALID_PARAM_ERR: Invalid parameter was designated<br>
 *     #JK_PIM_UNSUPPORTED_FIELD_ERR:
 *        The designated field does not exist, or the field data type is not #JK_PIM_STRING_ARRAY.<br>
 *     #JK_PIM_INDEX_ERR: Invalid index was designated.
 *  
 * This function gets string array data and attributes from the data designated by <i>index</i> in the 
 * designated field.
 * The attributes are set as a bitwise OR of the values defined as attribute identifiers.
 *  
 */
JKPIMErrCode AmPIMItemGetCalendar(JKPIMItem* item,JKSint32 fieldId,JKSint32 index,JKSint32* attributes,JKPIMCalendar* value);


/*	========== ========== ========== ========== ==========
	 @: Function for access to a field (JKPIMField).
	========== ========== ========== ========== ========== */
/**
 *  Gets the attribute values of the designated field data. 
 * 
 * @param *field
 *         A pointer to the JKPIMField structure containing the field.
 * @param index
 *         The index of the data. 
 *         The valid index range is from 0 to the number of field data minus 1.
 * @param *attributes
 *         A pointer to the memory area for receiving the attributes.
 * @return
 *     #JK_PIM_SUCCESS: Succeeded.<br>
 *     #JK_PIM_INVALID_PARAM_ERR: Invalid parameter was designated<br>
 *     #JK_PIM_INDEX_ERR: Invalid index was designated.
 * 
 * This function gets attributes from the data designated by <i>index</i> in the designated <i>field</i>.
 * The attributes are set as a bitwise OR of the values defined as attribute identifiers.
 * 
 */
JKPIMErrCode AmPIMFieldGetAttributes(JKPIMField* field,JKSint32 index,JKSint32* attributes);

/** @} end of group */


/**
 * @addtogroup Module_PIM_ACCESS_JBI_variable_8
 *
 * @{
 */

/*	========== ========== ========== ========== ==========
	 @: Function for access to an item array (JKPIMItemArray).
	========== ========== ========== ========== ========== */
/**
 *  Creates an item array.
 * 
 * @return
 *     A pointer to the created JKPIMItemArray structure: Succeeded<br>
 *     #JK_NULL: Failed
 * 
 *  This function creates and initializes structure JKPIMItemArray 
 *  representing an item array.
 */
JKPIMItemArray* AmPIMItemArrayCreate(void);

/**
 *  Frees the memory space added to an item array.
 * 
 * @param *array
 *         A pointer to the JKPIMItemArray structure containing the item array.
 * @return
 *     #JK_PIM_SUCCESS: Succeeded.<br>
 * 
 *  This function frees the pointer (JKPIMItem*) array
 *  indicated by the JKPIMItemArray structure <i>items</i> member,
 *  frees the structure memory space pointed to by the elements of that array,
 *  and initializes JKPIMItemArray structure member values as follows.
 *
 *   <i>size</i> = 0<br>
 *   <i>data</i> = #JK_NULL
 * 
 */
JKPIMErrCode AmPIMItemArrayClear(JKPIMItemArray* array);

/**
 * Deletes a JKPIMItemArray structure and frees its memory space.
 * 
 * @param *array
 *         A pointer to the JKPIMItemArray structure containing the item array.
 * @return 
 *     #JK_PIM_SUCCESS: Succeeded.
 * 
 *  This function frees the pointer (JKPIMItem*) array
 *  indicated by the JKPIMItemArray structure <i>items</i> member,
 *  frees the structure memory space pointed to by the elements of that array,
 *  and frees the memory space of the JKPIMItemArray structure itself.
 *  
 *  After this function is called, the pointer passed in the <i>array</i> argument
 *  is no longer valid.
 * 
 */
JKPIMErrCode AmPIMItemArrayDestroy(JKPIMItemArray* array);

/**
 *         Adds an item to the item array.
 *  @param *array
 *         A pointer to the JKPIMItemArray structure containing the item array.
 *  @param *item
 *         A pointer to the JKPIMItem structure containing the item to be added.
 * 
 *  @return
 *     #JK_PIM_SUCCESS: Succeeded.<br>
 *     #JK_PIM_MEMORY_ERR: Insufficient memory error.<br>
 *     #JK_PIM_INVALID_PARAM_ERR: Invalid parameter was designated.
 * 
 *  This function adds the item indicated by the <i>item</i>
 *  parameter to the item array.
 * 
 *  A typical use of this function is for adding to an item array an item converted 
 *  from serial data, in the implementation of JkPIMExecuteFromSerialFormat().
 *  In this case JBlend is responsible for freeing the memory space of the added item.
 *  The pointer passed in <i>item</i> must therefore remain valid even after this function terminates. 
 */
JKPIMErrCode AmPIMItemArrayAddItem(JKPIMItemArray* array,JKPIMItem* item);


/** @} end of group */

/**
 * @addtogroup Module_PIM_ACCESS_JBI_repeat_rule_3
 *
 * @{
 */

/*	========== ========== ========== ========== ==========
	 @: Function for access to a JKPIMFrequencyDef.
	========== ========== ========== ========== ========== */
/**
 *  Creates a frequency definition structure.
 *
 * @return
 *     A pointer to the created JKPIMFrequencyDef structure: Succeeded<br>
 *     #JK_NULL: Failed
 * 
 *  This function creates and initializes structure JKPIMFrequencyDef 
 *  representing a frequency definition array.
 */
JKPIMFrequencyDef* AmPIMFrequencyDefCreate(void);


/**
 *  Frees the memory space added to a frequency definition.
 * 
 *  @param *def
 *         A pointer to the JKPIMFrequencyDef structure containing the frequency definition.
 *  @return 
 *     #JK_PIM_SUCCESS: Succeeded.
 *  
 *  This function frees the memory space indicated by all pointer 
 *  type data in the JKPIMFrequencyDef structure designated by the 
 *  <i>def</i> parameter, 
 *  and initializes JKPIMFrequencyDef structure member values as follows.
 *  
 *  yearly.size = monthly.size = weekly.size = daily.size = 0<br>
 *  yearly.data = monthly.data = weekly.data = daily.data = #JK_NULL;
 */
JKPIMErrCode AmPIMFrequencyDefClear(JKPIMFrequencyDef* def);

/**
 *  Frees the memory space added to a frequendy definition structure.
 *
 * @param *def
 *         A pointer to the JKPIMFrequencyDef structure containing the frequency definition.
 * 
 *  @return 
 *     #JK_PIM_SUCCESS: Succeeded.<br>
 *     #JK_PIM_INVALID_PARAM_ERR: Invalid parameter was designated
 *  
 *  This function frees the memory space indicated by all pointer 
 *  type data in the JKPIMFrequencyDef structure designated by the 
 *  <i>def</i> parameter, and frees the memory space of the JKPIMFrequencyDef 
 *  structure itself.
 *  
 *  After this function is called, the pointer passed in the <i>def</i> argument
 *  is no longer valid.
 */
JKPIMErrCode AmPIMFrequencyDefDestroy(JKPIMFrequencyDef* def);


/**
 * Sets an integer to a member of a JKPIMFrequencyDef structure.
 * 
 * @param *def 
 *         A pointer to the JKPIMFrequencyDef structure containing the frequency definition.
 * @param frequency 
 *         A JKPIMFrequencyDef member to which the integer is set.
 *         Designate one of the following.
 *         <ul>
 *          <li> #JK_PIM_REPEAT_YEARLY
 *          <li> #JK_PIM_REPEAT_MONTHLY
 *          <li> #JK_PIM_REPEAT_WEEKLY
 *          <li> #JK_PIM_REPEAT_DAILY
 *         </ul>
 * @param value
 *         An interger to be added.
 * @return 
 *     #JK_PIM_SUCCESS: Succeeded<br>
 *     #JK_PIM_MEMORY_ERR: Insufficient memory<br>
 *     #JK_PIM_INVALID_PARAM_ERR: Invalid parameter was designated
 * 
 */
JKPIMErrCode AmPIMFrequencyDefAddInt(JKPIMFrequencyDef* def,JKSint32 frequency,JKSint32 value);


/**
 * Sets an integer array to a member of a JKPIMFrequencyDef structure.
 *
 * @param *def
 *         A pointer to the JKPIMFrequencyDef structure containing the frequency definition.
 * @param frequency 
 *         A JKPIMFrequencyDef member to which the integer array is set.
 *         Designate one of the following.
 *         <ul>
 *          <li> #JK_PIM_REPEAT_YEARLY
 *          <li> #JK_PIM_REPEAT_MONTHLY
 *          <li> #JK_PIM_REPEAT_WEEKLY
 *          <li> #JK_PIM_REPEAT_DAILY
 *         </ul>
 * @param *array
 *         A pointer to the memory area containing the integer array.
 * @param size
 *         The number of elements in the integer array.
 * @return
 *     #JK_PIM_SUCCESS: Succeeded<br>
 *     #JK_PIM_MEMORY_ERR: Insufficient memory<br>
 *     #JK_PIM_INVALID_PARAM_ERR: Invalid parameter was designated
 * 
 *  This function using AmPIMIntArrayClear() initializes a JKPIMIntArray structure
 *  which is a member of a JKPIMFrequencyDef structure, copies the integer array
 *  passed in the <i>array</i> parameter and sets it in the JKPIMIntArray structure.
 */
JKPIMErrCode AmPIMFrequencyDefSetIntArray(JKPIMFrequencyDef* def,JKSint32 frequency,JKSint32* array,JKSint32 size);

/** @} end of group */

/**
 * @addtogroup Module_PIM_ACCESS_JBI_repeat_rule_1
 *
 * @{
 */

/*	========== ========== ========== ========== ==========
	 @: Function for access to repeat rule (JKPIMRepeatRule).
	========== ========== ========== ========== ========== */
/**
 *  Creates a repeat rule.
 * 
 *  @return
 *     A pointer to the created JKPIMRepeatRule structure: Succeeded<br>
 *     #JK_NULL: Failed
 */
JKPIMRepeatRule* AmPIMRepeatRuleCreate(void);

/**
 *  Frees the memory space added to a repeat rule.
 *
 *  @param *repeat
 *         A pointer to the JKPIMRepeatRule structure containing the repeat rule.
 *  @return
 *     #JK_PIM_SUCCESS: Succeeded<br>
 *     #JK_PIM_INVALID_PARAM_ERR: Invalid parameter was designated
 *  
 *  This function frees the memory space added to the JKPIMLongArray structure which is 
 *  the <i>exceptDates</i> member of the JKPIMRepeatRule structure indicated by the <i>repeat</i> 
 *  parameter, and initializes JKPIMLongArray structure member values as follows.  
 *
 *   <i>size</i> = 0<br>
 *   <i>data</i> = #JK_NULL
 */
JKPIMErrCode AmPIMRepeatRuleClear(JKPIMRepeatRule* repeat);

/**
 * Deletes a JKPIMRepeatRule structure and frees its memory space.
 *
 * @param *repeat
  *         A pointer to the JKPIMRepeatRule structure containing the repeat rule.
 * @return 
 *     #JK_PIM_SUCCESS: Succeeded.
 * 
 *  This function frees the memory space added to the JKPIMLongArray structure which is 
 *  the <i>exceptDates</i> member of the JKPIMRepeatRule structure indicated by the <i>repeat</i> 
 *  parameter, and frees the memory space of the JKPIMRepeatRule structure itself.
 * 
 *  After this function is called, the pointer passed in the <i>repeat</i> argument
 *  is no longer valid.
 */
JKPIMErrCode AmPIMRepeatRuleDestroy(JKPIMRepeatRule* repeat);

/** @} end of group */

/**
 * @addtogroup Module_PIM_ACCESS_JBI_repeat_rule_2
 *
 * @{
 */

/**
 *  Sets integer data in the designated field of a repeat rule.
 * 
 *  @param *repeat
 *         A pointer to the JKPIMRepeatRule structure containing the repeat rule.
 *  @param fieldId
 *         The Field ID.
 *  @param data
 *         Integer data.
 *  @return
 *     #JK_PIM_SUCCESS: Succeeded<br>
 *     #JK_PIM_MEMORY_ERR: Insufficient memory<br>
 *     #JK_PIM_INVALID_PARAM_ERR: Invalid parameter was designated
 * 
 */
JKPIMErrCode AmPIMRepeatRuleSetInt(JKPIMRepeatRule* repeat,JKSint32 fieldId,JKSint32 value);

/**
 *  Gets a value from an integer field in a repeat rule. 
 *
 * @param *repeat
 *         A pointer to the JKPIMRepeatRule structure containing the repeat rule.
 * @param fieldId
 *         The field ID.
 * @param *value
 *         A pointer to the memory area for receiving the integer value.
 * @return
 *     #JK_PIM_SUCCESS: Succeeded.<br>
 *     #JK_PIM_INVALID_PARAM_ERR: Invalid parameter was designated.
 *     #JK_PIM_UNSUPPORTED_FIELD_ERR:
 *        The designated field does not exist, or the field data type is not #JK_PIM_INT.<br>
 *     #JK_PIM_FIELD_EMPTY: No value has been assigned to an indicated field.
 * 
 * 
 */
JKPIMErrCode AmPIMRepeatRuleGetInt(JKPIMRepeatRule* repeat,JKSint32 fieldId,JKSint32* value);

/**
 *  Gets a value from a date field in a repeat rule. 
 *
 * @param *repeat
 *         A pointer to the JKPIMRepeatRule structure containing the repeat rule.
 * @param fieldId
 *         The field ID.
 * @param *value
 *         A pointer to the memory area for receiving the date value.
 * @return
 *     #JK_PIM_SUCCESS: Succeeded.<br>
 *     #JK_PIM_INVALID_PARAM_ERR: Invalid parameter was designated.<br>
 *     #JK_PIM_FIELD_EMPTY: No value has been assigned to an indicated field.
 * 
 * 
 */
JKPIMErrCode AmPIMRepeatRuleGetDate(JKPIMRepeatRule* repeat,JKSint32 fieldId,JKPIMCalendar* value);

/**
 *  Sets date data in the designated field of a repeat rule.
 * 
 *  @param *repeat
 *         A pointer to the JKPIMRepeatRule structure containing the repeat rule.
 *  @param fieldId
 *         The Field ID.
 *  @param *data
 *         A pointer to the buffer holding the date to be set.
 *  @return
 *     #JK_PIM_SUCCESS: Succeeded<br>
 *     #JK_PIM_MEMORY_ERR: Insufficient memory<br>
 *     #JK_PIM_INVALID_PARAM_ERR: Invalid parameter was designated
 */
JKPIMErrCode AmPIMRepeatRuleSetDate(JKPIMRepeatRule* repeat,JKSint32 fieldId,JKPIMCalendar* value);

/**
 *  Adds an except date to a JKPIMRepeatRule.
 *  
 * @param *repeat
 *  Adds an exclusion date to a repeat rule. 
 * @param *date
 *         A pointer to the JKPIMCalendar structure containing the exclusion date to be added.
 * @return
 *     #JK_PIM_SUCCESS: Succeeded.<br>
 *     #JK_PIM_MEMORY_ERR: Insufficient memory error.<br>
 *     #JK_PIM_INVALID_PARAM_ERR: Invalid parameter was designated.
 * 
 *  This function adds an except date to the array indicated by the <i>exceptDates</i> member of
 *  the #JKPIMRepeatRule structure.
 *  Data added to the array are arranged in ascending order.
 *  If the same date already exists in the array, this function returns
 *  #JK_PIM_SUCCESS without performing any other action.
 *  
 *  This function creates a copy of the data indicated by the <i>text</i> parameter for use.
 */
JKPIMErrCode AmPIMRepeatRuleAddExceptDate(JKPIMRepeatRule* repeat,JKPIMCalendar* date);

/**
 *  Delete an except date from a JKPIMRepeatRule structure.
 *  
 * @param *repeat
 *         A pointer to the JKPIMRepeatRule structure containing the repeat rule.
 * @param *date
 *         A pointer to the JKPIMCalendar structure containing the except date to be deleted.
 * @return
 *     #JK_PIM_SUCCESS: Succeeded.<br>
 *     #JK_PIM_MEMORY_ERR: Insufficient memory<br>
 *     #JK_PIM_INVALID_PARAM_ERR: Invalid parameter was designated
 * 
 *  If the designated date does not exist in the array, this function returns
 *  #JK_PIM_SUCCESS without performing any other action.
 * 
 */
JKPIMErrCode AmPIMRepeatRuleRemoveExceptDate(JKPIMRepeatRule* repeat,JKPIMCalendar* date);

/**
 *  Gets from a repeat rule a list of dates matching the repeat conditions. 
 *  
 *  @param *repeat
 *         A pointer to the JKPIMRepeatRule structure containing the repeat rule.
 *  @param *start
 *         A pointer to the memory area holding the first repeat date. 
 *  @param *subsetBegin
 *         A pointer to the memory area holding the first date in the term of the list.
 *  @param *subsetEnd
 *         A pointer to the memory area holding the last date in the term of the list.
 *  @param *array
 *         A pointer to the JKPIMLongArray structure for receiving the date.
 *  @return
 *     #JK_PIM_SUCCESS: Succeeded<br>
 *     #JK_PIM_MEMORY_ERR: Insufficient memory<br>
 *     #JK_PIM_INVALID_PARAM_ERR: Invalid parameter was designated
 *  
 *  This function puts in the structure indicated by the <i>array</i> parameter a list of dates
 *  matching the term from <i>subsetBegin</i> to <i>subsetEnd</i>, 
 *  when a repeat operation is performed according to the repeat rule designated in the 
 *  <i>repeat</i> parameter, starting from the date designated in the <i>start</i> parameter. 
 *  
 *  Designate <i>start</i>, <i>subsetBegin</i>, and <i>subsetEnd</i> parameter the date as a value 
 *  expressing the elapsed time in milliseconds from 00:00:00.000, January 1, 1970 (GMT).
 *  
 *  The date added by this function to the structure indicated by the <i>array</i> parameter
 *  remains valid until the repeat rule indicated by the <i>repeat</i> parameter is changed
 *  or deleted. 
 */
JKPIMErrCode AmPIMRepeatRuleGetDates(JKPIMRepeatRule* rule,JKPIMCalendar* startDate,JKPIMCalendar* subStartDate,JKPIMCalendar* subEndDate,JKPIMCalendarArray* array);


/*	========== ========== ========== ========== ==========
	 @: Fucntion for access to an event item (JKPIMEventItem).
	========== ========== ========== ========== ========== */
/**
 *  Gets the repeat rule set in an event item. 
 *  
 *  @param *item
 *         A pointer to the JKPIMEventItem structure containing the event item data.
 *  @return
 *     A pointer to the JKPIMRepeatRule structure holding the repeat rule: Succeeded<br>
 *     #JK_NULL: The event item does not contain a repeat rule 
 *  
 */
JKPIMRepeatRule* AmPIMEventItemGetRepeatRule(JKPIMEventItem* item);

/**
 *  Sets a repeat rule to an event item.
 * 
 *  @param *item
 *         A pointer to the JKPIMEventItem structure containing the event item data.
 *  @param *repeat
 *         A pointer to the JKPIMRepeatRule structure containing the repeat rule to be set.
 *  @return
 *     #JK_PIM_SUCCESS: Succeeded<br>
 *     #JK_PIM_MEMORY_ERR: Insufficient memory<br>
 *     #JK_PIM_INVALID_PARAM_ERR: Invalid parameter was designated
 * 
 *  This function creates a copy of the repeat rule indicated by the <i>repeat</i> parameter for use.
 */
JKPIMErrCode AmPIMEventItemSetRepeatRule(JKPIMEventItem* item,JKPIMRepeatRule* repeat);



#ifdef __cplusplus
}
#endif

#endif /* ___AM_PIM_UTIL_H */

/** @} end of group */









