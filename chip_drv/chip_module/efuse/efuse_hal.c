/******************************************************************************
 **  File Name:    efuse_hal.c                                                *
 **  Author:       Jenny Deng                                                 *
 **  Date:         20/10/2009                                                 *
 **  Copyright:    2009 Spreadtrum, Incorporated. All Rights Reserved.        *
 **  Description:  This file defines the basic operation interfaces of        *
 **                EFuse initilize and operation. It provides read and        *
 **                writer interfaces of 0~5 efuse. Efuse 0 for Sn block.      *
 **                Efuse 1 to 4 for Hash blocks. Efuse 5 for control block.   *
 ******************************************************************************
 ******************************************************************************
 **  Edit History                                                             *
 **---------------------------------------------------------------------------*
 **  DATE               Author              Operation                         *
 **  20/10/2009         Jenny.Deng          Create.                           *
 **   26/06/2012        windy.wang          Update                            *
 ******************************************************************************/
#include "arm_reg.h"
#include "chip_plf_export.h"
#include "efuse_drvapi.h"
#include "efuse_phy.h"

/******************************************************************************
 * [Description] :                                                            *
 *      open efuse module                                                     *
 * [Global resource dependence]:                                              *
 * [Parameters]:                                                              *
 * [Return]:                                                                  *
 * [Note]:                                                                    *
******************************************************************************/
PUBLIC void EFUSE_HAL_Open (void)
{
    EFUSE_PHY_Init (TRUE);
}

/******************************************************************************
 * [Description] :                                                            *
 *      close efuse module                                                    *
 * [Global resource dependence]:                                              *
 * [Parameters]:                                                              *
 * [Return]:                                                                  *
 * [Note]:                                                                    *
******************************************************************************/
PUBLIC void EFUSE_HAL_Close (void)
{
    EFUSE_PHY_Init (FALSE);
}
/******************************************************************************
 * [Description] :                                                            *
 *      Get max num of the selected efuse                                     *
 * [Global resource dependence]:                                              *
 * [Parameters]:                                                              *
 * [Return]:                                                                  *
 *      max number of efuse blocks                                            *
 * [Note]:                                                                    *
 *    1.Efuse has several blocks.                                             *
 ******************************************************************************/
PUBLIC uint32 EFUSE_HAL_GetAllBlkNum (void)
{
    return (EFUSE_PHY_GetAppBlkLen (EFUSE_MAX_ID));
}

/******************************************************************************
 * [Description] :                                                            *
 *      Get max num of the selected efuse blocks, and these blocks are used   *
 *      to store hash                                                         *
 * [Global resource dependence]:                                              *
 * [Parameters]:                                                              *
 * [Return]:                                                                  *
 *      max bits of hash blocks                                               *
 * [Note]:                                                                    *
 *    1.Hash block is several efuse block, and there index of block are       *
 *      consecutive.                                                          *
 ******************************************************************************/
PUBLIC uint32 EFUSE_HAL_GetHashBlkLen (void)
{
    return (EFUSE_PHY_GetAppBlkLen (EFUSE_HASH_START_ID));
}

/******************************************************************************
 * [Description] :                                                            *
 *      Get max bits of the selected efuse blocks, and these blocks are used  *
 *      to store IMEA.                                                        *
 * [Global resource dependence]:                                              *
 * [Parameters]:                                                              *
 * [Return]:                                                                  *
 *      max bits of hash blocks                                               *
 * [Note]:                                                                    *
 *    1.Hash block is several efuse block, and there index of block are       *
 *      consecutive.                                                          *
 ******************************************************************************/
PUBLIC uint32 EFUSE_HAL_GetIMEABitLen (void)
{
    return EFUSE_PHY_GetAppBitLen (EFUSE_IMEI_START_ID);
}

/******************************************************************************
 * [Description] :                                                            *
 *      If an efuse block has not been written before, it is not locked.      *
 *      But if it had been written before, it is locked.                      *
 * [Global resource dependence]:                                              *
 * [Parameters]:                                                              *
 *      block_id : id number of the selected block                            *
 * [Return]:                                                                  *
 *      EFUSE_ID_ERROR  : block_id is wrong                                   *
 *      EFUSE_LOCKED    : block is protected                                  *
 *      EFUSE_NOT_LOCK  : block is not protected                              *
 * [Note]:                                                                    *
 *    1.If an efuse block is written, it would be protected at once.          *
 *    2.If an efuse block is protected, it would not be written any more.     *
 *    3.Each efuse block has only one writting operation.                     *
 ******************************************************************************/
PUBLIC EFUSE_RETURN_E EFUSE_HAL_IsLock (
    uint32 block_id)
{
    if (!EFUSE_PHY_IsBlkIdValid (block_id))
    {
        return EFUSE_ID_ERROR;
    }

    return EFUSE_PHY_IsLock (block_id);
}

/******************************************************************************
 * [Description] :                                                            *
 *      If these blocks have not been written before, they are not locked.    *
 *      But if they had been written before, they were locked. And these      *
 *      blocks are used to store IMEA                                         *
 * [Global resource dependence]:                                              *
 * [Parameters]:                                                              *
 *      bit_num         : number of bit                                       *
 * [Return]:                                                                  *
 *      EFUSE_PTR_NUM_ERROR    : num of bit is larger than limit              *
 *      EFUSE_LOCKED    : one of hash blocks is protected                     *
 *      EFUSE_NOT_LOCK  : all hash blocks are not protected                   *
 * [Note]:                                                                    *
 *    1.If an efuse block is written, it would be protected at once.          *
 *    2.If an efuse block is protected, it would not be written any more.     *
 *    3.Each efuse block has only one writting operation.                     *
 *    4.Hash block is several efuse block, and there index of block are       *
 *      consecutive.                                                          *
 ******************************************************************************/
PUBLIC EFUSE_RETURN_E EFUSE_HAL_IsLockImei (
    uint32 bit_num
)
{
    uint32 i,min_id,len;
    uint32 max_bit;
    EFUSE_RETURN_E result;

    max_bit = EFUSE_PHY_GetAppBitLen (EFUSE_IMEI_START_ID);
    min_id = EFUSE_PHY_GetBlkId (EFUSE_IMEI_START_ID);
    len = EFUSE_PHY_GetBlkNumFromBit (bit_num);

    if (max_bit < bit_num)
    {
        return EFUSE_PTR_NUM_ERROR;
    }

    result = EFUSE_NOT_LOCK;

    for (i=min_id; i< (min_id + len); i++)
    {
        if (EFUSE_LOCKED == EFUSE_PHY_IsLock (i))
        {
            result = EFUSE_LOCKED;
            break;
        }
    }

    return result;
}

