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
 * @file   sirf_pal_os_thread_syn.c
 *
 * @brief  SiRF PAL thread module. This file has been ported to use Synergy
 * Framework functions. This means that if the Framework is already ported
 * to a platform this files can be used directly on top of that without any
 * modifications.
 *
 */

#include "sirf_types.h"
#include "sirf_pal.h"

#include "sirf_threads.h"

#include "csr_framework_ext.h"

#define SIRF_THREAD_NAME_ARRAY_SIZE        (10)
#define SIRF_THREAD_NAME_LENGTH                (10)

static char SIRF_THREAD_NAME_ARRAY[SIRF_THREAD_NAME_ARRAY_SIZE][SIRF_THREAD_NAME_LENGTH] = {0};

/* ----------------------------------------------------------------------------
 *    Debugging Defines
 * ------------------------------------------------------------------------- */



/* ----------------------------------------------------------------------------
 *    Functions
 * ------------------------------------------------------------------------- */

/**
 * @brief Create a thread.
 * @param[in] thread_id                ID of the thread to create.
 * @param[in] function_ptr             Pointer to the thread entry rountine.
 * @param[out] thread_handle           Handle to the created thread.
 * @return                             Result code.
 */
tSIRF_RESULT SIRF_PAL_OS_THREAD_Create( tSIRF_UINT32 thread_id, tSIRF_HANDLE function_ptr, tSIRF_THREAD *thread_handle )
{
   tSIRF_UINT32   max_threads;
   tSIRF_UINT32   i;
   CsrResult      result;
   SIRF_PAL_OS_THREAD_RETURNTYPE (*cast_pointer)(SIRF_PAL_OS_THREAD_PARAMS);
   cast_pointer = (SIRF_PAL_OS_THREAD_RETURNTYPE(*)(SIRF_PAL_OS_THREAD_PARAMS))function_ptr;/*lint !e611*/

   max_threads = SIRF_THREAD_MaxThreads();
   
   for(i=0; i<max_threads; i++)
   {
      if ( SIRF_THREAD_Table[i].thread_id == thread_id)
	  {
         break;
	  }
   }

   if (i == max_threads)
   {
      return SIRF_PAL_OS_ERROR;  /* thread is not defined */
   }

   sprintf(SIRF_THREAD_NAME_ARRAY[i], "SiRFPal%d", thread_id);

   result = CsrThreadCreate(cast_pointer, &SIRF_THREAD_Table[i], SIRF_THREAD_Table[i].thread_stack_size, SIRF_THREAD_Table[i].thread_priority, SIRF_THREAD_NAME_ARRAY[i], thread_handle);/*lint !e64*/

   if (result != CSR_RESULT_SUCCESS)
   {
      return SIRF_PAL_OS_ERROR;
   }

   return SIRF_SUCCESS;

} /* SIRF_PAL_OS_THREAD_Create() */


/**
 * @brief Delete a thread.
 * @param[in] thread_handle            Handle to the thread to delete.
 * @return                             Result code.
 */
tSIRF_UINT32 SIRF_PAL_OS_THREAD_Delete( tSIRF_THREAD thread_handle )
{
   tSIRF_UINT32 result;
   result = SCI_DeleteThread((uint32)thread_handle);
   
    if (result == SCI_SUCCESS)
    {
        return SIRF_SUCCESS;
    }
    else
    {
        return SIRF_FAILURE;
    }
} /* SIRF_PAL_OS_THREAD_Delete() */


/**
 * @brief Put the current thread to sleep.
 * @param[in] milliseconds             Milliseconds to sleep for.
 */
tSIRF_VOID SIRF_PAL_OS_THREAD_Sleep( tSIRF_UINT32 milliseconds )
{
    CsrThreadSleep(milliseconds);
} /* SIRF_PAL_OS_THREAD_Sleep() */


/**
 * @brief Handler for thread startup.
 */
tSIRF_VOID SIRF_PAL_OS_THREAD_Entry( tSIRF_VOID )
{
   ;
} /* SIRF_PAL_OS_Thread_Start() */


/**
 * @brief Handler for thread shutdown.
 */
SIRF_PAL_OS_THREAD_RETURNTYPE SIRF_PAL_OS_THREAD_Return( tSIRF_VOID )
{
    //SCI_ThreadExit();
    return;   
} /* SIRF_PAL_OS_Thread_Return() */



/**
 * @}
 * End of file.
 */


