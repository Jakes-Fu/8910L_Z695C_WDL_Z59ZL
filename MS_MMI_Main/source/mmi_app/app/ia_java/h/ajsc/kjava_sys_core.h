/*
 * Copyright 2002-2003 Aplix Corporation. All rights reserved.
 */

#ifndef KJAVA_SYS_CORE_HEADER
#define KJAVA_SYS_CORE_HEADER

/**
 * @file kjava_sys_core.h
 * KJAVA Core System KSI
 */

#ifdef __cplusplus
extern "C"{
#endif

/** @addtogroup core_system_service_ksi
 *  @{
 */

/*
 *
 *  Interfaces listed in the group are used for:
 *      - huge memory allocation / release
 *      - semaphore operation
 *      - system property / option acquisition
 *      - time information
 *      - external application control
 *      - platform thread state acquisition
 *      - section information acquisition
 */

/*-----------------------------------------------------------------------------
 * Marcos, Constants, Types, and Structures
 *----------------------------------------------------------------------------*/

/** 
 * @addtogroup Event_Synchronization_KSI Syncrhonization Event Service
 * @{
 */

/** \brief A constant indicating an event was set. */
#define KJAVA_EVENT_OK            (0)

/** \brief A constant indicating no event was set.*/
#define KJAVA_EVENT_TIMEOUT       (-1)

/** @} */

/** 
 * @addtogroup External_Application_Control_KSI  External Application Control
 * @{
 */

/**
 * \brief The scheme of the URL to which connection was requested is not
 *        supported.
 */
#define KJAVA_EXTAPP_UNSUPPORTED_CONNECTION        0

/**
 * \brief The URL connection request was accepted. The application is started
 *        immediately in a thread context separate from the KVE thread.
 */
#define KJAVA_EXTAPP_CONNECT_IMMEDIATE             1

/**
 * \brief The URL connection request was accepted. The application is started
 *        immediately after the Java execution environment terminates.
 */
#define KJAVA_EXTAPP_CONNECT_LATER                2

/** @} */

/** 
 * @addtogroup Memory_Management_KSI Memory Management
 * @{
 */

#define KJAVA_INSTALL_BLOCK_SIZE     (100*1024)
/**
 *  \brief data structure for huge memory buffers' allocation
 */
typedef struct _KJavaHugeMemoryTag
{
    /** \brief The pointer to the Java heap area. */
    char*   firstBlock;

    /** \brief The size of firstBlock. */
    int     firstBlockSize;

    /** \brief The pointer to the memory used for native resource allocation. */
    char*   secondBlock;

    /** \brief The size of secondBlock. */
    int     secondBlockSize;
}
KJavaHugeMemory;

/** @} */


/*-----------------------------------------------------------------------------
 * Global Function(s) Prototype
 *----------------------------------------------------------------------------*/

/** 
 * @addtogroup Core_System_InitializationFinalization_KSI Service Initialization and Finalization
 * @{
 */

/** \brief Initializes the CORE subsystem.
 *
 * This function is called in the KVE thread context.
 */
void kjava_core_fini(void);


/** \brief Finalizes the CORE subsystem.
 *
 * This function is called in the KVE thread context.
 */
void kjava_core_init(void);

/** @} */

/** 
 * @addtogroup Memory_Management_KSI
 * @{
 */

/**
 * \brief Allocates two huge memory blocks.
 * @param kjavaHugeMemory_p
 *        A pointer to the #KJavaHugeMemory structure for putting 
 *        the information of memory blocks.
 * @return 
 *        Return 1 if the function succeeds. <br>
 *        Return 0 if it fails. 
 *
 * Allocate two contiguous memory block for Java heap and native heap and 
 * store the information of each heap in the following members of the structure indicated by 
 * <i>kjavaHugeMemory_p</i>.
 * <dl>
 *  <dt><b>Java heap</b></dt>
 *   <dd><i>firstBlock</i>, <i>firstBlockSize</i></dd>
 *  <dt><b>Native heap</b></dt>
 *   <dd><i>secondBlock</i>, <i>secondBlockSize</i></dd>
 * </dl>
 * The size of the Java heap must be up to 16 megabytes.
 *
 * The memory blocks allocated in this function must remain valid until 
 * #kjava_core_freeHugeMemory is called.
 *
 * This function is called when:
 *         - Immediately before VM starts.
 *         - Immediately before intsllation procedure starts.
 *         - Immediately before network connection without start VM, such as TCK.
 *         .
 */
int kjava_core_allocateHugeMemory(KJavaHugeMemory* kjavaHugeMemory_p,int actionType);


/**
 * \brief Releases the allocated memory blocks.
 * @param kjavaHugeMemory_p
 *        A pointer to the #KJavaHugeMemory structure holding
 *        the information of memory blocks.
 *
 * This function is called when:
 *         - Immediately after VM stops.
 *         - Immediately after intsllation procedure stops.
 *         - Immediately after network disconnection without start VM, such as TCK.
 */
void kjava_core_freeHugeMemory(KJavaHugeMemory* kjavaHugeMemory_p,int actionType);

/** @} */
/** 
 * @addtogroup Task_Mutual_Exclusion_KSI Semaphore Service
 * @{
 */

/**
 * \brief Gets a semaphore. 
 *
 * This function is used by JBlendia for mutual exclusion control.
 *
 * When this function is called, get a semaphore.
 * The type of semaphore used should be a binary semaphore, namely, one that only
 * the task that acquired it can execute. Accordingly, if a semaphore has already
 * been acquired when this function is called, lock the CPU context that called
 * this function inside the function. Then at the time #kjava_core_releaseSemaphore
 * is called, release the locked CPU context and return to the caller of
 * #kjava_core_setSemaphore.
 *
 * #kjava_core_releaseSemaphore is called for as many times as this function was
 * called, requesting release of the semaphore.
 *
 * The semaphore used by this function must be created ahead of time, before the
 * Java execution environment is started.
 * In the initial state when a semaphore is created, no owner should exist.
 *
 * @param sem_id The identifier of the semaphore. There are two possible values:
 *               - 1 The semaphore request from VM core. 
 *               - 2 The semaphore request during network.
 *
 * @see kjava_core_releaseSemaphore
 */
void kjava_core_setSemaphore(int sem_id);

/**
 * \brief Releases a semaphore.
 *
 * This function is used by JBlendia for mutual exclusion control.
 *
 * Release the semaphore acquired by #kjava_core_setSemaphore.
 * If #kjava_core_setSemaphore is called a second time before this function is called,
 * lock the calling CPU context.
 *
 * @param sem_id The semaphore identifier that given by #kjava_core_setSemaphore.
 *
 * @see kjava_core_setSemaphore
 */
void kjava_core_releaseSemaphore(int sem_id);

/** @} */
/** 
 * @addtogroup Property_Acquisition_KSI Additional Java Property and Customized String
 * @{
 */

/**
 * \brief Gets a system property to be returned by the <i>java.lang.System.getProperty</i> method.
 * @param key       
 *        A pointer to the buffer holding the null-terminated ASCII string used
 *        as key for obtaining the property.
 * @return 
 *        Return a pointer to the buffer holding the null-terminated ASCII property string 
 *        indicated by the <i>key</i> parameter. 
 *        In any of the following cases, return NULL.
 *        - The designated key does not exist.
 *        - The property does not need to be changed from the default value that JBlendia maintains.
 *        - No property is present for the designated key.
 *        .
 *
 * The string returned by this function should remain valid until the JVM terminates.
 * 
 * For details of the property to be returned by this function, see specifications 
 * of the configuration, profiles, and optional packages that are implemented in 
 * the native system. 
 *
 * This function is called also for JBlendia private properties. 
 * Return NULL if the designated property name is not defined in 
 * the specifications of configuration, profiles, and optional packages 
 * that are supported by the native system. 
 */
const char* kjava_core_getProperty(const char *key);

/**
 * \brief  Gets a JBlendia customization string.
 * @param key       
 *        A pointer to the buffer holding the null-terminated ASCII string used
 *        as key for obtaining the property.
 * @return 
 *        Return a pointer to the buffer holding the null-terminated ASCII property string 
 *        indicated by the <i>key</i> parameter. 
 *        In any of the following cases, return NULL.
 *        - Designated key does not exist.
 *        - The property does not need to be changed from the default value that JBlendia maintains.
 *        - No property is present for the designated key.
 *        .
 *
 * The string returned by this function should remain valid until the JVM terminates.
 *
 * Details of the property to be returned by this function are given in 
 * the documentation describing JBlendia specifications corresponding to 
 * the profiles or optional packages that are implemented in the native system.
 * 
 * This function is called also for JBlendia private properties. 
 * Return NULL if the designated property name is not defined in 
 * the specifications of configuration, profiles, and optional packages 
 * that are supported by the native system. 
 *
 * <b>See Also</b>: \ref page_core_customization
 */
const char* kjava_core_getOption(const char *key);

/** @} */
/** 
 * @addtogroup Time_Information_KSI System Time Service
 * @{
 */

/**
 * \brief Gets the current time.
 * @param high  
 *        A pointer to the memory area for storing the higher 32 bits.
 * @param low   
 *        A pointer to the memory area for storing the lower 32 bits.
 *
 * Store the current time (the elapsed time from 00:00:00.000, January 1, 1970
 * of Greenwich mean time (GMT)) in millisecond as a 64-bit value to the
 * arguments high and low. It is not required to compensate for summer
 * time (daylight saving time).
 *
 * Put 0, when the current time of the system has not been set. In case the
 * current time of the system is changed during JVM suspension, put the
 * changed value after JVM is resumed.
 *
 * Since the accuracy of the current time acquired by this function
 * influences the accuracy of the value obtained by
 * java.lang.System.currentTimeMills method, this value should be acquired
 * with the highest possible accuracy. It is recommended that the resolution
 * ranges within 100 milliseconds.
 *
 */
void kjava_core_getCurrentTime(unsigned long *high, unsigned long *low);

/**
 * \brief Gets the elapsed time.
 * @param high  
 *        A pointer to the memory area for storing the higher 32 bits.
 * @param low   
 *        A pointer to the memory area for storing the lower 32 bits.
 *
 * Store the elapsed time (monotonic increasing value) in millisecond as
 * a 64-bit value to the arguments high and low. It is recommended that
 * the elapsed time starts from 0. The JVM presupposes that the value
 * obtained by this function increases monotonically.
 *
 * In case of JVM suspension, the elapsed time may stop increasing.
 * However, when JVM restarts, the elapsed time needs to be updated as
 * if it were continuously increased during the JVM suspension.
 *
 * The JVM controls the thread using the value returned by this function.
 * Since the accuracy of the elapsed time influences the accuracy of JVM
 * execution directly, this value should be acquired with the highest
 * possible accuracy. It is recommended that the accuracy ranges within
 * 10 milliseconds.
 *
 *  @par For compliance with JTWI 1.0:
 *  The elapsed time returned by this function must have a resolution of 40 ms or better. 
 */
void kjava_core_getCurrentTick(unsigned long *high, unsigned long *low);

/** @} */
/** 
 * @addtogroup External_Application_Control_KSI
 * @{
 */

/**
 * \brief Requests a connection to the designated URL.
 * @param url  
 *        The destination URL.
 * @return 
 *        Return one of the following.
 *          - #KJAVA_EXTAPP_UNSUPPORTED_CONNECTION
 *          - #KJAVA_EXTAPP_CONNECT_IMMEDIATE
 *          - #KJAVA_EXTAPP_CONNECT_LATER
 *          .
 *
 * Execute a native application for making connection to the URL designated in 
 * the <i>url</i> parameter.
 * This function corresponds to the <code>javax.microedition.midlet.MIDlet
 * </code> class
 * <code>platformRequest</code> method.
 * Of the processings to be performed in the <code>platformRequest</code> method, 
 * the ones mandated by the MIDP 2.0 specification are as follows.
 * - If the URL specified refers to a MIDlet suite (either a JAD file or a JAR file), 
 *   the native application handling the request MUST interpret this as a request 
 *   for MIDlet installation.
 * - If the URL specified is of the form tel:number, as specified in RFC2806, 
 *   then the native application handling the request MUST interpret this as a 
 *   request to initiate a voice call. 
 * .
 * For details of processing for the URL given in the <i>url</i> parameter,
 * see the explanations of the <code>javax.microedition.midlet.MIDlet</code> 
 * class <code>platformRequest</code> method in the MIDP 2.0 specification.
 * 
 * Implement so that when this function is called, the native application 
 * for connecting to the designated URL does one of the following:
 * 1) Runs immediately, 2) Runs after the MIDlet exits, or 3) Does not run.
 * These three options are explained below.
 * 
 * <ol>
 * <li>Run immediately. <br>
 *   If the following conditions are met, start the application for URL connection, running it
 *   in a task separate from the JVM task and displaying it in the foreground.
 *   <ul>
 *   <li> The designated URL scheme is supported.
 *   <li> The designated application can be run without terminating the JVM.
 *        Running the designated application concurrently with the JVM is 
 *        the basic implementation that conforms to the MIDP specification.
 *        As a special measure, however, performing the operation in JVM suspended 
 *        state may be allowed. This implementation is also in this category.
 *   </ul>
 *   In this case, return #KJAVA_EXTAPP_CONNECT_IMMEDIATE.
 *   Implement this function only to request starting of the native application,
 *   returning control immediately to the caller.
 *   Do not employ an implementation that launches the application in this function
 *   and does not return to the caller of this function until the application terminates.
 *   </p><p>
 *   Consult Sun or the communication operator about whether or not performing 
 *   the processing in JVM suspended state is approved.
 *   </p><p>
 * Even after #KJAVA_EXTAPP_CONNECT_IMMEDIATE is returned, the JVM task continues
 * to function normally.
 * 
 * <li> Run after the MIDlet exits. <br>
 *   Even though the designated URL scheme is supported,
 *   if the application to be newly launched cannnot run unless the JVM task is
 *   suspended first due to a conflict for screen or resources between the 
 *   JVM task and the application, start the native application as soon as 
 *   the MIDlet terminates, and connect to the designated URL. 
 *   </p><p>
 *   In this case, copy the passed destination URL string to a native system memory
 *   area and return #KJAVA_EXTAPP_CONNECT_LATER.
 *   Discard any previously passed, unprocessed URL, saving only the last passed URL. 
 *   <p>
 *   When this function returns #KJAVA_EXTAPP_CONNECT_LATER, the 
 *   <code>platformRequest</code> method returns "true" to let the MIDlet know
 *   that it must exit so a native application can be executed. 
 *   Whether the MIDlet actually exits as a result is dependent on the MIDlet.
 * 
 * <li> Do not run. <br>
 *   Return #KJAVA_EXTAPP_UNSUPPORTED_CONNECTION in either of the following cases.
 *   <ul>
 *   <li> The designated URL scheme is not supported.
 *   <li> The designated string is not a proper URL string. 
 *   </ul>
 * </ol>
 * 
 * When #KJAVA_EXTAPP_CONNECT_IMMEDIATE is returned and an application launched,
 * or when #KJAVA_EXTAPP_UNSUPPORTED_CONNECTION is returned, discard the
 * requested destination URL remembered from the previous time this function
 * was called.
 * 
 * When #KJAVA_EXTAPP_CONNECT_LATER is returned, depending on the MIDlet it is possible
 * that this function will be called again after the function returns to its caller
 * and before the MIDlet exits. In such a case, rather than launching a native 
 * application successively for each designated URL, start a native application only
 * for the last designated URL.
 * 
 * When NULL was designated as the <i>url</i> parameter, discard any requested
 * destination URL remembered from the previous time this function was called, 
 * and return #KJAVA_EXTAPP_CONNECT_IMMEDIATE.
 * 
 * If a native application launched by this function executes features that may incur
 * charges, such as voice callout or HTTP communication, user confirmation must first
 * be obtained by a dialog display or other means.
 * This confirmation is similar to the MIDP 2.0 permission confirmation 
 * functionality because both of them check with the user for confirmation 
 * before allowing the use of the feature. 
 * However, the confirmation must be implemented by a means other than the 
 * MIDP 2.0 security framework.
 * 
 * The MIDP 2.0 states the following specification regarding the confirmation.
 * - The confirmation must be implemented by means other than the MIDP 2.0 security framework.
 * - The confirmation detail is implementation-dependent.
 * .
 * 
 * The device manufacturer needs to implement an original user confirmation feature 
 * based on the specification of the <code>platformRequest</code> method.
 * 
 *
 */
int kjava_core_requestConnection(const char *url);

/** @} */
/** 
 * @addtogroup Event_Synchronization_KSI
 * @{
 */

/**
 * \brief Waits for an event for a certain period.
 * @param msec Timeout interval (in milliseconds) 
 * @return 
 *        If an event occurs during the wait time, return #KJAVA_EVENT_OK. 
 *        If the wait times out, return #KJAVA_EVENT_TIMEOUT.
 *
 * If no event is being held at the time this function is called, release the 
 * CPU execute privilege of the thread calling this function and enter 
 * waiting state in the function until a new event occurs or the function 
 * times out. 
 *
 * When a function is called for event notification by the native system to 
 * JBlendia, #kjava_core_notifyEvent is called. After #kjava_core_notifyEvent 
 * is called, grant the CPU execute privilege to the CPU context in waiting 
 * state while #kjava_core_waitEvent is executing, and resume execution. 
 * Return #KJAVA_EVENT_OK as the #kjava_core_waitEvent return value in this case. 
 *
 * If the function times out with no event occurring during the waiting state, 
 * acquire the CPU execute privilege and return #KJAVA_EVENT_TIMEOUT. 
 *
 * If #kjava_core_notifyEvent was called before this function and an event 
 * occurrence state is being held, return #KJAVA_EVENT_OK without entering 
 * waiting state. 
 *
 * In no case should the event occurrence state be cleared in this function.
 * It must be cleared only when #kjava_core_clearEvent is called.
 *
 * The device manufacturer must implement #kjava_core_waitEvent in accord with the 
 * timer resolution. Moreover, the timeout duration when #kjava_core_waitEvent 
 * times out must be no longer than the timeout duration passed in a parameter. 
 * Exceeding the designated time will lower the precision of the 
 * <code>java.lang.Thread.sleep</code> method. 
 * Aplix recommends implementing as follows.
 * - Make the timeout resolution of #kjava_core_waitEvent the same as the 
 *   #kjava_core_getCurrentTick resolution.
 * - Adjust the #kjava_core_waitEvent timeout duration based on the 
 *   equation below, where <i>msec</i> is the value designated in the <i>msec</i>
 *   parameter of #kjava_core_waitEvent. 
 * .
 * <pre>
 * Actual timeout duration 
 *       = <i>msec</i> - (<i>msec</i> % <i>kjava_core_waitEvent()</i> timeout resolution) 
 * </pre>
 * If, for example, the time resolution returned by <i>kjava_core_waitEvent</i> 
 * is 40 ms, <i>kjava_core_waitEvent(50)</i> times out in 40 ms and 
 * <i>kjava_core_waitEvent(30)</i> times out in 0 ms.
 *
 * @if JTWI
 * @par For compliance with JTWI 1.0:
 *      The timeout interval as set in the timeout parameter must be detected with a 
 *      resolution of 40 ms or better.
 * @endif
 *
 * @see kjava_core_notifyEvent
 */
int kjava_core_waitEvent(unsigned int msec);


/**
 * \brief Sets an event.
 *
 * If this function is called when a thread is waiting in the  
 * #kjava_core_waitEvent function, grant the CPU execute privilege 
 * to the thread and return control to the caller of #kjava_core_waitEvent. 
 *
 * In no case should the event occurrence state be cleared in this function.
 * It must be cleared only when #kjava_core_clearEvent is called.
 *
 * If this function is called when any thread is not in 
 * waiting state inside the #kjava_core_waitEvent function, hold the 
 * event occurrence state. 
 * Then even if #kjava_core_waitEvent is called thereafter, return 
 * #KJAVA_EVENT_OK without going to waiting state. 
 *
 * @see kjava_core_waitEvent and kjava_core_clearEvent
 */
void kjava_core_notifyEvent(void);

/**
 * \brief Resets the event.
 *
 * Reset the event set by #kjava_core_notifyEvent.
 *
 * When no event has occurred, so long as no new event is notified by 
 * #kjava_core_notifyEvent, the next time #kjava_core_waitEvent is 
 * called go to event waiting state. 
 *
 * @see kjava_core_waitEvent, kjava_core_notifyEvent
 */
void kjava_core_clearEvent(void);

/** @} */

#ifdef __cplusplus
}
#endif

#endif /* KJAVA_SYS_CORE_HEADER */