/******************************************************************************
 * [Description] :                                                            *
 *      If these blocks have not been written before, they are not locked.    *
 *      But if they had been written before, they were locked. And these      *
 *      blocks are used to store hash.                                        *
 * [Global resource dependence]:                                              *
 * [Parameters]:                                                              *
 *      block_id : id number of the selected block                            *
 * [Return]:                                                                  *
 *      EFUSE_LOCKED    : one of hash blocks is protected                     *
 *      EFUSE_NOT_LOCK  : all hash blocks are not protected                   *
 * [Note]:                                                                    *
 *    1.If an efuse block is written, it would be protected at once.          *
 *    2.If an efuse block is protected, it would not be written any more.     *
 *    3.Each efuse block has only one writting operation.                     *
 *    4.Hash block is several efuse block, and there index of block are       *
 *      consecutive.                                                          *
 ******************************************************************************/
PUBLIC EFUSE_RETURN_E EFUSE_HAL_IsLockHash (void)
{
    uint32 min_id,max_id;
    uint32 i;
    EFUSE_RETURN_E result;

    min_id = EFUSE_PHY_GetBlkId (EFUSE_HASH_START_ID);
    max_id = EFUSE_PHY_GetBlkId (EFUSE_HASH_END_ID);

    result = EFUSE_NOT_LOCK;

    for (i=min_id; i<=max_id; i++)
    {
        if (EFUSE_LOCKED == EFUSE_PHY_IsLock (i))
        {
            result = EFUSE_LOCKED;
            break;
        }
    }

    return result;
}

/******************************************************************************
 * [Description] :                                                            *
 *      read data from the selected efuse block                               *
 * [Global resource dependence]:                                              *
 * [Parameters]:                                                              *
 *      block_id : id number of the selected bllock                           *
 *      des_data_ptr : the pointer of data reading from the selected block    *
 * [Return]:                                                                  *
 *      EFUSE_PTR_NULL_ERROR   : pointer is NULL                              *
 *      EFUSE_ID_ERROR         : block_id is wrong                            *
 *      EFUSE_READ_FAIL        : fail to read data                            *
 *      EFUSE_RESULT_SUCCESS   : successful to read data                      *
 * [Note]:                                                                    *
 *    1.Return is the status of reading operation, such as successful or fail.*
 *    2.Data of the selected efuse block is stored in the 2rd parameter.      *
 ******************************************************************************/
PUBLIC EFUSE_RETURN_E EFUSE_HAL_Read (
    uint32 block_id,
    uint32 *des_data_ptr)
{
    if (NULL == des_data_ptr)
    {
        return EFUSE_PTR_NULL_ERROR;
    }

    if (!EFUSE_PHY_IsBlkIdValid (block_id))
    {
        return EFUSE_ID_ERROR;
    }

    return EFUSE_PHY_Read (block_id, des_data_ptr);
}

/******************************************************************************
 * [Description] :                                                            *
 *      read data from the selected efuse block                               *
 * [Global resource dependence]:                                              *
 * [Parameters]:                                                              *
 *      block_id : id number of the selected bllock                           *
 *      des_data_ptr : the pointer of data reading from the selected block    *
 * [Return]:                                                                  *
 *      EFUSE_PTR_NULL_ERROR   : pointer is NULL                              *
 *      EFUSE_ID_ERROR         : block_id is wrong                            *
 *      EFUSE_READ_FAIL        : fail to read data                            *
 *      EFUSE_RESULT_SUCCESS   : successful to read data                      *
 * [Note]:                                                                    *
 *    1.Return is the status of reading operation, such as successful or fail.*
 *    2.Data of the selected efuse block is stored in the 2rd parameter.      *
 ******************************************************************************/
PUBLIC EFUSE_RETURN_E EFUSE_HAL_Read_Ex (
    uint32 block_id,
    uint32 *des_data_ptr)
{
    if (NULL == des_data_ptr)
    {
        return EFUSE_PTR_NULL_ERROR;
    }

    if (!EFUSE_PHY_IsBlkIdValid (block_id))
    {
        return EFUSE_ID_ERROR;
    }

    return EFUSE_PHY_Read_Ex (block_id, des_data_ptr);
}
/******************************************************************************
 * [Description] :                                                            *
 *      Write data to hash blocks                                                *
 * [Global resource dependence]:                                              *
 * [Parameters]:                                                              *
 *      src_dat_ptr : the pointer of data that is to be written               *
 * [Return]:                                                                  *
 *      EFUSE_PTR_NULL_ERROR          : pointer is NULL                       *
 *      EFUSE_PTR_NUM_ERROR           : num of bit is larger than limit       *
 *      EFUSE_LOCKED                  : block is protected                    *
 *      EFUSE_WRITE_FAIL              : write fail                            *
 *      EFUSE_RESULT_SUCCESS          : successful to write data, and data of *
 *                                      the block is equal to writting data   *
 *      EFUSE_WRITE_SOFT_COMPARE_FAIL : successful to write data, but data of *
 *                                      the block is not equal to writting    *
 *                                      data                                  *
 *      EFUSE_WRITE_HARD_COMPARE_FAIL : fail to write data                    *
 *      EFUSE_WRITE_SOFT_HARD_COMPARE_: fail to write data, and data of the   *
 *                                      block is not equal to writting data   *
 *      EFUSE_WRITE_VERIFY_FAIL       : fail to read after write              *
 * [Note]:                                                                    *
 *    1.Return is the status of reading operation, such as successful or fail.*
 *    2.Each block is only one writing operation.                             *
 *    3.Hash block is several efuse block, and there index of block are       *
 *      consecutive.                                                          *
 *      4.Don't write BIT_31                                                       *
 ******************************************************************************/
PUBLIC EFUSE_RETURN_E Efuse_HAL_WriteSecurityHash(uint32 *src_dat_ptr)
{   
    uint32 efuse_data[5];
    EFUSE_RETURN_E result = EFUSE_RESULT_SUCCESS;

    if (NULL == src_dat_ptr)
    {
        return EFUSE_PTR_NULL_ERROR;
    }
    
    //Firstly, Enable PGM mode, EFUSE_PHY_Init set PGM_EN and CLK_EFUSE_EN
    EFUSE_HAL_Open();    

    result = EFUSE_HAL_ReadHash(efuse_data);
    if(EFUSE_RESULT_SUCCESS != result)
    {
        return result;
    }

    if((efuse_data[0] & BIT_31) || (efuse_data[1] & BIT_31) || (efuse_data[2] & BIT_31)
        || (efuse_data[3] & BIT_31) || (efuse_data[4] & BIT_31))
    {
        return EFUSE_LOCKED;
    }
    
    result = EFUSE_HAL_WriteHash(src_dat_ptr);

    EFUSE_HAL_Close();

    return result;
}


