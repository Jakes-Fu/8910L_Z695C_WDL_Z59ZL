/*****************************************************************************
 **  File Name:    efuse_drvapi.h                                            *
 **  Author:       windy.wang                                                *
 **  Date:         20/10/2009                                                *
 **  Copyright:    2009 Spreadtrum, Incorporated. All Rights Reserved.       *
 **  Description:  This file defines the basic operation interfaces of       *
 **                EFuse initilize and operation. It provides read and       *
 **                writer interfaces of 0~5 EFUSE_. Efuse 0 for Sn block.    *
 **                Efuse 1 to 4 for Hash blocks. Efuse 5 for control block.  *
 *****************************************************************************
 *****************************************************************************
 **  Edit History                                                            *
 **--------------------------------------------------------------------------*
 **  DATE               Author              Operation                        *
 **  25/06/2012         windy.wang          Create.                          *
 *****************************************************************************/

#ifndef _EFUSE_DRVAPI_H
#define _EFUSE_DRVAPI_H

/* the function return value */
typedef enum
{
    EFUSE_RESULT_SUCCESS                 = 0x00,//success to read & write
    EFUSE_READ_FAIL                      = 0x01,//fail to read
    EFUSE_WRITE_FAIL                     = 0x02,//fail to write
    EFUSE_WRITE_SOFT_COMPARE_FAIL        = 0x03,//fail to write
    EFUSE_WRITE_VERIFY_FAIL              = 0x04,
    EFUSE_PARAMETER_ERROR                = 0x05,
    EFUSE_ID_ERROR                       = 0x06,//block id is invalid
    EFUSE_HAS_WRITEED_ERROR              = 0x07,
    EFUSE_PTR_NULL_ERROR                 = 0x08,//pointer is NULL
    EFUSE_PTR_NUM_ERROR                  = 0x09,//bit length error
    EFUSE_WRITE_HARD_COMPARE_FAIL        = 0x0A,//fail to write data
    EFUSE_WRITE_SOFT_HARD_COMPARE_FAIL   = 0x0B,//fail to write
    EFUSE_BIST_TEST_FAIL                 = 0x0C,//efuse data is not zero
    EFUSE_BIST_TIMEOUT                   = 0x0D,
    EFUSE_NOT_LOCK                       = 0x0E,//could be written again
    EFUSE_LOCKED                         = 0x0F //forbidden to be written again
} EFUSE_RETURN_E;

PUBLIC void EFUSE_HAL_Open (void);
PUBLIC void EFUSE_HAL_Close (void);
PUBLIC uint32 EFUSE_HAL_GetAllBlkNum (void);
PUBLIC uint32 EFUSE_HAL_GetHashBlkLen (void);
PUBLIC uint32 EFUSE_HAL_GetIMEABitLen (void);
PUBLIC EFUSE_RETURN_E EFUSE_HAL_IsLock (uint32 block_id);
PUBLIC EFUSE_RETURN_E EFUSE_HAL_IsLockImei (uint32 bit_num);
PUBLIC EFUSE_RETURN_E EFUSE_HAL_IsLockHash (void);
PUBLIC EFUSE_RETURN_E EFUSE_HAL_Read (uint32 block_id,uint32 *des_data_ptr);
PUBLIC EFUSE_RETURN_E EFUSE_HAL_Read_Ex (uint32 block_id,uint32 *des_data_ptr);
PUBLIC EFUSE_RETURN_E EFUSE_HAL_ReadImei (uint8 *des_data_ptr, uint32 bit_num);
PUBLIC EFUSE_RETURN_E EFUSE_HAL_ReadHash (uint32 *des_data_ptr);
PUBLIC EFUSE_RETURN_E EFUSE_HAL_Write (uint32 block_id,uint32 write_data);
PUBLIC EFUSE_RETURN_E EFUSE_HAL_WriteImei (uint8 *des_data_ptr, uint32 bit_num);
PUBLIC EFUSE_RETURN_E EFUSE_HAL_WriteHash (uint32 *des_data_ptr);
PUBLIC BOOLEAN EFUSE_HAL_CalibrationGet (uint32 *p_cal_data);
PUBLIC BOOLEAN EFUSE_HAL_CCCVPointGet (uint32 *p_cal_cccv);
PUBLIC void EFUSE_HAL_Test (void);
PUBLIC EFUSE_RETURN_E EFUSE_HAL_ReadUid(uint8 *des_data_ptr);
PUBLIC void EFUSE1_HAL_Open (void);
PUBLIC void EFUSE1_HAL_Close (void);
PUBLIC void EFUSE1_HAL_Write (int page_index, uint32 val);
PUBLIC uint32 EFUSE1_HAL_Read (int page_index);
PUBLIC void EFUSE1_HAL_enWCNuart (void);

#endif //_EFUSE_DRVAPI_H


