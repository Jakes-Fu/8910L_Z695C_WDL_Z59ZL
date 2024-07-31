/*
 * Copyright 2005 Aplix Corporation. All rights reserved.
 */
/**
 * @file JkTypesKeyCode.h
@if NEWTXT
 *  Keycode Customization JBI Data Type
@endif
 */
 /**
 * @addtogroup Module_KeyCode_Customize_JBI
 * @{
 */
#ifndef ___JKTYPES_KEYCODE_H
#define ___JKTYPES_KEYCODE_H

#include "jktypes.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
@if NEWTXT
 * The structure type for containing key code and key name to be passed to Java application.
 * 
 * This structure is used as elements in the #JKT_StdKeyTable and #JKT_ExtKeyTable structures.
@endif
 * @see AmSetKeyCodeTable()
 */
typedef struct JKT_KeyTableElement {
   /**
@if NEWTXT
    * The key code.
    * 
    * Set a value to be passed in the <code>keyCode</code> parameter of
    * the method <code>keyPressed</code>, <code>keyReleased</code>, or <code>keyRepeated</code>
    * of <code>Canvas</code> and <code>CustomItem</code> classes.
@endif
    */
    JKSint32 keyCode;
   /**
@if NEWTXT
    * A pointer to the string representing key name.
    * 
    * Set a pointer to the memory area containing the key name 
    * that is used by the <code>Canvas</code> class <code>getKeyName</code> method
    * in returning the name to the Java application.
@endif
    */
    JKUTF8Char *keyName;
   /**
@if NEWTXT
    * The length in bytes of the string indicated by the <i>keyName</i> member.
@endif
    */
    JKSint32 keyNameLength;
} JKT_KeyTableElement;


/**
@if NEWTXT
 * The structure type for containing the key code and key name of JBlend standard keys.
 *
 * This structure is used for customising key code and key name that are passed to the Java application,
 * by passing a pointer to this structure to the <i>stdKeyTableAddr</i> parameter of 
 * AmSetKeyCodeTable().
 * 
 * The following table shows correspondence between JBlend standard key codes and members 
 * of this structure, along with the key code and key name that are passed to the Java application.
 * 
 * <table border="1">
 * <tr><td><b>JBlend Standard Keys</b>
 *     <td><b>Values to be Passed to AmNotifyKeyEvent()</b></td>
 *     <td><b>Structure Members</b></td>
 *     <td><b>Prescribed Key Code to be Passed to Java</b></td>
 *     <td><b>Prescribed Key Name to be Passed to Java</b></td></tr>
 * <tr><td>The 0 key</td><td>#AM_KEYCODE_NUM0</td><td>num0</td><td>48 (Not changeable)</td><td>NUM0</td></tr>
 * <tr><td>The 1 key</td><td>#AM_KEYCODE_NUM1</td><td>num1</td><td>49 (Not changeable)</td><td>NUM1</td></tr>
 * <tr><td>The 2 key</td><td>#AM_KEYCODE_NUM2</td><td>num2</td><td>50 (Not changeable)</td><td>NUM2</td></tr>
 * <tr><td>The 3 key</td><td>#AM_KEYCODE_NUM3</td><td>num3</td><td>51 (Not changeable)</td><td>NUM3</td></tr>
 * <tr><td>The 4 key</td><td>#AM_KEYCODE_NUM4</td><td>num4</td><td>52 (Not changeable)</td><td>NUM4</td></tr>
 * <tr><td>The 5 key</td><td>#AM_KEYCODE_NUM5</td><td>num5</td><td>53 (Not changeable)</td><td>NUM5</td></tr>
 * <tr><td>The 6 key</td><td>#AM_KEYCODE_NUM6</td><td>num6</td><td>54 (Not changeable)</td><td>NUM6</td></tr>
 * <tr><td>The 7 key</td><td>#AM_KEYCODE_NUM7</td><td>num7</td><td>55 (Not changeable)</td><td>NUM7</td></tr>
 * <tr><td>The 8 key</td><td>#AM_KEYCODE_NUM8</td><td>num8</td><td>56 (Not changeable)</td><td>NUM8</td></tr>
 * <tr><td>The 9 key</td><td>#AM_KEYCODE_NUM9</td><td>num9</td><td>57 (Not changeable)</td><td>NUM9</td></tr>
 * <tr><td>The * key</td><td>#AM_KEYCODE_ASTERISK</td><td>asterisk</td><td>42 (Not changeable)</td><td>STAR</td></tr>
 * <tr><td>The # key</td><td>#AM_KEYCODE_POUND</td><td>pound</td><td>35 (Not changeable)</td><td>POUND</td></tr>
 * <tr><td>The Left key</td><td>#AM_KEYCODE_LEFT</td><td>left</td><td>-2</td><td>LEFT</td></tr>
 * <tr><td>The Up key</td><td>#AM_KEYCODE_UP</td><td>up</td><td>-1</td><td>UP</td></tr>
 * <tr><td>The Right key</td><td>#AM_KEYCODE_RIGHT</td><td>right</td><td>-5</td><td>RIGHT</td></tr>
 * <tr><td>The Down key</td><td>#AM_KEYCODE_DOWN</td><td>down</td><td>-6</td><td>DOWN</td></tr>
 * <tr><td>The Select key</td><td>#AM_KEYCODE_SELECT</td><td>select</td><td>-20</td><td>SELECT</td></tr>
 * <tr><td>The soft key 1</td><td>#AM_KEYCODE_SOFT1</td><td>soft1</td><td>-21</td><td>SOFT1</td></tr>
 * <tr><td>The soft key 2</td><td>#AM_KEYCODE_SOFT2</td><td>soft2</td><td>-22</td><td>SOFT2</td></tr>
 * <tr><td>The soft key 3</td><td>#AM_KEYCODE_SOFT3</td><td>soft3</td><td>-23</td><td>SOFT3</td></tr>
 * <tr><td>The Send key</td><td>#AM_KEYCODE_SEND</td><td>send</td><td>-10</td><td>SEND</td></tr>
 * <tr><td>The Clear key</td><td>#AM_KEYCODE_CLEAR</td><td>clear</td><td>-8</td><td>CLEAR</td></tr>
 * </table>
 * 
 * The key code for the keys marked "Not changeable" in the table above are specified in the MIDP 2.0.
 * For those keys, the key code value set in the structure must be same as the prescribed value indicated in the table above.
 *
 * The key name and  key codes of the keys that are not marked "Not changeable" can be customized.
 *
 * Note that the prescribed values need to be set in the structure member for the keys that are not customized, including 
 * keys marked "Not changeable."
@endif
 * @see AmSetKeyCodeTable()
 */