/******************************************************************************
 * [Description] :                                                            *
 *      Read data from these selected efuse blocks.And these blocks are used  *
 *      to store IMEA.                                                        *
 * [Global resource dependence]:                                              *
 * [Parameters]:                                                              *
 *      des_data_ptr : the pointer of data reading from the selected block    *
 *      bit_num      : number of bit                                          *
 * [Return]:                                                                  *
 *      EFUSE_PTR_NULL_ERROR   : pointer is NULL                              *
 *      EFUSE_PTR_NUM_ERROR    : num of bit is larger than limit              *
 *      EFUSE_ID_ERROR  : bit max length used for IMEI is less than 1st       *
 *                        parameter                                           *
 *      EFUSE_READ_FAIL        : fail to read data                            *
 *      EFUSE_RESULT_SUCCESS   : successful to read data                      *
 * [Note]:                                                                    *
 *    1.Return is the status of reading operation, such as successful or fail.*
 *    2.Data of the selected efuse block is stored in the 2rd parameter.      *
 *    3.Hash block is several efuse block, and there index of block are       *
 *      consecutive.                                                          *
 ******************************************************************************/
PUBLIC EFUSE_RETURN_E EFUSE_HAL_ReadImei (
    uint8 *des_data_ptr,
    uint32 bit_num
)
{
    uint32 i,min_id,len;
    uint32 max_bit;
    uint32 *efuse_ptr,*temp_ptr;
    EFUSE_RETURN_E result;

    if (NULL == des_data_ptr)
    {
        return EFUSE_PTR_NULL_ERROR;
    }

    result = EFUSE_RESULT_SUCCESS;
    max_bit = EFUSE_PHY_GetAppBitLen (EFUSE_IMEI_START_ID);
    min_id = EFUSE_PHY_GetBlkId (EFUSE_IMEI_START_ID);
    len    = EFUSE_PHY_GetBlkNumFromBit (bit_num);
    efuse_ptr = EFUSE_PHY_GetEfusePtr();
    temp_ptr = efuse_ptr;

    if (max_bit < bit_num)
    {
        return EFUSE_PTR_NUM_ERROR;
    }

    for (i = min_id; i < (min_id + len); i ++)
    {
        result = EFUSE_PHY_Read_Ex (i, temp_ptr);

        if (EFUSE_RESULT_SUCCESS != result)
        {
            return result;
        }

        temp_ptr ++;
    }

    if (FALSE == EFUSE_PHY_SplitData ( (uint8 *) efuse_ptr,des_data_ptr,len,bit_num,FALSE))
    {
        result = EFUSE_READ_FAIL;
    }

    return result;
}

/******************************************************************************
 * [Description] :                                                            *
 *      Read some data from these selected efuse blocks. And these blocks are *
 *      used to store hash.                                                   *
 * [Global resource dependence]:                                              *
 * [Parameters]:                                                              *
 *      des_data_ptr : the pointer of data reading from the selected block    *
 * [Return]:                                                                  *
 *      EFUSE_PTR_NULL_ERROR   : pointer is NULL                              *
 *      EFUSE_READ_FAIL        : fail to read data                            *
 *      EFUSE_RESULT_SUCCESS   : successful to read data                      *
 * [Note]:                                                                    *
 *    1.Return is the status of reading operation, such as successful or fail.*
 *    2.Data of the selected efuse block is stored in the 2rd parameter.      *
 *    3.Hash block is several efuse block, and there index of block are       *
 *      consecutive.                                                          *
 ******************************************************************************/
PUBLIC EFUSE_RETURN_E EFUSE_HAL_ReadHash (
    uint32 *des_data_ptr)
{
    uint32 i,min_id,max_id;
    EFUSE_RETURN_E result;

    if (NULL == des_data_ptr)
    {
        return EFUSE_PTR_NULL_ERROR;
    }

    result = EFUSE_RESULT_SUCCESS;
    min_id = EFUSE_PHY_GetBlkId (EFUSE_HASH_START_ID);
    max_id = EFUSE_PHY_GetBlkId (EFUSE_HASH_END_ID);

    for (i = min_id; i <= max_id; i++)
    {
        result = EFUSE_HAL_Read (i, des_data_ptr);

        if (EFUSE_RESULT_SUCCESS != result)
        {
            return result;
        }

        des_data_ptr++;
    }

    return result;
}
/******************************************************************************
 * [Description] :                                                            *
 *      write data to the selected efuse block                                *
 * [Global resource dependence]:                                              *
 * [Parameters]:                                                              *
 *      block_id : id number of the selected block                            *
 *      write_data : the pointer of data that is to be written                *
 * [Return]:                                                                  *
 *      EFUSE_ID_ERROR                : block_id is wrong                     *
 *      EFUSE_LOCKED                  : block is protected                    *
 *      EFUSE_RESULT_SUCCESS          : successful to write data, and data of *
 *                                      the block is equal to writting data   *
 *      EFUSE_WRITE_SOFT_COMPARE_FAIL : successful to write data, but data of *
 *                                      the block is not equal to writting    *
 *                                      data                                  *
 *      EFUSE_WRITE_HARD_COMPARE_FAIL : fail to write data                    *
 *      EFUSE_WRITE_SOFT_HARD_COMPARE_: fail to write data, and data of the   *
 *                                      block is not equal to writting data   *
 *      EFUSE_WRITE_VERIFY_FAIL       : fail to read after write              *
 * [Note]:                                                                    *
 *    1.Return is the status of reading operation, such as successful or fail.*
 *    2.Each block is only one writing operation.                             *
 ******************************************************************************/
PUBLIC EFUSE_RETURN_E EFUSE_HAL_Write (
    uint32 block_id,
    uint32 write_data)
{
    if (!EFUSE_PHY_IsBlkIdValid (block_id))
    {
        return EFUSE_ID_ERROR;
    }

#if 0
    /* if the block has locked return now */
    if (EFUSE_HAL_IsLock (block_id) == EFUSE_LOCKED)
    {
        return EFUSE_LOCKED;
    }
#endif

    return EFUSE_PHY_Write (block_id, write_data);
}

