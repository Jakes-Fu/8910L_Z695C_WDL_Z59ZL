/**
 * @addtogroup platform_src_sirf_pal_synergy
 * @{
 */

/*
 *                   SiRF Technology, Inc. GPS Software
 *
 *    Copyright (c) 2005-2009 by SiRF Technology, Inc.  All rights reserved.
 *
 *    This Software is protected by United States copyright laws and
 *    international treaties.  You may not reverse engineer, decompile
 *    or disassemble this Software.
 *
 *    WARNING:
 *    This Software contains SiRF Technology Inc.’s confidential and
 *    proprietary information. UNAUTHORIZED COPYING, USE, DISTRIBUTION,
 *    PUBLICATION, TRANSFER, SALE, RENTAL OR DISCLOSURE IS PROHIBITED
 *    AND MAY RESULT IN SERIOUS LEGAL CONSEQUENCES.  Do not copy this
 *    Software without SiRF Technology, Inc.’s  express written
 *    permission.   Use of any portion of the contents of this Software
 *    is subject to and restricted by your signed written agreement with
 *    SiRF Technology, Inc.
 *
 */

/**
 * @file   sirf_pal_os_semaphore_syn.c
 *
 * @brief  SiRF PAL semaphore module. This file has been ported to use Synergy
 * Framework functions. This means that if the Framework is already ported
 * to a platform this files can be used directly on top of that without any
 * modifications.
 *
 */



/* ----------------------------------------------------------------------------
 *    Included Files
 * ------------------------------------------------------------------------- */

#include "sirf_types.h"
#include "sirf_pal.h"

#include "sirf_pal_config.h"

#include "csr_framework_ext.h"

#include "csr_util.h"

#include "os_api.h"

/* ----------------------------------------------------------------------------
 *    Constants
 * ------------------------------------------------------------------------- */



/* ----------------------------------------------------------------------------
 *    Types
 * ------------------------------------------------------------------------- */



/** 
 * @brief Internal Semaphore structure. 
 *
 * This structure is used to order posix semaphores on a free list
 * in order to allow any sequencing of semaphore create/delete operations.
 */
typedef struct tPALSemaphore_tag
{
   struct tPALSemaphore_tag * m_pNext;     /**< Next in the free list. */
   tSIRF_BOOL                 m_bCreated;  /**< Created flag. */
   SCI_SEMAPHORE_PTR          m_sem;       /**< Synergy event (semaphore). */
} tPALSemaphore;



/* ----------------------------------------------------------------------------
 *    Debugging Defines
 * ------------------------------------------------------------------------- */




/* ----------------------------------------------------------------------------
 *    Local Variables
 * ------------------------------------------------------------------------- */

/** Mutex for managing semaphores. */
static CsrMutexHandle s_sem_crit;

/** Pool of application semaphores. */
static tPALSemaphore s_sem_pool[SIRF_PAL_OS_SEM_MAX];

/** Semaphore free list. */
static tPALSemaphore * s_sem_free = NULL;

/** Module Initialized flag. */
static tSIRF_BOOL s_sem_initialized = SIRF_FALSE;

/* ----------------------------------------------------------------------------
 *    Functions
 * ------------------------------------------------------------------------- */



