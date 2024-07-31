/**
 * @file 
 */

/**
 * @addtogroup Module_PIM_EVENT_JBI
 *
 * @{
 */

#ifndef ___AM_PIM_EVENT_H
#define ___AM_PIM_EVENT_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */



#include "jkpimcommon.h"


/**
 *  Notifies completion of an asynchronous JSI function.
 *  
 *  @param *event 
 *         A pointer to the array holding the event. 
 *         See the individual JSI descriptions regarding the specific values.
 *  @param size 
 *         The effective size of the event passed in the <i>event</i> parameter.
 *  
 *  @return
 *     0: Succeeded<br>
 *     A negative value: Failed
 * 
 *  If PIM JSI function is implemented as an asynchronous function, call this 
 *  function when the processing is completed to notify JBlend of an event indicating the result.
 *  
 *  Parameters are enqueued in the event queue after their values are copied. 
 *  The memory area passed in the <i>event</i> parameter must therefore be freed on the calling end. 
 */
JKSint32 AmPIMNotifyAsyncFunctionFinished(JKSint32* event,JKSint32 size);


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* ___AM_PIM_EVENT_H */

/** @} end of group */