/******************************************************************************
 * [Description] :                                                            *
 *      Write data to these selected blocks.And these blocks are used to      *
 *      store IMEA                                                            *
 * [Global resource dependence]:                                              *
 * [Parameters]:                                                              *
 *      src_dat_ptr : the pointer of data that is to be written               *
 *      bit_num     : number of bit                                           *
 * [Return]:                                                                  *
 *      EFUSE_PTR_NULL_ERROR          : pointer is NULL                       *
 *      EFUSE_PTR_NUM_ERROR           : num of bit is larger than limit       *
 *      EFUSE_LOCKED                  : block is protected                    *
 *      EFUSE_WRITE_FAIL              : write fail                            *
 *      EFUSE_RESULT_SUCCESS          : successful to write data, and data of *
 *                                      the block is equal to writting data   *
 *      EFUSE_WRITE_SOFT_COMPARE_FAIL : successful to write data, but data of *
 *                                      the block is not equal to writting    *
 *                                      data                                  *
 *      EFUSE_WRITE_HARD_COMPARE_FAIL : fail to write data                    *
 *      EFUSE_WRITE_SOFT_HARD_COMPARE_: fail to write data, and data of the   *
 *                                      block is not equal to writting data   *
 *      EFUSE_WRITE_VERIFY_FAIL       : fail to read after write              *
 * [Note]:                                                                    *
 *    1.Return is the status of reading operation, such as successful or fail.*
 *    2.Each block is only one writing operation.                             *
 *    3.Hash block is several efuse block, and there index of block are       *
 *      consecutive.                                                          *
 ******************************************************************************/
PUBLIC EFUSE_RETURN_E EFUSE_HAL_WriteImei (
    uint8 *src_dat_ptr,
    uint32 bit_num
)
{
    uint32 i,min_id,len;
    uint32 *efuse_ptr;
    uint32 max_bit;
    EFUSE_RETURN_E result;

    if (NULL == src_dat_ptr)
    {
        return EFUSE_PTR_NULL_ERROR;
    }

    result = EFUSE_RESULT_SUCCESS;
    max_bit = EFUSE_PHY_GetAppBitLen (EFUSE_IMEI_START_ID);
    min_id = EFUSE_PHY_GetBlkId (EFUSE_IMEI_START_ID);
    len = EFUSE_PHY_GetBlkNumFromBit (bit_num);
    efuse_ptr = EFUSE_PHY_GetEfusePtr();

    if (max_bit < bit_num)
    {
        return EFUSE_PTR_NUM_ERROR;
    }

    for (i = min_id; i < (min_id + len); i ++)
    {
        if (EFUSE_LOCKED == EFUSE_PHY_IsLock (i))
        {
            return EFUSE_LOCKED;
        }
    }

    if (FALSE == EFUSE_PHY_SplitData (src_dat_ptr, (uint8 *) efuse_ptr,len,bit_num,TRUE))
    {
        return EFUSE_PTR_NUM_ERROR;
    }
    else
    {
        for (i = min_id; i < (min_id + len); i ++)
        {
            result = EFUSE_PHY_Write (i, (*efuse_ptr) | BIT_31);

            if (EFUSE_RESULT_SUCCESS != result)
            {
                return result;
            }

            efuse_ptr ++;
        }
    }

    return result;
}

/******************************************************************************
 * [Description] :                                                            *
 *      Write some data to these selected blocks.And these blocks are used to *
 *      store hash.                                                           *
 * [Global resource dependence]:                                              *
 * [Parameters]:                                                              *
 *      src_dat_ptr : the pointer of data that is to be written               *
 * [Return]:                                                                  *
 *      EFUSE_PTR_NULL_ERROR          : pointer is NULL                       *
 *      EFUSE_LOCKED                  : block is protected                    *
 *      EFUSE_RESULT_SUCCESS          : successful to write data, and data of *
 *                                      the block is equal to writting data   *
 *      EFUSE_WRITE_SOFT_COMPARE_FAIL : successful to write data, but data of *
 *                                      the block is not equal to writting    *
 *                                      data                                  *
 *      EFUSE_WRITE_HARD_COMPARE_FAIL : fail to write data                    *
 *      EFUSE_WRITE_SOFT_HARD_COMPARE_: fail to write data, and data of the   *
 *                                      block is not equal to writting data   *
 *      EFUSE_WRITE_VERIFY_FAIL       : fail to read after write              *
 * [Note]:                                                                    *
 *    1.Return is the status of reading operation, such as successful or fail.*
 *    2.Each block is only one writing operation.                             *
 *    3.Hash block is several efuse block, and there index of block are       *
 *      consecutive.                                                          *
 ******************************************************************************/
PUBLIC EFUSE_RETURN_E EFUSE_HAL_WriteHash (
    uint32 *src_dat_ptr
)
{
    uint32 i,min_id,max_id;
    uint32 src = 0;
    EFUSE_RETURN_E result;

    if (NULL == src_dat_ptr)
    {
        return EFUSE_PTR_NULL_ERROR;
    }

    min_id = EFUSE_PHY_GetBlkId (EFUSE_HASH_START_ID);
    max_id = EFUSE_PHY_GetBlkId (EFUSE_HASH_END_ID);
    
    for (i = min_id; i <= max_id; i ++)
    {
        if (EFUSE_LOCKED == EFUSE_PHY_IsLock (i))
        {
            return EFUSE_LOCKED;
        }
    }

    result = EFUSE_RESULT_SUCCESS;
    
    for (i = min_id; i <= max_id; i++)
    {
        src = (*src_dat_ptr);
        result = EFUSE_HAL_Write (i, src);

        if (EFUSE_RESULT_SUCCESS != result)
        {
            return result;
        }

        src_dat_ptr++;
    }

    return result;
}

PUBLIC EFUSE_RETURN_E EFUSE_HAL_ReadSecurityFlag()
{
    EFUSE_RETURN_E rRet = EFUSE_RESULT_SUCCESS;
    uint32 efuse_harsh_data[5];

    EFUSE_HAL_Open();

    rRet = EFUSE_HAL_ReadHash((unsigned int *)efuse_harsh_data);
    EFUSE_HAL_Close();

#if defined(PLATFORM_SC6531EFM) || defined(PLATFORM_UIX8910)
    if((rRet == EFUSE_RESULT_SUCCESS) && (efuse_harsh_data[0]&BIT_31) && (efuse_harsh_data[1]&BIT_31) 
        && (efuse_harsh_data[2]&BIT_31) && (efuse_harsh_data[3]&BIT_31) && (efuse_harsh_data[4]&BIT_31))
#else
    if((rRet == EFUSE_RESULT_SUCCESS) && (efuse_harsh_data[0]&BIT_31))
#endif    
    {
        rRet = EFUSE_RESULT_SUCCESS;
    }
    else
    {
        rRet = EFUSE_READ_FAIL;
    }

    return rRet;
}