tSIRF_RESULT SIRF_PAL_OS_SEMAPHORE_Create(tSIRF_SEMAPHORE * sem_handle, tSIRF_UINT32 init_value)
{
   tPALSemaphore *p_sem;
   const char name[sizeof("CSRSiRF_Sem")] = "CSRSiRF_Sem";

   /* Validate the output port handle pointer. */
   if (NULL == sem_handle)
   {
      return SIRF_PAL_OS_ERROR;
   }

   /* Initialize the globals if needed. */
   if (!s_sem_initialized)
   {
      int i;
   
      /* Create the semaphore management mutex. */
      if (CsrMutexCreate(&s_sem_crit) != CSR_RESULT_SUCCESS)
	  {
         return SIRF_PAL_OS_ERROR;
	  }

      /* Zero all descriptors. */
      CsrMemSet(s_sem_pool, 0, sizeof(s_sem_pool));
      
      /* Initialize all descriptors. */
      s_sem_free = NULL;
      for (i = 0; i < SIRF_PAL_OS_SEM_MAX; ++i)
      {
         /* Add to the free list. */
         s_sem_pool[i].m_pNext = s_sem_free;
         s_sem_free = &s_sem_pool[i];
      }

      /* Flag the module as initialized. */
      s_sem_initialized = SIRF_TRUE;
   }
   
   /* Get the next free descriptor (if any). */
   CsrMutexLock(&s_sem_crit);
   p_sem = s_sem_free;
   if (NULL != p_sem)
   {
      s_sem_free = p_sem->m_pNext;
   }
   CsrMutexUnlock(&s_sem_crit);

   /* Save the tPALSemaphore (or NULL) for the return. */
   *sem_handle = (tSIRF_SEMAPHORE)p_sem;
   if (NULL == p_sem)
   {
      return SIRF_PAL_OS_ERROR;
   }

   /* Initialize the descriptor members. */
   p_sem->m_bCreated = SIRF_TRUE;

   p_sem->m_sem = SCI_CreateSemaphore(name, init_value);

   if (p_sem->m_sem == NULL)
   {
      return SIRF_PAL_OS_ERROR;
   }
   else
   {
      /* Return success. */
      return SIRF_SUCCESS;
   }


} /* SIRF_PAL_OS_SEMAPHORE_Create() */

tSIRF_RESULT SIRF_PAL_OS_SEMAPHORE_Delete(tSIRF_SEMAPHORE sem_handle)
{
   tPALSemaphore * p_sem = (tPALSemaphore*)sem_handle;

   /* Ensure the handle is valid and created. */
   if (NULL == p_sem || !p_sem->m_bCreated)
   {
      return SIRF_PAL_OS_ERROR;
   }

   /* Delete the resources. */
   SCI_DeleteSemaphore(p_sem->m_sem);

   /* Mark as deleted and place on the free list. */
   p_sem->m_bCreated = SIRF_FALSE;
   CsrMutexLock(&s_sem_crit);
   p_sem->m_pNext = s_sem_free;
   s_sem_free = p_sem;
   CsrMutexUnlock(&s_sem_crit);

   /* Return success. */
   return SIRF_SUCCESS;

} /* SIRF_PAL_OS_SEMAPHORE_Delete() */

tSIRF_RESULT SIRF_PAL_OS_SEMAPHORE_Wait(tSIRF_SEMAPHORE sem_handle, tSIRF_UINT32 timeout)
{
   tPALSemaphore * p_sem = (tPALSemaphore*)sem_handle;
   tSIRF_UINT32 res;

   /* Ensure the handle is valid and created. */
   if (NULL == p_sem || !p_sem->m_bCreated)
   {
      return SIRF_PAL_OS_ERROR;
   }

   /* Handle infinite timeout. */
   if ( timeout == SIRF_TIMEOUT_INFINITE )
   {
	   if (SCI_GetSemaphore(p_sem->m_sem, SCI_WAIT_FOREVER) != SCI_SUCCESS)
       {
           return SIRF_PAL_OS_ERROR;
       }
   }
   else
   {
   	res = SCI_GetSemaphore(p_sem->m_sem, timeout);
	   if (res == SCI_NO_INSTANCE)
	   	{
	   	return SIRF_PAL_OS_SEMAPHORE_WAIT_TIMEOUT;
	   	}
	   else if (res != SCI_SUCCESS)
       {
           return SIRF_PAL_OS_ERROR;
       }
   }
 
   return SIRF_SUCCESS;

} /* SIRF_PAL_OS_SEMAPHORE_Wait() */

tSIRF_RESULT SIRF_PAL_OS_SEMAPHORE_Release( tSIRF_SEMAPHORE sem_handle )
{
   tPALSemaphore * p_sem = (tPALSemaphore*)sem_handle;

   /* Ensure the handle is valid and created. */
   if (NULL == p_sem || !p_sem->m_bCreated)
   {
      return SIRF_PAL_OS_ERROR;
   }
      
   /* Attempt to release the semaphore. */
   if (SCI_PutSemaphore(p_sem->m_sem) != SCI_SUCCESS)
   {
      return SIRF_PAL_OS_ERROR;
   }

   return SIRF_SUCCESS;
} /* SIRF_PAL_OS_SEMAPHORE_Release() */



/**
 * @}
 * End of file.
 */


