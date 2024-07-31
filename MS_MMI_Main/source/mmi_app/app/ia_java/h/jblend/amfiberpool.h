/*
 * Copyright 2006 Aplix Corporation. All rights reserved.
 */
 
/**
 * @file AMFiberPool.h
 * Fiber Pool JBI
 */
 
#ifndef __AM_FIBERPOOL_H
#define __AM_FIBERPOOL_H

#include "jktypesfiber.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/**
 * @defgroup Fiber_Pool_JBI Fiber Pool JBI
 * @{
 *  An interface for operating Fiber Pool object.
 *
 *  The functions in this interface can be used to create and destroy a 
 *  Fiber Pool. A Fiber Pool is used to create, recycle and manage a 
 *  collection of Fiber objects.
 *
 *  The Fiber Pool also provides access to its internal Fiber Scheduler 
 *  object. This object is used to drive the concurrent execution model 
 *  employed by the Fiber Service. 
 *
 *  The memory resources used in the Fiber Pool are provided primarily 
 *  through two methods #AmFiberPoolCreate() and 
 *  #AmFiberPoolCreateFiber(). The primary memory pool, and most critical, 
 *  is associated with the #AmFiberPoolCreate() function. It is used to 
 *  manage all objects within the Fiber Pool, including Fiber and Fiber 
 *  Scheduler. The memory associated with the #AmFiberPoolCreateFiber() 
 *  function is strictly used for stack management within the active Fiber 
 *  execution context.
 * 
 *  @par Note:
 *  All Fiber objects within the same Fiber Pool must execute within the 
 *  same thread or process context. 
 *
 *  @par Warning: 
 *  The Fiber Pool API is almost entirely <b>not</b> thread-safe.
 */

/* IMPORTANT: JK_FIBERPOOL_FIBERS_MAX _must_ be a multiple of 32. */

/**
 * @name Fiber Pool Capacity
 * @{
 */

/**
 *  The minimum number of Fiber objects hosted by a Fiber Pool.
 */
#define JK_FIBERPOOL_FIBERS_MIN                     (2)       /*    2 Fibers */

/**
 *  The maximum number of Fiber objects hosted by a Fiber Pool.
 */
#define JK_FIBERPOOL_FIBERS_MAX                     (32 << 5) /* 1024 Fibers */

/**
 * @}
 */
                                                    
/**
 *  Creates a Fiber Pool necessary to host and manage Fiber objects.
 * 
 *  @param uiFiberLimit
 *   The maximum number of Fibers that may exist in the pool.
 *  @param pvFiberHeap
 *   A void pointer to the heap used for all Fiber Pool related activities. If 
 *   JK_NULL is set, the recommended heap size is returned in 
 *   <i>pulFiberHeapSize</i>.
 *  @param pulFiberHeapSize
 *   An pointer to the size of the data pointed to by <i>pvFiberHeap</i>. If 
 *   <i>pvFiberHeap</i> is JK_NULL, then the memory referenced by this pointer 
 *   will receive the recommended heap size. If set to JK_NULL, the function 
 *   will return #JK_RESULT_FIBER_BAD_PARAM.
 *  @param phFiberPool
 *   Upon function success, a valid #JKFiberPoolHandle is returned.<br>
 *   Upon function failure, the contents of this parameter remain unchanged.<br>
 *   This parameter must not be JK_NULL if <i>pvFiberHeap</i> points to a 
 *   valid heap segment (is not JK_NULL).
 * 
 *  @return
 *   - #JK_RESULT_FIBER_OK<br>
 *     if the function succeeds.
 *   - #JK_RESULT_FIBER_BAD_PARAM<br>
 *     when an invalid parameter is supplied.
 * 
 *  <p>
 *  This function creates a @ref Fiber_Pool_JBI "Fiber Pool" object based on 
 *  the parameters and amount of contiguous heap memory provided. If the 
 *  <i>pvFiberHeap</i> parameter is set to JK_NULL, the function will 
 *  return a recommended heap size in the memory referenced by 
 *  <i>pulFiberHeapSize</i>. The heap size calculation uses 
 *  <i>uiFiberLimit</i> to determine the maximum @ref Fiber_JBI "Fiber" 
 *  capacity of the Fiber Pool.
 * 
 *  Valid values for the <i>uiFiberLimit</i> parameter are between 
 *  #JK_FIBERPOOL_FIBERS_MIN and #JK_FIBERPOOL_FIBERS_MAX inclusive.
 * 
 *  If the <i>pvFiberHeap</i> parameter points to a segment of memory, the 
 *  function will validate that the requested memory size (as specified by 
 *  the value referenced in <i>pulFiberHeapSize</i>) does indeed support the 
 *  memory requirements of the Fiber Pool being created.
 * 
 *  The value returned in <i>phFiberPool</i> may be used with the other Fiber Pool 
 *  JBI functions.
 * 
 *  @par Optimization:
 *  Small performance increases can be gained by keeping the size of the Fiber 
 *  Pool at 32 Fibers or less when setting <i>uiFiberLimit</i>. Heap memory 
 *  requirements are related directly to the value of <i>uiFiberLimit</i> as 
 *  well.
 * 
 *  @par Note:
 *  The memory provided via <i>pvFiberHeap</i> must remain valid until the 
 *  Fiber Pool object is destroyed via the #AmFiberPoolDestroy() function.
 *  @par
 *  This function is thread-safe.
 * 
 *  @par Invocation:
 *  This function may be called from a Fiber within a different (host) 
 *  Fiber Pool or directly from a thread or process context. See the Fiber 
 *  Service @ref section_invocation_conditions "invocation rules" for 
 *  details.
 *
 *  @see
 *  AmFiberPoolDestroy
 * 
 */

long AmFiberPoolCreate( unsigned int uiFiberLimit, void *pvFiberHeap, unsigned long *pulFiberHeapSize, JKFiberPoolHandle *phFiberPool );

/**
 *  Terminates hosted Fibers and destroys a Fiber Pool object.
 * 
 *  @param hFiberPool
 *   A valid Fiber Pool object handle.
 * 
 *  @return
 *   - #JK_RESULT_FIBER_OK<br>
 *     if the function succeeds.
 *   - #JK_RESULT_FIBER_BAD_PARAM<br>
 *     when an invalid parameter is supplied.
 *   - #JK_RESULT_FIBER_BAD_STATE<br>
 *     when called from the context of a Fiber from within the Fiber Pool.
 *
 *  <p>
 *  This function will automatically terminate any existing 
 *  @ref Fiber_JBI "Fibers" regardless of their current operational state. 
 *  Terminated Fibers will follow all of the same shutdown semantics as 
 *  described in the #AmFiberStop() function. After a successful return, 
 *  the memory associated with the Fiber Pool may be released.
 * 
 *  All associated @ref JKFiberHandle "JKFiberHandles" obtained from the @ref 
 *  Fiber_Pool_JBI "Fiber Pool" should be considered invalid at the moment the 
 *  function begins processing and should no longer be used upon 
 *  successful return.
 *
 *  Any exposed #JKFiberSchedulerHandle obtained from the Fiber Pool is 
 *  also invalidated and should not be used after this function call.
 *
 *  Fibers from other Fiber Pool contexts may use this function to 
 *  terminate a hosted Fiber Pool context.
 *
 *  @par Optimization:
 *  Because the shutdown semantics of the #AmFiberStop() function are used, the 
 *  Fiber's <i>pfStop</i> function will also be called during Fiber Pool 
 *  termination. As with normal Fiber termination, it is recommended that the 
 *  code within the scope of the <i>pfStop</i> function be limited to small and 
 *  fast cleanup operations. Long-term processing in the <i>pfStop</i> 
 *  function will lead to delays in the return of this function to the caller.
 * 
 *  @par Invocation:
 *  This function may <b>not</b> be called from a Fiber inside the 
 *  target Fiber Pool context. The function call must originate on the 
 *  thread or process which hosts the Fiber Pool. See the Fiber Service 
 *  @ref section_invocation_conditions "invocation rules" for details.
 *
 *  @see
 *  AmFiberStop
 * 
 */

long AmFiberPoolDestroy( JKFiberPoolHandle hFiberPool );

/**
 *  Retrieves the number of Fibers in the Fiber Pool.
 * 
 *  @param hFiberPool
 *   A valid Fiber Pool object handle.
 *  @param puiFiberCount
 *   Upon function success, the number of Fiber slots in the Fiber Pool is 
 *   returned in the memory referenced by this pointer.<br>
 *   Upon function failure, the contents of the memory referenced by this 
 *   pointer remain unchanged.<br>
 *   If set to JK_NULL, the function will return #JK_RESULT_FIBER_BAD_PARAM.
 * 
 *  @return
 *   - #JK_RESULT_FIBER_OK<br>
 *     if the function succeeds.
 *   - #JK_RESULT_FIBER_BAD_PARAM<br>
 *     when an invalid parameter is supplied.
 * 
 *  <p>
 *  This function will return, in memory referenced by <i>puiFiberCount</i>, 
 *  the number of @ref Fiber_JBI "Fibers" that the 
 *  @ref Fiber_Pool_JBI "Fiber Pool" may host. This value is essentially the 
 *  same as <i>uiFiberLimit</i> which was passed into the #AmFiberPoolCreate() 
 *  function during the creation of the Fiber Pool.
 *
 *  @par Invocation:
 *  This function may be called from a Fiber, thread or process context. 
 *  See the Fiber Service @ref 
 *  section_invocation_conditions "invocation rules" for details.
 *
 *  @see
 *  AmFiberPoolCreate
 * 
 */

long AmFiberPoolGetFiberCount( JKFiberPoolHandle hFiberPool, unsigned int *puiFiberCount );

/**
 * @name Fiber Stack Size
 * @{
 */

/**
 *  The minimum stack size accepted by the Fiber creation function.
 */
#define JK_FIBERPOOL_FIBERSTACK_MIN                 (1024)    /*        1 KB */

/**
 *  The maximum stack size accepted by the Fiber creation function.
 */
#define JK_FIBERPOOL_FIBERSTACK_MAX                 (2097152) /*        2 MB */

/**
 *  The default stack size recommended for the Fiber creation function.
 */
#define JK_FIBERPOOL_FIBERSTACK_DEFAULT             (262144)  /*      256 KB */ /* Suggestion only. */

/**
 * @}
 */

/**
 *  Frees memory.
 * 
 *  @param pvData
 *    A pointer to memory which is to be released.
 * 
 *  <p>
 *  Release allocated memory referenced by <i>pvData</i>.
 * 
 *  @par Note:
 *  This function should be thread-safe.
 * 
 */

typedef void ( * JK_FIBER_MEMORY_FREE ) ( void *pvData );

/**
 *  Fiber stack information structure.
 */
typedef struct _JKT_Fiber_Stack {
    /**
     *  Void pointer to the Fiber stack memory.
     */
    void                    *m_pvStack;

    /**
     *  Size of the Fiber stack memory referenced by m_pvStack.
     */
    unsigned long           m_ulFiberStackSize;

    /**
     *  Pointer to a function used to free the stack memory.
     */
    JK_FIBER_MEMORY_FREE    m_pfStackFree;
} JKT_Fiber_Stack;

/**
 * @name Fiber Lifecycle
 * @{
 */

/**
 *  The Fiber will have a normal life cycle whereby it will be automatically 
 *  recycled upon termination.
 */
#define JK_FIBER_LIFECYCLE_NORMAL                   (0x00000000)

/**
 *  The Fiber will have an immortal life cycle whereby it will not be  
 *  automatically recycled upon termination.
 */
#define JK_FIBER_LIFECYCLE_IMMORTAL                 (0x00000001)

/**
 *  The bit-field mask used to access the life cycle control flag in the Fiber 
 *  creation flags.
 */
#define JK_FIBER_LIFECYCLE_MASK                     (JK_FIBER_LIFECYCLE_NORMAL | JK_FIBER_LIFECYCLE_IMMORTAL)

/**
 *  The bit-field position (zero-based offset) of the life cycle control flag
 *  in the Fiber creation flags.
 */
#define JK_FIBER_LIFECYCLE_POS                      (0x00)

/**
 * @}
 */


/**
 *  Creates a Fiber in the Fiber Pool.
 * 
 *  @param hFiberPool
 *   A valid Fiber Pool object handle.
 *  @param ulFlags
 *   A bit-field specifying various flags for Fiber creation.
 *  @param psStack
 *   A pointer to a Fiber stack structure.
 *  @param phFiber
 *   Upon function success, a valid #JKFiberHandle is returned.<br>
 *   Upon function failure, the contents of this parameter remain 
 *   unchanged.
 * 
 *  @return
 *   - #JK_RESULT_FIBER_OK<br>
 *     if the function succeeds.
 *   - #JK_RESULT_FIBER_BAD_PARAM<br>
 *     when an invalid parameter is supplied.
 *   - #JK_RESULT_FIBER_NO_MEMORY<br>
 *     when there is not enough memory available to complete the operation.
 * 
 *  <p>
 *  This function allocates a @ref Fiber_JBI "Fiber" from the @ref 
 *  Fiber_Pool_JBI "Fiber Pool" specified by <i>hFiberPool</i> and returns the 
 *  handle to it in the memory referenced by <i>phFiber</i>.
 *
 *  The Fiber may be created with <i>ulFlags</i> specifying either 
 *  #JK_FIBER_LIFECYCLE_NORMAL or #JK_FIBER_LIFECYCLE_IMMORTAL.
 * 
 *  Fibers created with the #JK_FIBER_LIFECYCLE_NORMAL flag set will 
 *  automatically be recycled as they terminate. Because of this, it is not 
 *  possible to get the exit code of the Fiber unless another Fiber had locked 
 *  onto it with a call to the #AmFiberJoin() function. 
 *
 *  Fibers created with the #JK_FIBER_LIFECYCLE_IMMORTAL flag set will 
 *  not be automatically recycled upon termination. Resources used by immortal
 *  Fibers must be manually reclaimed by calling #AmFiberPoolRecycleFiber(). It 
 *  is possible to obtain the exit code for immortal Fibers via the 
 *  #AmFiberGetState() function before it is recycled. 
 *
 *  Immortal Fibers can be restarted after they have been stopped. An immortal 
 *  Fiber will keep its hold on the stack memory provided to the Fiber Pool 
 *  when it was created. For security reasons, the stack memory will be cleared 
 *  upon restart of the immortal Fiber. 
 * 
 *  Restarting an immortal Fiber may provide performance benefits over using 
 *  normal Fibers. A negative aspect of immortal Fibers is that Fiber Pool and 
 *  stack resources allocated to the Fiber are locked and unused until the 
 *  immortal Fiber is recycled via #AmFiberPoolRecycleFiber() or restarted via 
 *  #AmFiberStart().
 * 
 *  For details of state transitions of normal and immortal Fibers, see 
 *  the @ref page_state_transition "Fiber state transition" 
 *  documentation.
 *
 *  A pointer to a valid and complete #JKT_Fiber_Stack object must be specified 
 *  in the structure referenced by <i>psStack</i>. A valid stack size between 
 *  #JK_FIBERPOOL_FIBERSTACK_MIN and #JK_FIBERPOOL_FIBERSTACK_MAX inclusive 
 *  must also be specified.
 *
 *  The Fiber object handle returned by this function may be used by the 
 *  @ref Fiber_JBI API to start and manage the Fiber execution life cycle.
 * 
 *  Fibers can use this function to create new Fibers.
 * 
 *  @par Note:
 *  The memory referenced by the <i>m_pvStack</i> pointer in the 
 *  #JKT_Fiber_Stack object must remain valid until the Fiber object is 
 *  recycled via the #AmFiberPoolRecycleFiber() function. During the Fiber 
 *  recycling process, the associated <i>m_pfStackFree</i> function will be 
 *  used to release the stack memory. The value of <i>m_pfStackFree</i> 
 *  must not be set to JK_NULL. If set to JK_NULL, the function will 
 *  return #JK_RESULT_FIBER_BAD_PARAM.
 * 
 *  @par Invocation:
 *  This function may be called from a Fiber, thread or process context.
 *  See the Fiber Service @ref 
 *  section_invocation_conditions "invocation rules" for details.
 * 
 *  @see
 *  AmFiberPoolRecycleFiber, AmFiberGetState, AmFiberStart, 
 *  AmFiberJoin
 * 
 */

long AmFiberPoolCreateFiber( JKFiberPoolHandle hFiberPool, unsigned long ulFlags, JKT_Fiber_Stack *psStack, JKFiberHandle *phFiber );

/**
 *  Recycles a Fiber within the Fiber Pool.
 * 
 *  @param hFiberPool
 *   A valid Fiber Pool object handle.
 *  @param hFiber
 *   A valid Fiber object handle.
 * 
 *  @return
 *   - #JK_RESULT_FIBER_OK<br>
 *     if the function succeeds.
 *   - #JK_RESULT_FIBER_BAD_PARAM<br>
 *     when an invalid parameter is supplied.
 *   - #JK_RESULT_FIBER_BAD_STATE<br>
 *     when called on a Fiber that cannot be recycled.
 * 
 *  <p>
 *  This function recycles a @ref Fiber_JBI "Fiber" (as specified by 
 *  <i>hFiber</i>) or changes its lifecycle configuration depending on its 
 *  state.
 * 
 *  @par When the Fiber is in the JK_FIBER_STATE_READY state:
 * 
 *  @par
 *  Only Fibers in the @ref JK_FIBER_STATE_READY "ready" state will be 
 *  recycled and taken @ref JK_FIBER_STATE_OFFLINE "offline". This is done 
 *  within the @ref Fiber_Pool_JBI "Fiber Pool" specified by 
 *  <i>hFiberPool</i>. 
 * 
 *  @par
 *  During the recycling process, the Fiber's stack memory is released 
 *  using the memory deallocation function provided in the #JKT_Fiber_Stack 
 *  object when it was created. See the #AmFiberPoolCreateFiber() 
 *  function for details.
 * 
 *  @par
 *  Once a Fiber is recycled, its handle should no longer be used. 
 *  If another new Fiber handle is needed, it must be created using the 
 *  #AmFiberPoolCreateFiber() function.
 * 
 *  @par When the Fiber is in a state other than JK_FIBER_STATE_READY: 
 *
 *  @par
 *  A Fiber which is 
 *  @ref JK_FIBER_STATE_RUNNING "running", 
 *  @ref JK_FIBER_STATE_STARTED "started", 
 *  @ref JK_FIBER_STATE_STOPPED "stopped",
 *  @ref JK_FIBER_STATE_SUSPENDED "suspended", or
 *  @ref JK_FIBER_STATE_SLEEPING  "sleeping" will be downgraded from an @ref 
 *  JK_FIBER_LIFECYCLE_IMMORTAL "immortal" Fiber to a @ref 
 *  JK_FIBER_LIFECYCLE_NORMAL "normal" Fiber when specified in this 
 *  function call. See the #AmFiberPoolCreateFiber() function for more 
 *  details on immortal and normal Fibers.
 *
 *  Fibers may use this function to recycle or reconfigure other Fibers' 
 *  lifecycles or simply change their own lifecycle configuration.
 * 
 *  @par Invocation:
 *  This function may be called from a Fiber, thread or process context.
 *  See the Fiber Service @ref 
 *  section_invocation_conditions "invocation rules" for details.
 *
 *  @see
 *  AmFiberPoolCreateFiber
 *
 */

long AmFiberPoolRecycleFiber( JKFiberPoolHandle hFiberPool, JKFiberHandle hFiber );

/**
 *  Gets a time tick sample.
 * 
 *  @param pulTickHigh
 *    A pointer to memory to store the top 32-bits of a time sample.
 *  @param pulTickLow
 *    A pointer to memory to store the bottom 32-bits of a time sample.
 * 
 *  <p>
 *  Sample the system clock and return the current tick count in memory 
 *  referenced by <i>pulTickHigh</i> (for the upper 32-bits) and 
 *  <i>pulTickLow</i> (for the lower 32-bits). The values of 
 *  <i>pulTickHigh</i> and <i>pulTickLow</i> will never be JK_NULL.
 * 
 *  @par Note:
 *  This function should be thread-safe.
 * 
 *  @see
 *  AmFiberPoolCreateFiberScheduler
 * 
 */

typedef void ( * JK_FIBER_TICKER ) ( unsigned long *pulTickHigh, unsigned long *pulTickLow );

/**
 *  Creates a Fiber Scheduler in the Fiber Pool.
 * 
 *  @param hFiberPool
 *   A valid Fiber Pool object handle.
 *  @param pfTicker
 *   A pointer to a ticker function.
 *  @param phFiberScheduler
 *   Upon function success, a valid #JKFiberSchedulerHandle is returned.<br>
 *   Upon function failure, the contents of this parameter remain unchanged.<br>
 * 
 *  @return
 *   - #JK_RESULT_FIBER_OK<br>
 *     if the function succeeds.
 *   - #JK_RESULT_FIBER_BAD_PARAM<br>
 *     when an invalid parameter is supplied.
 *   - #JK_RESULT_FIBER_NO_MEMORY<br>
 *     when more than one Fiber Scheduler is being allocated from the Fiber 
 *     Pool.
 * 
 *  <p>
 *  This function allocates a @ref Fiber_Scheduler_JBI "Fiber Scheduler" from 
 *  the @ref Fiber_Pool_JBI "Fiber Pool" specified by <i>hFiberPool</i> and returns the 
 *  handle to it in the memory referenced by <i>phFiberScheduler</i>.
 * 
 *  Only one Fiber Scheduler object may exist for a Fiber Pool.
 * 
 *  The Fiber Scheduler is used to schedule and run @ref Fiber_JBI "Fibers" in 
 *  the Fiber Pool. Without the scheduler, the Fibers will not execute instructions.
 * 
 *  The <i>pfTicker</i> function is used by the Fiber Scheduler to measure time 
 *  during various Fiber scheduling activities. While it is possible and 
 *  preferable to sample time in 64-bit snapshots, providing 32-bit samples 
 *  (via memory referenced by <i>pulTickLow</i>) is also possible. In 
 *  such cases, be sure to set the memory referenced by <i>pulTickHigh</i> to 
 *  <code>0x00000000</code> and ensure that the function consistently operates 
 *  in 32-bit sampling mode.
 * 
 *  @par Important:
 *  The granularity and accuracy of the <i>pfTicker</i> function is critical to 
 *  proper scheduling and responsiveness by the components in the Fiber Pool. 
 *  Highly accurate samples with at least 5ms granularity is desired.
 * 
 *  @par Note:
 *  If possible, the initial call to the <i>pfTicker</i> function should yield 
 *  a time sample close to zero. This is especially applicable when only 32-bit 
 *  samples are used.
 *
 *  @par Invocation:
 *  This function may be called from a Fiber, thread or process context.
 *  See the Fiber Service @ref 
 *  section_invocation_conditions "invocation rules" for details.
 *
 */

long AmFiberPoolCreateFiberScheduler( JKFiberPoolHandle hFiberPool, JK_FIBER_TICKER pfTicker, JKFiberSchedulerHandle *phFiberScheduler );

/**
 * @}
 */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __AM_FIBERPOOL_H */