#if !defined(PLATFORM_SC6531EFM) && !defined(PLATFORM_UIX8910)
/******************************************************************************
 * [Description] :                                                            *
 *      read calibration data stored in calibration data                      *
 * [Global resource dependence]:                                              *
 * [Parameters]:                                                              *
 * [Return]:                                                                  *
 * [Note]:                                                                    *
 *      This function is used for calibartion.                                *
 ******************************************************************************/
PUBLIC BOOLEAN EFUSE_HAL_CalibrationGet (uint32 *p_cal_data)
{
    uint32 data = 0;
    uint32 efuse_id = EFUSE_PHY_GetBlkId (EFUSE_CAL_ID);
    unsigned short adc_temp;

    EFUSE_HAL_Open();
    EFUSE_HAL_Read (efuse_id,&data);
    EFUSE_HAL_Close();

    if ( (!data) || (p_cal_data == NULL))
    {
        return SCI_FALSE;
    }

    //adc 3.6V
    adc_temp = ( (data>>8) & 0x00FF) + BASE_ADC_P0 - ADC_DATA_OFFSET;
    p_cal_data[1] = (VOL_P0) | (adc_temp << 16);

    //adc 4.2V
    adc_temp = (data & 0x00FF) + BASE_ADC_P1 - ADC_DATA_OFFSET;
    p_cal_data[0] = (VOL_P1) | (adc_temp << 16);

    return SCI_TRUE;
}

/******************************************************************************
 * [Description] :                                                            *
 *      read CCCV data stored in calibarion data                              *
 * [Global resource dependence]:                                              *
 * [Parameters]:                                                              *
 * [Return]:                                                                  *
 * [Note]:                                                                    *
 *      This function is used for calibration.                                *
 ******************************************************************************/
PUBLIC BOOLEAN EFUSE_HAL_CCCVPointGet (uint32 *p_cal_cccv)
{
    uint32 data = 0;
    uint32 efuse_id = EFUSE_PHY_GetBlkId (EFUSE_CAL_ID);

    EFUSE_HAL_Open();
    EFUSE_HAL_Read (efuse_id,&data);
    EFUSE_HAL_Close();

    if ( (!data) || (p_cal_cccv == NULL))
    {
        return SCI_FALSE;
    }

    *p_cal_cccv = (data >> 16) &0x1F;
    return SCI_TRUE;
}

#else

//stander adc value and vol
/*
 * VBAT_SENSE input 4.2V and 3.6V separately and get the ADC output data Da0 and Db0, then divided by 4 to get Da=Da0/4, Db=Db0/4. 
 * The ideal value Da_ideal is 856 and Db_ideal is 733.
 * Calculate deltaA=Da-Da_ideal+128=Da-856+128, save deltaA in EFUSE block18[7:0],  
 * deltaB=Db-Db_ideal+128=Db-733+128, save deltaB in EFUSE block18[15:8].
*/
#define EFUSE_HAL_ADC_4200_DELTA        (856)
#define EFUSE_HAL_ADC_3600_DELTA        (733)

/*
 * The ideal value De_ideal=80, Df_ideal=833. 
 * Calculate deltaE=De-De_ideal+128=De-80+128, save deltaE in EFUSE block19[15:7],  
 * deltaF=Df-Df_ideal+128=Df-833+128, save deltaF in EFUSE block19[7:0]
*/
#define EFUSE_HAL_ADC_1000_DELTA          (833)
#define EFUSE_HAL_ADC_100_DELTA           (80)

#define EFUSE_HAL_ADC_1000_DELTA_MIC      818
#define EFUSE_HAL_ADC_100_DELTA_MIC        82

/******************************************************************************
 * [Description] : compatible with efuse map v0.5 and versions earlier        *
 *      get bt/fm calibarion data frome efuse                                 *
 * [Global resource dependence]:                                              *
 * [Parameters]:                                                              *
 * [Return]:                                                                  *
 * [Note]:                                                                    *
 *      This function is used for calibration.                                *
 ******************************************************************************/
PUBLIC BOOLEAN EFUSE_HAL_GetBTCaliData (uint32 *cali_data)
{
    uint32 data = 0;
    uint32 efuse_id = EFUSE_PHY_GetBlkId (EFUSE_BT_CAL_ID);
    EFUSE_RETURN_E result = EFUSE_RESULT_SUCCESS;

    if (cali_data == NULL)
    {
        return SCI_FALSE; //Don't need calibration
    }

    EFUSE_HAL_Open();
    
    result = EFUSE_HAL_Read (efuse_id,&data);
    if(EFUSE_RESULT_SUCCESS != result)
    {
        EFUSE_HAL_Close();
        *cali_data = 0;
        return SCI_FALSE; //Don't need calibration
    }
    
    EFUSE_HAL_Close();

    *cali_data = (data >> 14) & 0xf; //R calibration

    return SCI_TRUE;
}

/******************************************************************************
 * [Description] : compatible with efuse map v0.5 and versions earlier        *
 *      read IB data stored in calibarion data                                *
 * [Global resource dependence]:                                              *
 * [Parameters]:                                                              *
 * [Return]:                                                                  *
 * [Note]:                                                                    *
 *      This function is used for calibration.                                *
 ******************************************************************************/
PUBLIC BOOLEAN EFUSE_HAL_GetIBCaliData (uint32 *cali_data)
{
    uint32 data = 0;
    uint32 efuse_id = EFUSE_PHY_GetBlkId (EFUSE_IB_CAL_ID);
    EFUSE_RETURN_E result = EFUSE_RESULT_SUCCESS;

    if (cali_data == NULL)
    {
        return SCI_FALSE; //Don't need calibration
    }

    EFUSE_HAL_Open();
    
    result = EFUSE_HAL_Read (efuse_id,&data);
    if(EFUSE_RESULT_SUCCESS != result)
    {
        EFUSE_HAL_Close();
        *cali_data = 0;
        return SCI_FALSE; //Don't need calibration
    }
    
    EFUSE_HAL_Close();

    *cali_data = (data >> 6) &0x3F;
    
    return SCI_TRUE;
}

/******************************************************************************
 * [Description] : compatible with efuse map v0.5 and versions earlier        *
 *      read charge cv data stored in calibarion data                         *
 * [Global resource dependence]:                                              *
 * [Parameters]:                                                              *
 * [Return]:                                                                  *
 * [Note]:                                                                    *
 *      This function is used for calibration.                                *
 ******************************************************************************/
