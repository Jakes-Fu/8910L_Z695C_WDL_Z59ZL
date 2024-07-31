#ifndef LISTBASE_H
#define LISTBASE_H

/*
 * Define list head structure
 */
typedef unsigned int size_t;
typedef struct _ListHead ListHead;
struct _ListHead {
  ListHead *prev,  *next;
};


/* Define macro functions */

/*
 * Define offsetof macro
 * Parameter:
 *    s: type
 *    m: member name in s
 */
#ifndef offsetof
#define offsetof(s, m)        ((size_t)&(((s *)0)->m))
#endif


/*
 * Get the container of the member
 * Parameter:
 *    p: member pointer
 *    s: type
 *    m: member name in s
 */
#define ContainerOf(p, s, m)  ((s *)((char *)p - offsetof(s, m)))


/*
 * Get the list entry
 * Parameter:
 *    p: member pointer
 *    s: type
 *    m: list node member name in s
 */
#define ListEntry(p, s, m)    ContainerOf(p, s, m)


/*
 * for each list node(forward)
 * Parameter:
 *    h: list head pointer
 *    p: s pointer
 *    s: type
 *    m: list node member name in s
 */
#define ForEachEntryFWD(h, p, s, m) \
   for (p = ListEntry((h)->next, s, m); \
        &(p->m) != (h); \
        p = ListEntry(p->m.next, s, m))


/*
 * for each list node(forward), safe
 *    h: list head pointer
 *    p: s pointer
 *    t: temp s pointer
 *    s: type
 *    m: list node member name in s
 */
#define ForEachSafeEntryFWD(h, p, t, s, m)   \
   for (p = ListEntry((h)->next, s, m), \
        t = ListEntry(p->m.next, s, m); \
        &(p->m) != (h); \
        p = t, \
        t = ListEntry(p->m.next, s, m))

/*
 * for each list node(reverse)
 *    h: list head pointer
 *    p: s pointer
 *    s: type
 *    m: list node member name in s
 */
#define ForEachEntryREV(h, p, s, m) \
   for (p = ListEntry((h)->prev, s, m); \
        &(p->m) != (h); \
        p = ListEntry(p->m.prev, s, m))

/*
 * for each list node(reverse), safe
 *    h: list head pointer
 *    p: s pointer
 *    t: temp s pointer
 *    s: type
 *    m: list node member name in s
 */
#define ForEachSafeEntryREV(h, p, t, s, m)   \
   for (p = ListEntry((h)->prev, s, m), \
        t = ListEntry(p->m.prev, s, m); \
        &(p->m) != (h); \
        p = t, \
        t = ListEntry(p->m.prev, s, m))


/* Define inline functions */

/*
 * Initial the list node
 */
static __inline void List_Init(ListHead *node) {
   node->next = node->prev = node;
}

/*
 * Check if the list is empty
 */
static __inline int List_IsEmpty(ListHead *head) {
   return head->next == head;
}

/*
 * Insert one node after specified node
 */
static __inline void List_InsertAfter(ListHead *before, ListHead *node) {
   node->next = before->next;
   node->prev = before;
   before->next->prev = node;
   before->next = node;
}

/*
 * Insert one node before specified node
 */
static __inline void List_InsertBefore(ListHead *after, ListHead *node) {
   node->prev = after->prev;
   node->next = after;
   after->prev->next = node;
   after->prev = node;
}

#define List_Add_Head(head, node)	List_InsertAfter(head, node)
#define List_Add_Tail(head, node)	List_InsertBefore(head, node)

/*
 * Remove node from the list
 */
static __inline void List_Remove(ListHead *node) {
   node->prev->next = node->next;
   node->next->prev = node->prev;
   
   List_Init(node);
}

#endif /* LISTBASE_H */
