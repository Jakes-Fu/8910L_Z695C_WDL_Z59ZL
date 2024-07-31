/*
 * Copyright 2006-2007 Esmertec AG. All Rights Reserved.
 * $Id$
 */

#ifndef JBED_UPCALL_H
#define JBED_UPCALL_H

#if defined(__cplusplus)
extern "C" {
#endif

/**
 * @file
 * Jbed Porting Layer: Upcall support.
 */

/**
 * Constants to use when calculating queue size. All specified in words.
 */

/** Minimum size of an event. */
#define JBED_UPCALL_MINEVENTSIZE 4

/** Size of object headers */
#define JBED_UPCALL_OBJECTHEADERSIZE 1

/** Extra spare space needed in the queue */
#define JBED_UPCALL_SPAREQUEUESPACE 1

/** Calculates the required queue space in words for a string of a given
  * number of characters.
  */
#define JBED_UPCALL_STRINGSIZE(length) (((length)+1)/2)

/**
 * Structure used to represent an upcall event queue.
 */
typedef struct Jbed_upcall_queue_s Jbed_upcall_queue;

/**
 * Create a new upcall event queue.
 * The queue is added to the list of queues such that the queue is sorted by
 * priority.
 * When calculating the size required for a queue, note that a simple event
 * requires #JBED_UPCALL_MINEVENTSIZE words. A complex event requires
 * #JBED_UPCALL_MINEVENTSIZE words plus #JBED_UPCALL_OBJECTHEADERSIZE words per
 * object in the event plus enough space for the object's data. The
 * #JBED_UPCALL_STRINGSIZE macro can be used to calculate the size of strings.
 * The available queue size is always #JBED_UPCALL_SPAREQUEUESPACE words less
 * than allocated.
 *
 * @param words     The size in words (32bit ints) of the queue.
 * @param priority  The priority of the queue. Higher numbered queues are
 *                  processed first.
 * @return A pointer to the new queue or NULL on memory allocation failure.
 */
Jbed_upcall_queue *Jbed_upcall_createQueue(int32_t words, int32_t priority);

/**
 * Delete a queue previously allocated by Jbed_upcall_createQueue().
 *
 * @param queue  The queue to delete.
 */
void Jbed_upcall_deleteQueue(Jbed_upcall_queue *queue);

/**
 * Send a simple upcall event with just a simple integer payload.
 *
 * @param queue      The queue to put the event in.
 * @param isolateId  The id of the isolate to send the event to.
 *                   #JBED_ISOLATE_ALL broadcasts the event to all isolates.
 * @param eventId    The event id.
 * @param payload    The integer payload to send with the event.
 * @return JBNI_TRUE on success or JBNI_FALSE if the queue is full.
 */
bool_t Jbed_upcall_sendSimple(Jbed_upcall_queue *queue, int32_t isolateId,
        int32_t eventId, int32_t payload);

/**
 * Send a complex upcall event with integer, array and string payload.
 * The payload is specified by the format string which must consist of a
 * combination of three character:
 *
 * 'i' indicates a simple integer. The corresponding argument should be
 * an int32_t.
 * 'I' indicates an array of integers. There should be two corresponding
 * arguments: firstly an int32_t* pointing to the array of integers, followed
 * by an int32_t specifying the size of the array.
 * 's' indicates a string. The corresponding argument should be a uint16_t *
 * pointing to the string, followed by an int32_t specifying the number of
 * characters in the string.
 * 'a' indicates a NUL terminated ascii string. The corresponding argument
 * is simply a pointer to the string.
 *
 * When the upcall is passed to java, the object payload will be one of
 * three types:
 * If there is just a single integer or an integer array, java will be
 * passed an integer array.
 * If there is just a single string then java will be passed a java string.
 * Otherwise java will be passed a reference array consisting of int arrays
 * and strings. Single integers are converted to single element integer
 * arrays.
 *
 * e.g.
 *
 * @code
 *
 * int32_t myarray[4] = { 1, 2, 3, 4 };
 * uint16_t str1[] = { 'f', 'o', 'o' };
 * Jbed_upcall_sendSimple(queue, 4, 87, 98, "iIsia", 42, myarray, 4,
 *                        str1, 3, 66, "bar");
 * @endcode
 *
 * Will send an upcall event to isolate 4 with eventId 87, integer payload of
 * 98 and an object payload consisting of a reference array containing:
 * A single element int array containing 42.
 * A four element int array containing { 1, 2, 3, 4 }
 * The java string "foo".
 * A single element int array containing 66.
 * The java string "bar".
 *
 * @param queue      The queue to put the event in.
 * @param isolateId  The id of the isolate to send the event to.
 *                   #JBED_ISOLATE_ALL broadcasts the event to all isolates.
 * @param eventId    The event id.
 * @param intPayload The integer payload.
 * @param format     String describing subsequent arguments.
 * @return JBNI_TRUE on success or JBNI_FALSE if the queue is full.
 */
bool_t Jbed_upcall_sendComplex(Jbed_upcall_queue *queue, int32_t isolateId,
        int32_t eventId, int32_t intPayload, char *format, ...);

/**
 * Called from the java main loop to poll the upcall event queues and pass
 * upcalls to java.
 *
 * If upcalls cannot be sent due to out of memory when trying to allocate
 * objects with upcall events will be left in the queue and processed the
 * next time around.
 *
 * @return JBNI_TRUE if all pending upcalls were processed or JBNI_FALSE
 *         if one or more upcalls could not be processed due to object
 *         allocation failure.
 */
bool_t Jbed_upcall_poll(void);

/**
 * Called from the java main loop to check if some event queue is populated.
 *
 * @return JBNI_TRUE if at least one event queue is not empty
 */
bool_t Jbed_upcall_check(void);


#ifdef JVM_FDAC
/**
 * Called during VM startup to initialise the upcall module
 */
void Jbed_upcall_init(void);
#endif


#if defined(__cplusplus)
}
#endif

#endif /* JBED_UPCALL_H */

