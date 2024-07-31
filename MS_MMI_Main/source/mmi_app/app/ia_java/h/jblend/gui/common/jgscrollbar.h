/*
 * Copyright 2005 Aplix Corporation. All rights reserved.
 */

/**
 * @file
 *   Common API functions for the scrollbar component.
 */

#ifndef ___GUI_COMMON_SCROLLBAR_H
#define ___GUI_COMMON_SCROLLBAR_H

#include "jktypes.h"

#include "jgguievent.h"
#include "jgcom.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/**
 * @addtogroup GUI_ScrollBar
 * @{
 */

/**
 *   Scroll bar position: Terminating end.
 *   
 *   For an LTR language, scroll bar is positioned at the right side of a GUI component; 
 *   for RTL launguage, at the left side.
 */
#define JG_SBAR_VPOS_TAIL       (0x01)

/**
 *   Scroll bar position: Starting end.
 *   
 *   For an LTR language, scroll bar is positioned at the left side of a GUI component; 
 *   for RTL launguage, at the right side.
 */
#define JG_SBAR_VPOS_HEAD       (0x02)

/**
 *   Scroll bar display mode: Displayed when necessary.
 */
#define JG_SBAR_SHOW_MODE_IF_NEEDED     (0x01)

/**
 *   Scroll bar display mode: Always visible
 */
#define JG_SBAR_SHOW_MODE_ALWAYS        (0x02)

/**
 *   Scroll bar display mode: Always non-visible
 */
#define JG_SBAR_SHOW_MODE_NEVER         (0x03)

/**
 * @internal
 *   The maximum GUI scrollbar event value.
 */
#define JG_GUI_SBAR_EVT_END     (JG_GUI_EVT_END + 0x00)

/**
 *   The scroll bar component type
 * 
 * @see JgComGetComponentType
 */
#define JG_SCROLLBAR    21

/**
 *   The scroll bar handle.
 *   
 *   A scroll bar handle is data identifying the scroll bar.
 *   The scroll bar handle can be used as a component handle.
 * 
 * @see JGCom
 */
typedef JGCom       JGScrollBar;

/**
 *   Gets the page size of a scroll bar.
 *   
 *   @param scrollbar The scroll bar handle.
 *   @return The page size of the scroll bar is returned.
 *   
 *   In the following case @ref page_gui_trouble_shooting "Error" results.
 *   <ul>
 *     <li>When <i>scrollbar</i> is an illegal handle.</li>
 *   </ul>
 */
JKSint32 JgScrollBarGetPageSize(JGScrollBar scrollbar);

/**
 *   Gets the scroll bar maximum value.
 *   
 *   @param scrollbar The scroll bar handle.
 *   @return The maximum value is returned.
 *   
 *   In the following case @ref page_gui_trouble_shooting "Error" results.
 *   <ul>
 *     <li>When <i>scrollbar</i> is an illegal handle.</li>
 *   </ul>
 */
JKSint32 JgScrollBarGetMaxValue(JGScrollBar scrollbar);

/**
 *   Gets the scroll bar current value.
 *   
 *   @param scrollbar The scroll bar handle.
 *   @return The current value is returned.
 *   
 *   In the following case @ref page_gui_trouble_shooting "Error" results.
 *   <ul>
 *     <li>When <i>scrollbar</i> is an illegal handle.</li>
 *   </ul>
 */
JKSint32 JgScrollBarGetValue(JGScrollBar scrollbar);


/** @} */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* !___GUI_COMMON_SCROLLBAR_H */