typedef struct JKT_StdKeyTable {
   /**
@if NEWTXT
    * Information of the 0 key.
@endif
    */
   JKT_KeyTableElement num0;
   /**
@if NEWTXT
    * Information of the 1 key.
@endif
    */
   JKT_KeyTableElement num1;
   /**
@if NEWTXT // reuse
    * Information of the 2 key.
@endif
    */
   JKT_KeyTableElement num2;
   /**
@if NEWTXT // reuse
    * Information of the 3 key.
@endif
    */
   JKT_KeyTableElement num3;
   /**
@if NEWTXT // reuse
    * Information of the 4 key.
@endif
    */
   JKT_KeyTableElement num4;
   /**
@if NEWTXT // reuse
    * Information of the 5 key.
@endif
    */
   JKT_KeyTableElement num5;
   /**
@if NEWTXT // reuse
    * Information of the 6 key.
@endif
    */
   JKT_KeyTableElement num6;
   /**
@if NEWTXT // reuse
    * Information of the 7 key.
@endif
    */
   JKT_KeyTableElement num7;
   /**
@if NEWTXT // reuse
    * Information of the 8 key.
@endif
    */
   JKT_KeyTableElement num8;
   /**
@if NEWTXT // reuse
    * Information of the 9 key.
@endif
    */
   JKT_KeyTableElement num9;
   /**
@if NEWTXT
    * Information of the asterisk (*) key.
@endif
    */
   JKT_KeyTableElement asterisk;
   /**
@if NEWTXT
    * Information of the pound (#) key.
@endif
    */
   JKT_KeyTableElement pound;
   /**
@if NEWTXT
    * This member is reserved for future use.
    *
    * In the <i>KeyCode</i> member, set 0 ; <i>KeyName</i>, #JK_NULL; <i>KeyNameLength</i>, 0.
@endif
    */
   JKT_KeyTableElement reserve1;
   /**
@if NEWTXT // reuse
    * This member is reserved for future use.
    *
    * In the <i>KeyCode</i> member, set 0 ; <i>KeyName</i>, #JK_NULL; <i>KeyNameLength</i>, 0.
@endif
    */
   JKT_KeyTableElement reserve2;
   /**
@if NEWTXT // reuse
    * This member is reserved for future use.
    *
    * In the <i>KeyCode</i> member, set 0 ; <i>KeyName</i>, #JK_NULL; <i>KeyNameLength</i>, 0.
@endif
    */
   JKT_KeyTableElement reserve3;
   /**
@if NEWTXT // reuse
    * This member is reserved for future use.
    *
    * In the <i>KeyCode</i> member, set 0 ; <i>KeyName</i>, #JK_NULL; <i>KeyNameLength</i>, 0.
@endif
    */
   JKT_KeyTableElement reserve4;
   /**
@if NEWTXT
    * Information of the Left arrow key.
@endif
    */
   JKT_KeyTableElement left;
   /**
@if NEWTXT
    * Information of the Up arrow key.
@endif
    */
   JKT_KeyTableElement up;
   /**
@if NEWTXT
    * Information of the Right arrow key.
@endif
    */
   JKT_KeyTableElement right;
   /**
@if NEWTXT
    * Information of the Down arrow key.
@endif
    */
   JKT_KeyTableElement down;
   /**
@if NEWTXT
    * Information of the Select arrow key.
@endif
    */
   JKT_KeyTableElement select;
   /**
@if NEWTXT
    * Information of the soft key 1.
@endif
    */
   JKT_KeyTableElement soft1;
   /**
@if NEWTXT
    * Information of the soft key 2.
@endif
    */
   JKT_KeyTableElement soft2;
   /**
@if NEWTXT
    * Information of the soft key 3.
@endif
    */
   JKT_KeyTableElement soft3;
   /**
@if NEWTXT // reuse
    * This member is reserved for future use.
    *
    * In the <i>KeyCode</i> member, set 0 ; <i>KeyName</i>, #JK_NULL; <i>KeyNameLength</i>, 0.
@endif
    */
   JKT_KeyTableElement reserve5;
   /**
@if NEWTXT
    * Information of the Send key.
@endif
    */
   JKT_KeyTableElement send;
   /**
@if NEWTXT
    * Information of the Clear key.
@endif
    */
   JKT_KeyTableElement clear;
} JKT_StdKeyTable;


