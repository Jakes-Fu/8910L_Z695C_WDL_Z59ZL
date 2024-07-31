/**
 * @addtogroup platform_src_sirf_pal_synergy
 * @{
 */

/*
 *                   SiRF Technology, Inc. GPS Software
 *
 *    Copyright (c) 2009 by SiRF Technology, Inc.  All rights reserved.
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
 * @file   sirf_pal_os_mem_syn.c
 *
 * @brief  SiRF PAL dynamic memory module. This file has been ported to use Synergy
 * Framework functions. This means that if the Framework is already ported
 * to a platform this files can be used directly on top of that without any
 * modifications.
 *
 */

#include "sirf_types.h"
#include "sirf_pal.h"
#include "csr_framework_ext.h"


static tSIRF_BOOL s_mem_initialized = SIRF_FALSE;

/* ----------------------------------------------------------------------------
 *    Functions
 * ------------------------------------------------------------------------- */

 /**
 * @brief Initializes the TLSF based PAL dynamic memory subsystem.
 *
 * FYI: TLSF == Two-Level Segregated Fit
 *
 * Initialize Heap with static pool if it isn't already initialized.
 *
 * @return SIRF_SUCCESS if successful, SIRF_FAILURE otherwise.
 */
tSIRF_RESULT SIRF_PAL_OS_MEM_Init(void)
{
   if (SIRF_FALSE == s_mem_initialized)
   {
        s_mem_initialized = SIRF_TRUE;
   }

   return SIRF_SUCCESS;
}
 
/**
 * @brief Teardown the TLSF based PAL dynamic memory subsystem
 *
 * Mark subsystem as uninitialized if currently initialized.
 *
 * @return SIRF_SUCCESS if successful, SIRF_FAILURE otherwise
 */
tSIRF_RESULT SIRF_PAL_OS_MEM_Destroy(void)
{
   if (SIRF_TRUE == s_mem_initialized)
   {
      s_mem_initialized = SIRF_FALSE;
   }

   return SIRF_SUCCESS;
}

/**
 * @brief TLSF based PAL allocation function
 *
 * @param size Amount of memory to allocate
 *
 * @return Memory address if successful, NULL otherwise.
 */
 void *SIRF_PAL_OS_MEM_Alloc(tSIRF_UINT32 size)
{
   if (SIRF_FALSE == s_mem_initialized)
   {
      return NULL;
   }
   return (void*)CsrMemAlloc((size_t)size);
}

/**
 * @brief TLSP based PAL free function
 *
 * @param pMem   Memory block to free
 *
 * @return SIRF_SUCCESS if successful, SIRF_FAILURE otherwise
 */  
tSIRF_RESULT SIRF_PAL_OS_MEM_Free(void *pMem)
{
    if (SIRF_TRUE == s_mem_initialized)
    {
        if (NULL != pMem)
        {
            CsrMemFree(pMem);
            return SIRF_SUCCESS;
        }
        else
        {
            return SIRF_FAILURE;
        }
    }
    else
    {
        return SIRF_FAILURE;
    }
}
   

/**
 * @}
 */