PUBLIC BOOLEAN EFUSE_HAL_GetCHGCV(uint32 *cali_data)
{
    uint32 data = 0;
    uint32 efuse_id = EFUSE_PHY_GetBlkId (EFUSE_CHRG_CV_CAL_ID);
    EFUSE_RETURN_E result = EFUSE_RESULT_SUCCESS;

    if (cali_data == NULL)
    {
        return SCI_FALSE; //Don't need calibration
    }

    EFUSE_HAL_Open();
    
    result = EFUSE_HAL_Read (efuse_id,&data);
    if(EFUSE_RESULT_SUCCESS != result)
    {
        EFUSE_HAL_Close();
        *cali_data = 0;
        return SCI_FALSE; //Don't need calibration
    }
    
    EFUSE_HAL_Close();

    SCI_TRACE_LOW("EFUSE_HAL_GetCHGCV result=%d, data =%d, efuse_id=%d", result, data, efuse_id);
    *cali_data = data &0x3F;
    
    return SCI_TRUE;
}

/******************************************************************************
 * [Description] : compatible with efuse map v0.5 and versions earlier        *
 *      get adc headmic calibarion data frome efuse                           *
 * [Global resource dependence]:                                              *
 * [Parameters]:                                                              *
 * [Return]:                                                                  *
 * [Note]:                                                                    *
 *      This function is used for calibration.                                *
 ******************************************************************************/
PUBLIC BOOLEAN EFUSE_HAL_GetADCHeadmicCali(uint32 *cali_data)
{
    uint32 data = 0;
    uint32 efuse_id = EFUSE_PHY_GetBlkId (EFUSE_ADC_CAL_HEADMIC_ID);
    uint8 cali_in_efuse[2] = {0};
    EFUSE_RETURN_E result = EFUSE_RESULT_SUCCESS;

    if (cali_data == NULL)
    {
        return SCI_FALSE; //Don't need calibration
    }
    
    EFUSE_HAL_Open();
    
    result = EFUSE_HAL_Read (efuse_id,&data);
    if(EFUSE_RESULT_SUCCESS != result)
    {
        EFUSE_HAL_Close();
        cali_data[0] = 0;
        cali_data[1] = 0;
        return SCI_FALSE; //Don't need calibration
    }
    
    EFUSE_HAL_Close();

    cali_in_efuse[0] = ((data >> 15) & 0xff); //(small range) @1.0V offset
    cali_in_efuse[1] = ((data >> 23) & 0xff); //(small range) @0.1V offset
    
    if((EFUSE_HAL_ADC_100_DELTA_MIC + cali_in_efuse[1]) < 128)
    {
        cali_data[0] = 0;
        cali_data[1] = 0;
        return SCI_FALSE; //Don't need calibration
    }

    cali_data[0] = (EFUSE_HAL_ADC_1000_DELTA_MIC+ cali_in_efuse[0] - 128) * 4;
    cali_data[1] = (EFUSE_HAL_ADC_100_DELTA_MIC+ cali_in_efuse[1] - 128) * 4;

    return SCI_TRUE;
}

/******************************************************************************
 * [Description] : compatible with efuse map v0.5 and versions earlier        *
 *      get adc big scale calibarion data frome efuse                         *
 * [Global resource dependence]:                                              *
 * [Parameters]:                                                              *
 * [Return]:                                                                  *
 * [Note]:                                                                    *
 *      This function is used for calibration.                                *
 ******************************************************************************/
PUBLIC BOOLEAN EFUSE_HAL_GetADCBigScaleCali (uint32 *cali_data)
{
    uint32          data = 0;
    uint32          efuse_id = EFUSE_PHY_GetBlkId (EFUSE_ADC_CAL_BIG_ID);
    uint8           cali_in_efuse[2] = {0};
    EFUSE_RETURN_E  result = EFUSE_RESULT_SUCCESS;

    SCI_TRACE_LOW("EFUSE_HAL_GetADCBigScaleCali, efuse_id=%d", efuse_id);
    
    if (cali_data == NULL)
    {
        return SCI_FALSE; //Don't need calibration
    }
    
    EFUSE_HAL_Open();
    
    result = EFUSE_HAL_Read (efuse_id,&data);

    SCI_TRACE_LOW("EFUSE_HAL_GetADCBigScaleCali, efuse_id=%d, result=%d, data=%d", efuse_id, result, data);
    
    if (EFUSE_RESULT_SUCCESS != result)
    {
        EFUSE_HAL_Close();
        cali_data[0] = 0;
        cali_data[1] = 0;
        return SCI_FALSE; //Don't need calibration
    }
    
    EFUSE_HAL_Close();
    
    cali_in_efuse[0] = ((data >> 0) & 0xff); //(Big range) @4.2V offset
    cali_in_efuse[1] = ((data >> 8) & 0xff); //(Big range) @3.6V offset

    cali_data[0] = (EFUSE_HAL_ADC_4200_DELTA + cali_in_efuse[0] - 128) * 4;
    cali_data[1] = (EFUSE_HAL_ADC_3600_DELTA + cali_in_efuse[1] - 128) * 4;

    return SCI_TRUE;
}

/******************************************************************************
 * [Description] : compatible with efuse map v0.5 and versions earlier        *
 *      get adc small scale calibarion data frome efuse                       *
 * [Global resource dependence]:                                              *
 * [Parameters]:                                                              *
 * [Return]:                                                                  *
 * [Note]:                                                                    *
 *      This function is used for calibration.                                *
 ******************************************************************************/
PUBLIC BOOLEAN EFUSE_HAL_GetADCSmallScaleCali (uint32 *cali_data)
{
    uint32          data = 0;
    uint32          efuse_id;
    uint32 read_efuse[2];
    uint32 * ptr = NULL;
    uint8 cali_in_efuse[2] = {0};
    EFUSE_RETURN_E result = EFUSE_RESULT_SUCCESS;

    if (NULL == cali_data)
    {
        return SCI_FALSE; //Don't need calibration
    }
    
    efuse_id = EFUSE_PHY_GetBlkId (EFUSE_ADC_CAL_SMALL_ID);

    EFUSE_HAL_Open();
    
    result = EFUSE_HAL_Read (efuse_id, &data);
    if(EFUSE_RESULT_SUCCESS != result)
    {
        EFUSE_HAL_Close();
        cali_data[0] = 0;
        cali_data[1] = 0;
        return SCI_FALSE; //Don't need calibration
    }
    
    SCI_TRACE_LOW("EFUSE_HAL_GetADCSmallScaleCali, efuse_id=%d, result=%d, data=%d", efuse_id, result, data);

    EFUSE_HAL_Close();
    
    cali_in_efuse[0] = ((data >> 0) & 0xff); //(small range) @1.0V offset
    cali_in_efuse[1] = ((data >> 8) & 0xff);  //(small range) @0.1V offset
    
    if((EFUSE_HAL_ADC_100_DELTA + cali_in_efuse[1]) < 128)
    {
        cali_data[0] = 0;
        cali_data[1] = 0;
        return SCI_FALSE; //Don't need calibration
    }
    
    cali_data[0] = (EFUSE_HAL_ADC_1000_DELTA + cali_in_efuse[0]  - 128) * 4;
    cali_data[1] = (EFUSE_HAL_ADC_100_DELTA  + cali_in_efuse[1]  - 128) * 4;

    return SCI_TRUE;
}

