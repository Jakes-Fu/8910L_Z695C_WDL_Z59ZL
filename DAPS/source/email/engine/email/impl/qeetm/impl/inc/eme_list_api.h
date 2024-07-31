/*=====================================================================================
FILE NAME   : eme_list_api.h
MODULE NAME : Email engine

    GENERAL DESCRIPTION
      Provide some api for managing list.
      
    TechFaith Software Confidential Proprietary
    Copyright (c) 2007 by TechFaith Software. All Rights Reserved.
    =======================================================================================
    Revision History
    
    Modification                   Tracking
    Date              Author       Number       Description of changes
    ----------   --------------   ---------   --------------------------------------
    2007-03-02     Jiang Dingqing                  create eme_list_api.h
          
=====================================================================================*/
#ifndef _EME_LIST_API_H
#define _EME_LIST_API_H
 
/*---------------------------------------------------------------
 *       Include Files
 * ------------------------------------------------------------*/
/*Include System head file*/
 

/*Include Program Global head file*/
/*Include module head file*/

typedef unsigned int size_t;

typedef struct list_head {
   struct list_head *next, *prev;
}stListHead;
/*---------------------------------------------------------------------
 *             Definitions and Constants 
 * ------------------------------------------------------------------*/
/*Define Constant Macro start*/
//used to initialize node parameters after deleted from list 
//used to indicate that the node is useless.
#define LIST_POISON1  ((void *) 0x00100100)
#define LIST_POISON2  ((void *) 0x00200200)

/*Define Constant Macro end*/

/*Define Macro Function start*/
#ifndef offsetof
#define offsetof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)
#endif

#define container_of(ptr, type, member)\
(type *)( (char*)ptr - offsetof(type,member) )

#define list_entry(ptr, type, member) \
   container_of(ptr, type, member)

#define list_for_each_entry(type, pos, head, member)            \
   for (pos = list_entry((head)->next, type, member);   \
       &pos->member != (head); \
      pos = list_entry(pos->member.next, type, member))


#define list_for_each_entry_safe(type, pos, n, head, member)         \
   for (pos = list_entry((head)->next, type, member),   \
      n = list_entry(pos->member.next, type, member);   \
        &pos->member != (head);                \
        pos = n, n = list_entry(n->member.next, type, member))        

/*Define Macro Function end*/

/*Define ENUM type start*/
/*Define ENUM type end*/
/*---------------------------------------------------------------------
 *  Here you may want to include extra documentation on data structures
 * ------------------------------------------------------------------*/
/*Define base type start*/

/*Define base type end*/

/*Define function type start*/
/*Define function type end*/

/*Define struct &  union type start*/

/*Define struct &  union type end*/

/*---------------------------------------------------------------------
 *             Function Declarations
 * ------------------------------------------------------------------*/



/*---------------------------------------------------------------------
 *             Function Documentation
 * --------------------------------------------------------------------
 * 
 *    FUNCTION        EXAConform_InitIsComplete()
 * 
 *    DESCRIPTION     Initializes the EXAConform Services    
 * 
 *    DEPENDENCIES    Must be called prior to any other EXAConform
 *                    function calls  
 * 
 *    PARAMETERS      None
 * 
 *    RETURN VALUE    boolean:  TRUE if initialization was successful
 *                               FALSE if initialization failed   
 *     
 *    SIDE EFFECTS    None   
 * 
 *    ================================================================
 * 
 *    FUNCTION        EXAConform_Check()
 * 
 *    DESCRIPTION     Checks for the level of coding standard
 *                    conformity that is specified by input parameter   
 * 
 *    DEPENDENCIES    NONE 
 * 
 *    PARAMETERS      newCmd 
 * 
 *    RETURN VALUE    EXACmdStsType
 *      
 *    SIDE EFFECTS    May wake up your boss
 * 
 * ------------------------------------------------------------------*/

/* Remember header files should contain all the information a user
 * would need to use this service */
void EME_INIT_LIST_HEAD(stListHead *list);
void EME_list_del(stListHead *entry); 
void EME_list_add_tail(stListHead *fresh, stListHead *head);
void EME_list_add(stListHead *fresh, stListHead *head);
int EME_list_empty(const stListHead *head);
void EME_list_move(stListHead *list, stListHead *head);



#endif             /* #ifndef _EME_LIST_API_H */
