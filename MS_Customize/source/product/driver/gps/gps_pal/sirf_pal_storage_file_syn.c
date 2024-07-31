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
 * @file   sirf_pal_storage_file_syn.c
 *
 * @brief  SiRF PAL storage module using the filesystem. This file has been
 * ported to use Synergy Framework functions. This means that if the Framework
 * is already ported to a platform this files can be used directly on top of
 * that without any modifications.
 *
 */

 #include "os_api.h" //KST DEBUG

#include "sirf_types.h"
#include "sirf_pal.h"
#include "csr_file.h"
#include "csr_util.h"
#include "csr_framework_ext.h"

CsrFileHandle *store_id[SIRF_PAL_STORAGE_MAX];

/* ----------------------------------------------------------------------------
 *    Functions
 * ------------------------------------------------------------------------- */



/**
 * @brief Close the storage device for a given storage ID.
 *        The storage ID is derived from sirf_pal_storage.h
 *        The storage ID is attached the NVM to denote the type
 * @param[in] storage_id               Storage ID.
 * @return                             Result code.
 */
tSIRF_RESULT SIRF_PAL_STORAGE_Open( tSIRF_UINT32 storage_id )
{
    tSIRF_RESULT result = SIRF_SUCCESS;
    char *filename = NULL;
    tSIRF_UINT32 stringSize;
    CsrFileHandle *fh = NULL;
    CsrResult res;

    stringSize = CsrStrLen("D:\\NVMX");
    filename = CsrMemAlloc(stringSize + 1);
    snprintf(filename, (stringSize + 1), "D:\\NVM%d", (int)storage_id);

    /* return if the ID is beyond the max possible */
    result = SIRF_SUCCESS;

      res = CsrFileOpen(&fh, (const uint8*)filename, 
         CSR_FILE_OPEN_FLAGS_READ_WRITE|CSR_FILE_OPEN_FLAGS_CREATE, 
         CSR_FILE_PERMS_USER_READ|CSR_FILE_PERMS_USER_WRITE|
         CSR_FILE_PERMS_GROUP_READ|CSR_FILE_PERMS_GROUP_WRITE|
         CSR_FILE_PERMS_OTHERS_READ|CSR_FILE_PERMS_OTHERS_WRITE);

      CsrMemFree(filename);
      filename = NULL;

      if (res != CSR_RESULT_SUCCESS)
      {
         fh = NULL;
         result = SIRF_FAILURE;
      }

      if(result == SIRF_SUCCESS)
      {
         store_id[storage_id] = fh;
      }
      
   return result;

} /* SIRF_PAL_STORAGE_Open() */


/**
 * @brief Close the storage device for a given storage ID.
 *        The storage ID is derived from sirf_pal_storage.h
 *        The storage ID is attached the NVM to denote the type
 * @param[in] storage_id               Storage ID.
 * @return                             Result code.
 */
tSIRF_RESULT SIRF_PAL_STORAGE_Close( tSIRF_UINT32 storage_id )
{
   tSIRF_RESULT result = SIRF_FAILURE;
   CsrFileHandle *fh = NULL;

   /* return if the ID is beyond the max possible */
   if (storage_id < SIRF_PAL_STORAGE_MAX)
   {
      result = SIRF_SUCCESS;
   }

   if (SIRF_SUCCESS == result)
   {
      fh = store_id[storage_id];
      if (fh != NULL)
      {
         CsrFileClose(fh);
         store_id[storage_id] = NULL;
         result = SIRF_SUCCESS;
      }
      else
      {
         result = SIRF_FAILURE;
      }
   }
   return result;

} /* SIRF_PAL_STORAGE_Close() */


/**
 * @brief Write data to the storage device.
 * @param[in] storage_id               Storage ID - from sirf_pal_storage.h.
 * @param[in] offset                   Offset into the storage area.
 * @param[in] data                     Data to write.
 * @param[in] length                   Length of the data.
 * @return                             Result code.
 */
tSIRF_RESULT SIRF_PAL_STORAGE_Write( tSIRF_UINT32 storage_id, tSIRF_UINT32 offset, tSIRF_UINT8 *data, tSIRF_UINT32 length )
{
   tSIRF_UINT32 result = SIRF_FAILURE;
   CsrFileHandle *fh = NULL;
   
   /* return if the ID is beyond the max possible */
   if (storage_id < SIRF_PAL_STORAGE_MAX)
   {
       result = SIRF_SUCCESS;
   }

   if (SIRF_SUCCESS == result)
   {
      fh = store_id[storage_id];

      /* check if the file exists and the data pointer is not NULL */
      if((fh != NULL) && (data != NULL))
      {
         result = SIRF_FAILURE;
         if (CsrFileSeek(fh, offset, CSR_SEEK_SET) == CSR_RESULT_SUCCESS)/* check if we can go to the place we need get to */
         {
            CsrSize written;
            CsrResult res = CsrFileWrite(data,length,fh,&written); /* check if the write succeeded */
            if ((res == CSR_RESULT_SUCCESS) && (written == length))
            {
               if (CsrFileFlush(fh) == CSR_RESULT_SUCCESS)/* flush the file */
               {
                  result = SIRF_SUCCESS;
               }
            }
         }
      }
      else
      {
         result = SIRF_FAILURE;
      }
   }
   return result;
} /* SIRF_PAL_STORAGE_Write() */


/**
 * @brief Read data from the storage device.
 * @param[in] storage_id               Storage ID.
 * @param[in] offset                   Offset into the storage area.
 * @param[in] data                     Buffer to hold data.
 * @param[in] length                   Length of the data to read.
 * @return                             Result code.
 */
tSIRF_RESULT SIRF_PAL_STORAGE_Read( tSIRF_UINT32 storage_id, tSIRF_UINT32 offset, tSIRF_UINT8 *data, tSIRF_UINT32 length )
{
   tSIRF_UINT32 result = SIRF_PAL_STORAGE_NOT_AVAILABLE;
   CsrFileHandle *fh;
   CsrResult res;
   CsrSize bytesRead;

   /* return if the ID is beyond the max possible */
   if (storage_id >= SIRF_PAL_STORAGE_MAX)
   {
      return result;
   }
      
   fh = store_id[storage_id];

   /* check if the file exists and the data pointer is not NULL */
   if((fh != NULL) && (data != NULL))
   {
      if (CsrFileSeek(fh, offset, CSR_SEEK_SET) == CSR_RESULT_SUCCESS)/* check if we can go to the place we need get to */
      {
         res = CsrFileRead(data,length,fh,&bytesRead);
         if ((res == CSR_RESULT_SUCCESS) && (bytesRead > 0))/* check if the read succeeded */
         {
            result = SIRF_SUCCESS;
         }
         else
         {
            result = SIRF_PAL_STORAGE_EMPTY;
         }
      }
   }
      
   return result;

} /* SIRF_PAL_STORAGE_Read() */



/**
 * @}
 * End of file.
 */