/******************************************************************************
 * [Description] : compatible with efuse map v0.5 and versions earlier        *
 *      get ldo core ref calibarion data frome efuse                          *
 * [Global resource dependence]:                                              *
 * [Parameters]:                                                              *
 * [Return]:                                                                  *
 * [Note]:                                                                    *
 *      This function is used for calibration.                                *
 ******************************************************************************/
PUBLIC BOOLEAN EFUSE_HAL_GetLDOBRefCaliData (uint32 *cali_data)
{
    uint32 data = 0;
    uint32 efuse_id = EFUSE_PHY_GetBlkId (EFUSE_LDOB_REF_CAL_ID);
    EFUSE_RETURN_E result = EFUSE_RESULT_SUCCESS;

    if (NULL == cali_data)
    {
        return SCI_FALSE; //Don't need calibration
    }

    EFUSE_HAL_Open();
    
    result = EFUSE_HAL_Read (efuse_id, &data);
    if(EFUSE_RESULT_SUCCESS != result)
    {
        EFUSE_HAL_Close();
        *cali_data = 0;
        return SCI_FALSE; //Don't need calibration
    }

    EFUSE_HAL_Close();

    *cali_data = data & 0x1f; //LDOB_REFTRIM
    
    return SCI_TRUE;
}

/******************************************************************************
 * [Description] : efuse map v0.6                                             *
 *      get cpll ldo calibarion data frome efuse                              *
 * [Global resource dependence]:                                              *
 * [Parameters]:                                                              *
 * [Return]:                                                                  *
 * [Note]:                                                                    *
 *      This function is used for calibration.                                *
 ******************************************************************************/
PUBLIC BOOLEAN EFUSE_HAL_GetCpllLdoCaliData (uint32 *cali_data)
{
    uint32 data = 0;
    uint32 efuse_id = EFUSE_PHY_GetBlkId (EFUSE_CPLL_LDO_CAL_ID);
    EFUSE_RETURN_E result = EFUSE_RESULT_SUCCESS;

    if (cali_data == NULL)
    {
        return SCI_FALSE; //Don't need calibration
    }

    EFUSE_HAL_Open();
    
    result = EFUSE_HAL_Read (efuse_id,&data);
    if(EFUSE_RESULT_SUCCESS != result)
    {
        EFUSE_HAL_Close();
        *cali_data = 0;
        return SCI_FALSE; //Don't need calibration
    }
    
    EFUSE_HAL_Close();

    *cali_data = (data >> 18) & 0x1f; //CPLL LDO calibration

    if(0 == *cali_data)
    {
        return SCI_FALSE; //Don't need calibration
    }

    return SCI_TRUE;
}

/******************************************************************************
 * [Description] : efuse map v0.6                                             *
 *      get audio calibarion data frome efuse                                 *
 * [Global resource dependence]:                                              *
 * [Parameters]:                                                              *
 * [Return]:                                                                  *
 * [Note]:                                                                    *
 *      This function is used for calibration.                                *
 ******************************************************************************/
PUBLIC BOOLEAN EFUSE_HAL_GetAVDDVBCaliData (uint32 *cali_data)
{
    uint32 data = 0;
    uint32 efuse_id = EFUSE_PHY_GetBlkId (EFUSE_AUD_CAL_ID);
    EFUSE_RETURN_E result = EFUSE_RESULT_SUCCESS;

    if (cali_data == NULL)
    {
        return SCI_FALSE; //Don't need calibration
    }

    EFUSE_HAL_Open();
    
    result = EFUSE_HAL_Read (efuse_id,&data);
    if(EFUSE_RESULT_SUCCESS != result)
    {
        EFUSE_HAL_Close();
        *cali_data = 0;
        return SCI_FALSE; //Don't need calibration
    }
    
    EFUSE_HAL_Close();

    *cali_data = (data >> 13) & 0x1f; //LDO_AVDDVB

    if(0 == *cali_data)
    {
        return SCI_FALSE; //Don't need calibration
    }

    return SCI_TRUE;
}

/******************************************************************************
 * [Description] : efuse map v0.6                                             *
 *      get ldo core sleep calibarion data frome efuse                        *
 * [Global resource dependence]:                                              *
 * [Parameters]:                                                              *
 * [Return]:                                                                  *
 * [Note]:                                                                    *
 *      This function is used for calibration.                                *
 ******************************************************************************/
PUBLIC BOOLEAN EFUSE_HAL_GetLDOCoreSLPCaliData (uint32 *cali_data)
{
    uint32 data = 0;
    uint32 efuse_id = EFUSE_PHY_GetBlkId (EFUSE_LDOCORE_SLP_CAL_ID);
    EFUSE_RETURN_E result = EFUSE_RESULT_SUCCESS;

    if (cali_data == NULL)
    {
        return SCI_FALSE; //Don't need calibration
    }

    EFUSE_HAL_Open();
    
    result = EFUSE_HAL_Read (efuse_id,&data);
    if(EFUSE_RESULT_SUCCESS != result)
    {
        EFUSE_HAL_Close();
        *cali_data = 0;
        return SCI_FALSE; //Don't need calibration
    }
    
    EFUSE_HAL_Close();

    *cali_data = (data >> 7) & 0x2f; //LDO_VDDCORE_SLP

    if(0 == *cali_data)
    {
        return SCI_FALSE; //Don't need calibration
    }

    return SCI_TRUE;
}

/******************************************************************************
 * [Description] : compatible with efuse map v0.5 and versions earlier        *
 *      read dcxo trim regs stored in calibarion data                         *
 * [Global resource dependence]:                                              *
 * [Parameters]:                                                              *
 * [Return]:                                                                  *
 * [Note]:                                                                    *
 *      This function is used for calibration.                                *
 ******************************************************************************/
PUBLIC BOOLEAN EFUSE_HAL_GetDCXOTrim(uint32 *dcxotrim_data)
{
    uint32 data = 0;
    uint32 efuse_id = EFUSE_PHY_GetBlkId (EFUSE_LDODCXO_REF_CAL_ID);
    EFUSE_RETURN_E result = EFUSE_RESULT_SUCCESS;

    if (dcxotrim_data == NULL)
    {
        return SCI_FALSE; //Don't need calibration
    }

    EFUSE_HAL_Open();
    
    result = EFUSE_HAL_Read (efuse_id,&data);
    if(EFUSE_RESULT_SUCCESS != result)
    {
        EFUSE_HAL_Close();
        *dcxotrim_data = 0;
        return SCI_FALSE; //Don't need calibration
    }
    
    EFUSE_HAL_Close();

    SCI_TRACE_LOW("EFUSE_HAL_GetDCXOTrim result=%d, data =%d, efuse_id=%d", result, data, efuse_id);
    *dcxotrim_data = (data >> 10) & 0x1F;
    
    return SCI_TRUE;
}

