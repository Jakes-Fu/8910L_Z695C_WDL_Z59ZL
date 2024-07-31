/*
 * Copyright 1998-2007 Esmertec AG. All Rights Reserved.
 * $Id$
 */

/**
 * @file
 * Jbed Porting Layer: VM Life-Cycle Support (FastBCC only)
 *
 * In JbedME releases before 4.1, the VM life-cycle had to be controlled at a
 * low level by the port-specific code, by calling Jbed_initialize(),
 * Jbed_iterate() and Jbed_shutdown() defined in jbed_opl.h.  Since release
 * 4.1, this task is performed by the module documented here.
 *
 * The Jbed_run() function is a wrapper around the Jbed_initialize(),
 * Jbed_iterate() and Jbed_shutdown() functions.  Jbed_run() should be used
 * instead of calling the latter functions directly, since it takes care of the
 * interaction with the AMS and applications and allows correct handling of
 * events like incoming phone calls.
 *
 * <b>Definition</b>: The "Jbed task" is the OS task that executes the
 * Jbed_run() function.
 *
 * All Java threads run as part of the Jbed task.  The context switches between
 * Java threads occur internally in the VM.  There will always be a context
 * switch back to the main Java thread before Jbed_run() returns.
 *
 * The porting layer can request VM state transitions with Jbed_requestState().
 * State transitions can also occur independent of Jbed_requestState() in
 * response to MIDlet or AMS actions.  The VM reports actual state transitions
 * to the porting layer with the CPL_vmStateNotifier_t instance.
 *
 * Note that these states have nothing to do with the idleness of the CPU.
 * E.g., state #JBED_MAIN_IDLE does not imply that the CPU is idle, and state
 * #JBED_MAIN_ACTIVE does not imply that the CPU is active.  That is indicated
 * by the return value of Jbed_run().
 *
 * The associated JbedLinker map file is <code>OPL_main_Map.txt</code>.
 */

#ifndef __JBED_OPL_MAIN_H__
#define __JBED_OPL_MAIN_H__

#include <jbed_jbni.h>

/**
 * @name VM State Values
 *
 * These are the possible states the VM can be in.
 */

/*@{*/

/**
 * The VM has not been initialized, or it has terminated.
 */
#define JBED_MAIN_DOWN              ((jint)0)

/**
 * The VM and AMS have been started, or are in the process of starting up, but
 * no application is running yet.
 */
#define JBED_MAIN_IDLE              ((jint)1)

/**
 * An application is active in the background and has no access to the display
 * and events.
 */
#define JBED_MAIN_ACTIVE            ((jint)2)

/**
 * An application is active in the foreground and has access to the display and
 * events.
 */
#define JBED_MAIN_ACTIVE_FOREGROUND ((jint)3)

/*@}*/

