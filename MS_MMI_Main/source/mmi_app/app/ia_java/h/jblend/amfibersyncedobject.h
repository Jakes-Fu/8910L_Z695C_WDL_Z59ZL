/*
 * Copyright 2006 Aplix Corporation. All rights reserved.
 */

/**
 * @file AMFiberSyncedObject.h
 * Fiber Synced Object JBI
 */

#ifndef __AM_FIBERSYNCEDOBJECT_H
#define __AM_FIBERSYNCEDOBJECT_H

#include "jktypesfiber.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/**
 * @defgroup Fiber_Synced_Object_JBI Fiber Synced Object JBI
 *
 * @{
 *  Interface for operating a Synchronized Object
 *
 *  The functions in this interface can be used to interact with 
 *  a Synchronized Object within the associated Synchronized Object 
 *  Pool.
 *
 *  The memory resources used in the Fiber Scheduler object are provided 
 *  through the #AmFiberSyncedObjectPoolCreate() function.
 * 
 *  @par Warning: 
 *  The Fiber Synced Object JBI is <b>not</b> thread-safe.
 */

/**
 * @name Fiber Synchronization Object Type
 * @{
 */

/**
 *  Used to identify a Synchronized Event object.
 */
#define JK_FIBER_SO_SUBCLASS_EVENT         (0x01)
                                                    
/**
 *  Used to identify a Synchronized Section object.
 */
#define JK_FIBER_SO_SUBCLASS_SECTION       (0x02)

/**
 * @}
 */

/**
 *  Gets the Synchronized Object type.
 * 
 *  @param hFiberSyncedObject
 *   A valid Synchronized Object handle.
 *  @param puchSubclass
 *   A pointer to a buffer to receive the subclass of the Synchronized Object. 
 * 
 *  @return
 *   - #JK_RESULT_FIBER_OK<br>
 *     if the function succeeds.
 *   - #JK_RESULT_FIBER_BAD_PARAM<br>
 *     when an invalid parameter is supplied.
 * 
 *  <p>
 *  The memory referenced by the <i>puchSubclass</i> pointer is updated with 
 *  the type of the @ref Fiber_Synced_Object_JBI "Synchronized Object" 
 *  specified by <i>hFiberSyncedObject</i>. 
 * 
 *  Synchronized Object subclasses currently supported include 
 *  #JK_FIBER_SO_SUBCLASS_EVENT and #JK_FIBER_SO_SUBCLASS_SECTION.
 * 
 *  @par Invocation:
 *  This function may be called from a @ref Fiber_JBI "Fiber", thread or 
 *  process context. See the Fiber Service @ref 
 *  section_invocation_conditions "invocation rules" for details.
 *
 */

long AmFiberSyncedObjectIdentify( JKFiberSyncedObjectHandle hFiberSyncedObject, unsigned char *puchSubclass );

/**
 * @}
 */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __AM_FIBERSYNCEDOBJECT_H */