/******************************************************************************
 * [Description] : compatible with efuse map v0.5 and versions earlier        *
 *      read rg_ldo_rtc_cal stored in calibarion data                         *
 * [Global resource dependence]:                                              *
 * [Parameters]:                                                              *
 * [Return]:                                                                  *
 * [Note]:                                                                    *
 *      This function is used for calibration.                                *
 ******************************************************************************/
PUBLIC BOOLEAN EFUSE_HAL_GetRGLdoRtcCal(uint32 *rgrtccal_data)
{
    uint32 data = 0;
    uint32 efuse_id = EFUSE_PHY_GetBlkId (EFUSE_RG_LDO_RTC_CAL_ID);
    EFUSE_RETURN_E result = EFUSE_RESULT_SUCCESS;

    if (rgrtccal_data == NULL)
    {
        return SCI_FALSE; //Don't need calibration
    }

    EFUSE_HAL_Open();
    
    result = EFUSE_HAL_Read (efuse_id,&data);
    if(EFUSE_RESULT_SUCCESS != result)
    {
        EFUSE_HAL_Close();
        *rgrtccal_data = 0;
        return SCI_FALSE; //Don't need calibration
    }
    
    EFUSE_HAL_Close();

    SCI_TRACE_LOW("EFUSE_HAL_GetRGLdoRtcCal result=%d, data =%d, efuse_id=%d", result, data, efuse_id);
    *rgrtccal_data = (data >> 4) & 0x1F;
    
    return SCI_TRUE;
}
#endif


#if 0
PUBLIC EFUSE_RETURN_E EFUSE_HAL_ReadUid(uint8 *des_data_ptr)
{
    EFUSE_RETURN_E result;
    uint32 block0 = 0,block1 = 0;
    uint32 start_id,end_id;

    if (NULL == des_data_ptr)
    {
        return EFUSE_PTR_NULL_ERROR;
    }

#if 0
    start_id = EFUSE_PHY_GetBlkId (EFUSE_UID_START_ID);
    end_id = EFUSE_PHY_GetBlkId (EFUSE_UID_END_ID);

    EFUSE_HAL_Open();

    result = EFUSE_RESULT_SUCCESS;
    result = EFUSE_HAL_Read (start_id, &block0);
    if (EFUSE_RESULT_SUCCESS != result)
    {
        SCI_TRACE_LOW ("efuse block [%d] read error",start_id);
    }
    result = EFUSE_HAL_Read (end_id, &block1);
     if (EFUSE_RESULT_SUCCESS != result)
    {
        SCI_TRACE_LOW ("efuse block [%d] read error",end_id);
    }

    EFUSE_HAL_Close();
#else
    block0 = 0xAAAA;
    block1 = 0x5555;
    result = EFUSE_RESULT_SUCCESS;    
#endif

//CI_TRACE_LOW("block0=0x%08x,block1=0x%08x\n",block0,block1);

    sprintf(des_data_ptr,"%08x%08x",block0,block1);

    return result;
}
#endif


#if defined PLATFORM_UIX8910    // #define PLATFORM_UIX8910
#define RDA_EFUSE_UNIQUE_ID_LOW_INDEX (24)
#define RDA_EFUSE_UNIQUE_ID_HIGH_INDEX (26)

PUBLIC EFUSE_RETURN_E EFUSE_HAL_ReadUid(uint8 *id)
{

       EFUSE_RETURN_E result;
	uint8 *des;
	uint32 id_low, id_high;
	id_low =  EFUSE1_HAL_Read(RDA_EFUSE_UNIQUE_ID_LOW_INDEX);
	id_high = EFUSE1_HAL_Read(RDA_EFUSE_UNIQUE_ID_HIGH_INDEX);
    
        if (NULL == id)
        {
            return EFUSE_PTR_NULL_ERROR;
        }

        des = id;
	
	*des++ = (id_low & 0xff000000) >> 24;
	*des++ = (id_low & 0x00ff0000) >> 16;
	*des++ = (id_low & 0x0000ff00) >> 8;
	*des++ = (id_low & 0x000000ff) >> 0;

	*des++ = (id_high & 0xff000000) >> 24;
	*des++ = (id_high & 0x00ff0000) >> 16;
	*des++ = (id_high & 0x0000ff00) >> 8;
	*des++ = (id_high & 0x000000ff) >> 0;
    
       result = EFUSE_RESULT_SUCCESS;

    return result;
}
#endif


/******************************************************************************
 * [Description] :                                                            *
 *      read all blocks data of efuse module                                  *
 * [Global resource dependence]:                                              *
 * [Parameters]:                                                              *
 * [Return]:                                                                  *
 * [Note]:                                                                    *
 *      This function is used for debug.                                      *
 ******************************************************************************/
PUBLIC void EFUSE_HAL_Test (void)
{
    uint32 id,min_id,max_id;
    uint32 read_data;
    EFUSE_RETURN_E result;

    min_id = EFUSE_PHY_GetBlkId (EFUSE_MIN_ID);
    max_id = EFUSE_PHY_GetBlkId (EFUSE_MAX_ID);

    EFUSE_HAL_Open();

    for (id = min_id; id <= max_id; id++)
    {
        result = EFUSE_HAL_Read (id, &read_data);

        if (EFUSE_RESULT_SUCCESS != result)
        {
            SCI_TRACE_LOW ("efuse block [%d] read error",id);
        }

        SCI_TRACE_LOW ("efuse block %d, data = 0x%08x\r\n", id, read_data);
    }

    EFUSE_HAL_Close();
}

PUBLIC void EFUSE1_HAL_Open (void)
{
	EFUSE_PHY1_Open();
}

PUBLIC void EFUSE1_HAL_Close (void)
{
	EFUSE_PHY1_Close();
}

PUBLIC void EFUSE1_HAL_Write (int page_index, uint32 val)
{
    EFUSE_PHY1_Write(page_index, val);
}

PUBLIC uint32 EFUSE1_HAL_Read (int page_index)
{
	return EFUSE_PHY1_Read(page_index);
}

PUBLIC void EFUSE1_HAL_enWCNuart (void)
{
    EFUSE_PHY1_enWCNuart();
}