#if defined(__cplusplus)
extern "C" {
#endif

/*
 * ----------- Types -----------
 */

/**
 * Callback type for state transitions.
 *
 * @param id The ID of the isolate changing state. Always 0 for
 * non MVM builds.
 * @param commit When #JBNI_FALSE, the transition is just beginning,
 * when #JBNI_TRUE, the transition has just completed.
 * @param oldState The state being transitioned from.
 * @param newState The state being transitioned to.
 * @param arg A profile specific argument to the state transition.
 * @param curReason The platform-specific reason reference most recently
 * set by Jbed_requestState()
 *
 * @return When commit is #JBNI_FALSE, this return value is ignored. When commit
 * is #JBNI_TRUE, returning #JBNI_TRUE here will force the VM to immediately
 * break out of the Jbed_run() function.
 */
typedef jboolean (*CPL_vmStateNotifier_t)(jint id, jboolean commit,
        jint oldState, jint newState, jint arg, void *curReason);

/**
 * Structure to represent the VM state.
 */
typedef struct _vmInstance
{
    /**
     * Copies of Arguments.
     */
    jint argc;
    char **argv;

    /**
     * This is the current desired state. Only calls to
     * <code>Jbed_requestState</code> will modify this
     * value.
     */
    int desiredState;

    /**
     * This is the current optional platform specific data which can be
     * associated with any request to change state.
     *
     * It is entirely up to the platform what information is stored here.
     */
    void *curReason;

    /**
     * This is the current state of the VM. Only calls to
     * the <code>OPL_main_finishStateTransition</code> method
     * will modify this value, and only the Jbed Task
     * can call this.
     */
    int curState;

    /**
     * The next state of the VM. Will only be different from #curState
     * when in the middle of a state transition.
     * The OPL_main_startStateTransition() method will modify this
     * value, only from the Jbed Task.
     */
    int nextState;

    /**
     * A reference to the Jbed <code>Signal</code> object which must be notified in
     * order to wake up the appropriate VM thread for making state transitions.
     */
    struct INSTANCE_com_jbed_runtime_Signal_s *signalPtr;

    /**
     * A reference to the natively registered callback method for
     * notification of state transitions.
     */
    CPL_vmStateNotifier_t notifier;
} vmInstance;


/*
 * ----------- Functions -----------
 */

/**
 * Request a VM state change.
 *
 * Should be called by porting code when a VM state change is desired, e.g., to
 * change the state to #JBED_MAIN_ACTIVE_FOREGROUND when the VM is started, or
 * to change the state to #JBED_MAIN_ACTIVE when an incoming phone calls occurs.
 *
 * @param newState is the desired state of the VM.
 *
 * @param reason is a pointer to arbitrary platform-specific data that will be
 * remembered and passed to the CPL_vmStateNotifier_t instance.
 *
 * @param oldReason is a reference parameter that returns the reason value that
 * was set previously.  If oldReason is NULL, nothing is returned.
 *
 * @return #JBNI_TRUE if the VM is already in the requested state and is not
 * currently in the process of changing state, #JBNI_FALSE otherwise.
 */
extern jboolean Jbed_requestState(jint newState, void *reason,
        void **oldReason);

/**
 * Entry point to give CPU time to Jbed.
 *
 * This function initializes and shuts down the VM.  If the VM is running, this
 * call will enter the Java thread scheduler, which will select the next Java
 * thread to run and start executing it.  Execution of Java threads continues
 * until the specified timeout is reached, or there are no ready threads
 * available.
 *
 * @param maxRun specifies the number of milliseconds Java threads are allowed
 * to run before this function will return.  The time base is defined by
 * OPL_os_getElapsedMicros() in jbed_os.h.  The timeout will only occur after
 * Jbed_timerEntry() is called next (unless the VM is configured for
 * non-preemptive scheduling).  The function can return earlier if all Java
 * threads are idle.
 * For JBED_MVM builds: Note the restriction, that the minimum number of
 * milliseconds the VM is allowed to run must not be smaller than the value
 * configured by com.jbed.runtime.MvmConfig.minSlice. Otherwise the VM would
 * not be able to schedule an isolate for that run.
 *
 * @return result code (a subset of the Jbed_iterate() return codes in
 * jbed_opl.h):
 *
 * #JBED_ITERATE_TIMEOUT indicates that Jbed_run() timed out, but at least one
 * Java thread is still ready to run.  The recommended response is to call
 * Jbed_run() again immediately (this case can often be treated the same as a 0
 * return value).
 *
 * A return value in the range #JBED_MIN_IDLE to #JBED_MAX_IDLE-1 indicates how
 * many milliseconds it will be before at least one Java thread is ready to run
 * again.  It is recommended that the Jbed task sleep for this long and only
 * then call Jbed_run() again, to save power (but see note below).  The task
 * should not sleep longer than this, since that will cause delays for Java
 * threads.
 *
 * #JBED_MAX_IDLE indicates that all Java threads are idle.  The native task can
 * sleep indefinitely before calling Jbed_run() (but see below).
 *
 * The Jbed_iterate() codes #JBED_SYSTEM_EXIT and #JBED_ITERATE_EXCEPTION are
 * handled internally by Jbed_run().
 *
 * For example, if 1500 is returned, the Jbed native task can sleep for 1.5
 * seconds before calling Jbed_run() again.  It is not an error to call
 * Jbed_run() earlier than this, since the scheduler will just recompute the
 * new idle time and return it.
 *
 * <b>IMPORTANT</b>: While the Jbed task is sleeping, it must sometimes be
 * woken due to events occurring in the native world.  For example, the state
 * of a Java thread can be changed from the native world with a Jbed_signal()
 * call or one of the event upcalls (Jbed_doKeyDown(), Jbed_doKeyUp(), etc.).
 * Similarly, calls to Jbed_requestState() should wake up the VM.  In all these
 * cases, the Jbed task must stop sleeping and call Jbed_run() again as soon as
 * possible to guarantee prompt response to the signal or event.  It is the
 * responsibility of the porting code to implement this.
 *
 * see Jbed_initialize()
 * see Jbed_iterate()
 * see Jbed_shutdown()
 * see com.jbed.runtime.MvmConfig.minSlice
 */
extern jint Jbed_run(jint maxRun);

/**
 * Initialize the fields needed for the lifecycle code to start the VM.  Must
 * be called before any other lifecycle method can be called.
 *
 * @param argc The number of arguments to pass to the VM
 * @param argv A pointer to an array of pointers to null terminated argument strings.
 * @param notifier A callback for notification of VM state changes.
 */
extern void Jbed_initVmLifeCycle(jint argc, char **argv,
        CPL_vmStateNotifier_t notifier);

/**
 * Register a callback for state transitions.
 *
 * Implementations can register an #OPL_mainStateNotifier_t callback
 * so appropriate native actions can be taken on each VM state transition.
 *
 * <b>Important</b>: Only one callback can be registered. Further calls to this
 * function will replace the old callback.
 */
extern void Jbed_setVmStateNotifier(CPL_vmStateNotifier_t cb);

/**
 * Replace the arguments of the VM (override arguments set by Jbed_initVmLifeCycle()).
 *
 * This will replace the arguments passed to the VM with new arguments.
 *
 * <b>Important</b>: Only the pointer is copied in this call. The
 * actual string data isn't copied until later, when the VM is
 * running. Memory allocated for argv, can only be freed when this
 * method is called again, or when the VM exits.
 */
extern void Jbed_setVmArguments(jint argc, char **argv);

#ifdef JBED_CONCURRENT_ISOLATES

/**
 * Start a new Isolate.
 *
 * @param argc The number of arguments.
 * @param argv The argument list.
 * @param pri The priority of the isolate.
 */
extern jint JBNI_IMPORT Jbed_newIsolate(jint argc, char **argv, jint pri);

#endif

#ifdef JBED_NATIVE_VMTEST
extern void Jbed_initVmTest();
extern jint Jbed_runVmTest(jint);
extern void Jbed_shutdownVmTest();
#endif

#if defined(__cplusplus)
}
#endif

#endif /* __JBED_OPL_MAIN_H__ */
