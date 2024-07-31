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
 * @file   sirf_pal_os_mutex_syn.c
 *
 * @brief  SiRF PAL mutex module. This file has been ported to use Synergy
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

/* ----------------------------------------------------------------------------
 *    Constants
 * ------------------------------------------------------------------------- */


/* ----------------------------------------------------------------------------
 *    Debugging Defines
 * ------------------------------------------------------------------------- */


/* ----------------------------------------------------------------------------
 *   Types
 * ------------------------------------------------------------------------- */



/** 
 * @brief Internal Mutex structure. 
 *
 * This structure is used to order posix mutexes on a free list
 * in order to allow any sequencing of mutex create/delete operations.
 */
typedef struct tPALMutex_tag
{
   struct tPALMutex_tag * m_pNext;     /**< Next in the free list. */
   tSIRF_BOOL             m_bCreated;  /**< Created flag. */
   CsrMutexHandle         m_mutex;     /**< CSR Synergy mutex. */
} tPALMutex;



/* ----------------------------------------------------------------------------
 *   Local Variables
 * ------------------------------------------------------------------------- */



/** Mutex for managing mutexes. */
static CsrMutexHandle s_mutex_crit;

/** Pool of application mutexes. */
static tPALMutex s_mutex_pool[SIRF_PAL_OS_MUTEX_MAX];

/** Mutex free list. */
static tPALMutex * s_mutex_free = NULL;

/** Module Initialized flag. */
static tSIRF_BOOL s_mutex_initialized = SIRF_FALSE;



/* ----------------------------------------------------------------------------
 *    Functions
 * ------------------------------------------------------------------------- */

tSIRF_RESULT SIRF_PAL_OS_MUTEX_Create( tSIRF_MUTEX *mx_handle )
{
   tPALMutex *p_mutex;
   CsrResult result;

   /* Validate the output semaphore handle pointer. */
   if (NULL == mx_handle)
   {
      return SIRF_PAL_OS_ERROR;
   }

   /* Initialize the globals if needed. */
   if (!s_mutex_initialized)
   {
      int i;
   
      /* Create the mutex management mutex. */
      if (CsrMutexCreate(&s_mutex_crit) != CSR_RESULT_SUCCESS)
	  {
         return SIRF_PAL_OS_ERROR;
	  }

      /* Zero all descriptors. */
      CsrMemSet(s_mutex_pool, 0, sizeof(s_mutex_pool));
      
      /* Initialize all descriptors. */
      s_mutex_free = NULL;
      for (i = 0; i < SIRF_PAL_OS_MUTEX_MAX; ++i)
      {
         /* Add to the free list. */
         s_mutex_pool[i].m_pNext = s_mutex_free;
         s_mutex_free = &s_mutex_pool[i];
      }

      /* Flag the module as initialized. */
      s_mutex_initialized = SIRF_TRUE;
   }
   
   /* Get the next free descriptor (if any). */
   CsrMutexLock(&s_mutex_crit);
   p_mutex = s_mutex_free;
   if (NULL != p_mutex)
   {
      s_mutex_free = p_mutex->m_pNext;
   }
   CsrMutexUnlock(&s_mutex_crit);

   /* Save the tPALMutex (or NULL) for the return. */
   *mx_handle = (tSIRF_MUTEX)p_mutex;
   if (NULL == p_mutex)
   {
      return SIRF_PAL_OS_ERROR;
   }
      
   /* Create the mutex. */
   result = CsrMutexCreate(&p_mutex->m_mutex);

   if (result == CSR_RESULT_SUCCESS)
   {
	   /* Return success. */
	   p_mutex->m_bCreated = SIRF_TRUE;
	   return SIRF_SUCCESS;
   }
   else
   {
	   return SIRF_PAL_OS_ERROR;
   }
   
} /* SIRF_PAL_OS_MUTEX_Create() */

tSIRF_RESULT SIRF_PAL_OS_MUTEX_Delete( tSIRF_MUTEX mx_handle )
{
   tPALMutex * p_mutex = (tPALMutex*)mx_handle;

   /* Ensure the handle is valid and created. */
   if (NULL == p_mutex || !p_mutex->m_bCreated)
   {
      return SIRF_PAL_OS_ERROR;
   }

   /* Delete the resources. */
   CsrMutexDestroy(&p_mutex->m_mutex);
   
   /* Mark as deleted and place on the free list. */
   p_mutex->m_bCreated = SIRF_FALSE;

   CsrMutexLock(&s_mutex_crit);
   p_mutex->m_pNext = s_mutex_free;
   s_mutex_free = p_mutex;
   CsrMutexUnlock(&s_mutex_crit);

   /* Return success. */
   return SIRF_SUCCESS;

} /* SIRF_PAL_OS_MUTEX_Delete() */

tSIRF_RESULT SIRF_PAL_OS_MUTEX_Enter( tSIRF_MUTEX mx_handle )
{
   tPALMutex * p_mutex = (tPALMutex*)mx_handle;
   CsrResult result;

   /* Ensure the handle is valid and created. */
   if (NULL == p_mutex || !p_mutex->m_bCreated)
   {
      return SIRF_PAL_OS_ERROR;
   }

   /* Enter the mutex. */
   result = CsrMutexLock(&p_mutex->m_mutex);

   /* Examine the return value. */
   if (result == CSR_RESULT_SUCCESS)
   {
      return SIRF_SUCCESS;
   }
   else
   {
      return SIRF_PAL_OS_ERROR;
   }

} /* SIRF_PAL_OS_MUTEX_Enter() */

tSIRF_RESULT SIRF_PAL_OS_MUTEX_Exit( tSIRF_MUTEX mx_handle )
{
   tPALMutex * p_mutex = (tPALMutex*)mx_handle;
   CsrResult result;

   /* Ensure the handle is valid and created. */
   if (NULL == p_mutex || !p_mutex->m_bCreated)
   {
      return SIRF_PAL_OS_ERROR;
   }

   /* Enter the mutex. */
   result = CsrMutexUnlock(&p_mutex->m_mutex);

   /* Examine the return value. */
   if (result == CSR_RESULT_SUCCESS)
   {
      return SIRF_SUCCESS;
   }
   else
   {
      return SIRF_PAL_OS_ERROR;
   }

} /* SIRF_PAL_OS_MUTEX_Exit() */



/**
 * @}
 * End of file.
 */


