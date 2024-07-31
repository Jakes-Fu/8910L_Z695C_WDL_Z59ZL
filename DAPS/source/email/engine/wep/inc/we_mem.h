/*=====================================================================================

    HEADER NAME : we_mem.h
    MODULE NAME : WE Framework Memory Interface Define

    PRE-INCLUDE FILES DESCRIPTION

    GENERAL DESCRIPTION
        This file can be included by all WE Programmer.In this file all Memory-Interface
    is defined.
        These Info is  aptotic and is not changed by switching Project.

    TECHFAITH Wireless Confidential Proprietary
    (c) Copyright 2006 by TechSoft. All Rights Reserved.
=======================================================================================
    Revision History
   
    Modification                 Tracking
    Date           Author         Number        Description of changes
    ----------   ------------    ---------   --------------------------------------------
    2006-05-30   HiroWang				        create this file

    Self-documenting Code
    Describe/explain low-level design, especially things in header files, of this module and/or group 
    of funtions and/or specific funtion that are hard to understand by reading code and thus requires 
    detail description.
    Free format !

=====================================================================================*/
/*--------------------------START-----------------------------*/


/*******************************************************************************
*   Multi-Include-Prevent Section
*******************************************************************************/
#ifndef WE_MEM_H
#define WE_MEM_H

#include "we_scl.h"



/*******************************************************************************
*   Include File Section
*******************************************************************************/
/*Include System head file*/

/*Include Program Global head file*/

/*Include Module head file*/

/*******************************************************************************
*   Macro Define Section
*******************************************************************************/
/*Conditional Compilation Directives start*/
/*Conditional Compilation Directives end*/

/*Define Constant Macro start*/
/*Define Constant Macro end*/

/*Define Macro Function start*/

/*=====================================================================================
FUNCTION: 
    We_Malloc
CREATE DATE: 
    2006-05-30
AUTHOR: 
    Hiro Wang
DESCRIPTION:
    Allocates memory blocks.
ARGUMENTS PASSED:
    uiSize: Size of buffer in bytes.
RETURN VALUE:
    Returns a void pointer to the allocated memory.If failed return NULL.
USED GLOBAL VARIABLES:
    none
USED STATIC VARIABLES:
    none
CALL BY:
    All program need malloc memory.
IMPORTANT NOTES:
    The block may be larger than size bytes because of space required for
    alignment and maintenance information.

  
=====================================================================================*/
#define WE_MALLOC  We_Malloc

/*void * We_Malloc(WE_UINT32 uiSize)*/

#define We_Malloc  WE_SCL_MALLOC

/*=====================================================================================
FUNCTION: 
    We_Free
CREATE DATE: 
    2006-05-30
AUTHOR: 
    Hiro Wang
DESCRIPTION:
    Deallocates or frees a memory block.
ARGUMENTS PASSED:
    pvMemBlock: Previously allocated memory block to be freed.
RETURN VALUE:
    none.
USED GLOBAL VARIABLES:
    none
USED STATIC VARIABLES:
    none
CALL BY:
    All program need Free Memory.
IMPORTANT NOTES:
    If pvMemBlock is NULL, the pointer is ignored and free immediately returns. 
    Attempting to free an invalid pointer (a pointer to a memory block that 
    was not allocated by WE_MALLOC or freed) may affect subsequent allocation 
    requests and cause errors.      

  
=====================================================================================*/

#define WE_FREE  We_Free

/*void We_Free(void *pvMemBlock)*/
#define We_Free  WE_SCL_FREE

/*=====================================================================================
FUNCTION: 
    We_Memset
CREATE DATE: 
    2006-05-30
AUTHOR: 
    Hiro Wang
DESCRIPTION:
    Sets buffers to a specified character.
ARGUMENTS PASSED:
    pvDest: Pointer to destination.
    iValue: Character to set      
    uiCount:Number of characters
RETURN VALUE:
    returns the value of pvDest.
USED GLOBAL VARIABLES:
    none
USED STATIC VARIABLES:
    none
CALL BY:
    All program need memset.
IMPORTANT NOTES:
    none.      

  
=====================================================================================*/

#define WE_MEMSET  We_Memset

/*void * We_Memset(void *pvDest, WE_INT32 iValue, WE_UINT32 uiCount)*/
#define We_Memset  WE_SCL_MEMSET


