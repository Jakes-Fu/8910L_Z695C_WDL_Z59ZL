/*=====================================================================================
FILE NAME   : eme_list.c
MODULE NAME : Email engine

    GENERAL DESCRIPTION
      Provide some function for managing list.
      
    TechFaith Software Confidential Proprietary
    Copyright (c) 2007 by TechFaith Software. All Rights Reserved.
    =======================================================================================
    Revision History
    
    Modification                   Tracking
    Date              Author       Number       Description of changes
    ----------   --------------   ---------   --------------------------------------
    2007-03-02     Jiang Dingqing                  create eme_list.c
          
=====================================================================================*/

/*===================================================================
* 
*                     Include Files
* 
====================================================================*/
/*Include System head file*/

/*Include Program Global head file*/

/*Include Module head file*/

/*Include Private head file*/
#include "eme_list_api.h"

/*=============================================================================
* 
*     DEFINITIONS AND CONSTANTS(It is not recommended)
* It is recommended that this section is defined to private head file.
* 
=============================================================================*/

   
/*=============================================================================
*
*   Static Function Prototype Declare Section
*
=============================================================================*/


/*=============================================================================
*
*   Global Variable Declare Section
*
=============================================================================*/
static void __list_add(stListHead * fresh,
               stListHead *prev,
               stListHead *next);
static void __list_del(stListHead * prev, stListHead * next);
/*=============================================================================
*
*   File Static Variable Define Section
*
=============================================================================*/




/*===================================================================
* 
*                    Functions
* 
* ==================================================================*/


 /*============================================================================ 
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
 *===========================================================================*/



/* This is where the real code goes. In this example the function
 * EXAConform_INIT will let your boss know it is coming and
 * EXAConform_Check which will contact your boss and get a value of
 * EXACmdStsType returned. simply describe effect of public function */
void EME_INIT_LIST_HEAD(stListHead *list)
{
   list->next = list;
   list->prev = list;
}

//Insert a new entry before the specified head.
void EME_list_add_tail(stListHead *fresh, stListHead *head)
{
   __list_add(fresh, head->prev,head);
}

void EME_list_add(stListHead *fresh, stListHead *head)
{
   __list_add(fresh, head, head->next);
}

int EME_list_empty(const stListHead *head)
{
   return head->next == head;
}

void EME_list_move(stListHead *list, stListHead *head)
{
   __list_del(list->prev, list->next);
   EME_list_add(list, head);
}

void EME_list_del(stListHead *entry)
{
   __list_del(entry->prev, entry->next);
   entry->next = LIST_POISON1;
   entry->prev = LIST_POISON2;
}

static  void __list_add(stListHead * fresh,
                        stListHead *prev,
                        stListHead *next)
{
   next->prev = fresh;
   fresh->next = next;
   fresh->prev = prev;
   prev->next = fresh;
}

static void __list_del(stListHead * prev, stListHead * next)
{
   next->prev = prev;
   prev->next = next;
}