/**
@if NEWTXT
 * The structure type for containing the key code and key name of native system original keys.
 *
 * This structure is used by the native system in order to add key code and key name that are passed to the Java application,
 * by passing a pointer to this structure to the <i>extKeyTableAddr</i> parameter of 
 * AmSetKeyCodeTable().
 * 
 * To pass to the Java application a key event of a native system original key 
 * (i.e. a key that does not correspond to any of the JBlend standard keys), 
 * an original key code to be passed to AmNotifyKeyEvent() needs to be defined.
 * The original key codes must be assigned with successive values starting from #AM_KEYCODE_USER_BASE.
 *
 * Set values in this structure as follows.
 * - In the JKT_KeyTableElement structure positioned at 
 *   (native system original key code - #AM_KEYCODE_USER_BASE) of 
 *   the structure array indicated by the <i>extKeyList</i> member, 
 *   - the <i>keyCode</i> member should indicate the key code to be passed to the <code>Canvas</code> object, and 
 *   - the <i>keyName</i> and <i>keuNameLength</i> members should indicate the key name that is returned by the 
 *     <code>Canvas</code> class <code>getKeyName</code> method.
 *   .
 * - The <i>keyExtNum</i> member should indicate the number of elements in the array indicated by the <i>extKeyList</i> member.
 *   That is, the value is the number of the native system original keys.
 * .
@endif
 * @see AmSetKeyCodeTable()
 */
typedef struct JKT_ExtKeyTable {
   /**
@if NEWTXT
    * The initial address of the array containing key codes and key names of the 
    * native system original keys.
    * The array must contain the same number of elements as the number of the original keys to be added.
@endif
    */
   const JKT_KeyTableElement *extKeyList;
   /**
@if NEWTXT
    * The element count of the array indicated by the <i>extKeyList</i> member.
@endif
    */
   JKSint32   keyExtNum;
} JKT_ExtKeyTable;


#ifdef __cplusplus
}
#endif

#endif


/**
 * @} end of group
 */
