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
 * @file   sirf_pal_log_file_syn.c
 *
 * @brief  Logging module implementation using file I/O.  This file has been
 * ported to use Synergy Framework functions. This means that if the Framework
 * is already ported to a platform this files can be used directly on top of
 * that without any modifications.
 *
 */

// #ifdef SIRF_EXT_LOG

#include "ms_customize_trc.h"
#include "sirf_types.h"
#include "sirf_pal.h"
#include "csr_file.h"

/* ----------------------------------------------------------------------------
 *    Local Variables
 * ------------------------------------------------------------------------- */


/* ----------------------------------------------------------------------------
 *    Local Functions
 * ------------------------------------------------------------------------- */


/* ----------------------------------------------------------------------------
 *    Functions
 * ------------------------------------------------------------------------- */


/**
 * @brief Open the specified file for storing logging data.
 * @param[in]  filename           File name to open.
 * @param[out] log                Handle of the opened file.
 * @param[in]  mode               File open mode bitmask; modes can be combined.
 * @return                       Success code.
 */
tSIRF_RESULT SIRF_PAL_LOG_Open( tSIRF_CHAR *filename, tSIRF_LOG_HANDLE *log, tSIRF_UINT32 mode  )
{
   CsrFileHandle *log_file = NULL;
   CsrResult res;

//KST DEBUG This define is to make log files append instead of overwriting. Used during debugging
#ifndef DEBUG
   if ( (mode & SIRF_PAL_LOG_MODE_MASK) == SIRF_PAL_LOG_MODE_OVERWRITE)
   {
      res = CsrFileOpen(&log_file, filename, /*lint !e64*/
         CSR_FILE_OPEN_FLAGS_CREATE|
         CSR_FILE_OPEN_FLAGS_WRITE_ONLY |CSR_FILE_OPEN_FLAGS_TRUNCATE, 
         CSR_FILE_PERMS_USER_READ|CSR_FILE_PERMS_USER_WRITE|
         CSR_FILE_PERMS_GROUP_READ|CSR_FILE_PERMS_GROUP_WRITE|
         CSR_FILE_PERMS_OTHERS_READ|CSR_FILE_PERMS_OTHERS_WRITE);
      if (res != CSR_RESULT_SUCCESS)
      {
         log_file = NULL;
      }
   }
   else if ( (mode & SIRF_PAL_LOG_MODE_MASK) == SIRF_PAL_LOG_MODE_APPEND)
   {
#endif /* DEBUG */
      res = CsrFileOpen(&log_file, filename, /*lint !e64*/
         CSR_FILE_OPEN_FLAGS_CREATE|
         CSR_FILE_OPEN_FLAGS_WRITE_ONLY | CSR_FILE_OPEN_FLAGS_APPEND, 
         CSR_FILE_PERMS_USER_READ|CSR_FILE_PERMS_USER_WRITE|
         CSR_FILE_PERMS_GROUP_READ|CSR_FILE_PERMS_GROUP_WRITE|
         CSR_FILE_PERMS_OTHERS_READ|CSR_FILE_PERMS_OTHERS_WRITE);
      if (res != CSR_RESULT_SUCCESS)
      {
         log_file = NULL;
      }
#ifndef DEBUG
   }
#endif /* DEBUG */

   *log = (tSIRF_LOG_HANDLE)log_file;

   if ( NULL == log_file )
   {
      return SIRF_PAL_LOG_OPEN_ERROR;
   }
    
   return SIRF_SUCCESS;

} /* SIRF_PAL_LOG_Open() */


/**
 * @brief Close the current log file.
 * @param[in] log                Handle of the opened file.
 * @return                       Success code.
 */
tSIRF_RESULT SIRF_PAL_LOG_Close( tSIRF_LOG_HANDLE log )
{
   if ( NULL == log )
   {
      return SIRF_PAL_LOG_ALREADY_CLOSED;
   }

   CsrFileClose(log);

   return SIRF_SUCCESS;

} /* SIRF_PAL_LOG_Close() */


/**
 * @brief Write data to the currently open log file.
 * @param[in] log                Handle of the opened file.
 * @param[in] text               Text data to log.
 * @param[in] length             Length of the text data.
 * @return                       Success code.
 */
tSIRF_RESULT SIRF_PAL_LOG_Write( tSIRF_LOG_HANDLE log, tSIRF_CHAR *text, tSIRF_UINT32 length )
{
   if ( NULL == log )
   {
      return SIRF_PAL_LOG_NOT_OPEN;
   }

   {
      CsrSize written;
      CsrResult res = CsrFileWrite(text, length, log, &written);

      if ((res != CSR_RESULT_SUCCESS) || (written != length))
      {
         return SIRF_PAL_OS_ERROR;
      }
   }

   CsrFileFlush(log);
   return SIRF_SUCCESS;

} /* SIRF_PAL_LOG_Write() */

/**
 * @brief Return the file size
 * @param[in] log                Handle of the opened file.
 * @param[out] fileSize          File length.
 * @return                       Success code.
 */
tSIRF_RESULT SIRF_PAL_LOG_FileSize( tSIRF_LOG_HANDLE log, tSIRF_UINT32* fileSize  )
{
   tSIRF_UINT32 curPos;
   if ( NULL == log )
   {
      return SIRF_PAL_LOG_NOT_OPEN;
   }

   if ( NULL == fileSize )
   {
      return SIRF_PAL_LOG_NULL_PARAMETER;
   }
   
   CsrFileTell(log, &curPos);          /* Tells the current position of file pointer and store it */
   
   CsrFileSeek(log, 0, CSR_SEEK_SET);  /* The file pointer log is moved to the start of the file from current position */
   CsrFileSeek(log, 0, CSR_SEEK_END);  /* The file pointer log is moved to the End of the file */
   CsrFileTell(log, fileSize);         /* Tell the current position of the file pointer */

   CsrFileSeek(log, curPos, CSR_SEEK_SET); /* The file pointer log is moved back to original position */

   return SIRF_SUCCESS;

} /* SIRF_PAL_LOG_FileSize() */

// #endif /* SIRF_EXT_LOG */

void CsrDebugTraceHigh(const char* const format, ...)
{
#ifdef SCI_CSR_LOG_ENABLE
    FORMAT_TRACE(CsrLogBuffer+DEBUG_BUFFER_MARK_SIZE, DEBUG_BUFFER_SIZE-DEBUG_BUFFER_MARK_SIZE, format);/*lint !e64 */
    SCI_TRACE_LOW("%s", CsrLogBuffer); /*lint !e64 */
#endif
}
/**
 * @}
 * End of file.
 */