/*=====================================================================================
FUNCTION: 
    We_Memcpy
CREATE DATE: 
    2006-05-30
AUTHOR: 
    Hiro Wang
DESCRIPTION:
    Copies characters between buffers.
ARGUMENTS PASSED:
    pvDest: pointer to the destination buffer.
    pvSrc:pointer to the source buffer.
    uiCount: Specifies the number of bytes that must be coped from pvSrc to pvDest.
RETURN VALUE:
    Returns a pointer to the pvDest buffer .
USED GLOBAL VARIABLES:
    none
USED STATIC VARIABLES:
    none
CALL BY:
    All program need memcpy.
IMPORTANT NOTES:
    The memcpy function copies count bytes of src to dest. If the source 
    and destination overlap, this function does not ensure that the 
    original source bytes in the overlapping region are copied before 
    being overwritten. Use MEMMOVE to handle overlapping regions.
  
=====================================================================================*/

#define WE_MEMCPY  We_Memcpy
/*void * We_Memcpy(void *pvDest, const void *pvSrc, WE_UINT32 uiCount)*/
#define We_Memcpy  WE_SCL_MEMCPY

/*=====================================================================================
FUNCTION: 
    We_Memmove
CREATE DATE: 
    2006-05-30
AUTHOR: 
    Hiro Wang
DESCRIPTION:
    Moves one buffer to another.
ARGUMENTS PASSED:
    pvDest: pointer to the destination buffer.
    pvSrc:pointer to the source buffer.
    uiCount: Specifies the number of bytes that must be coped from pvSrc to pvDest.
RETURN VALUE:
    Returns a pointer to the pvDest buffer .
USED GLOBAL VARIABLES:
    none
USED STATIC VARIABLES:
    none
CALL BY:
    All program need memmove.
IMPORTANT NOTES:
    The memmove function copies count bytes of characters from src to dest. 
    If some regions of the source area and the destination overlap, Memmove ensures 
    that the original source bytes in the overlapping region are copied before 
    being overwritten.
    It is not recommended to be used.
  
=====================================================================================*/

#define WE_MEMMOVE  We_Memmove
/*void * We_Memmove(void *pvDest, const void *pvSrc, WE_UINT32 uiCount)*/
#define We_Memmove  WE_SCL_MEMMOVE

/*=====================================================================================
FUNCTION: 
    We_Memcmp
CREATE DATE: 
    2006-05-30
AUTHOR: 
    Hiro Wang
DESCRIPTION:
    Compare characters in two buffers.
ARGUMENTS PASSED:
    pvBuffer1: pointer to first mem buffers to be compared .
    pvBuffer2: pointer to second mem buffers to be compared .
    uiCount: Specifies the number of bytes that must be compared .
RETURN VALUE:
    0 if the buffers contain the same count bytes.
    < 0 if the first differing byte of p1 is less than that of p2. 
    > 0 if the first differing byte of p1 is greater than that of p2.
USED GLOBAL VARIABLES:
    none
USED STATIC VARIABLES:
    none
CALL BY:
    All program need memcmp.
IMPORTANT NOTES:
    none.
  
=====================================================================================*/

#define WE_MEMCMP  We_Memcmp
/*WE_INT32 We_Memcmp(void *pvBuffer1, const void *pvBuffer2, WE_UINT32 uiCount)*/
#define We_Memcmp  WE_SCL_MEMCMP

#define WE_STRCPY    WE_SCL_STRCPY
#define WE_STRNCPY   WE_SCL_STRNCPY
#define WE_STRCAT    WE_SCL_STRCAT
#define WE_STRRCHR   WE_SCL_STRRCHR
#define WE_STRDUP   WE_SCL_STRDUP

 
/*Define Macro Function end*/


/*******************************************************************************
*   Type Define Section
*******************************************************************************/

/*Define base type start*/
/*Define base type end*/

/*Define Enum type start*/
/*Define Enum type end*/

/*Define function type start*/
/*Define function type end*/

/*Define struct &  union type start*/
/*Define struct &  union type end*/

/*******************************************************************************
*   Prototype Declare Section
*******************************************************************************/

#endif/*endif WE_MEM_H*/
/*--------------------------END-----------------------------*/
